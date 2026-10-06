#include "carriageW.h"
#include "controllerHistory.h"
carriageWidgetClass::carriageWidgetClass(QWidget *p):QGroupBox(p)
{
	logGuiName="crane";
	activeCraneStep=-1;
	plcCraneStepTexts.add(carriageClass::plcStepIdle							,tr("Idle")			                                    ,tr("Plc status:%1\nIdle")																				.arg(carriageClass::plcStepIdle)	);
	plcCraneStepTexts.add(carriageClass::plcStepStart							,tr("Active mission")																,tr("Plc status:%1\nActive mission")															.arg(carriageClass::plcStepStart)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToXYLoad				,tr("Go to xy target load position")								,tr("Plc status:%1\nGo to xy target load position")								.arg(carriageClass::plcStepGoToXYLoad)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToZLoad					,tr("Go to z target load position")									,tr("Plc status:%1\nGo to z target load position")								.arg(carriageClass::plcStepGoToZLoad)	);
	plcCraneStepTexts.add(carriageClass::plcStepPinsGetBasket			,tr("Pins out to get basket")												,tr("Plc status:%1\nPins out to get basket")											.arg(carriageClass::plcStepPinsGetBasket)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoTo0Z						,tr("Go to 0 Z")																		,tr("Plc status:%1\nGo to 0 Z")																		.arg(carriageClass::plcStepGoTo0Z)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToXYUnLoad			,tr("Go to xy target unload position with basket")	,tr("Plc status:%1\nGo to xy target unload position with basket")	.arg(carriageClass::plcStepGoToXYUnLoad)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToZUnload				,tr("Go to z target unload position with basket")		,tr("Plc status:%1\nGo to z target unload position with basket")	.arg(carriageClass::plcStepGoToZUnload)	);
	plcCraneStepTexts.add(carriageClass::plcStepPinsReleaseBasket	,tr("Pins in to release basket")										,tr("Plc status:%1\nPins in to release basket")										.arg(carriageClass::plcStepPinsReleaseBasket)	);
	plcCraneStepTexts.add(carriageClass::plcStepGoToZeroZ					,tr("Go to 0 Z")																		,tr("Plc status:%1\nGo to 0 Z")																		.arg(carriageClass::plcStepGoToZeroZ)	);
	plcCraneStepTexts.add(carriageClass::plcStepFinished					,tr("Mission finished")															,tr("Plc status:%1\nThe mission finished succesfully")						.arg(carriageClass::plcStepFinished)	);

	setTitle(tr("Crane"));
	setToolTip(tr("Crane status"));
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);setLayout(mainLayout);
	//active
	QGroupBox *activeGroupBox=new QGroupBox(tr("Active mission"));mainLayout->addWidget(activeGroupBox);
	QVBoxLayout *activeGroupBoxLayout=new QVBoxLayout();activeGroupBox->setLayout(activeGroupBoxLayout);
	activeMissionLabel=new QLabel();activeMissionLabel->setWordWrap(true);activeMissionLabel->setObjectName("activeMission");activeGroupBoxLayout->addWidget(activeMissionLabel);
	activeCraneStepLabel=new QLabel(tr("Awaiting PLC status"));activeCraneStepLabel->setObjectName("activeCraneStep");activeGroupBoxLayout->addWidget(activeCraneStepLabel);
	activeHooksStepLabel=new QLabel();activeGroupBoxLayout->addWidget(activeHooksStepLabel);
	//next
	QGroupBox *nextGroupBox=new QGroupBox(tr("Next mission"));mainLayout->addWidget(nextGroupBox);
	QVBoxLayout *nextGroupBoxLayout=new QVBoxLayout();nextGroupBox->setLayout(nextGroupBoxLayout);
	nextMissionLabel=new QLabel();nextMissionLabel->setWordWrap(true);nextMissionLabel->setObjectName("nextMission");nextGroupBoxLayout->addWidget(nextMissionLabel);
	//logGuiList
	QLabel *historyTitle=new QLabel(tr("Mission event history"));historyTitle->setObjectName("controllerHistoryTitle");mainLayout->addWidget(historyTitle);
	logGuiList=new QListWidget();logGuiList->setMinimumHeight(20);mainLayout->addWidget(logGuiList);
	logGuiList->setToolTip(tr("Saved controller events, newest first. Timestamps use the controller database clock."));
	logGuiList->setWordWrap(true);
	QPushButton *popUpListBtn=new QPushButton(QIcon(":/popUp.png"),"");
	logGuiList->setCornerWidget(popUpListBtn);
	connect(popUpListBtn,SIGNAL(clicked()),this,SLOT(popUpList()));
	connect(logGuiList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(popUpList()));
	loadFromDataBase();
}
carriageWidgetClass::~carriageWidgetClass()
{
}
void carriageWidgetClass::popUpList()
{
	QDialog *dlg=new QDialog(this);
	dlg->setWindowTitle(tr("Controller event history"));
	dlg->resize(1100,600);
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);dlg->setLayout(mainLayout);
	QTableWidget *logTable=new QTableWidget();mainLayout->addWidget(logTable);
	logTable->setAlternatingRowColors(true);
	logTable->horizontalHeader()->setStretchLastSection(true);
	logTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	logTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	logTable->setColumnCount(4);
	logTable->setHorizontalHeaderLabels(QStringList()<<tr("Message")<<tr("Description")<<tr("type")<<"Time");
	logTable->setWordWrap (true);
	QVector<QVector<QVariant>> dataVV=readCraneControllerHistory(300);
	logTable->setRowCount(dataVV.count());
	for (int i=0;i<dataVV.count();i++)
	{
		logTable->setItem(i,0,new QTableWidgetItem(dataVV[i][0].toString().trimmed()));
		logTable->setItem(i,1,new QTableWidgetItem(dataVV[i][1].toString().trimmed()));
		logTable->setItem(i,2,new QTableWidgetItem(dataVV[i][2].toString().trimmed()));
		logTable->setItem(i,3,new QTableWidgetItem(dataVV[i][3].toString().trimmed()));
		if (dataVV[i][2].toString().trimmed()==errorStr)
		{
			logTable->item(i,0)->setBackground(Qt::red);
			logTable->item(i,1)->setBackground(Qt::red);
			logTable->item(i,2)->setBackground(Qt::red);
			logTable->item(i,3)->setBackground(Qt::red);
		}
	}
	logTable->resizeColumnsToContents();
	dlg->exec();
}
void carriageWidgetClass::loadFromDataBase()
{
	QVector<QVector<QVariant>> dataVV=readCraneControllerHistory(100);
	bool unchanged=logGuiList->count()==dataVV.count();
	for (int i=0;unchanged && i<dataVV.count();++i)
		unchanged=logGuiList->item(i)->text()==controllerHistoryText(dataVV[i][0].toString(),dataVV[i][3])
			&& logGuiList->item(i)->toolTip()==QString("%1\n%2\n%3").arg(dataVV[i][2].toString().trimmed()).arg(dataVV[i][1].toString().trimmed()).arg(dataVV[i][3].toString().trimmed());
	if (unchanged) return;
	logGuiList->clear();
	for (int i=0;i<dataVV.count();i++)
	{
		QListWidgetItem *item=new QListWidgetItem(controllerHistoryText(dataVV[i][0].toString(),dataVV[i][3]));
		item->setToolTip(QString("%1\n%2\n%3").arg(dataVV[i][2].toString().trimmed()).arg(dataVV[i][1].toString().trimmed()).arg(dataVV[i][3].toString().trimmed()));
		if (dataVV[i][2].toString().trimmed()==errorStr)
			item->setForeground(QColor("#f87171"));
		else
			item->setForeground(QColor("#7dd3fc"));
		logGuiList->addItem(item);
	}
}
void carriageWidgetClass::addMessageToGui(QString mess,QString details,QString type)
{
	// Observer history comes from the controller's persisted events, not local polls.
	if (!basketCraneConfig().databaseWritesAllowed()) return;
	// Keep the captured mission identity after pickup empties the source position.
	if (activeBasketNumber > 0 && !mess.contains("basket #", Qt::CaseInsensitive))
		mess = QString("Basket #%1: %2").arg(activeBasketNumber).arg(mess);
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 1 mess,details,type,time from c2logGui where messenger='%1' order by tid desc").arg(logGuiName),bDb);
	bool isTheSameWithLast=false;
	if (dataVV.count()>0)
	{
		if (  (mess==dataVV[0][0].toString().trimmed())  &&  (details==dataVV[0][1].toString().trimmed())  &&  (type==dataVV[0][2].toString().trimmed())  )   
			isTheSameWithLast=true;
	}
	if (!isTheSameWithLast)
	{
		logGui(mess,details,logGuiName,type);
		QListWidgetItem *item=new QListWidgetItem(controllerHistoryText(mess,timeForLog));
		item->setToolTip(QString("%1\n%2\n%3").arg(type).arg(details).arg(timeForLog));
		if (type==errorStr)
			item->setForeground(QColor("#f87171"));
		else
			item->setForeground(QColor("#7dd3fc"));
		logGuiList->insertItem(0,item);
	}
	else
	{
		if (logGuiList->count()>0)
		{
			basketExecQuery(QString("update c2logGui set time='%1' where messenger='%2' and tid in (select max(tid) from c2logGui where messenger='%2')").arg(timeForLog).arg(logGuiName),bDb);
			logGuiList->item(0)->setToolTip(QString("%1\n%2\n%3").arg(type).arg(details).arg(timeForLog));
			logGuiList->item(0)->setText(controllerHistoryText(mess,timeForLog));
		}
	}
}
void carriageWidgetClass::setActiveFrom(QString fDescription)
{
	activeFromDescription=fDescription;
	updateGui();
}
void carriageWidgetClass::setActiveTo(QString tDescription)
{
	activeToDescription=tDescription;
	updateGui();
}

void carriageWidgetClass::setNextFrom(QString fDescription)
{
	nextFromDescription=fDescription;
	updateGui();
}
void carriageWidgetClass::setNextTo(QString tDescription)
{
	nextToDescription=tDescription;
	updateGui();
}
void carriageWidgetClass::setActiveCraneStep(int s)
{
	if (activeCraneStep==s) return;
	activeCraneStep=s;
	addMessageToGui(QString("Plc step:%1").arg(activeCraneStep),QString("Plc step of active mission has changed:\n%1\n%2").arg(getCraneStepText(activeCraneStep)).arg(getCraneStepDescription(activeCraneStep)),infoStr);
	updateGui();
}
QString carriageWidgetClass::getCraneStepText(int s)
{
	return plcCraneStepTexts.getTextOfIndex(s);
}
QString carriageWidgetClass::getCraneStepDescription(int s)
{
	return plcCraneStepTexts.getDescriptionOfIndex(s);
}
void carriageWidgetClass::updateGui()
{
	activeCraneStepLabel->setText(activeCraneStep<0 ? tr("Awaiting PLC status") : getCraneStepText(activeCraneStep));
	activeMissionLabel->setText((activeBasketNumber > 0 ? tr("Basket #%1\n").arg(activeBasketNumber) : QString()) + QString("%1 %2").arg((activeFromDescription.length()>0)?tr("from:%1").arg(activeFromDescription):"").arg((activeToDescription.length()>0)?tr("to:%1").arg(activeToDescription):""));
	nextMissionLabel->setText((nextBasketNumber > 0 ? tr("Basket #%1\n").arg(nextBasketNumber) : QString()) + QString("%1 %2").arg((nextFromDescription.length()>0)?tr("from:%1").arg(nextFromDescription):"").arg((nextToDescription.length()>0)?tr("to:%1").arg(nextToDescription):""));
}
