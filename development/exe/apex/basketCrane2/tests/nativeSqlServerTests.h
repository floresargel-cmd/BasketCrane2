#pragma once
#include "../nativeSqlServer.h"
#include "../readOnlySql.h"
#include <QSqlQuery>
#include <cstdio>

class BasketNativeSessionFixture final : public basket::SqlServerSession {
public:
    bool Open(const QString&,const QString&,QSqlError& error) override { error=QSqlError(); return true; }
    void Close() override {}
    bool Execute(const QString& sql,QVector<basket::SqlServerResultSet>& sets,QSqlError& error) override {
        statements<<sql; sets.clear(); error=QSqlError();
        if (sql=="FAIL") { error=QSqlError("SQL Server","deadlock victim",QSqlError::StatementError,"1205"); return false; }
        basket::SqlServerResultSet first; first.record.append(QSqlField("basket",QVariant::Int));
        first.record.append(QSqlField("description",QVariant::String));
        first.rows.append(QVector<QVariant>()<<42<<QString::fromWCharArray(L"Unicode \x03a9")); first.affected=1;
        sets<<first;
        if (sql=="MULTIPLE") { basket::SqlServerResultSet second; second.record.append(QSqlField("empty",QVariant::String)); second.affected=0; sets<<second; }
        return true;
    }
    QStringList statements;
};
inline bool basketNativeSqlServerTests() {
    if (QSqlDatabase::drivers().contains("QODBC") || QSqlDatabase::drivers().contains("QODBC3")) return false;
    if (!basket::nativeSqlServerValueTests() || basket::sqlServerConnectionValue("a;\"b")!="\"a;\"\"b\"") return false;
    auto fixture=std::make_shared<BasketNativeSessionFixture>();
    QSqlDatabase database=QSqlDatabase::addDatabase(new basket::NativeSqlServerDriver("fixture","",fixture),"direct_sql_fixture");
    if (!database.open() || database.driver()->hasFeature(QSqlDriver::PreparedQueries)) return false;
    {
        QSqlQuery query(database);
        if (!query.prepare("SELECT ?") ) return false;
        const QString text=QString::fromWCharArray(L"Unicode \x03a9 O'Brien"); query.addBindValue(text);
        if (!query.exec() || !fixture->statements.last().startsWith("SELECT N'")
            || !fixture->statements.last().contains("O''Brien") || !query.next()
            || query.value("basket").toInt()!=42 || query.value("description").toString()!=QString::fromWCharArray(L"Unicode \x03a9")
            || query.next() || !query.first() || query.size()!=1 || query.numRowsAffected()!=1) return false;
        if (!query.exec("MULTIPLE") || !query.next() || !query.nextResult() || query.record().fieldName(0)!="empty"
            || query.next() || query.nextResult()) return false;
        if (!query.prepare("SELECT ?")) return false;
        query.addBindValue(QByteArray::fromHex("007ffe"));
        if (!query.exec() || fixture->statements.last()!="SELECT 0x007ffe") return false;
        if (query.exec("FAIL") || query.lastError().nativeErrorCode()!="1205") return false;
        if (!query.prepare("INSERT INTO fixture VALUES (?)")) return false;
        query.addBindValue(QVariantList()<<1<<2<<3);
        const int before=fixture->statements.size();
        if (!query.execBatch() || fixture->statements.size()!=before+3) return false;
        if (database.driver()->escapeIdentifier("dbo.table]name",QSqlDriver::TableName)!="[dbo].[table]]name]") return false;
    }
    QSqlDatabase guarded=QSqlDatabase::addDatabase(new BasketReadOnlyDriver(database),"direct_sql_guard_fixture");
    if (!guarded.open()) return false;
    {
        const int before=fixture->statements.size(); QSqlQuery query(guarded);
        if (query.exec("DELETE FROM fixture") || fixture->statements.size()!=before
            || !query.exec("SELECT 42") || !query.next()) return false;
    }
    guarded.close(); guarded=QSqlDatabase(); QSqlDatabase::removeDatabase("direct_sql_guard_fixture");
    database.close(); database=QSqlDatabase(); QSqlDatabase::removeDatabase("direct_sql_fixture");
    std::puts("PASS: direct SQL Server values, Unicode/binary bindings, result metadata, multiple results, native errors, batch emulation and read-only guard.");
    return true;
}
