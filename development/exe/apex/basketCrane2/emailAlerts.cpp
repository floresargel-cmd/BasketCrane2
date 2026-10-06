#include "emailAlerts.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutexLocker>
#include <QProcess>
#include <QTextCodec>
#include <QSysInfo>
#include <cstdlib>
#include <cstdio>

namespace {
QMutex sinkMutex;
EmailAlerts* sink = 0;
const int maxBody = 16384;

// Qt in this application is built without SSL. Windows PowerShell/.NET uses
// Windows TLS and certificate validation instead. Only fixed code is executed;
// credentials and message content travel as JSON on stdin, never command args.
const char mailScript[] = R"PS(
$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'
$client = $null
$mail = $null
try {
    [Console]::InputEncoding = New-Object System.Text.UTF8Encoding($false)
    $data = [Console]::In.ReadLine() | ConvertFrom-Json
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
    $mail = New-Object Net.Mail.MailMessage
    $mail.From = New-Object Net.Mail.MailAddress([string]$data.from)
    foreach ($address in ([string]$data.to -split ';')) { $mail.To.Add($address.Trim()) }
    $mail.Subject = [string]$data.subject
    $mail.SubjectEncoding = [Text.Encoding]::UTF8
    $mail.BodyEncoding = [Text.Encoding]::UTF8
    $mail.Body = [string]$data.body
    $client = New-Object Net.Mail.SmtpClient([string]$data.host, [int]$data.port)
    $client.UseDefaultCredentials = $false
    $client.EnableSsl = [bool]$data.startTls
    $client.Timeout = [int]$data.timeoutMs
    if ($data.username) {
        $client.Credentials = New-Object Net.NetworkCredential([string]$data.username, [string]$data.password)
    }
    $client.Send($mail)
} catch {
    # Do not echo exceptions: server responses can contain sensitive data.
    exit 1
} finally {
    if ($client) { $client.Dispose() }
    if ($mail) { $mail.Dispose() }
}
)PS";
}

EmailAlerts::EmailAlerts(const basket::EmailAlertSettings& settings)
    : settings_(settings), count_(0), stopping_(false)
{
    if (!settings_.Enabled())
        return;
    if (!settings_.ValidationError().isEmpty()) {
        std::fprintf(stderr,"%s\n",settings_.ValidationError().toLocal8Bit().constData());
        return;
    }
    {
        QMutexLocker lock(&sinkMutex);
        sink = this;
    }
    std::atexit(&EmailAlerts::StopActive);
    start();

}

EmailAlerts::~EmailAlerts()
{
    stop();
}

void EmailAlerts::stop()
{
    {
        QMutexLocker lock(&sinkMutex);
        if (sink == this) {

            sink = 0;
        }
    }
    {
        QMutexLocker lock(&mutex_);
        stopping_ = true;
        wake_.wakeAll();
    }
    // Worker gives the last batch at most five seconds during normal shutdown.
    wait();
}

void EmailAlerts::Notify(const QString& title,const QString& message)
{
    QMutexLocker sinkLock(&sinkMutex);
    if (!sink)
        return;
    QMutexLocker lock(&sink->mutex_);
    if (sink->stopping_)
        return;
    ++sink->count_;
    if (sink->pending_.size() < maxBody) {
        const QString entry = QDateTime::currentDateTime().toString(Qt::ISODate)
            + " | " + title.left(512) + "\n" + message.left(maxBody)
            + "\n\n";
        sink->pending_ += entry.left(maxBody - sink->pending_.size());
    }
    sink->wake_.wakeAll();
}

void EmailAlerts::run()
{
    for (;;) {
        QString batch;
        quint64 count;
        bool finalBatch;
        {
            QMutexLocker lock(&mutex_);
            while (!stopping_) {
                const qint64 remaining = lastAttempt_.isValid()
                    ? settings_.CooldownSeconds() * 1000LL - lastAttempt_.elapsed() : 0;
                if (count_ && remaining <= 0)
                    break;
                if (!count_)
                    wake_.wait(&mutex_);
                else
                    wake_.wait(&mutex_, static_cast<unsigned long>(remaining));
            }
            if (!count_)
                return;
            batch.swap(pending_);
            count = count_;
            count_ = 0;
            finalBatch = stopping_;
            lastAttempt_.start();
        }
        const QString body = QString("Basket Crane 2 application error\nHost: %1\nVersion: %2\nError events in this batch: %3\n"
            "Details are limited to 16384 characters; see the application log for all events.\n\n%4")
            .arg(QSysInfo::machineHostName()).arg(QCoreApplication::applicationVersion()).arg(count).arg(batch);
        const bool delivered = send(body);
        std::fprintf(stderr,"Email alert: %s\n",delivered ? "SMTP server accepted error alert" : "Delivery failed or timed out; batch retained for retry while running.");
        if (!delivered) {
            QMutexLocker lock(&mutex_);
            if (!stopping_) {
                pending_ = (batch + pending_).left(maxBody);
                count_ += count;
            }
        }
        if (finalBatch)
            return;
    }
}

bool EmailAlerts::send(const QString& body)
{
    QJsonObject data;
    data["host"] = settings_.SmtpHost();
    data["port"] = settings_.SmtpPort();
    data["startTls"] = settings_.StartTls();
    data["username"] = settings_.Username();
    data["password"] = settings_.Password();
    data["from"] = settings_.From();
    data["to"] = settings_.To();
    data["timeoutMs"] = settings_.TimeoutSeconds() * 1000;
    data["body"] = body;
    data["subject"] = settings_.Subject();
    QProcess process;
    const QString powershell = QString::fromLocal8Bit(qgetenv("SystemRoot"))
        + "/System32/WindowsPowerShell/v1.0/powershell.exe";
    const QString script = QString::fromLatin1(mailScript);
    QTextCodec::ConverterState conversion(QTextCodec::IgnoreHeader);
    const QByteArray encoded = QTextCodec::codecForName("UTF-16LE")
        ->fromUnicode(script.constData(), script.size(), &conversion).toBase64();
    process.start(powershell, QStringList() << "-NoLogo" << "-NoProfile" << "-NonInteractive"
        << "-WindowStyle" << "Hidden" << "-EncodedCommand" << QString::fromLatin1(encoded));
    if (!process.waitForStarted(2000)) {
        return false;
    }
    process.write(QJsonDocument(data).toJson(QJsonDocument::Compact) + '\n');
    // ReadLine uses the newline as framing. Do not close stdin here: Qt 5.7's
    // Windows closeWriteChannel flushes with an infinite wait if a child stalls.
    QElapsedTimer elapsed;
    QElapsedTimer shutdown;
    elapsed.start();
    while (!process.waitForFinished(100)) {
        {
            QMutexLocker lock(&mutex_);
            if (stopping_ && !shutdown.isValid())
                shutdown.start();
        }
        if (elapsed.elapsed() >= (settings_.TimeoutSeconds() + 5) * 1000LL
            || (shutdown.isValid() && shutdown.elapsed() >= 5000)) {
            process.kill();
            process.waitForFinished(1000);

            return false;
        }
    }

    return process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0;
}

void EmailAlerts::StopActive() {
    EmailAlerts* active=nullptr;
    { QMutexLocker lock(&sinkMutex); active=sink; }
    if (active) active->stop();
}
