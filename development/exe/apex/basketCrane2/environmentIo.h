#ifndef BASKET_ENVIRONMENT_IO_H
#define BASKET_ENVIRONMENT_IO_H
#include "fatalErrorHooks.h"
#include "config.h"
#include "sqlTransaction.h"
#include "readOnlySql.h"
#include "nativeSqlServer.h"
#include "utilities.h"
#include "emailAlerts.h"

inline basket::SqlTransactionResult basketExecTransaction(
    const QVector<basket::SqlCommand>& commands, const QString& connection)
{
    const basket::SqlTransactionResult result = basket::SqlTransactionExecutor(basketCraneConfig(), connection).Execute(commands);
    if (!result.succeeded)
        uExit(errorStr, QString("SQL transaction failed after %1 attempt(s): %2").arg(result.attempts).arg(result.error.text()));
    return result;
}

inline basket::SqlTransactionResult basketExecBulk(const QString& statement,
    const QVector<QVector<QVariant>>& rows, const QString& connection)
{
    return basketExecTransaction(QVector<basket::SqlCommand>() << basket::SqlCommand::Bulk(statement, rows), connection);
}

inline QVector<QVariant> basketExecQuery(const QString& statement, const QString& connection)
{
    // Preserve the legacy SELECT result path and read-only driver protections.
    // Application mutations use the transaction/deadlock runner instead.
    if (basketReadOnlyStatement(statement)) return execQuery(statement, connection);
    return basketExecTransaction(QVector<basket::SqlCommand>() << basket::SqlCommand(statement), connection).values;
}
inline QSqlDatabase basketOpenDatabase(const QString &section, const QString &connection)
{
    const BasketCraneConfig &config = basketCraneConfig();
    const bool readOnly = !config.databaseWritesAllowed();
    const QString backendName = readOnly ? "basket_read_only_backend_" + connection : connection;
    QSqlDatabase backend = QSqlDatabase::addDatabase(new basket::NativeSqlServerDriver(
        basket::sqlServerConnectionString(config,section),config.text(section+"/Password")), backendName);
    backend.setDatabaseName(config.text(section + "/Catalog"));
    if (!backend.open())
        uExit(errorStr, QString("Cannot open %1 database. Error:%2").arg(backend.databaseName()).arg(backend.lastError().text()));
    if (!readOnly) return backend;
    QSqlDatabase exposed = QSqlDatabase::addDatabase(new BasketReadOnlyDriver(backend), connection);
    exposed.setDatabaseName(config.text(section + "/Catalog"));
    return exposed;
}
#endif
