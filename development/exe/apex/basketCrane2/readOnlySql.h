#ifndef BASKET_READ_ONLY_SQL_H
#define BASKET_READ_ONLY_SQL_H
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlResult>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlIndex>
#include <QStringList>

// Conservative SELECT-only policy. All query execution (including code in
// linked widgets, prepared statements and batches) goes through this driver.
inline bool basketReadOnlyStatement(const QString &sql)
{
    QStringList words;
    bool ended = false;
    for (int i = 0; i < sql.size();) {
        const QChar c = sql[i];
        if (c.isSpace()) { ++i; continue; }
        if (sql.mid(i, 2) == "--") {
            while (i < sql.size() && sql[i] != '\n') ++i;
            continue;
        }
        if (sql.mid(i, 2) == "/*") {
            int depth = 1; i += 2;
            while (i < sql.size() && depth) {
                if (sql.mid(i, 2) == "/*") { ++depth; i += 2; }
                else if (sql.mid(i, 2) == "*/") { --depth; i += 2; }
                else ++i;
            }
            if (depth) return false;
            continue;
        }
        if (ended) return false; // No SQL batches, even SELECT followed by DML.
        if (c == ';') { ended = true; ++i; continue; }
        if (c == '\'' || c == '"' || c == '[') {
            const QChar closing = c == '[' ? QChar(']') : c;
            bool closed = false; ++i;
            while (i < sql.size()) {
                if (sql[i++] != closing) continue;
                if (i < sql.size() && sql[i] == closing) { ++i; continue; }
                closed = true; break;
            }
            if (!closed) return false;
            continue;
        }
        if (c.isLetter() || c == '_' || c == '@') {
            const int start = i++;
            while (i < sql.size() && (sql[i].isLetterOrNumber() || sql[i] == '_' || sql[i] == '@')) ++i;
            words << sql.mid(start, i - start).toUpper();
        } else ++i;
    }
    if (words.isEmpty() || words.first() != "SELECT") return false;
    const QStringList forbidden = QStringList()
        << "INSERT" << "UPDATE" << "DELETE" << "MERGE" << "INTO" << "CREATE"
        << "ALTER" << "DROP" << "TRUNCATE" << "EXEC" << "EXECUTE" << "GRANT"
        << "DENY" << "REVOKE" << "DBCC" << "BACKUP" << "RESTORE" << "BULK"
        << "OPENROWSET" << "OPENQUERY" << "OPENDATASOURCE" << "NEXT"
        << "UPDLOCK" << "XLOCK" << "TABLOCKX" << "HOLDLOCK";
    foreach (const QString &word, words) if (forbidden.contains(word)) return false;
    return true;
}

class BasketReadOnlyResult : public QSqlResult
{
public:
    BasketReadOnlyResult(const QSqlDriver *driver, QSqlDatabase backend)
        : QSqlResult(driver), backend(backend), query(backend) {}
protected:
    bool reset(const QString &sql) override {
        setActive(false); setSelect(false); setAt(QSql::BeforeFirstRow);
        if (!basketReadOnlyStatement(sql)) {
            setLastError(QSqlError("Read-only environment", "Only a single SELECT statement is permitted", QSqlError::StatementError));
            return false;
        }
        query = QSqlQuery(backend);
        query.setForwardOnly(isForwardOnly());
        query.setNumericalPrecisionPolicy(numericalPrecisionPolicy());
        const bool ok = query.exec(sql);
        setLastError(query.lastError());
        setSelect(query.isSelect()); setActive(ok);
        return ok;
    }
    QVariant data(int i) override { return query.value(i); }
    bool isNull(int i) override { return query.isNull(i); }
    bool fetch(int i) override { return position(query.seek(i)); }
    bool fetchNext() override { return position(query.next()); }
    bool fetchPrevious() override { return position(query.previous()); }
    bool fetchFirst() override { return position(query.first()); }
    bool fetchLast() override { return position(query.last()); }
    int size() override { return query.size(); }
    int numRowsAffected() override { return query.numRowsAffected(); }
    QSqlRecord record() const override { return query.record(); }
    void detachFromResultSet() override { query.finish(); }
private:
    bool position(bool ok) { setAt(query.at()); setLastError(query.lastError()); return ok; }
    QSqlDatabase backend;
    QSqlQuery query;
};

class BasketReadOnlyDriver : public QSqlDriver
{
public:
    explicit BasketReadOnlyDriver(QSqlDatabase backend) : backend(backend) {
        setOpen(backend.isOpen()); setOpenError(backend.isOpenError());
    }
    bool hasFeature(DriverFeature f) const override {
        // Qt expands prepared bindings using our formatValue; reset is checked
        // again for every execution. Do not expose native handles or transactions.
        return (f == QuerySize || f == BLOB || f == Unicode || f == LowPrecisionNumbers)
            && backend.driver()->hasFeature(f);
    }
    QSqlResult *createResult() const override { return new BasketReadOnlyResult(this, backend); }
    bool open(const QString &, const QString &, const QString &, const QString &, int, const QString &) override {
        setOpen(backend.isOpen()); return backend.isOpen();
    }
    void close() override { backend.close(); setOpen(false); }
    QString formatValue(const QSqlField &field, bool trim = false) const override {
        return backend.driver()->formatValue(field, trim);
    }
    QString escapeIdentifier(const QString &name, IdentifierType type) const override {
        return backend.driver()->escapeIdentifier(name, type);
    }
    QSqlRecord record(const QString &name) const override { return backend.record(name); }
    QSqlIndex primaryIndex(const QString &name) const override { return backend.primaryIndex(name); }
    QStringList tables(QSql::TableType type) const override { return backend.tables(type); }
private:
    QSqlDatabase backend;
};
#endif
