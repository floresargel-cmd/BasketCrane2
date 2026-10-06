#pragma once
#include <QCheckBox>
#include <QVBoxLayout>
#include <QVector>
#include <QVariant>
#include "destinationCatalog.h"
namespace basket {
struct AutomaticMissionOption { const char* label; int flag; const char* queue; };
inline const QVector<AutomaticMissionOption>& AutomaticMissionOptions() {
    static const QVector<AutomaticMissionOption> options = {
        {"Exports to Destacker B", 0x01, "c2exportsDestacker"},
        {"Exports to Destacker A1", 0x20, "c2exportsPackingDestacker1"},
        {"Exports to Destacker A2", 0x40, "c2exportsPackingDestacker2"},
        {"Exports to Anodizing", 0x02, "c2exportsPacking"},
        {"Imports from North", 0x04, ""},
        {"Imports from South", 0x08, ""},
        {"Epics", 0x80, ""}
    };
    return options;
}
inline QList<QCheckBox*> CreateAutomaticMissionControls(QWidget* parent) {
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setMargin(1);
    QList<QCheckBox*> controls;
    for (const auto& option : AutomaticMissionOptions()) {
        QCheckBox* control = new QCheckBox(QWidget::tr(option.label), parent);
        control->setProperty("automaticMissionFlag", option.flag);
        layout->addWidget(control);
        controls.append(control);
    }
    layout->addStretch(1);
    return controls;
}
inline int ExportPlcDestination(const QString& queue, int length) {
    if (queue == "c2exportsPackingDestacker1" || queue == "c2exportsPackingDestacker2")
        return DestinationCatalog::PlcValue(length <= 7660 ? DestinationId::PackingDestacker : DestinationId::PackingOverLength);
    if (queue == "c2exportsDestacker")
        return DestinationCatalog::PlcValue(length < 7821 ? DestinationId::Destacker : DestinationId::PackingOverLength);
    if (queue == "c2exportsPacking")
        return DestinationCatalog::PlcValue(length < 7821 ? DestinationId::Packing : DestinationId::PackingOverLength);
    return 0;
}
} // namespace basket
