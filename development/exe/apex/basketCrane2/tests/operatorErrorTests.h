#pragma once
#include "../fatalErrorHooks.h"
#include <cstdio>

inline bool basketOperatorErrorTests() {
    class FailedAction : public QObject {
    public:
        bool continued=false;
        bool event(QEvent* event) override {
            if (event->type()!=QEvent::User) return QObject::event(event);
            uExit("error","moveBasket from==1090.3 to==1110.1, fromBasket:52 toBasket:93");
            continued=true;
            return true;
        }
    } action;
    bool dialogSeen=false,operatorContext=false,technicalDetails=false;
    QTimer dismiss;
    QObject::connect(&dismiss,&QTimer::timeout,[&]() {
        foreach (QWidget* widget,QApplication::topLevelWidgets()) {
            QMessageBox* box=qobject_cast<QMessageBox*>(widget);
            if (!box || box->objectName()!="operatorErrorDialog") continue;
            dialogSeen=true;
            operatorContext=box->text().contains("Source: 1090.3") && box->text().contains("Basket at destination: 93");
            technicalDetails=box->detailedText().contains("fromBasket:52");
            box->accept();
        }
    });
    dismiss.start(10);
    QEvent event(QEvent::User);
    QApplication::sendEvent(&action,&event);
    dismiss.stop();
    if (action.continued || !dialogSeen || !operatorContext || !technicalDetails) return false;
    // Duplicate polling errors must still abort the operation, without reopening
    // the same dialog every timer tick.
    QApplication::sendEvent(&action,&event);
    if (action.continued) return false;
    if (!basket::operatorErrorExplanation("Cannot open crane database").contains("network")) return false;
    if (!basket::operatorErrorExplanation("SQL transaction failed after 3 attempts").contains("read or save")) return false;
    if (!basket::operatorErrorExplanation("Could not find layout.dxf file").contains("installation")) return false;
    std::puts("PASS: failed action stops; operator dialog provides positions, basket numbers and IT details; application survives; repeated alerts are coalesced.");
    return true;
}
