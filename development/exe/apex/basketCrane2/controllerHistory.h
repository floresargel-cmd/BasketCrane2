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
// 1111 in c2logGui. Match both endpoints and the exact recorded timestamp.
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

inline void recoverControllerBasketNumbers(QVector<QVector<QVariant>>& events,
                                           const QVector<QVector<QVariant>>& audits) {
    QHash<QString,QSet<int>> baskets;
    for (const auto& audit : audits) {
        if (audit.size()<4 || audit[1].toInt()<=0) continue;
        const QString info=audit[2].toString().trimmed();
        if (!info.startsWith("from ")) continue;
        const int from=controllerAuditPlcId(info.mid(5));
        const int to=controllerAuditPlcId(audit[0]);
        if (from<0 || to<0) continue;
        baskets[controllerMovementKey(controllerEventTime(audit[3]),from,to)].insert(audit[1].toInt());
    }
    const auto pattern=legacyBasketMovePattern();
    for (auto& event : events) {
        if (event.size()<4) continue;
        const auto match=pattern.match(event[0].toString().trimmed());
        if (!match.hasMatch()) continue;
        const auto found=baskets.value(controllerMovementKey(controllerEventTime(event[3]),
            match.captured(1).toInt(),match.captured(2).toInt()));
        // Ambiguous or unavailable audit data must never identify the wrong basket.
        if (found.size()!=1) continue;
        const int basket=*found.constBegin();
        event[0]=QString("Move basket #%1 from %2 to %3").arg(basket).arg(match.captured(1)).arg(match.captured(2));
        event[1]=event[1].toString().trimmed()+QString("\nBasket #%1 recovered from the matching position audit.").arg(basket);
    }
}

inline QVector<QVector<QVariant>> readCraneControllerHistory(int limit) {
    auto events=execTableQuery(QString("select top %1 mess,details,type,time from c2logGui where messenger='crane' order by tid desc")
        .arg(limit==300 ? 300 : 100),bDb);
    QStringList times;
    const auto pattern=legacyBasketMovePattern();
    for (const auto& event : events) {
        if (event.size()>=4 && pattern.match(event[0].toString().trimmed()).hasMatch()) {
            const QString time=controllerEventTime(event[3]);
            if (!time.isEmpty() && !times.contains(time)) times.append(time);
        }
    }
    if (times.isEmpty()) return events;
    QStringList placeholders;
    for (int i=0;i<times.size();++i) placeholders.append("?");
    QSqlQuery query(QSqlDatabase::database(bDb));
    if (!query.prepare(QString("select pos,basket,info,time from c2logPos where basket>0 and info like 'from %' and time in (%1)")
        .arg(placeholders.join(",")))) return events;
    for (const auto& time : times) query.addBindValue(time);
    if (!query.exec()) return events;
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
