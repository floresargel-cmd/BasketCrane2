#pragma once
#include <QDateTime>
#include <QVariant>
#include <QListWidget>
#include <QHash>
#include <QSet>
#include <QRegularExpression>
#include <QSqlQuery>
#include <cmath>

inline QString controllerEventTime(const QVariant& timestamp) {
    return timestamp.type()==QVariant::DateTime
        ? timestamp.toDateTime().toString("yyyy-MM-dd HH:mm:ss") : timestamp.toString().trimmed();
}

// Legacy controllers record positions as 1110.1 in c2logPos and PLC IDs as
// 1111 in c2logGui. Match both endpoints and the bounded recorded timestamp window.
inline int controllerAuditPlcId(const QVariant& position) {
    bool valid=false;
    const double value=position.toString().trimmed().toDouble(&valid);
    if (!valid || !std::isfinite(value) || value<0 || value>1000000) return -1;
    const int number=static_cast<int>(std::floor(value));
    return number+qRound((value-number)*10);
}

inline QRegularExpression legacyBasketMovePattern() {
    return QRegularExpression("^Move basket from ([0-9]+) to ([0-9]+)$");
}

inline QString controllerMovementKey(const QString& time,int from,int to) {
    return time+"|"+QString::number(from)+"|"+QString::number(to);
}

inline QDateTime controllerMovementTime(const QVariant& timestamp) {
    if (timestamp.type()==QVariant::DateTime) return timestamp.toDateTime();
    const QString text=timestamp.toString().trimmed();
    const QStringList formats=QStringList()<<"yyyy/MM/dd HH:mm:ss"<<"yyyy-MM-dd HH:mm:ss";
    for (const auto& format : formats) {
        const auto time=QDateTime::fromString(text,format);
        if (time.isValid()) return time;
    }
    return QDateTime();
}

// The legacy controller writes its event before the position audit. Live
// records show 1-3 seconds between them; allow up to five seconds after it.
inline void recoverControllerBasketNumbers(QVector<QVector<QVariant>>& events,
                                           const QVector<QVector<QVariant>>& audits) {
    struct MovementAudit { QDateTime time; int basket; };
    QHash<QString,QVector<MovementAudit>> movements;
    for (const auto& audit : audits) {
        if (audit.size()<4 || audit[1].toInt()<=0) continue;
        const QString info=audit[2].toString().trimmed();
        if (!info.startsWith("from ")) continue;
        const int from=controllerAuditPlcId(info.mid(5));
        const int to=controllerAuditPlcId(audit[0]);
        const auto time=controllerMovementTime(audit[3]);
        if (from<0 || to<0 || !time.isValid()) continue;
        movements[controllerMovementKey(QString(),from,to)].append(MovementAudit{time,audit[1].toInt()});
    }
    const auto pattern=legacyBasketMovePattern();
    for (auto& event : events) {
        if (event.size()<4) continue;
        const auto match=pattern.match(event[0].toString().trimmed());
        const auto time=controllerMovementTime(event[3]);
        if (!match.hasMatch() || !time.isValid()) continue;
        QSet<int> found;
        const auto candidates=movements.value(controllerMovementKey(QString(),match.captured(1).toInt(),match.captured(2).toInt()));
        for (const auto& audit : candidates) {
            const qint64 delay=time.msecsTo(audit.time);
            if (delay>=0 && delay<=5000) found.insert(audit.basket);
        }
        // Ambiguous or unavailable audit data must never identify the wrong basket.
        if (found.size()!=1) continue;
        const int basket=*found.constBegin();
        event[0]=QString("Move basket #%1 from %2 to %3").arg(basket).arg(match.captured(1)).arg(match.captured(2));
        event[1]=event[1].toString().trimmed()+QString("\nBasket #%1 recovered from the matching position audit (within 5 seconds after the controller event).").arg(basket);
    }
}

inline QVector<QVector<QVariant>> readCraneControllerHistory(int limit) {
    auto events=execTableQuery(QString("select top %1 mess,details,type,time from c2logGui where messenger='crane' order by tid desc")
        .arg(limit==300 ? 300 : 100),bDb);
    QStringList times;
    const auto pattern=legacyBasketMovePattern();
    for (const auto& event : events) {
        if (event.size()>=4 && pattern.match(event[0].toString().trimmed()).hasMatch()
            && controllerMovementTime(event[3]).isValid()) {
            const QString time=controllerEventTime(event[3]);
            if (!times.contains(time)) times.append(time);
        }
    }
    if (times.isEmpty()) return events;
    QStringList ranges;
    for (int i=0;i<times.size();++i) ranges.append("(time>=? and time<=?)");
    QSqlQuery query(QSqlDatabase::database(bDb));
    if (!query.prepare(QString("select pos,basket,info,time from c2logPos where basket>0 and info like 'from %' and (%1)")
        .arg(ranges.join(" or ")))) return events;
    for (const auto& time : times) {
        query.addBindValue(time);
        const QString format=time.contains('/') ? "yyyy/MM/dd HH:mm:ss" : "yyyy-MM-dd HH:mm:ss";
        query.addBindValue(controllerMovementTime(time).addSecs(5).toString(format));
    }
    if (!query.exec()) { qWarning("Position audit lookup failed: %s",qPrintable(query.lastError().text())); return events; }
    QVector<QVector<QVariant>> audits;
    while (query.next()) audits.append(QVector<QVariant>() << query.value(0) << query.value(1) << query.value(2) << query.value(3));
    if (query.lastError().isValid()) return events;
    recoverControllerBasketNumbers(events,audits);
    return events;
}
// Preserve the database's time zone; do not convert controller events to local time.
inline QString controllerHistoryText(const QString& message, const QVariant& timestamp) {
    const QString time = timestamp.type()==QVariant::DateTime
        ? timestamp.toDateTime().toString("yyyy-MM-dd HH:mm:ss") : timestamp.toString().trimmed();
    return QString("[%1] %2").arg(time.isEmpty() ? QString("Time unavailable") : time).arg(message.trimmed());
}

// The compact crane panel shows the recorded event time alongside each message.
inline void loadCraneMessageSnapshot(QListWidget *logGuiList, const QVector<QVector<QVariant>>& dataVV, const QString& errorType, bool& loaded) {
    if (loaded) return;
    loaded = true;
	logGuiList->clear();
	for (int i=0;i<dataVV.count();i++)
	{
		QListWidgetItem *item=new QListWidgetItem(controllerHistoryText(dataVV[i][0].toString(),dataVV[i][3]));
		item->setToolTip(QString("%1\n%2\n%3").arg(dataVV[i][2].toString().trimmed()).arg(dataVV[i][1].toString().trimmed()).arg(dataVV[i][3].toString().trimmed()));
		if (dataVV[i][2].toString().trimmed()==errorType)
			item->setForeground(QColor("#f87171"));
		else
			item->setForeground(QColor("#7dd3fc"));
		logGuiList->addItem(item);
	}
}
