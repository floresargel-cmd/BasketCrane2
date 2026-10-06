#pragma once

#include "../auditLogger.h"
#include "../destinationCatalog.h"
#include "../automaticMissions.h"
#include "../passwordAuthorization.h"
#include <cstdio>

class BasketRecordingExecutor final : public basket::ICommandExecutor {
public:
    bool Execute(const QString& statement, const QVector<QVariant>& values) override
    {
        statements.append(statement);
        bindings.append(values);
        return statements.size() != failAt;
    }

    bool ExecuteTransaction(const QVector<basket::SqlCommand>& commands) override
    {
        ++transactions;
        bool succeeded = true;
        for (const basket::SqlCommand& command : commands) {
            const bool executed = Execute(command.statement, command.values);
            succeeded = executed && succeeded;
        }
        return succeeded;
    }

    int transactions = 0;
    QVector<QString> statements;
    QVector<QVector<QVariant>> bindings;
    int failAt = -1;
};

inline bool basketArchitectureTests()
{
    const basket::PasswordAuthorization authorization("strong-test-password",
        [](const QString& password) { return password == "verified-account"; });
    if (authorization.Authorize("") || authorization.Authorize("wrong")
        || !authorization.Authorize("strong-test-password")
        || !authorization.Authorize("verified-account")
        || !authorization.Authorize("weak-test-password", "weak-test-password")
        || authorization.Authorize("strong-test-password", "weak-test-password")) return false;
    const basket::PasswordAuthorization unconfigured(QString(), {});
    if (unconfigured.Authorize("") || unconfigured.Authorize("anything")) return false;
    std::puts("PASS: password authorization rejects empty/incorrect credentials in every build configuration.");
    const auto& destinations = basket::DestinationCatalog::Entries();
    const QVector<int> expected = QVector<int>() << 0 << 1 << 9 << 6 << 7 << 11 << 12;
    if (destinations.size() != expected.size()) return false;
    for (int i = 0; i < expected.size(); ++i) {
        if (basket::DestinationCatalog::PlcValue(destinations[i].id) != expected[i]
            || destinations[i].title.isEmpty()) return false;
    }

    if (basket::ExportPlcDestination("c2exportsDestacker",7820)!=9
        || basket::ExportPlcDestination("c2exportsDestacker",7821)!=11
        || basket::ExportPlcDestination("c2exportsPacking",7820)!=1
        || basket::ExportPlcDestination("c2exportsPacking",7821)!=11
        || basket::ExportPlcDestination("c2exportsPackingDestacker1",7660)!=12
        || basket::ExportPlcDestination("c2exportsPackingDestacker1",7661)!=11
        || basket::ExportPlcDestination("c2exportsPackingDestacker2",7660)!=12
        || basket::ExportPlcDestination("c2exportsPackingDestacker2",7661)!=11
        || basket::ExportPlcDestination("unknown",1000)!=0) return false;
    std::puts("PASS: four export queues map to the production PLC routes and length limits.");

    BasketRecordingExecutor executor;
    basket::AuditLogger logger(executor, 12, 30);
    const QString text = "O'Brien; DELETE FROM positions";
    const QString timestamp = "2026.10.05 08:00:00";
    if (!logger.Operational(text, "information-too-long", timestamp)) return false;
    if (executor.transactions != 1 || executor.statements.size() != 2
        || executor.statements[0].contains(text)
        || executor.statements[1].contains(text)
        || executor.bindings[0][0].toString() != "information-"
        || executor.bindings[0][1].toString() != text
        || executor.bindings[1].size() != 3
        || executor.bindings[1][0] != executor.bindings[0][0]
        || executor.bindings[1][1] != executor.bindings[0][1]
        || executor.bindings[1][2] != executor.bindings[0][2]) return false;

    if (!logger.Gui("long message with apostrophe '", QString(40, 'x'),
        "long-sender-name", "long-type-name", timestamp)) return false;
    if (executor.transactions != 2 || executor.bindings[2].size() != 5
        || executor.bindings[2][0].toString().size() != 12
        || executor.bindings[2][1].toString().size() != 30
        || executor.bindings[2][2].toString() != "long-sende"
        || executor.bindings[2][3].toString() != "long-type-"
        || executor.bindings[2][4].toString() != timestamp) return false;

    executor.failAt = 5;
    if (logger.Operational("failed write", "error", timestamp)
        || executor.statements.size() != 6) return false;
    executor.failAt = 8;
    if (logger.Operational("failed verbose write", "error", timestamp)) return false;

    // A fail-closed configuration must suppress the write before even looking
    // up a connection; the offline tests never open ODBC or PLC connections.
    BasketCraneConfig unloaded;
    basket::SqlCommandExecutor guarded(unloaded, "architecture_nonexistent");
    if (!guarded.Execute("INSERT INTO positions VALUES (?)", QVector<QVariant>() << 42)
        || QSqlDatabase::contains("architecture_nonexistent")) return false;

    std::puts("PASS: immutable PLC destinations; parameterized audit values, limits, failures, and offline write policy.");
    return true;
}
