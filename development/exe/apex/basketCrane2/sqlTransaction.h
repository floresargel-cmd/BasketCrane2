#pragma once

#include "config.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>
#include <QVariant>
#include <QVector>
#include <functional>
#include <random>
#include <utility>

namespace basket {

struct SqlCommand
{
    QString statement;
    QVector<QVariant> values;
    QVector<QVector<QVariant>> rows;
    bool bulk = false;

    SqlCommand() = default;

    explicit SqlCommand(QString sql, QVector<QVariant> bindings = QVector<QVariant>())
        : statement(std::move(sql)), values(std::move(bindings)) {}

    static SqlCommand Bulk(QString sql, QVector<QVector<QVariant>> bindings)
    {
        SqlCommand command(std::move(sql));
        command.rows = std::move(bindings);
        command.bulk = true;
        return command;
    }
};

struct SqlTransactionResult
{
    bool succeeded = false;
    bool skipped = false;
    int attempts = 0;
    QSqlError error;
    QVector<QVariant> values;
};

inline QSqlError TransactionError(const QString& message)
{
    return QSqlError(message, QString(), QSqlError::TransactionError);
}

// Only SQL Server's confirmed deadlock-victim code is safe to replay.
// Timeouts, connection failures and ambiguous commit errors are not retried.
inline bool IsSqlServerDeadlock(const QSqlError& error)
{
    if (!error.isValid()) return false;
    const QStringList codes = error.nativeErrorCode().split(';');
    for (const QString& code : codes) {
        bool valid = false;
        if (code.trimmed().toInt(&valid) == 1205 && valid) return true;
    }
    return false;
}

class ITransactionSession
{
public:
    virtual ~ITransactionSession() = default;
    virtual bool Begin() = 0;
    virtual bool Execute(const SqlCommand& command, QVector<QVariant>& values) = 0;
    virtual bool Commit() = 0;
    virtual bool Rollback() = 0;
    virtual QSqlError LastError() const = 0;
};

// SQL Server-specific transaction control. Native T-SQL commands retain the
// statement's native error code through the direct SQL Server adapter.
// This class never opens a connection or changes its autocommit settings.
class QtSqlTransactionSession final : public ITransactionSession
{
public:
    explicit QtSqlTransactionSession(QSqlDatabase database) : database_(std::move(database)) {}

    bool Begin() override
    {
        QSqlQuery probe(database_);
        if (!probe.exec("SELECT @@TRANCOUNT") || !probe.next()) {
            lastError_ = probe.lastError();
            if (!lastError_.isValid()) lastError_ = TransactionError("Cannot determine transaction ownership.");
            return false;
        }
        bool valid = false;
        const int count = probe.value(0).toInt(&valid);
        probe.finish();
        if (!valid || count != 0) {
            lastError_ = TransactionError("An existing transaction belongs to the caller; nested execution is refused.");
            return false;
        }
        return Control("BEGIN TRANSACTION");
    }

    bool Execute(const SqlCommand& command, QVector<QVariant>& values) override
    {
        QSqlQuery query(database_);
        query.setForwardOnly(true);
        if (command.bulk) {
            if (command.rows.isEmpty()) return true;
            if (!query.prepare(command.statement)) return Fail(query.lastError());
            for (int column = 0; column < command.rows.first().size(); ++column) {
                QVariantList bindings;
                for (const QVector<QVariant>& row : command.rows) bindings.append(row[column]);
                query.addBindValue(bindings);
            }
            // Drivers without native batching emulate this within the same
            // explicit transaction: no row can commit independently.
            if (!query.execBatch(QSqlQuery::ValuesAsRows)) return Fail(query.lastError());
        } else if (command.values.isEmpty()) {
            if (!query.exec(command.statement)) return Fail(query.lastError());
        } else {
            if (!query.prepare(command.statement)) return Fail(query.lastError());
            for (const QVariant& value : command.values) query.addBindValue(value);
            if (!query.exec()) return Fail(query.lastError());
        }
        do {
            while (query.next()) values.append(query.value(0));
            if (query.lastError().isValid()) return Fail(query.lastError());
        } while (query.nextResult());
        if (query.lastError().isValid()) return Fail(query.lastError());
        // Release active result sets before committing the connection.
        query.finish();
        return true;
    }

    bool Commit() override { return Control("COMMIT TRANSACTION"); }
    bool Rollback() override { return Control("IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION"); }
    QSqlError LastError() const override { return lastError_; }

private:
    bool Fail(const QSqlError& error) { lastError_ = error; return false; }
    bool Control(const QString& statement)
    {
        QSqlQuery query(database_);
        if (!query.exec(statement)) return Fail(query.lastError());
        query.finish();
        lastError_ = QSqlError();
        return true;
    }

    QSqlDatabase database_;
    QSqlError lastError_;
};

class TransactionScope final
{
public:
    explicit TransactionScope(ITransactionSession& session) : session_(session) {}
    ~TransactionScope() { if (active_) session_.Rollback(); }
    TransactionScope(const TransactionScope&) = delete;
    TransactionScope& operator=(const TransactionScope&) = delete;

    bool Begin() { active_ = session_.Begin(); return active_; }
    bool Commit()
    {
        if (!session_.Commit()) return false;
        active_ = false;
        return true;
    }
    bool Rollback()
    {
        // Explicit cleanup is attempted once; its failure stops the retry loop.
        active_ = false;
        return session_.Rollback();
    }

private:
    ITransactionSession& session_;
    bool active_ = false;
};

class SqlTransactionRunner final
{
public:
    static constexpr int MaxRetries = 3;
    using RetryWait = std::function<void(int)>;

    explicit SqlTransactionRunner(RetryWait wait = DefaultRetryWait) : wait_(std::move(wait)) {}

    SqlTransactionResult Run(ITransactionSession& session, const QVector<SqlCommand>& commands) const
    {
        SqlTransactionResult result;
        for (const SqlCommand& command : commands) {
            if (command.statement.trimmed().isEmpty()) {
                result.error = TransactionError("A SQL command must not be empty.");
                return result;
            }
            if (!command.bulk) continue;
            if (!command.values.isEmpty()) {
                result.error = TransactionError("Bulk commands use rows, not scalar bindings.");
                return result;
            }
            if (command.rows.isEmpty()) continue;
            const int columns = command.rows.first().size();
            for (const QVector<QVariant>& row : command.rows) {
                if (columns == 0 || row.size() != columns) {
                    result.error = TransactionError("All bulk rows must have the same nonzero column count.");
                    return result;
                }
            }
        }
        if (commands.isEmpty()) { result.succeeded = true; return result; }
        for (int retry = 0; retry <= MaxRetries; ++retry) {
            ++result.attempts;
            TransactionScope transaction(session);
            if (!transaction.Begin()) { result.error = session.LastError(); return result; }
            QVector<QVariant> attemptValues;
            bool executed = true;
            for (const SqlCommand& command : commands) {
                if (!session.Execute(command, attemptValues)) { executed = false; break; }
            }
            if (executed && transaction.Commit()) {
                result.succeeded = true;
                result.error = QSqlError();
                result.values = std::move(attemptValues);
                return result;
            }
            const QSqlError failure = session.LastError();
            result.error = failure;
            if (!transaction.Rollback()) {
                result.error = QSqlError(failure.driverText() + "; rollback failed: " + session.LastError().text(),
                    failure.databaseText(), failure.type(), failure.nativeErrorCode());
                return result;
            }
            if (!IsSqlServerDeadlock(failure) || retry == MaxRetries) return result;
            // The entire transaction is replayed, including earlier commands
            // and bulk rows. Never wait while still holding the transaction.
            if (wait_) wait_(retry);
        }
        return result;
    }

private:
    static void DefaultRetryWait(int retry)
    {
        static thread_local std::mt19937 random(std::random_device{}());
        const unsigned long delay = static_cast<unsigned long>((100 << retry)
            + std::uniform_int_distribution<int>(0, 50)(random));
        QThread::msleep(delay);
    }
    RetryWait wait_;
};

// A connection is used only on its owning thread. This executor owns each
// transaction; callers must not supply BEGIN/COMMIT/ROLLBACK commands or procedures
// that manage their own transactions. PLC/UI side effects stay outside replay.
class SqlTransactionExecutor final
{
public:
    SqlTransactionExecutor(const BasketCraneConfig& config, QString connection)
        : config_(config), connection_(std::move(connection)) {}

    SqlTransactionResult Execute(const QVector<SqlCommand>& commands) const
    {
        SqlTransactionResult result;
        if (!config_.databaseWritesAllowed()) {
            result.succeeded = true;
            result.skipped = true;
            return result;
        }
        if (!QSqlDatabase::contains(connection_)) {
            result.error = TransactionError("SQL connection does not exist.");
            return result;
        }
        QSqlDatabase database = QSqlDatabase::database(connection_, false);
        if (!database.isValid() || !database.isOpen()) {
            result.error = TransactionError("SQL connection is not open.");
            return result;
        }
        QtSqlTransactionSession session(database);
        return SqlTransactionRunner().Run(session, commands);
    }

private:
    const BasketCraneConfig& config_;
    QString connection_;
};

} // namespace basket
