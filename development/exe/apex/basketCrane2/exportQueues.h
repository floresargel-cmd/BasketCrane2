#ifndef BASKET_CRANE_EXPORT_QUEUES_H
#define BASKET_CRANE_EXPORT_QUEUES_H
#include <QString>
#include <QVector>
struct BasketExportQueue { QString table; QString title; };
inline QVector<BasketExportQueue> basketExportQueues() {
    return QVector<BasketExportQueue>()
        << BasketExportQueue{"c2exportsDestacker","Destacker B"}
        << BasketExportQueue{"c2exportsPacking","Anodizing"}
        << BasketExportQueue{"c2exportsPackingDestacker1","Packing Destacker A1"}
        << BasketExportQueue{"c2exportsPackingDestacker2","Packing Destacker A2"};
}
#endif
