#include "version.h"
#include "mW.h"
#include "databaseInitializer.h"
#include "observerTelemetry.h"
#include "modernUi.h"
#include "exportQueues.h"
#include "plant3D.h"

mainWindowClass::mainWindowClass():QMainWindow()
{
	resize(1920,1080);
	const BasketCraneConfig &config = basketCraneConfig();
	const bool emulatePlc = config.plcOffline();

	basket::StartupProgress::ShowMessage(tr("Starting..."));
	setWindowIcon(QIcon(":/assimakis.png"));
	setStyleSheet(modernCraneTheme());
	basket::StartupProgress::ShowMessage(tr("Connecting to plc..."));
	tuxipServer=new tuxipServerClass(this);
//	tuxipServer->setPacketRequestInterval(10000);
	tuxipClass *basketTuxipConnection=tuxipServer->addConnection(crane2Ip,"Crane 2",emulatePlc);
	tuxipServer->addTable("toPcI",plcTableClass::TYPE_INT,0,75,config.number("Plc/PollIntervalMs"),crane2Ip);
	tuxipServer->addTable("toPcF",plcTableClass::TYPE_FLOAT,0,50,config.number("Plc/PollIntervalMs"),crane2Ip);

	tuxipServer->addTable("PcCom",plcTableClass::TYPE_INT,0,9,config.number("Plc/PollIntervalMs"),crane2Ip);
	tuxipServer->addTable("bConveyorBasket",plcTableClass::TYPE_INT,0,29,config.number("Plc/PollIntervalMs"),crane2Ip);
	tuxipServer->addTable("bConveyorState",plcTableClass::TYPE_INT,0,29,config.number("Plc/PollIntervalMs"),crane2Ip);
	tuxipServer->addTable("bConveyorData",plcTableClass::TYPE_INT,0,29,config.number("Plc/PollIntervalMs"),crane2Ip);
	//
	tuxipClass *oldOvenConnection=tuxipServer->addConnection(oldOvenIp,"Old oven",emulatePlc);
	tuxipServer->addTable("N11",plcTableClass::TYPE_INT,0,20,config.number("Plc/PollIntervalMs"),oldOvenIp);
	//
	tuxipClass *newOvenConnection=tuxipServer->addConnection(newOvenIp,"New oven",emulatePlc);
	newOvenConnection->addTable("N11",plcTableClass::TYPE_INT,0,10,config.number("Plc/PollIntervalMs"));
	//
	basket::StartupProgress::ShowMessage(tr("Connecting to database..."));
	basketOpenDatabase("BasketDatabase", bDb);
	basketOpenDatabase("EpicsDatabase", aDb);
	basketOpenDatabase("Epics2Database", aDb2);

    basket::DatabaseInitializer databaseInitializer(bDb);
    databaseInitializer.Initialize(config);
	layoutGraphicsView=new myQGraphicsViewClass(true,this);
	QGraphicsScene *layoutScene=new QGraphicsScene(this);

	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));
	resetGraphicsViewZoom();
	
	gItemClass *layoutGraphicsItem=new gItemClass(":/dxfs/layout.dxf",QPen(QColor(200,200,200)),false,false,QColor(),this);
	layoutGraphicsView->setScene(layoutScene);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	layoutGraphicsView->scale(0.05,0.05);
	basket::StartupProgress::ShowMessage(tr("Creating postions..."));
	allPos=new allPossClass(basketTuxipConnection,oldOvenConnection,newOvenConnection,layoutScene,this);
	basket::StartupProgress::ShowMessage(tr("Creating crane..."));
	crane=new craneClass(tuxipServer,basketTuxipConnection,allPos,layoutScene,this);
	basket::StartupProgress::ShowMessage(tr("Creating exports..."));
    QVector<exportStationWidgetClass*> queueWidgets;
    foreach (const BasketExportQueue& queue, basketExportQueues())
        queueWidgets << new exportStationWidgetClass(queue.table,queue.title,this);
    exportStationWidgetDestacker=queueWidgets[0];
    exportStationWidgetPacking=queueWidgets[1];

	allPos->setCraneClass(crane);
	crane->setStations();
    connect(crane->getCarriage(),SIGNAL(exportListChangedSignal()),allPos,SLOT(exportListChangedSlot()));
    foreach (exportStationWidgetClass *queueWidget, queueWidgets) {
        connect(queueWidget,SIGNAL(listChangedSignal()),allPos,SLOT(exportListChangedSlot()));
        connect(allPos,SIGNAL(exportListChangedSignal()),queueWidget,SLOT(dbUpdateTimerSlot()));
    }
	//
	autoWidget=new autoWidgetClass(crane,this);
	connect(allPos,SIGNAL(sendBasketsToCrane1Signal()),autoWidget,SLOT(sendBasketsToCrane1Slot()));
	connect(autoWidget,SIGNAL(calculateExportGroupsSignal()),crane,SLOT(calculateExportGroupsSLot()));
	//conveyorsWidget
	conveyorsWidget=new conveyorsWidgetClass(basketTuxipConnection,this);
	conveyorsDlg=new QDialog(this);
	conveyorsDlg->setWindowTitle("Conveyors");
	QHBoxLayout *conveyorsDlgL=new QHBoxLayout();conveyorsDlg->setLayout(conveyorsDlgL);conveyorsDlgL->setMargin(2);
	conveyorsDlgL->addWidget(conveyorsWidget);
	conveyorsDlg->resize(1800,900);
	//destackerWidget
	destackerWidget=new destackerWidgetClass(basketTuxipConnection,this);
	destackerDlg=new QDialog(this);
	destackerDlg->setWindowTitle("Destacker");
	QHBoxLayout *destackerDlgL=new QHBoxLayout();destackerDlg->setLayout(destackerDlgL);destackerDlgL->setMargin(2);
	destackerDlgL->addWidget(destackerWidget);
//	destackerDlg->resize(1200,800);
	//
	connect(tuxipServer,SIGNAL(dataInPlcChangedSignal(QString,QString,int,QVariant)),this,SLOT(dataInPlcChangedSlot(QString,QString,int,QVariant)));
	//mainToolBar
	QToolBar *mainToolBar=new QToolBar(tr("Tools"));mainToolBar->setAllowedAreas(Qt::AllToolBarAreas);
	addToolBar(Qt::TopToolBarArea,mainToolBar);
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	showTuxServerAction=new QAction(QIcon(":/notOk.png"),tr("Plc comms"),this);
	showTuxServerAction->setToolTip(tr("Shows the plc comunication table dialog.\nCtrl+P"));
	showTuxServerAction->setStatusTip(tr("Shows the plc comunication table dialog."));
	mainToolBar->addAction(showTuxServerAction);
	connect(showTuxServerAction,SIGNAL(triggered()),this,SLOT(showServerSlot()));

	QAction *writeBasketToReturningConveyorAction=new QAction(QIcon(":/ok.png"),tr("Send baskets to crane 1"),this);
	writeBasketToReturningConveyorAction->setToolTip(tr("Writes the basket number to position 700 for the roller to move the basket to crane 1."));
	writeBasketToReturningConveyorAction->setStatusTip(tr("Writes the basket number to position 700 for the roller to move the basket to crane 1."));
	mainToolBar->addAction(writeBasketToReturningConveyorAction);
	connect(writeBasketToReturningConveyorAction,SIGNAL(triggered()),this,SLOT(writeBasketToReturningConveyorActionSlot()));

	QAction *showUsageDialogAction=new QAction(QIcon(":/usage.png"),tr("Usage"),this);
	showUsageDialogAction->setToolTip(tr("Show crane usage.\nCtrl+U"));
	showUsageDialogAction->setStatusTip(tr("Show crane usage."));
	showUsageDialogAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
	connect(showUsageDialogAction,SIGNAL(triggered()),this,SLOT(showUsageDialogSlot()));
	
	QAction *showConVDlgAction=new QAction(QIcon(":/autoDisabled.png"),tr("Conveyors"),this);
	connect(showConVDlgAction,SIGNAL(triggered()),this,SLOT(showConVDlgActionTriggeredSlot()));
	addAction(showConVDlgAction);

	QAction *showDestackerDlgAction=new QAction(QIcon(":/crane.png"),tr("Destacker"),this);
	connect(showDestackerDlgAction,SIGNAL(triggered()),this,SLOT(showDestackerDlgActionTriggeredSlot()));
	addAction(showDestackerDlgAction);

	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("24 feet"),"",basketTuxipConnection,"PcCom",9,6,0,"","toPcI",23,29,"","","","","",buttonStyleActive,buttonStyleO,tr("more than 24 feet"),this);mainToolBar->addWidget(pushButtonsVector.last());pushButtonsVector.last()->setMaximumWidth(150);

	QAction *qtAboutAction=new QAction(this);
	qtAboutAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
	connect(qtAboutAction,SIGNAL(triggered()),this,SLOT(qtAboutSlot()));
	addAction(qtAboutAction);
	
	QAction *stopAction=new QAction(this);
	stopAction->setShortcut(QKeySequence(Qt::Key_F1));
	connect(stopAction,SIGNAL(triggered()),this,SLOT(stopSlot()));
	addAction(stopAction);

	mainToolBar->addSeparator();
	mainToolBar->addAction(showTuxServerAction);
	mainToolBar->addAction(showUsageDialogAction);
	mainToolBar->addAction(showConVDlgAction);
	mainToolBar->addAction(showDestackerDlgAction);
	mainToolBar->addWidget(missionsRadioButton	=new QRadioButton(tr("Missions mode")));missionsRadioButton->setChecked(true);
	mainToolBar->addWidget(exportsRadioButton		=new QRadioButton(tr("Exports mode")));
	QWidget *spacerWdgt=new QWidget();
	spacerWdgt->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
	mainToolBar->addWidget(spacerWdgt);
	mainToolBar->addWidget(modifyRadioButton		=new QRadioButton(tr("Modify mode")));

	QPushButton *hmiDlgBasicButton=new QPushButton(QIcon(":/androidRed.png"),tr("Basic HMI"));
	mainToolBar->addWidget(hmiDlgBasicButton);
	connect(hmiDlgBasicButton,SIGNAL(clicked()),this,SLOT(hmiDlgBasicButtonClickedSlot()));
	hmiDlgBasic=new hmiDlgBasicClass(tuxipServer,basketTuxipConnection,this);
	QWidget *toolBoxWidget=new QWidget();QVBoxLayout *toolBoxWidgetLayout=new QVBoxLayout();toolBoxWidget->setLayout(toolBoxWidgetLayout);toolBoxWidgetLayout->setMargin(3);
	toolBox=new QToolBox(this);toolBoxWidgetLayout->addWidget(toolBox);
	int index=0;
	toolBox->addItem(autoWidget,QIcon(":/apexIcon.png"),tr("Automatic missions"));
	toolBox->widget(index)->setToolTip(tr("Enables automatic missions"));
    foreach (exportStationWidgetClass *queueWidget, queueWidgets) {
        const int queueIndex=toolBox->addItem(queueWidget,QIcon(":/export.png"),queueWidget->windowTitle());
        queueWidget->attachToToolBox(toolBox,queueIndex);
    }

	QSplitter *centralSplitter=new QSplitter(Qt::Horizontal);
	setCentralWidget(centralSplitter);
	statusBar();
	centralSplitter->addWidget(toolBoxWidget);
	centralSplitter->addWidget(layoutGraphicsView);
	centralSplitter->addWidget(crane->getCraneWidget());
	centralSplitter->setSizes(QList<int>()<<300<<1850-600<<300);
    modernizeCraneWorkspace(this, mainToolBar, centralSplitter, layoutGraphicsView, toolBoxWidget, config.environment());
    const QList<QPolygonF> floorPaths=layoutGraphicsItem->displayPath().toSubpathPolygons();
    installPlant3DView(layoutGraphicsView,[this,floorPaths]() {
        Plant3DScene snapshot; snapshot.floorPaths=floorPaths;
        foreach (posClass *position,allPos->displayPositions()) {
            Plant3DSlot slot; slot.footprint=position->displayFootprint();
            slot.pickupAnchor=QPointF(position->getPlcX(),position->getPlcY()); slot.hasPickupAnchor=true;
            slot.key=position->getPositionNumber()*100+position->getPositionIndex();
            slot.basket=position->getBasketNumber(); slot.destination=position->displayDestination();
            slot.details=position->displayBasketDetails(); slot.rows=position->displayBasketRows(); slot.locked=position->getIsLocked();
            slot.hoverDetails=position->displayHoverDetails();
            slot.exportCodes=position->displayExportCodes();
            slot.source=position->getIsFrom(); slot.target=position->displayIsTarget();
            if (slot.source) slot.sourcePaths=position->displaySourcePaths();
            if (slot.target) slot.targetPaths=position->displayTargetPaths();
            snapshot.positions3D<<slot;
        }
        snapshot.crane=crane->displayCoordinates(); snapshot.telemetryValid=crane->displayTelemetryValid();
        snapshot.loweredZ=crane->displayLoweringTarget();
        snapshot.telemetryTime=crane->displayTelemetryTime(); snapshot.carriedBasket=crane->getCarriage()->displayBasketNumber();
        snapshot.carriedRows=crane->getCarriage()->displayBasketRows(); snapshot.carriedDestination=crane->getCarriage()->displayDestination();
        return snapshot;
    });
	//
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(basketCraneConfig().number("Timers/MainMs"));
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	basket::StartupProgress::ShowMessage(tr("Reading plc tables..."));
	//
	//exit(0);
	handShakeCrane2=new handShakeClass(tuxipServer->getTuxipClassWithIp(crane2Ip),crane2Ip,"",-1,"toPcI",26,config.number("Plc/HandshakeTimeoutMs"),handShakeClass::typeReadOnly);
	connect(handShakeCrane2,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeCrane2ChangedSlot(bool)));
	//handShakeR2=new handShakeClass(tuxipServer->getTuxipClassWithIp(r2Ip),r2Ip,"",-1,aw1Int,30,2000,handShakeClass::typeReadOnly);
	//connect(handShakeR2,SIGNAL(stateChangedSignal(bool)),this,SLOT(handShakeStateR2ChangedSlot(bool)));

	basket::StartupProgress::ShowMessage(tr("Checking for open missions..."));
	if (config.databaseWritesAllowed()) crane->checkForNextMissionInDatabase();

	connect(missionsRadioButton,SIGNAL(clicked(bool)),this,SLOT(missionsRadioButtonClickedSlot(bool)));
	connect(modifyRadioButton,SIGNAL(clicked(bool)),this,SLOT(modifyRadioButtonClickedSlot(bool)));
	connect(exportsRadioButton,SIGNAL(clicked(bool)),this,SLOT(exportsRadioButtonClickedSlot(bool)));
	connect(toolBox,SIGNAL(currentChanged(int)),this,SLOT(toolBoxCurrentChangedSlot(int)));
	
    const QString mode = config.environment();
    statusBar()->showMessage(config.liveControlAllowed()
        ? "Live: database writes and live PLC control enabled"
        : mode + ": databases read-only; live PLC connections disabled");
    if (!config.databaseWritesAllowed()) {
        autoWidget->setEnabled(false);
        modifyRadioButton->setEnabled(false);
        exportsRadioButton->setEnabled(false);
        writeBasketToReturningConveyorAction->setEnabled(false);
    }
    if (config.observer()) {
        foreach (QPushButton *button, crane->getCraneWidget()->findChildren<QPushButton *>()) button->setEnabled(false);
        conveyorsWidget->setEnabled(false);
        destackerWidget->setEnabled(false);
        hmiDlgBasic->setEnabled(false);
        foreach (hmiPushButtonWidgetClass *button, pushButtonsVector) button->setEnabled(false);
        stopAction->setEnabled(false);
        showTuxServerAction->setEnabled(false);
        missionsRadioButton->setEnabled(false);
        layoutGraphicsView->setInteractive(false);
    }
	tuxipServer->setIsAutoUpdatingFromPlc(!config.observer());
    if (config.observer()) {
        new ObserverTelemetry(this, [this](const QVector<float>& floats, const QVector<int>& integers, const QString& error) {
            if (!error.isEmpty()) {
                statusBar()->showMessage("LiveObserver: databases read-only; " + error + " (display may be stale)");
                showTuxServerAction->setIcon(QIcon(":/notOk.png"));
                return;
            }
            // Deliver telemetry directly to the display; no PLC control API or
            // handshake handler participates in this read path.
            for (int i = 0; i < floats.size(); ++i) {
                crane->dataInPlcChanged(crane2Ip, "toPcF", i, floats[i]);
            }
            for (int i = 0; i < integers.size(); ++i) {
                crane->dataInPlcChanged(crane2Ip, "toPcI", i, integers[i]);
            }
            statusBar()->showMessage("LiveObserver: live crane telemetry; databases read-only; PLC outputs disabled");
            showTuxServerAction->setIcon(QIcon(":/ok.png"));
        });
    }
//	resetGraphicsViewZoom();
}
mainWindowClass::~mainWindowClass()
{
    // Stop the network thread before the UI objects it feeds are destroyed.
    foreach (QObject *child, children()) {
        ObserverTelemetry *reader = dynamic_cast<ObserverTelemetry *>(child);
        if (reader) delete reader;
    }
	log("programm ended",infoStr);
}
void mainWindowClass::handShakeCrane2ChangedSlot(bool s)
{
	showTuxServerAction->setIcon(QIcon(s?":/ok.png":":/notOk.png"));
}
void mainWindowClass::hmiDlgBasicButtonClickedSlot()
{
	hmiDlgBasic->exec();
}
void mainWindowClass::toolBoxCurrentChangedSlot(int index)
{
	if (index==0)
	{
		missionsRadioButton->setChecked(true);
		allPos->setSelectMode(allPossClass::selectForMission);
	}
    else if (index>=1 && index<=basketExportQueues().size() && exportsRadioButton->isChecked())
        allPos->setSelectMode(allPossClass::selectToExportDestacker+index-1);
}
void mainWindowClass::missionsRadioButtonClickedSlot(bool s)
{
	allPos->setSelectMode(allPossClass::selectForMission);
}
void mainWindowClass::modifyRadioButtonClickedSlot(bool s)
{
	allPos->setSelectMode(allPossClass::selectToModify);
}
void mainWindowClass::exportsRadioButtonClickedSlot(bool s)
{
    const int index=toolBox->currentIndex();
    if (index>=1 && index<=basketExportQueues().size())
        allPos->setSelectMode(allPossClass::selectToExportDestacker+index-1);
	if (toolBox->currentIndex()==0)
		missionsRadioButton->setChecked(true);
}

void mainWindowClass::dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value)
{
    if (basketCraneConfig().observer()) return;
	if (ip==crane2Ip)
	{
		allPos->dataInPlcChanged(ip,table,index,value);
		crane->dataInPlcChanged(ip,table,index,value);
		hmiDlgBasic->dataInPlcChanged(ip,table,index,value);
		handShakeCrane2->dataInPlcChanged(ip,table,index,value);
		conveyorsWidget->dataInPlcChanged(ip,table,index,value);
		destackerWidget->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
	}
}
void mainWindowClass::showServerSlot()
{
	if (getPassword())
		tuxipServer->showDialog();
}
void mainWindowClass::qtAboutSlot()
{
	 QMessageBox::aboutQt(this,tr("Apex basket crane build:  %1 ").arg(uCompilerData));
}
void mainWindowClass::showUsageDialogSlot()
{
	usageDialogClass *usage=new usageDialogClass(this);
	usage->exec();
}
void mainWindowClass::showConVDlgActionTriggeredSlot()
{
	conveyorsDlg->exec();
}
void mainWindowClass::showDestackerDlgActionTriggeredSlot()
{
	destackerDlg->exec();
}
void mainWindowClass::stopSlot()
{
	{ if (!basketCraneConfig().observer()) tuxipServer->writeInteger("toPcI",22,226,crane2Ip); }
	{ if (!basketCraneConfig().observer()) tuxipServer->writeInteger("toPcI",22,0,crane2Ip); }
}
void mainWindowClass::resetGraphicsViewZoom()
{
//	layoutGraphicsView->resetView();
	layoutGraphicsView->fitInView(-3000.f,-1750.,24000.f,14000.f,Qt::KeepAspectRatio);
}
void mainWindowClass::writeBasketToReturningConveyorActionSlot()
{
	allPos->writeBasketToReturningConveyor();
}
void mainWindowClass::mainTimerSlot()
{
	// The legacy upper list is loaded once; only the lower mission list advances.
	setWindowTitle(QString("Apex Basket Crane 2 v" BASKET_VERSION_STRING " [%1] %2").arg(basketCraneConfig().environment()).arg(timeVerbose));
}
