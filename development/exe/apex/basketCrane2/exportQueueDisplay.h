#pragma once
#include "exportQueues.h"
#include "environmentIo.h"
#include <QMap>

// Read once per position refresh, never while rendering or hovering.
inline QMap<int,QStringList> readBasketExportAssignments(const QString& connection) {
    QMap<int,QStringList> assignments;
    foreach (const BasketExportQueue& queue,basketExportQueues()) {
        const auto rows=execTableQuery(QString("select basket,priority from %1 order by priority asc").arg(queue.table),connection);
        foreach (const QVector<QVariant>& row,rows) {
            if (row.isEmpty() || row.first().toInt()<=0) continue;
            QStringList& tables=assignments[row.first().toInt()];
            if (!tables.contains(queue.table)) tables << queue.table;
        }
    }
    return assignments;
}
