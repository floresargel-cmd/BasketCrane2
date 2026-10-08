#ifndef BASKET_CRANE_EXPORT_QUEUES_H
#define BASKET_CRANE_EXPORT_QUEUES_H
#include <QString>
#include <QVector>
#include <QStringList>
struct BasketExportQueue { QString table; QString title; QString code; };
inline QVector<BasketExportQueue> basketExportQueues() {
    return QVector<BasketExportQueue>()
        << BasketExportQueue{"c2exportsDestacker","Destacker B","B"}
        << BasketExportQueue{"c2exportsPacking","Anodizing","A"}
        << BasketExportQueue{"c2exportsPackingDestacker1","Packing Destacker A1","P1"}
        << BasketExportQueue{"c2exportsPackingDestacker2","Packing Destacker A2","P2"};
}
inline QString basketExportAssignmentText(const QStringList& tables, bool codes) {
    QStringList labels;
    foreach (const BasketExportQueue& queue,basketExportQueues())
        if (tables.contains(queue.table)) labels << (codes?queue.code:queue.title);
    return labels.join(codes?",":", ");
}
#endif
