#pragma once

#include "emailAlertSettings.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>


// A bounded queue and a separate worker keep SMTP off the crane/UI threads.
class EmailAlerts : public QThread
{
public:
    explicit EmailAlerts(const basket::EmailAlertSettings& settings);
    ~EmailAlerts();
    void stop();
    static void Notify(const QString& title,const QString& message);
    static void StopActive();

protected:
    void run() override;

private:

    bool send(const QString& body);
    basket::EmailAlertSettings settings_;
    QMutex mutex_;
    QWaitCondition wake_;
    QString pending_;
    quint64 count_;
    bool stopping_;
    QElapsedTimer lastAttempt_;
};
