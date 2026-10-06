#pragma once
#include <QDateTime>
#include <QVariant>
#include <QListWidget>

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
