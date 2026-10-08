#ifndef BASKET_CRANE_UI_PREVIEW_H
#define BASKET_CRANE_UI_PREVIEW_H
#include "../modernUi.h"
#include "../autoWdg.h"
#include "../automaticMissions.h"
#include "../gItem.h"
#include "../exportQueues.h"
#include "../expWdg.h"
#include "../plant3D.h"
#include "../carriageW.h"
#include "../controllerHistory.h"
#include <QSqlField>

// Read-only in-memory queue fixture. No ODBC/plant connection is constructed.
class BasketPreviewQueueResult : public QSqlResult {
public:
    BasketPreviewQueueResult(const QSqlDriver *driver, int *calls, QString *message):QSqlResult(driver), calls(calls), message(message) {}
protected:
    bool reset(const QString& sql) override {
        ++*calls; rows.clear(); logQuery=sql.contains("from c2logGui",Qt::CaseInsensitive);
        auditQuery=sql.contains("from c2logPos",Qt::CaseInsensitive);
        if (auditQuery && sql.startsWith("select ",Qt::CaseInsensitive)) { rows << 52; setSelect(true); setActive(true); setAt(-1); return true; }
        if (logQuery && sql.startsWith("select ",Qt::CaseInsensitive)) {
            rows << 5; setSelect(true); setActive(true); setAt(-1); return true;
        }
        if (!sql.startsWith("select basket,priority from ",Qt::CaseInsensitive)) return false;
        if (sql.contains("c2exportsDestacker ")) rows << 52;
        if (sql.contains("c2exportsPackingDestacker2 ")) rows << 8 << 93;
        setSelect(true); setActive(true); setAt(-1); return true;
    }
    QVariant data(int column) override {
        if (auditQuery) return (QVector<QVariant>()<<1300.0<<52<<"from 1110.1"<<"2026-10-05 12:00:00").value(column);
        if (logQuery) return (QStringList()<<*message<<"Controller event"<<"info"<<"2026-10-05 12:00:00").value(column);
        return column==0 ? rows.value(at()) : at()+1;
    }
    bool isNull(int) override { return false; }
    bool fetch(int i) override { if (i<0 || i>=rows.size()) { setAt(QSql::AfterLastRow); return false; } setAt(i); return true; }
    bool fetchFirst() override { return fetch(0); }
    bool fetchLast() override { return fetch(rows.size()-1); }
    int size() override { return rows.size(); }
    int numRowsAffected() override { return 0; }
    QSqlRecord record() const override {
        QSqlRecord r;
        if (auditQuery) { foreach (const QString& name,QStringList()<<"pos"<<"basket"<<"info"<<"time") r.append(QSqlField(name,QVariant::String)); }
        else if (logQuery) { foreach (const QString& name,QStringList()<<"mess"<<"details"<<"type"<<"time") r.append(QSqlField(name,QVariant::String)); }
        else { r.append(QSqlField("basket",QVariant::Int)); r.append(QSqlField("priority",QVariant::Int)); }
        return r;
    }
private:
    bool logQuery=false;
    bool auditQuery=false;
    QVector<int> rows;
    int *calls;
    QString *message;
};
class BasketPreviewQueueDriver : public QSqlDriver {
public:
    mutable int queryCalls = 0;
    mutable QString historyMessage = "Plc step:5";
    BasketPreviewQueueDriver() { setOpen(true); }
    bool hasFeature(DriverFeature) const override { return false; }
    QSqlResult *createResult() const override { return new BasketPreviewQueueResult(this, &queryCalls, &historyMessage); }
    bool open(const QString&,const QString&,const QString&,const QString&,int,const QString&) override { setOpen(true); return true; }
    void close() override { setOpen(false); }
};

// Offline visual fixture: shares the production workspace and theme, uses the
// real plant drawing, and never constructs DB/PLC/mission control objects.
inline bool basketUiPreview(const QString& file, const QString& mode) {
    {
        const QString time="2026-10-06 14:48:01";
        const auto movement=[&](const QString& message) { return QVector<QVariant>()<<message<<"Moving basket"<<"info"<<time; };
        QVector<QVector<QVariant>> events;
        events << movement("Move basket from 1111 to 1300") << movement("Move basket from 1300 to 702")
            << movement("Move basket #93 from 1111 to 1300") << movement("Plc step:6");
        QVector<QVector<QVariant>> audits;
        audits << (QVector<QVariant>()<<1300.0<<52<<"from 1110.1"<<time)
            << (QVector<QVariant>()<<700.2<<81<<"from 1300.0"<<"2026-10-06 14:48:02");
        recoverControllerBasketNumbers(events,audits);
        if (events[0][0].toString()!="Move basket #52 from 1111 to 1300"
            || events[1][0].toString()!="Move basket #81 from 1300 to 702"
            || events[2][0].toString()!="Move basket #93 from 1111 to 1300"
            || events[3][0].toString()!="Plc step:6") return false;
        events[0]=movement("Move basket from 1111 to 1300");
        audits << (QVector<QVariant>()<<1300.0<<93<<"from 1110.1"<<time);
        recoverControllerBasketNumbers(events,audits);
        if (events[0][0].toString()!="Move basket from 1111 to 1300") return false;
        QVector<QVector<QVariant>> delayed;
        delayed << (QVector<QVariant>()<<"Move basket from 1300 to 1111"<<"Moving basket"<<"info"<<"2026/10/06 15:00:18");
        QVector<QVector<QVariant>> actualAudit;
        actualAudit << (QVector<QVariant>()<<"1110.1    "<<110<<"from 1300.0    "<<"2026/10/06 15:00:20 ");
        recoverControllerBasketNumbers(delayed,actualAudit);
        if (delayed[0][0].toString()!="Move basket #110 from 1300 to 1111") return false;
        delayed[0][0]="Move basket from 1300 to 1111";
        actualAudit[0][3]="2026/10/06 15:00:17";
        recoverControllerBasketNumbers(delayed,actualAudit);
        if (delayed[0][0].toString()!="Move basket from 1300 to 1111") return false;
        actualAudit[0][3]="2026/10/06 15:00:24";
        recoverControllerBasketNumbers(delayed,actualAudit);
        if (delayed[0][0].toString()!="Move basket from 1300 to 1111") return false;
        std::puts("PASS: actual controller/audit timestamp delay recovers basket #110; earlier and out-of-window audits are excluded.");
        std::puts("PASS: legacy movement history recovers basket IDs from delayed, unambiguous position audit matches.");
    }
    BasketPreviewQueueDriver *fixtureDriver = new BasketPreviewQueueDriver;
    QSqlDatabase::addDatabase(fixtureDriver,bDb);
    fixtureDriver->historyMessage="Move basket from 1111 to 1300";
    const auto legacyHistory=readCraneControllerHistory(100);
    if (legacyHistory.size()!=1 || legacyHistory[0][0].toString()!="Move basket #52 from 1111 to 1300") return false;
    fixtureDriver->historyMessage="Plc step:5";
    std::puts("PASS: observer history loads matching position audits through a SELECT-only prepared query.");
    if (mode!="Live") {
        carriageWidgetClass mission;
        QLabel *step=mission.findChild<QLabel*>("activeCraneStep");
        QListWidget *history=mission.findChild<QListWidget*>();
        if (!step || step->text()!="Awaiting PLC status" || !history || history->count()!=1) return false;
        mission.setActiveFrom("To rotating south"); mission.setActiveTo("returning 4");
        mission.setActiveBasket(52);
        mission.setNextFrom("Export buffer"); mission.setNextTo("To rotating north"); mission.setNextBasket(93);
        QLabel *active=mission.findChild<QLabel*>("activeMission");
        QLabel *next=mission.findChild<QLabel*>("nextMission");
        if (!active || !next || !active->text().contains("Basket #52\n")
            || !next->text().contains("Basket #93\n") || !active->text().contains("returning 4")) return false;
        mission.setNextBasket(0); mission.setNextFrom(""); mission.setNextTo("");
        if (!next->text().trimmed().isEmpty() || !active->text().contains("Basket #52")) return false;
        if (step->text()!="Awaiting PLC status") return false;
        const int before=fixtureDriver->queryCalls;
        for (int i=0;i<100;++i) mission.setActiveCraneStep(0);
        if (step->text()!="Idle" || fixtureDriver->queryCalls!=before || history->count()!=1) return false;
        mission.setActiveCraneStep(5);
        mission.setActiveTo("returning 4");
        for (int i=0;i<100;++i) mission.setActiveCraneStep(5);
        if (step->text()!="Go to 0 Z" || fixtureDriver->queryCalls!=before || history->count()!=1) return false;
        mission.loadFromDataBase(); mission.loadFromDataBase();
        QLabel *historyTitle=mission.findChild<QLabel*>("controllerHistoryTitle");
        if (!historyTitle || historyTitle->text()!="Mission event history"
            || history->count()!=1 || history->item(0)->text()!="[2026-10-05 12:00:00] Plc step:5" || step->text()!="Go to 0 Z") return false;
        if (!active->text().contains("Basket #52")) return false;
        std::puts("PASS: mission basket numbers survive step transitions and next-mission clearing; observer polling stays read-only.");
        // The legacy upper snapshot must stay fixed while lower mission events advance.
        QListWidget upperList;
        bool snapshotLoaded=false;
        QListWidget *upper=&upperList;
        const auto refreshUpper=[&]() {
            loadCraneMessageSnapshot(upper,execTableQuery("select top 100 mess,details,type,time from c2logGui where messenger='crane' order by tid desc",bDb),errorStr,snapshotLoaded);
        };
        refreshUpper();
        if (!upper || upper->count()!=1 || upper->wordWrap()
            || upper->item(0)->text()!="[2026-10-05 12:00:00] Plc step:5") return false;
        fixtureDriver->historyMessage="Making next mission active";
        refreshUpper();
        if (upper->count()!=1 || upper->item(0)->text()!="[2026-10-05 12:00:00] Plc step:5"
            || !upper->item(0)->toolTip().contains("2026-10-05 12:00:00")) return false;
        mission.loadFromDataBase();
        if (history->count()!=1 || history->item(0)->text()!="[2026-10-05 12:00:00] Making next mission active") return false;
        QListWidgetItem *unchangedItem=upper->item(0);
        refreshUpper();
        if (upper->count()!=1 || upper->item(0)!=unchangedItem) return false;
        fixtureDriver->historyMessage="Plc step:5";
        std::puts("PASS: upper startup messages stay fixed while the lower mission history advances.");
    }
    // Exercise the actual linked HMI widget: first update, transitions and forced
    // logging must display telemetry without SQL when no log table is configured.
    {
        const int before = fixtureDriver->queryCalls;
        bitClass position("toPcI",40,4,bitClass::TYPE_CHANGE_COLOR,"Position X,Y 701-704.",
            "Inactive","","",bDb,"Positions",QString(),QString(),nullptr);
        position.updateValue("toPcI",40,0);
        if (position.getValue()) return false;
        position.updateValue("toPcI",40,16);
        if (!position.getValue()) return false;
        position.updateValue("toPcI",40,0);
        if (position.getValue()) return false;
        position.updateValue("toPcI",40,16,true);
        if (!position.getValue() || fixtureDriver->queryCalls != before) return false;
        std::puts("PASS: PLC bit transitions remain visible with database logging disabled.");
    }
    QMainWindow window;
    QToolBar *toolbar = new QToolBar; window.addToolBar(toolbar);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    foreach (const QString& label, QStringList() << "Send baskets to crane 1" << "24 feet" << "PLC comms" << "Usage" << "Conveyors" << "Destacker") toolbar->addAction(label);
    toolbar->addSeparator();
    foreach (const QString& label, QStringList() << "Missions mode" << "Exports mode" << "Modify mode") {
        QRadioButton *button = new QRadioButton(label); toolbar->addWidget(button); button->setChecked(label == "Missions mode");
    }
    toolbar->addWidget(new QPushButton("Basic HMI"));
    QWidget *operations = new QWidget; QVBoxLayout *operationsLayout = new QVBoxLayout(operations);
    QToolBox *toolbox = new QToolBox; operationsLayout->addWidget(toolbox);
    autoWidgetClass* automatic = new autoWidgetClass(nullptr);
    const QList<QCheckBox*> automaticControls = automatic->findChildren<QCheckBox*>();
    const auto& automaticOptions = basket::AutomaticMissionOptions();
    if (automaticControls.size() != 7) return false;
    for (int i=0; i<7; ++i) {
        if (automaticControls[i]->text()!=automaticOptions[i].label
            || automaticControls[i]->property("automaticMissionFlag").toInt()!=automaticOptions[i].flag
            || automaticControls[i]->isChecked()) return false;
    }
    toolbox->addItem(automatic,"Automatic missions");
    foreach (const BasketExportQueue& queue,basketExportQueues()) {
        exportStationWidgetClass *widget=new exportStationWidgetClass(queue.table,queue.title);
        const int index=toolbox->addItem(widget,queue.title); widget->attachToToolBox(toolbox,index);
        if (mode!="Live") foreach (QToolButton *button,widget->findChildren<QToolButton*>()) if (button->isEnabled()) return false;
    }
    if (toolbox->count()!=5 || toolbox->itemText(1)!="Destacker B (1 Basket)"
        || toolbox->itemText(2)!="Anodizing" || toolbox->itemText(3)!="Packing Destacker A1"
        || toolbox->itemText(4)!="Packing Destacker A2 (2 Baskets)") return false;
    toolbox->setCurrentIndex(0);
    QGraphicsView *map = new QGraphicsView; QGraphicsScene *scene = new QGraphicsScene(&window); map->setScene(scene);
    gItemClass *drawing = new gItemClass(":/dxfs/layout.dxf",QPen(QColor(200,200,200),0),false,false,QColor(),&window);
    scene->addItem(drawing);
    for (int i=0; i<12; ++i) {
        const int x=3000+(i%4)*1900, y=1500+(i/4)*2400;
        PlantBasketCard *card=new PlantBasketCard(QRectF(x,y,1700,1100)); scene->addItem(card);
        const QString destination=i%3==0?"HCA":(i%3==1?"HCB":"None");
        card->setBasket(20+i,destination,QStringList()<<QString("2109:%1").arg(25+i),i==11,i==0,i==1);
    }
    QWidget *crane = new QWidget; QVBoxLayout *craneLayout = new QVBoxLayout(crane); craneLayout->setMargin(0);
    QGroupBox *coordinates = new QGroupBox("Crane"); QVBoxLayout *coordinateLayout = new QVBoxLayout(coordinates);
    QGridLayout *grid = new QGridLayout; coordinateLayout->addLayout(grid);
    grid->addWidget(new QLabel("Axis"),0,0); grid->addWidget(new QLabel("Position [mm]"),0,1); grid->addWidget(new QLabel("Target [mm]"),0,2);
    QStringList axes=QStringList()<<"X"<<"Y"<<"Z", values=QStringList()<<"16308.4"<<"4955.16"<<"-0.804932";
    for (int i=0; i<3; ++i) { grid->addWidget(new QLabel(axes[i]),i+1,0); grid->addWidget(new QLabel(values[i]),i+1,1); grid->addWidget(new QLabel(i==0?"16315":(i==1?"4962":"0")),i+1,2); }
    QLabel *icon = new QLabel; icon->setAlignment(Qt::AlignCenter); icon->setPixmap(QPixmap(":/okBig.png").scaled(56,56,Qt::KeepAspectRatio,Qt::SmoothTransformation)); coordinateLayout->addWidget(icon);
    QListWidget *log = new QListWidget;
    foreach (const QString& message,QStringList()<<"Clear mission"<<"Making next mission active"<<"PLC step: 5"<<"Move basket from 1300 to 1111")
        log->addItem(controllerHistoryText(message,QDateTime(QDate(2026,10,5),QTime(12,0,0))));
    coordinateLayout->addWidget(log);
    coordinateLayout->addWidget(new QPushButton("HMI")); craneLayout->addWidget(coordinates,1);
    foreach (const QString& title, QStringList()<<"Active mission"<<"Next mission") { QGroupBox *mission = new QGroupBox(title); QVBoxLayout *layout=new QVBoxLayout(mission); layout->addWidget(new QLabel(title=="Active mission"?"Idle":"No pending mission")); craneLayout->addWidget(mission); }
    QSplitter *splitter = new QSplitter; window.setCentralWidget(splitter); splitter->addWidget(operations); splitter->addWidget(map); splitter->addWidget(crane);
    modernizeCraneWorkspace(&window,toolbar,splitter,map,operations,mode,true);
    const QLabel *title=window.findChild<QLabel*>("applicationTitle");
    if (!title || title->text()!=QString("Basket Crane 2 v" BASKET_VERSION_STRING)) { std::fprintf(stderr,"Preview: header version failed\n"); return false; }
    if (mode != "Live") automatic->setEnabled(false);
    for (QCheckBox* control : automaticControls) if (control->isEnabled() != (mode == "Live")) return false;
    if (mode == "LiveObserver") { foreach (QPushButton *button, window.findChildren<QPushButton*>()) button->setEnabled(false); foreach (QRadioButton *button,window.findChildren<QRadioButton*>()) button->setEnabled(false); }
    Plant3DScene sample;
    {
        Plant3DScene contact;
        Plant3DSlot source; source.source=true; source.hasPickupAnchor=true;
        source.key=1; source.basket=52; source.destination="HCA"; source.rows<<"2109:30:HCA";
        source.pickupAnchor=QPointF(3000,1200); source.footprint=QRectF(5000,1000,1800,1500);
        Plant3DSlot target=source; target.source=false; target.target=true;
        target.key=2; target.basket=0;
        target.pickupAnchor=QPointF(9000,4000); target.footprint=QRectF(10000,5000,1800,1500);
        contact.positions3D<<source<<target; contact.crane=QVector3D(3000,1200,2400);
        contact.telemetryValid=true; contact.telemetryTime=QDateTime::currentMSecsSinceEpoch();
        if (plantCraneFootprint(contact)!=source.footprint.adjusted(80,80,-80,-80) || plantCraneHookHeight(contact)!=450) return false;
        {
            Plant3DView pickup([contact]() { return contact; }); pickup.resize(1000,700); pickup.show(); QApplication::processEvents();
            if (!pickup.grab().save(file+".pickup.png")) return false;
        }
        contact.carriedBasket=52; contact.crane=QVector3D(9000,4000,2400);
        if (plantCraneFootprint(contact)!=target.footprint.adjusted(80,80,-80,-80) || plantCraneHookHeight(contact)-350!=100) return false;
        {
            contact.positions3D[0].basket=0; contact.carriedRows=source.rows; contact.carriedDestination=source.destination;
            Plant3DView placement([contact]() { return contact; }); placement.resize(1000,700); placement.show(); QApplication::processEvents();
            if (!placement.grab().save(file+".placement.png")) return false;
        }
        contact.crane=QVector3D(6500,2500,0);
        if (plantCraneFootprint(contact).center()!=QPointF(6500,2500) || plantCraneHookHeight(contact)!=3200) return false;
        std::puts("PASS: display-only pickup/drop-off align with slot offsets, basket rests on platform, and hooks rise at Z=0; travel coordinates stay unchanged.");
    }
    gItemClass sourceDrawing(":dxfs/basketRectFrom.dxf",QPen(),false,false,QColor());
    gItemClass targetDrawing(":dxfs/basketRectTo.dxf",QPen(),false,false,QColor());
    if (sourceDrawing.displayPath().isEmpty() || targetDrawing.displayPath().isEmpty()) return false;
    for (int i=0;i<24;++i) {
        Plant3DSlot slot; slot.key=i+1;
        slot.footprint=QRectF(1000+(i%6)*2600,500+(i/6)*2500,1800,1500);
        slot.basket=i<12?20+i:0; slot.destination=i%2?"HCB":"HCA";
        slot.rows << QString("2109:%1:%2").arg(i+25).arg(slot.destination);
        if (i==2) slot.rows << "6760:12:None";
        slot.locked=i==11; slot.source=i==0; slot.target=i==13;
        if (slot.source || slot.target) {
            const QPainterPath path=slot.source?sourceDrawing.displayPath():targetDrawing.displayPath();
            const QRectF bounds=path.boundingRect(); QTransform transform;
            transform.translate(slot.footprint.left(),slot.footprint.top());
            transform.scale(slot.footprint.width()/bounds.width(),slot.footprint.height()/bounds.height());
            transform.translate(-bounds.left(),-bounds.top());
            if (slot.source) slot.sourcePaths=transform.map(path).toSubpathPolygons();
            else slot.targetPaths=transform.map(path).toSubpathPolygons();
        }
        slot.details=QString("Basket %1 | Position %2 | Profile 2109: %3 pieces, 7000 mm").arg(slot.basket).arg(slot.key).arg(i+25);
        sample.positions3D<<slot;
    }
    // Cross station footprints so previews reveal equipment covering the beam.
    sample.crane=QVector3D(12300,6000,700); sample.telemetryValid=true; sample.telemetryTime=QDateTime::currentMSecsSinceEpoch(); sample.carriedBasket=94; sample.carriedRows<<"6072:21:None"; sample.carriedDestination="None";
    Plant3DView *threeD=installPlant3DView(map,[sample](){return sample;});
    if (!threeD || threeD->visibleBasketCount()!=12) return false;
    if (!window.findChildren<QTabWidget*>().isEmpty() || !map->isHidden()) { std::fprintf(stderr,"Preview: tabs/map visibility failed\n"); return false; }
    window.statusBar()->showMessage("OFFLINE PREVIEW - sample data; no database or PLC connections");
    window.resize(1600,950); window.show(); QApplication::processEvents();
    {
        // Compare camera renders at a constant viewport size. Deferred splitter
        // layout can otherwise resize the preview between captures.
        threeD->setFixedSize(threeD->size()); QApplication::processEvents();
        const QImage original=threeD->grab().toImage();
        if (threeD->visibleMissionMarkerCount()!=2) { std::fprintf(stderr,"Preview: occupied source / empty target arrows failed\n"); return false; }
        if (threeD->visibleDetailCardCount()!=13) { std::fprintf(stderr,"Preview: visible basket detail cards failed\n"); return false; }
        threeD->topView(); QApplication::processEvents();
        if (threeD->grab().toImage()==original) { std::fprintf(stderr,"Preview: top view failed\n"); return false; }
        if (threeD->visibleMissionMarkerCount()!=2) { std::fprintf(stderr,"Preview: top view arrows failed\n"); return false; }
        QScrollBar* horizontal=window.findChild<QScrollBar*>("plantHorizontalScrollBar");
        QScrollBar* vertical=window.findChild<QScrollBar*>("plantVerticalScrollBar");
        if (!horizontal || !vertical || !horizontal->isVisible() || !vertical->isVisible()) { std::fprintf(stderr,"Preview: scrollbar visibility failed\n"); return false; }
        threeD->zoomBy(2); QApplication::processEvents();
        const QImage zoomed=threeD->grab().toImage();
        if (horizontal->maximum()<=0 || vertical->maximum()<=0) { std::fprintf(stderr,"Preview: scrollbar ranges failed: %d %d\n",horizontal->maximum(),vertical->maximum()); return false; }
        horizontal->setValue(horizontal->maximum()); QApplication::processEvents();
        const QImage horizontalPan=threeD->grab().toImage();
        if (horizontalPan==zoomed) { std::fprintf(stderr,"Preview: horizontal scrolling failed\n"); return false; }
        vertical->setValue(vertical->maximum()); QApplication::processEvents();
        if (threeD->grab().toImage()==horizontalPan) { std::fprintf(stderr,"Preview: vertical scrolling failed\n"); return false; }
        threeD->resetView(); QApplication::processEvents();
        const QImage reset=threeD->grab().toImage();
        if (horizontal->value()!=0 || vertical->value()!=0) { std::fprintf(stderr,"Preview: scrollbar reset failed\n"); return false; }
        if (reset!=original) { original.save(file+".original.png"); reset.save(file+".reset.png"); std::fprintf(stderr,"Preview: camera reset failed\n"); return false; }
        foreach (QPushButton *button,threeD->parentWidget()->findChildren<QPushButton*>()) if (!button->isEnabled()) return false;
        QString inspected;
        const auto showDetails=threeD->inspected;
        threeD->inspected=[&inspected,showDetails](const QString& text){inspected=text; showDetails(text);};
        for (int y=50;y<threeD->height()-50 && !inspected.startsWith("Basket ");y+=14)
            for (int x=20;x<threeD->width()-20 && !inspected.startsWith("Basket ");x+=14) {
                QMouseEvent press(QEvent::MouseButtonPress,QPointF(x,y),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
                QMouseEvent release(QEvent::MouseButtonRelease,QPointF(x,y),Qt::LeftButton,Qt::NoButton,Qt::NoModifier);
                QApplication::sendEvent(threeD,&press); QApplication::sendEvent(threeD,&release);
            }
        if (!inspected.startsWith("Basket ") || !inspected.contains("Profile 2109:")) { std::fprintf(stderr,"Preview: basket inspection failed\n"); return false; }
        threeD->inspected=showDetails;
        QMouseEvent press(QEvent::MouseButtonPress,QPointF(1,1),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
        QMouseEvent release(QEvent::MouseButtonRelease,QPointF(1,1),Qt::LeftButton,Qt::NoButton,Qt::NoModifier);
        QApplication::sendEvent(threeD,&press); QApplication::sendEvent(threeD,&release); QApplication::processEvents();
    }
    return window.grab().save(file,"PNG");
}
#endif
