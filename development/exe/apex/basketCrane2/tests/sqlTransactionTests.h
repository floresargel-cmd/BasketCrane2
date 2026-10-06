#pragma once

#include "../sqlTransaction.h"
#include <QSqlDriver>
#include <QSqlResult>
#include <cstdio>
#include <stdexcept>

class BasketScriptedTransaction final : public basket::ITransactionSession
{
public:
    bool Begin() override
    {
        ++begins;
        commandIndex = 0;
        if (failBegin) { error = basket::TransactionError("begin failed"); return false; }
        active = true;
        staged.clear();
        return true;
    }
    bool Execute(const basket::SqlCommand& command, QVector<QVariant>& output) override
    {
        ++executions;
        ++commandIndex;
        statements.append(command.statement);
        if (command.bulk) {
            int rowIndex = 0;
            for (const QVector<QVariant>& row : command.rows) {
                ++bulkRows;
                if (++rowIndex == failBulkRow && ShouldFail()) return Fail();
                staged.append(row.first());
                output.append(row.first());
            }
            return true;
        }
        if (commandIndex == failCommand && ShouldFail()) return Fail();
        staged.append(command.values.isEmpty() ? QVariant(command.statement) : command.values.first());
        output.append(staged.last());
        return true;
    }
    bool Commit() override
    {
        ++commits;
        if (failCommit && ShouldFail()) return Fail();
        committed += staged;
        staged.clear();
        active = false;
        return true;
    }
    bool Rollback() override
    {
        ++rollbacks;
        staged.clear();
        active = false;
        if (failRollback) { error = basket::TransactionError("cleanup unavailable"); return false; }
        return true;
    }
    QSqlError LastError() const override { return error; }

    int begins = 0, executions = 0, commits = 0, rollbacks = 0, bulkRows = 0;
    int failuresRemaining = 0, failCommand = 2, failBulkRow = 2;
    QString nativeCode = "1205";
    bool failCommit = false, failBegin = false, failRollback = false, active = false;
    QVector<QString> statements;
    QVector<QVariant> staged, committed;
private:
    bool ShouldFail() const { return failuresRemaining > 0; }
    bool Fail()
    {
        --failuresRemaining;
        error = QSqlError("simulated failure", "database failure", QSqlError::StatementError, nativeCode);
        return false;
    }
    int commandIndex = 0;
    QSqlError error;
};

// Qt adapter fixture: statements and emulated execBatch reach this driver.
// No ODBC connection, SQL Server or application UI is constructed.
struct BasketSqlTransactionFixture
{
    int depth = 0, begins = 0, commits = 0, rollbacks = 0, writes = 0, failWriteAt = 0;
    QStringList staged, committed;
};

class BasketTransactionSqlResult final : public QSqlResult
{
public:
    BasketTransactionSqlResult(const QSqlDriver* driver, BasketSqlTransactionFixture& fixture)
        : QSqlResult(driver), fixture_(fixture) {}
protected:
    bool reset(const QString& statement) override
    {
        setLastError(QSqlError());
        setSelect(false);
        setActive(true);
        setAt(QSql::BeforeFirstRow);
        if (statement == "SELECT @@TRANCOUNT") {
            depth_ = fixture_.depth;
            setSelect(true);
        } else if (statement == "BEGIN TRANSACTION") {
            ++fixture_.begins;
            fixture_.depth = 1;
        } else if (statement == "COMMIT TRANSACTION") {
            ++fixture_.commits;
            fixture_.committed += fixture_.staged;
            fixture_.staged.clear();
            fixture_.depth = 0;
        } else if (statement == "IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION") {
            ++fixture_.rollbacks;
            fixture_.staged.clear();
            fixture_.depth = 0;
        } else if (statement.startsWith("INSERT")) {
            if (++fixture_.writes == fixture_.failWriteAt) {
                // SQL Server has already rolled back a deadlock victim.
                fixture_.depth = 0;
                fixture_.staged.clear();
                setLastError(QSqlError("deadlock", "victim", QSqlError::StatementError, "1205"));
                setActive(false);
                return false;
            }
            if (fixture_.depth != 1) {
                setLastError(basket::TransactionError("write occurred outside its transaction"));
                return false;
            }
            fixture_.staged.append(statement);
        } else {
            setLastError(basket::TransactionError("unexpected fixture statement"));
            return false;
        }
        return true;
    }
    QVariant data(int) override { return depth_; }
    bool isNull(int) override { return false; }
    bool fetch(int index) override
    {
        if (!isSelect() || index != 0) { setAt(QSql::AfterLastRow); return false; }
        setAt(0);
        return true;
    }
    bool fetchFirst() override { return fetch(0); }
    bool fetchLast() override { return fetch(0); }
    int size() override { return isSelect() ? 1 : 0; }
    int numRowsAffected() override { return 1; }
private:
    BasketSqlTransactionFixture& fixture_;
    int depth_ = 0;
};

class BasketTransactionSqlDriver final : public QSqlDriver
{
public:
    explicit BasketTransactionSqlDriver(BasketSqlTransactionFixture& fixture) : fixture_(fixture) { setOpen(true); }
    bool hasFeature(DriverFeature) const override { return false; }
    QSqlResult* createResult() const override { return new BasketTransactionSqlResult(this, fixture_); }
    bool open(const QString&, const QString&, const QString&, const QString&, int, const QString&) override
    { setOpen(true); return true; }
    void close() override { setOpen(false); }
private:
    BasketSqlTransactionFixture& fixture_;
};

inline bool basketSqlTransactionTests()
{
    auto require = [](bool passed, const char* name) {
        if (!passed) std::fprintf(stderr, "FAIL: SQL transaction test: %s\n", name);
        return passed;
    };
    QVector<int> waits;
    basket::SqlTransactionRunner runner([&waits](int retry) { waits.append(retry); });
    const QVector<basket::SqlCommand> commands = QVector<basket::SqlCommand>()
        << basket::SqlCommand("first", QVector<QVariant>() << 10)
        << basket::SqlCommand("second", QVector<QVariant>() << 20);
    BasketScriptedTransaction success;
    auto result = runner.Run(success, commands);
    if (!require(result.succeeded && result.attempts == 1 && success.committed.size() == 2
        && success.commits == 1 && success.rollbacks == 0 && waits.isEmpty(), "single atomic commit")) return false;

    BasketScriptedTransaction deadlock;
    deadlock.failuresRemaining = 1;
    result = runner.Run(deadlock, commands);
    if (!require(result.succeeded && result.attempts == 2 && deadlock.begins == 2
        && deadlock.executions == 4 && deadlock.rollbacks == 1 && deadlock.committed.size() == 2
        && result.values.size() == 2 && waits.size() == 1 && waits[0] == 0
        && deadlock.statements[0] == deadlock.statements[2], "replay entire deadlocked transaction without duplicate rows/results")) return false;

    waits.clear();
    BasketScriptedTransaction exhausted;
    exhausted.failuresRemaining = 10;
    result = runner.Run(exhausted, commands);
    if (!require(!result.succeeded && result.attempts == 4 && exhausted.rollbacks == 4
        && exhausted.commits == 0 && exhausted.committed.isEmpty() && result.values.isEmpty()
        && waits.size() == 3 && waits[0] == 0 && waits[1] == 1 && waits[2] == 2,
        "initial attempt plus at most three retries")) return false;

    waits.clear();
    BasketScriptedTransaction lastAttempt;
    lastAttempt.failuresRemaining = 3;
    result = runner.Run(lastAttempt, commands);
    if (!require(result.succeeded && result.attempts == 4 && lastAttempt.rollbacks == 3
        && lastAttempt.commits == 1 && lastAttempt.committed.size() == 2 && !result.error.isValid()
        && waits.size() == 3, "fourth attempt can succeed and clears earlier error")) return false;
    waits.clear();
    BasketScriptedTransaction permanent;
    permanent.failuresRemaining = 1;
    permanent.nativeCode = "2627";
    result = runner.Run(permanent, commands);
    if (!require(!result.succeeded && result.attempts == 1 && permanent.rollbacks == 1
        && permanent.committed.isEmpty() && waits.isEmpty(), "constraint errors are not retried")) return false;

    BasketScriptedTransaction commitDeadlock;
    commitDeadlock.failuresRemaining = 1;
    commitDeadlock.failCommand = -1;
    commitDeadlock.failCommit = true;
    result = runner.Run(commitDeadlock, commands);
    if (!require(result.succeeded && result.attempts == 2 && commitDeadlock.rollbacks == 1
        && commitDeadlock.commits == 2 && commitDeadlock.committed.size() == 2, "confirmed commit deadlock retries safely")) return false;

    waits.clear();
    BasketScriptedTransaction ambiguousCommit;
    ambiguousCommit.failuresRemaining = 1;
    ambiguousCommit.failCommand = -1;
    ambiguousCommit.failCommit = true;
    ambiguousCommit.nativeCode = "08S01";
    result = runner.Run(ambiguousCommit, commands);
    if (!require(!result.succeeded && result.attempts == 1 && ambiguousCommit.rollbacks == 1
        && waits.isEmpty(), "ambiguous commit/connection failure is never replayed")) return false;

    BasketScriptedTransaction cleanupFailure;
    cleanupFailure.failuresRemaining = 1;
    cleanupFailure.failRollback = true;
    result = runner.Run(cleanupFailure, commands);
    if (!require(!result.succeeded && result.attempts == 1 && cleanupFailure.rollbacks == 1
        && result.error.text().contains("rollback failed") && waits.isEmpty(), "rollback failure stops retry")) return false;

    BasketScriptedTransaction beginFailure;
    beginFailure.failBegin = true;
    result = runner.Run(beginFailure, commands);
    if (!require(!result.succeeded && beginFailure.executions == 0 && beginFailure.rollbacks == 0,
        "failed begin does not execute or roll back caller state")) return false;

    QVector<QVector<QVariant>> rows;
    rows << (QVector<QVariant>() << 1 << "O'Brien")
         << (QVector<QVariant>() << 2 << "second")
         << (QVector<QVariant>() << 3 << "third");
    const auto bulk = basket::SqlCommand::Bulk("INSERT INTO fixture VALUES(?,?)", rows);
    BasketScriptedTransaction bulkDeadlock;
    bulkDeadlock.failuresRemaining = 1;
    result = runner.Run(bulkDeadlock, QVector<basket::SqlCommand>() << bulk);
    if (!require(result.succeeded && result.attempts == 2 && bulkDeadlock.bulkRows == 5
        && bulkDeadlock.committed.size() == 3 && bulkDeadlock.committed[0].toInt() == 1
        && bulkDeadlock.committed[2].toInt() == 3 && bulkDeadlock.rollbacks == 1,
        "partial bulk failure rolls back all rows and replays whole batch")) return false;

    BasketScriptedTransaction mixed;
    result = runner.Run(mixed, QVector<basket::SqlCommand>() << bulk << commands[0]);
    if (!require(result.succeeded && mixed.commits == 1 && mixed.committed.size() == 4,
        "bulk and scalar commands share one transaction")) return false;

    auto ragged = bulk;
    ragged.rows.append(QVector<QVariant>() << 4);
    BasketScriptedTransaction invalid;
    result = runner.Run(invalid, QVector<basket::SqlCommand>() << ragged);
    if (!require(!result.succeeded && result.attempts == 0 && invalid.begins == 0,
        "ragged bulk rows rejected before starting transaction")) return false;
    result = runner.Run(invalid, QVector<basket::SqlCommand>() << basket::SqlCommand(" "));
    if (!require(!result.succeeded && invalid.begins == 0, "empty SQL rejected")) return false;
    result = runner.Run(invalid, QVector<basket::SqlCommand>());
    if (!require(result.succeeded && result.attempts == 0, "empty transaction is a no-op")) return false;

    BasketScriptedTransaction exceptionCleanup;
    try {
        basket::TransactionScope scope(exceptionCleanup);
        if (!scope.Begin()) return false;
        throw std::runtime_error("simulated unwinding");
    } catch (const std::runtime_error&) {}
    if (!require(exceptionCleanup.rollbacks == 1 && !exceptionCleanup.active, "RAII rollback on exception")) return false;

    if (!require(!basket::IsSqlServerDeadlock(QSqlError("1205 in text", "deadlock", QSqlError::StatementError, "12050"))
        && !basket::IsSqlServerDeadlock(QSqlError("serialization", "failure", QSqlError::StatementError, "40001")),
        "retry classification uses exact native code")) return false;
    BasketCraneConfig unloaded;
    result = basket::SqlTransactionExecutor(unloaded, "transaction_nonexistent").Execute(QVector<basket::SqlCommand>() << bulk);
    if (!require(result.succeeded && result.skipped && result.attempts == 0
        && !QSqlDatabase::contains("transaction_nonexistent"), "read-only policy blocks bulk before connection lookup")) return false;

    BasketSqlTransactionFixture fixture;
    bool adapterPassed = false;
    {
        QSqlDatabase database = QSqlDatabase::addDatabase(new BasketTransactionSqlDriver(fixture), "transaction_fixture");
        basket::QtSqlTransactionSession session(database);
        fixture.failWriteAt = 2;
        result = runner.Run(session, QVector<basket::SqlCommand>() << bulk);
        adapterPassed = result.succeeded && result.attempts == 2 && fixture.begins == 2
            && fixture.rollbacks == 1 && fixture.commits == 1 && fixture.depth == 0
            && fixture.writes == 5 && fixture.committed.size() == 3
            && fixture.committed[0].contains("O''Brien");
        fixture.depth = 1; // A caller owns an already-open transaction.
        result = runner.Run(session, QVector<basket::SqlCommand>() << bulk);
        adapterPassed = adapterPassed && !result.succeeded && fixture.depth == 1
            && fixture.rollbacks == 1 && fixture.commits == 1 && fixture.begins == 2;
    }
    QSqlDatabase::removeDatabase("transaction_fixture");
    if (!require(adapterPassed, "Qt execBatch bindings, atomic rollback, replay, and caller transaction ownership")) return false;

    std::puts("PASS: SQL transaction commit/rollback; deadlocks retry at most three times; scalar/bulk atomicity; permanent/ambiguous errors and ownership guards.");
    return true;
}