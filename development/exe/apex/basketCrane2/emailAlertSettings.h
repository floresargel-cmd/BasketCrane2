#pragma once
#include <QSettings>
#include <QRegExp>
#include <QStringList>
namespace basket {
class EmailAlertSettings
{
public:
    EmailAlertSettings() : m_enabled(false), m_smtpPort(587), m_startTls(true),
        m_cooldownSeconds(3600), m_timeoutSeconds(20) {}
    bool Enabled() const { return m_enabled; }
    const QString& SmtpHost() const { return m_smtpHost; }
    int SmtpPort() const { return m_smtpPort; }
    bool StartTls() const { return m_startTls; }
    const QString& Username() const { return m_username; }
    const QString& Password() const { return m_password; }
    const QString& From() const { return m_from; }
    const QString& To() const { return m_to; }
    QString Subject() const { return m_subjectPrefix.isEmpty() ? "Basket Crane 2 Application Error" : m_subjectPrefix + "Application Error"; }
    int CooldownSeconds() const { return m_cooldownSeconds; }
    int TimeoutSeconds() const { return m_timeoutSeconds; }
    const QString& ValidationError() const { return m_validationError; }

public:
    void Load(QSettings& settings) {
        *this = EmailAlertSettings();
    const QString enabled = settings.value("EmailAlerts/EnableErrorEmailNotification", "false").toString().trimmed().toLower();
    m_enabled = enabled != "false";
    if (!m_enabled)
        return;
    m_smtpHost = ReadString(settings, "EmailAlerts/SmtpHost").trimmed();
    bool portOk, cooldownOk, timeoutOk;
    m_smtpPort = settings.value("EmailAlerts/SmtpPort", 587).toInt(&portOk);
    const QString tls = settings.value("EmailAlerts/SmtpEnableSsl", "true").toString().trimmed().toLower();
    m_startTls = tls == "true";
    m_username = ReadString(settings, "EmailAlerts/SmtpUsername");
    m_password = ReadString(settings, "EmailAlerts/SmtpPassword");
    m_from = ReadString(settings, "EmailAlerts/SmtpFrom").trimmed();
    m_to = ReadString(settings, "EmailAlerts/SmtpTo").trimmed();
    const int intervalMinutes = settings.value("EmailAlerts/ErrorEmailIntervalMinutes", 60).toInt(&cooldownOk);
    m_cooldownSeconds = (intervalMinutes >= 1 && intervalMinutes <= 1440) ? intervalMinutes * 60 : 0;
    m_subjectPrefix = ReadString(settings, "EmailAlerts/SmtpSubjectPrefix");
    m_timeoutSeconds = settings.value("EmailAlerts/SmtpTimeoutSeconds", 20).toInt(&timeoutOk);
    QStringList invalid;
    if (enabled != "true" && enabled != "false")
        invalid << "EnableErrorEmailNotification (true or false)";
    if (m_smtpHost.isEmpty() || m_smtpHost.contains(QRegExp("[\\s]")))
        invalid << "SmtpHost";
    if (!portOk || m_smtpPort < 1 || m_smtpPort > 65535 || m_smtpPort == 465)
        invalid << "SmtpPort (1-65535; implicit TLS/465 is unsupported)";
    if (tls != "true" && tls != "false")
        invalid << "SmtpEnableSsl (true or false)";
    if (!m_username.isEmpty() && (!m_startTls || m_password.isEmpty()))
        invalid << "SmtpUsername requires SmtpEnableSsl=true and SmtpPassword";
    const QRegExp address("[^\\s<>@;,]+@[^\\s<>@;,]+");
    if (!address.exactMatch(m_from))
        invalid << "SmtpFrom (one email address)";
    const QStringList recipients = m_to.split(';', QString::KeepEmptyParts);
    foreach (const QString& recipient, recipients) {
        if (!address.exactMatch(recipient.trimmed())) {
            invalid << "SmtpTo (email addresses separated by semicolons)";
            break;
        }
    }
    if (!cooldownOk || m_cooldownSeconds == 0)
        invalid << "ErrorEmailIntervalMinutes (1-1440)";
    if (m_subjectPrefix.contains('\r') || m_subjectPrefix.contains('\n'))
        invalid << "SmtpSubjectPrefix (one line)";
    if (!timeoutOk || m_timeoutSeconds < 1 || m_timeoutSeconds > 60)
        invalid << "SmtpTimeoutSeconds (1-60)";
    if (!invalid.isEmpty())
        m_validationError = "Email alerts disabled: invalid [EmailAlerts] settings: " + invalid.join(", ");
    }
    static QString ReadString(QSettings& settings,const QString& key) {
        const QVariant raw=settings.value(key);
        QString value=raw.type()==QVariant::StringList ? raw.toStringList().join(';') : raw.toString();
        const QRegExp token("\\$\\{ENV:([^}]+)\\}");
        int offset=0;
        while ((offset=token.indexIn(value,offset))>=0) {
            const QString replacement=QString::fromLocal8Bit(qgetenv(token.cap(1).toLocal8Bit().constData()));
            value.replace(offset,token.matchedLength(),replacement); offset+=replacement.size();
        }
        return value;
    }
    static QStringList Keys() {
        return QStringList() << "EmailAlerts/EnableErrorEmailNotification" << "EmailAlerts/ErrorEmailIntervalMinutes"
            << "EmailAlerts/SmtpHost" << "EmailAlerts/SmtpPort" << "EmailAlerts/SmtpEnableSsl"
            << "EmailAlerts/SmtpFrom" << "EmailAlerts/SmtpTo" << "EmailAlerts/SmtpSubjectPrefix"
            << "EmailAlerts/SmtpUsername" << "EmailAlerts/SmtpPassword" << "EmailAlerts/SmtpTimeoutSeconds";
    }
private:
    friend class ConfigurationManager;
    bool m_enabled;
    QString m_smtpHost;
    int m_smtpPort;
    bool m_startTls;
    QString m_username, m_password, m_from, m_to;
    QString m_subjectPrefix;
    int m_cooldownSeconds, m_timeoutSeconds;
    QString m_validationError;
};


}
