#ifndef BASKET_CRANE_CONFIG_H
#define BASKET_CRANE_CONFIG_H

#include <QCoreApplication>
#include <QFileInfo>
#include <QHostAddress>
#include <QMap>
#include <QSettings>
#include <QVariant>
#include <QStringList>
#include "emailAlertSettings.h"

// Loaded once, before constructing any widgets or opening plant connections.
class BasketCraneConfig
{
public:
    bool load(const QString &path, QString &error)
    {
        loaded = false; values.clear(); emailSettings=basket::EmailAlertSettings();
        error.clear();
        if (!QFileInfo(path).isFile() || !QFileInfo(path).isReadable()) {
            error = QString("Cannot read configuration file: %1").arg(path);
            return false;
        }
        QSettings ini(path, QSettings::IniFormat);
        ini.setIniCodec("UTF-8");
        ini.setFallbacksEnabled(false);
        const QStringList strings = QStringList()
            << "Environment/Mode" << "Plc/Crane2Ip" << "Plc/OldOvenIp" << "Plc/NewOvenIp"
            << "BasketDatabase/Server" << "BasketDatabase/User" << "BasketDatabase/Password" << "BasketDatabase/Catalog" << "BasketDatabase/Encrypt" << "BasketDatabase/TrustServerCertificate"
            << "EpicsDatabase/Server" << "EpicsDatabase/User" << "EpicsDatabase/Password" << "EpicsDatabase/Catalog" << "EpicsDatabase/Encrypt" << "EpicsDatabase/TrustServerCertificate"
            << "Epics2Database/Server" << "Epics2Database/User" << "Epics2Database/Password" << "Epics2Database/Catalog" << "Epics2Database/Encrypt" << "Epics2Database/TrustServerCertificate"
            << "Access/WeakPassword" << "Access/StrongPassword";
        const QStringList integers = QStringList()
            << "Plc/PollIntervalMs" << "Plc/HandshakeTimeoutMs"
            << "Timers/MainMs" << "Timers/PositionsMs" << "Timers/ConveyorsMs"
            << "Timers/CarriageStatusMs" << "Timers/MissionsMs" << "Timers/ExportQueueMs"
            << "Logging/RetainedRows";

        const QStringList keys = strings + integers;
        foreach (const QString &key, keys) {
            if (!ini.contains(key)) {
                error = QString("Missing configuration key: %1 (%2)").arg(key, path);
                return false;
            }
            const QVariant raw=ini.value(key);
            // Qt INI treats an unquoted comma as a list separator. SQL Server
            // endpoints conventionally use host,port, so reconstruct that value.
            const QString value = key.endsWith("/Server") && raw.type()==QVariant::StringList
                ? raw.toStringList().join(',') : raw.toString();
            if (strings.contains(key)) {
                if (value.trimmed().isEmpty()) {
                    error = QString("Configuration key must not be empty: %1").arg(key);
                    return false;
                }
                if (key.endsWith("Ip")) {
                    QHostAddress address;
                    if (!address.setAddress(value)) {
                        error = QString("Invalid PLC IP address in: %1").arg(key);
                        return false;
                    }
                }
                values.insert(key, value);
            } else if (integers.contains(key)) {
                bool ok = false;
                const int number = value.toInt(&ok);
                if (!ok || number <= 0) {
                    error = QString("Configuration key must be a positive integer: %1").arg(key);
                    return false;
                }
                values.insert(key, number);
            }
        }
        // Keep direct connection settings explicit and reject legacy DSN keys.
        foreach (const QString &section, QStringList() << "BasketDatabase" << "EpicsDatabase" << "Epics2Database") {
            const QString server=text(section+"/Server");
            if (server.contains(';') || server.contains('"') || server.contains('\n') || server.contains('\r') || server.startsWith("tcp:",Qt::CaseInsensitive)) {
                error=QString("Invalid SQL Server endpoint in: %1/Server (use host,port or host\\instance)").arg(section); return false;
            }
            if (text(section+"/Encrypt")!="Mandatory" && text(section+"/Encrypt")!="Strict" && text(section+"/Encrypt")!="Optional") {
                error=QString("%1/Encrypt must be Mandatory, Strict, or Optional").arg(section); return false;
            }
            if (text(section+"/TrustServerCertificate")!="true" && text(section+"/TrustServerCertificate")!="false") {
                error=QString("%1/TrustServerCertificate must be true or false").arg(section); return false;
            }
            const QString catalog = text(section + "/Catalog");
            foreach (const QChar character, catalog) {
                if (!character.isLetterOrNumber() && character != '_') {
                    error = QString("Invalid database catalog identifier in: %1/Catalog").arg(section);
                    return false;
                }
            }
        }
        foreach (const QString &key, ini.allKeys()) {
            if (!keys.contains(key) && !basket::EmailAlertSettings::Keys().contains(key)) {
                error = QString("Unknown configuration key: %1").arg(key);
                return false;
            }
        }
        if (ini.status() != QSettings::NoError) {
            error = QString("Invalid INI format or read error: %1").arg(path);
            return false;
        }
        const QString mode = text("Environment/Mode");
        if (mode != "Test" && mode != "Live" && mode != "LiveObserver") {
            error = "Environment/Mode must be Test, Live, or LiveObserver";
            return false;
        }
        emailSettings.Load(ini);
        loaded = true;
        return true;
    }

    const basket::EmailAlertSettings& EmailSettings() const { return emailSettings; }
    QString text(const QString &key) const { return values.value(key).toString(); }
    int number(const QString &key) const { return values.value(key).toInt(); }
    QString environment() const { return text("Environment/Mode"); }
    bool liveControlAllowed() const { return loaded && environment() == "Live"; }
    bool databaseWritesAllowed() const { return liveControlAllowed(); }
    bool observer() const { return environment() == "LiveObserver"; }
    bool plcOffline() const { return !liveControlAllowed(); }
    QString plcEndpoint(const QString &key) const {
        if (liveControlAllowed()) return text(key);
        // Even accidental network use by a library cannot target the live PLC.
        if (key == "Plc/Crane2Ip") return "127.0.0.60";
        if (key == "Plc/OldOvenIp") return "127.0.0.40";
        return "127.0.0.135";
    }

private:
    basket::EmailAlertSettings emailSettings;
    bool loaded = false;
    QMap<QString, QVariant> values;
};

inline BasketCraneConfig &basketCraneConfig()
{
    static BasketCraneConfig config;
    return config;
}
#endif
