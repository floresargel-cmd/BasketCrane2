#ifndef BASKET_ENVIRONMENT_SAFETY_TESTS_H
#define BASKET_ENVIRONMENT_SAFETY_TESTS_H
#include "../readOnlySql.h"
#include "../config.h"
#include <QColor>
#include "../observerDisplay.h"
#include <QSqlField>
#include <QSharedMemory>
#include <cstdio>
#include "architectureTests.h"
#include "sqlTransactionTests.h"
#include "nativeSqlServerTests.h"

class BasketFakeSqlResult : public QSqlResult {
public:
    BasketFakeSqlResult(const QSqlDriver *driver, int *calls) : QSqlResult(driver), calls(calls) {}
protected:
    bool reset(const QString &) override { ++*calls; setActive(true); setSelect(true); setAt(-1); return true; }
    QVariant data(int) override { return 42; }
    bool isNull(int) override { return false; }
    bool fetch(int i) override { if (i != 0) { setAt(QSql::AfterLastRow); return false; } setAt(0); return true; }
    bool fetchFirst() override { return fetch(0); }
    bool fetchLast() override { return fetch(0); }
    int size() override { return 1; }
    int numRowsAffected() override { return 0; }
    QSqlRecord record() const override { QSqlRecord r; r.append(QSqlField("value", QVariant::Int)); return r; }
private:
    int *calls;
};
class BasketFakeSqlDriver : public QSqlDriver {
public:
    explicit BasketFakeSqlDriver(int *calls) : calls(calls) { setOpen(true); }
    bool hasFeature(DriverFeature) const override { return false; }
    QSqlResult *createResult() const override { return new BasketFakeSqlResult(this, calls); }
    bool open(const QString &, const QString &, const QString &, const QString &, int, const QString &) override { setOpen(true); return true; }
    void close() override { setOpen(false); }
private:
    int *calls;
};
inline bool basketEnvironmentSafetyTests()
{
    if (!basketArchitectureTests() || !basketSqlTransactionTests()) return false;
    QVector<QVariant> profile;
    profile << 52 << 6525 << 334 << 7000 << "T6" << "000" << "" << "" << "DESTACK";
    if (observerDestination(profile) != "HCA") return false;
    profile[6] = "ANODIZING";
    if (observerDestination(profile) != "HCB") return false;
    profile[6] = "CUTBACK";
    if (observerDestination(profile) != "HCB") return false;
    profile[8] = "PACK";
    if (observerDestination(profile) != "None") return false;
    if (!basketReadOnlyStatement(observerContentsSql()) || !basketReadOnlyStatement(observerContentsSql(52))) return false;
    std::puts("PASS: observer EPICS classification and SELECT-only display queries.");
    BasketCraneConfig unloaded;
    if (unloaded.liveControlAllowed() || unloaded.databaseWritesAllowed() || !unloaded.plcOffline()) return false;
    const QStringList reads = QStringList()
        << "SELECT 42" << "select 'delete; update' as [value]"
        << "/* outer /* nested */ comment */ SELECT 42; -- trailing comment"
        << "SELECT basket FROM [positions] WHERE description='O''Brien'";
    foreach (const QString &sql, reads) if (!basketReadOnlyStatement(sql)) return false;
    const QStringList writes = QStringList()
        << "INSERT INTO positions VALUES(1)" << "UPDATE positions SET basket=1"
        << "DELETE FROM positions" << "CREATE TABLE p(id int)" << "DROP TABLE positions"
        << "MERGE positions AS t USING positions AS s ON 1=1 WHEN MATCHED THEN DELETE;"
        << "EXEC sp_executesql N'UPDATE positions SET basket=1'"
        << "SELECT 1; DELETE FROM positions" << "SELECT 1 INTO new_table"
        << "SELECT NEXT VALUE FOR basket_sequence"
        << "SELECT * FROM OPENQUERY(linked, 'UPDATE positions SET basket=1')"
        << "SELECT * FROM positions WITH (UPDLOCK)" << "SELECT 1 /* unterminated"
        << "SELECT 'unterminated" << "USE live_db";
    int backendCalls = 0;
    {
        QSqlDatabase backend = QSqlDatabase::addDatabase(new BasketFakeSqlDriver(&backendCalls), "safety_backend");
        QSqlDatabase guarded = QSqlDatabase::addDatabase(new BasketReadOnlyDriver(backend), "safety_guarded");
        QSqlQuery query(guarded);
        foreach (const QString &sql, writes) {
            const int before = backendCalls;
            if (query.exec(sql) || backendCalls != before) return false;
            query.prepare(sql);
            if (query.exec() || backendCalls != before) return false;
        }
        query.setForwardOnly(true);
        if (!query.exec("SELECT 42") || !query.next() || query.value(0).toInt() != 42
            || query.record().count() != 1 || query.next()) return false;
        query.prepare("UPDATE positions SET basket=?"); query.addBindValue(1);
        const int before = backendCalls;
        if (query.exec() || backendCalls != before) return false;
        query.prepare("SELECT ? AS value"); query.addBindValue("'; DELETE FROM positions; --");
        if (!query.exec() || backendCalls != before + 1) return false;
        query.prepare("UPDATE positions SET basket=?"); query.addBindValue(QVariantList() << 1 << 2);
        const int beforeBatch = backendCalls;
        if (query.execBatch() || backendCalls != beforeBatch) return false;
        if (guarded.transaction()) return false;
        if (guarded.driver()->handle().isValid()) return false;
    }
    QSqlDatabase::removeDatabase("safety_guarded");
    QSqlDatabase::removeDatabase("safety_backend");
    // The same mode locks out duplicates, while a second mode can run alongside it.
    const QString prefix = "basket_safety_" + QString::number(QCoreApplication::applicationPid());
    QSharedMemory live(prefix + "Live"), duplicate(prefix + "Live"), observer(prefix + "LiveObserver");
    if (!live.create(1) || duplicate.create(1) || !observer.create(1)) return false;
    std::puts("PASS: SELECT results and bindings; direct/prepared/batch writes blocked before backend; per-environment instance locks.");
    return true;
}
#endif
