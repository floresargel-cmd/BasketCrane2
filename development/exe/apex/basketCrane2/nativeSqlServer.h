#pragma once
#include <QSqlDriver>
#include <QSqlResult>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QSqlError>
#include <QVector>
#include <memory>
#include "config.h"

namespace basket {
struct SqlServerResultSet {
    QSqlRecord record;
    QVector<QVector<QVariant>> rows;
    int affected=-1;
};
// The implementation uses MSOLEDBSQL19 directly. No ODBC bridge or DSN lookup.
class SqlServerSession {
public:
    virtual ~SqlServerSession() = default;
    virtual bool Open(const QString& connectionString,const QString& password,QSqlError& error)=0;
    virtual void Close()=0;
    virtual bool Execute(const QString& sql,QVector<SqlServerResultSet>& sets,QSqlError& error)=0;
};
bool nativeSqlServerValueTests();
class NativeSqlServerSession final : public SqlServerSession {
public:
    NativeSqlServerSession();
    ~NativeSqlServerSession();
    bool Open(const QString& connectionString,const QString& password,QSqlError& error) override;
    void Close() override;
    bool Execute(const QString& sql,QVector<SqlServerResultSet>& sets,QSqlError& error) override;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
inline QString sqlServerConnectionValue(QString value) {
    value.replace('"',"\"\""); return '"'+value+'"';
}
inline QString sqlServerConnectionString(const BasketCraneConfig& config,const QString& section) {
    return "Provider=MSOLEDBSQL19;DataTypeCompatibility=80;Persist Security Info=False;"
        "Application Name=Apex Basket Crane 2;Data Source="+sqlServerConnectionValue("tcp:"+config.text(section+"/Server"))
        +";Initial Catalog="+sqlServerConnectionValue(config.text(section+"/Catalog"))
        +";User ID="+sqlServerConnectionValue(config.text(section+"/User"))
        +";Password="+sqlServerConnectionValue(config.text(section+"/Password"))
        +";Use Encryption for Data="+config.text(section+"/Encrypt")
        +";Trust Server Certificate="+config.text(section+"/TrustServerCertificate")
        +";Application Intent="+(config.databaseWritesAllowed()?QString("ReadWrite"):QString("ReadOnly"))+";";
}
class NativeSqlServerResult final : public QSqlResult {
public:
    NativeSqlServerResult(const QSqlDriver* driver,std::shared_ptr<SqlServerSession> session):QSqlResult(driver),session(std::move(session)) {}
protected:
    bool reset(const QString& sql) override {
        sets.clear(); current=0; setActive(false); setSelect(false); setAt(QSql::BeforeFirstRow);
        QSqlError error;
        if (!session->Execute(sql,sets,error)) { setLastError(error); return false; }
        setLastError(QSqlError()); setActive(true);
        if (sets.isEmpty()) sets.append(SqlServerResultSet());
        setSelect(!sets[current].record.isEmpty()); return true;
    }
    QVariant data(int column) override {
        if (at()<0 || at()>=sets[current].rows.size() || column<0 || column>=sets[current].record.count()) return QVariant();
        return sets[current].rows[at()][column];
    }
    bool isNull(int column) override { return data(column).isNull(); }
    bool fetch(int row) override {
        if (row<0) { setAt(QSql::BeforeFirstRow); return false; }
        if (row>=sets[current].rows.size()) { setAt(QSql::AfterLastRow); return false; }
        setAt(row); return true;
    }
    bool fetchNext() override { return fetch(at()==QSql::BeforeFirstRow?0:(at()<0?sets[current].rows.size():at()+1)); }
    bool fetchFirst() override { return fetch(0); }
    bool fetchLast() override { return fetch(sets[current].rows.size()-1); }
    int size() override { return sets.isEmpty()?-1:sets[current].rows.size(); }
    int numRowsAffected() override { return sets.isEmpty()?-1:sets[current].affected; }
    QSqlRecord record() const override { return sets.isEmpty()?QSqlRecord():sets[current].record; }
    bool nextResult() override {
        if (current+1>=sets.size()) { setActive(false); return false; }
        ++current; setAt(QSql::BeforeFirstRow); setSelect(!sets[current].record.isEmpty()); return true;
    }
    void detachFromResultSet() override { sets.clear(); current=0; }
private:
    std::shared_ptr<SqlServerSession> session;
    QVector<SqlServerResultSet> sets;
    int current=0;
};
class NativeSqlServerDriver final : public QSqlDriver {
public:
    explicit NativeSqlServerDriver(QString connectionString,QString password,std::shared_ptr<SqlServerSession> session=std::make_shared<NativeSqlServerSession>()):connectionString(std::move(connectionString)),password(std::move(password)),session(std::move(session)) {}
    ~NativeSqlServerDriver() { close(); }
    bool hasFeature(DriverFeature feature) const override {
        return feature==Transactions || feature==QuerySize || feature==BLOB || feature==Unicode || feature==MultipleResultSets || feature==LowPrecisionNumbers;
    }
    QSqlResult* createResult() const override { return new NativeSqlServerResult(this,session); }
    bool open(const QString&,const QString&,const QString&,const QString&,int,const QString&) override {
        QSqlError error; const bool opened=session->Open(connectionString,password,error);
        setLastError(error); setOpen(opened); setOpenError(!opened); return opened;
    }
    void close() override { session->Close(); setOpen(false); }
    QString formatValue(const QSqlField& field,bool trim=false) const override {
        if (field.isNull()) return "NULL";
        if (field.type()==QVariant::ByteArray) return "0x"+field.value().toByteArray().toHex();
        if (field.type()==QVariant::String) return "N"+QSqlDriver::formatValue(field,trim);
        return QSqlDriver::formatValue(field,trim);
    }
    QString escapeIdentifier(const QString& name,IdentifierType) const override {
        QStringList parts=name.split('.');
        for (QString& part:parts) { if (part.startsWith('[') && part.endsWith(']')) continue; part.replace(']',"]]"); part='['+part+']'; }
        return parts.join('.');
    }
    QSqlRecord record(const QString& table) const override {
        QVector<SqlServerResultSet> sets; QSqlError error;
        if (session->Execute("SELECT TOP 0 * FROM "+escapeIdentifier(table,TableName),sets,error) && !sets.isEmpty()) return sets.first().record;
        return QSqlRecord();
    }
    QStringList tables(QSql::TableType type) const override {
        QVector<SqlServerResultSet> sets; QSqlError error; QStringList names,types;
        if (type & QSql::Tables) types<<"'BASE TABLE'";
        if (type & QSql::Views) types<<"'VIEW'";
        if (!types.isEmpty() && session->Execute("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE IN ("+types.join(',')+")",sets,error))
            for (const SqlServerResultSet& set:sets) for (const QVector<QVariant>& row:set.rows) if (!row.isEmpty()) names<<row.first().toString();
        return names;
    }
    QSqlIndex primaryIndex(const QString& table) const override {
        QSqlIndex index(table); const QSqlRecord columns=record(table);
        QString object=escapeIdentifier(table,TableName); object.replace('\'',"''");
        QVector<SqlServerResultSet> sets; QSqlError error;
        if (session->Execute("SELECT c.name FROM sys.indexes i JOIN sys.index_columns ic ON i.object_id=ic.object_id AND i.index_id=ic.index_id JOIN sys.columns c ON c.object_id=ic.object_id AND c.column_id=ic.column_id WHERE i.is_primary_key=1 AND i.object_id=OBJECT_ID(N'"+object+"') ORDER BY ic.key_ordinal",sets,error))
            for (const SqlServerResultSet& set:sets) for (const QVector<QVariant>& row:set.rows) if (!row.isEmpty()) index.append(columns.field(row.first().toString()));
        return index;
    }
    bool beginTransaction() override { return control("BEGIN TRANSACTION"); }
    bool commitTransaction() override { return control("COMMIT TRANSACTION"); }
    bool rollbackTransaction() override { return control("IF @@TRANCOUNT>0 ROLLBACK TRANSACTION"); }
private:
    bool control(const QString& sql) { QVector<SqlServerResultSet> sets; QSqlError error; const bool ok=session->Execute(sql,sets,error); setLastError(error); return ok; }
    QString connectionString;
    QString password;
    std::shared_ptr<SqlServerSession> session;
};
}
