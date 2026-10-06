#pragma once

#include "sqlTransaction.h"
#include <utility>

namespace basket {

class ICommandExecutor
{
public:
    virtual ~ICommandExecutor() = default;
    virtual bool Execute(const QString& statement, const QVector<QVariant>& values) = 0;
    virtual bool ExecuteTransaction(const QVector<SqlCommand>& commands) = 0;
};

// Construction never opens a connection. Every write owns a transaction.
class SqlCommandExecutor final : public ICommandExecutor
{
public:
    SqlCommandExecutor(const BasketCraneConfig& config, QString connection)
        : executor_(config, std::move(connection)) {}

    bool Execute(const QString& statement, const QVector<QVariant>& values) override
    {
        return ExecuteTransaction(QVector<SqlCommand>() << SqlCommand(statement, values));
    }

    bool ExecuteTransaction(const QVector<SqlCommand>& commands) override
    {
        lastResult_ = executor_.Execute(commands);
        lastError_ = lastResult_.error.text();
        return lastResult_.succeeded;
    }

    bool ExecuteBulk(const QString& statement, const QVector<QVector<QVariant>>& rows)
    {
        return ExecuteTransaction(QVector<SqlCommand>() << SqlCommand::Bulk(statement, rows));
    }

    const QString& LastError() const { return lastError_; }
    const SqlTransactionResult& LastResult() const { return lastResult_; }

private:
    SqlTransactionExecutor executor_;
    SqlTransactionResult lastResult_;
    QString lastError_;
};

class AuditLogger final
{
public:
    AuditLogger(ICommandExecutor& executor, int messageLimit, int descriptionLimit)
        : executor_(executor), messageLimit_(messageLimit), descriptionLimit_(descriptionLimit) {}

    bool Verbose(const QString& description, const QString& type, const QString& timestamp)
    {
        return executor_.Execute("insert into c2logV (type,description,time) values(?,?,?)",
            LogValues(description, type, timestamp));
    }

    bool Operational(const QString& description, const QString& type, const QString& timestamp)
    {
        const QVector<QVariant> values = LogValues(description, type, timestamp);
        return executor_.ExecuteTransaction(QVector<SqlCommand>()
            << SqlCommand("insert into c2log (type,description,time) values(?,?,?)", values)
            << SqlCommand("insert into c2logV (type,description,time) values(?,?,?)", values));
    }

    bool Gui(const QString& message, const QString& description, const QString& sender,
        const QString& type, const QString& timestamp)
    {
        return executor_.ExecuteTransaction(QVector<SqlCommand>()
            << SqlCommand("insert into c2logGui (mess,details,messenger,type,time) values(?,?,?,?,?)",
                QVector<QVariant>() << message.left(messageLimit_) << description.left(descriptionLimit_)
                    << sender.left(10) << type.left(10) << timestamp)
            << SqlCommand("insert into c2logV (type,description,time) values(?,?,?)",
                LogValues(QString("%1(%2) sender:%3").arg(message, description, sender), type, timestamp)));
    }

private:
    QVector<QVariant> LogValues(const QString& description, const QString& type, const QString& timestamp) const
    {
        return QVector<QVariant>() << type.left(messageLimit_) << description.left(descriptionLimit_) << timestamp;
    }

    ICommandExecutor& executor_;
    int messageLimit_;
    int descriptionLimit_;
};

} // namespace basket