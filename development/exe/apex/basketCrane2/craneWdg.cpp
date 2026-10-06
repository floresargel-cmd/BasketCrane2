#include "craneWdg.h"
#include "controllerHistory.h"
craneWidgetClass::craneWidgetClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip,QWidget *conv,craneClass *crane):QWidget(crane)
{
	QVBoxLayout *topLayout=new QVBoxLayout();setLayout(topLayout);topLayout->setMargin(1);
	QSplitter *centralSplitter=new QSplitter(Qt::Vertical);topLayout->addWidget(centralSplitter);
	QWidget *myWidget=new QWidget();
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(1);myWidget->setLayout(mainLayout);
	QGroupBox *craneGroupBox=new QGroupBox(tr("Crane"));mainLayout->addWidget(craneGroupBox);
	QVBoxLayout *craneGroupBoxLayout=new QVBoxLayout();craneGroupBox->setLayout(craneGroupBoxLayout);
	QGridLayout *posLayout=new QGridLayout();posLayout->setMargin(1);craneGroupBoxLayout->addLayout(posLayout);
	posLayout->addWidget(new QLabel(tr("Position[mm]")),0,1);
	posLayout->addWidget(new QLabel(tr("Target[mm]")),0,2);

	posLayout->addWidget(new QLabel(tr("X")),1,0);
	posLayout->addWidget(xPosLabel=new QLabel(tr("")),1,1);xPosLabel->setToolTip(tr("Crane X position in mm"));
	posLayout->addWidget(xPosTargetLabel=new QLabel(tr("")),1,2);xPosTargetLabel->setToolTip(tr("Crane X target in mm"));

	posLayout->addWidget(new QLabel(tr("Y")),2,0);
	posLayout->addWidget(yPosLabel=new QLabel(tr("")),2,1);yPosLabel->setToolTip(tr("Crane Y position in mm"));
	posLayout->addWidget(yPosTargetLabel=new QLabel(tr("")),2,2);yPosTargetLabel->setToolTip(tr("Crane Y target in mm"));

	posLayout->addWidget(new QLabel(tr("Z")),3,0);
	posLayout->addWidget(zPosLabel=new QLabel(tr("")),3,1);zPosLabel->setToolTip(tr("Crane Z position in mm"));
	posLayout->addWidget(zPosTargetLabel=new QLabel(tr("")),3,2);zPosTargetLabel->setToolTip(tr("Crane Z target in mm"));

	activeIconsVector<<new activeIconClass("toPcI",21,21,QPixmap::fromImage(QImage(":/okBig.png")),QPixmap::fromImage(QImage(":/notOkBig.png")),"","","","",this);
	activeIconsVector.last()->setAlignment(Qt::AlignCenter);
	craneGroupBoxLayout->addWidget(activeIconsVector.last());
	//craneGroupBoxLayout->addStretch(1);

	logGuiName=QString("crane");
	logGuiList=new QListWidget();logGuiList->setObjectName("craneMessages");logGuiList->setMinimumHeight(180);craneGroupBoxLayout->addWidget(logGuiList);
	logGuiList->setToolTip(tr("Crane messages captured when this application starts, newest first. Timestamps use the controller database clock. Hover for description and type."));
	logGuiList->setWordWrap(false);
    logGuiList->setStyleSheet("QListWidget#craneMessages { padding:1px; } QListWidget#craneMessages::item { padding:0px; }");
	QPushButton *popUpListBtn=new QPushButton(QIcon(":/popUp.png"),"");
	logGuiList->setCornerWidget(popUpListBtn);
	connect(popUpListBtn,SIGNAL(clicked()),this,SLOT(popUpList()));
	connect(logGuiList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(popUpList()));
	loadFromDataBase();
	QPushButton *loadHmiBtn=new QPushButton(QIcon(":/androidRed.png"),tr("HMI"));craneGroupBoxLayout->addWidget(loadHmiBtn);
	connect(loadHmiBtn,SIGNAL(released()),this,SLOT(loadHmiDlgSlot()));
	
	QWidget *convWidget=new QWidget();
	QVBoxLayout *convWidgetLayout=new QVBoxLayout();convWidgetLayout->setMargin(1);
	convWidget->setLayout(convWidgetLayout);
	convWidgetLayout->addWidget(conv);
	//
	centralSplitter->addWidget(myWidget);
	centralSplitter->addWidget(convWidget);
	centralSplitter->setSizes(QList<int>()<<400<<900-400);
	hmiDlg=new hmiDlgClass(tuxipServer,tuxip,crane);
}
craneWidgetClass::~craneWidgetClass()
{
}
void craneWidgetClass::setCarriageClass(carriageClass *c)
{
	hmiDlg->setCarriageClass(c);
}
void craneWidgetClass::loadFromDataBase()
{
    if (historyLoaded) return;
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 100 mess,details,type,time from c2logGui where messenger='%1' order by tid desc").arg(logGuiName),bDb);
	loadCraneMessageSnapshot(logGuiList,dataVV,errorStr,historyLoaded);
}
void craneWidgetClass::addMessageToGui(QString mess,QString details,QString type)
{
	if (!basketCraneConfig().databaseWritesAllowed()) return;
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
void craneWidgetClass::popUpList()
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
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select top 300 mess,details,type,time from c2logGui where messenger='%1' order by tid desc").arg(logGuiName),bDb);
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
void craneWidgetClass::loadHmiDlgSlot()
{
	hmiDlg->exec();
}
void craneWidgetClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		if(table=="toPcI")
		{
			for (int i=0;i<activeIconsVector.count();i++)
				activeIconsVector[i]->updateValue(table,index,value.toInt());
		}
		if (table=="toPcF")
		{
			if (index==0)xPosLabel->setText(QString("%1").arg(value.toFloat()));
			if (index==1)xPosTargetLabel->setText(QString("%1").arg(value.toFloat()));
			if (index==6)yPosLabel->setText(QString("%1").arg(value.toFloat()));
			if (index==7)yPosTargetLabel->setText(QString("%1").arg(value.toFloat()));
			if (index==12)zPosLabel->setText(QString("%1").arg(value.toFloat()));
			if (index==13)zPosTargetLabel->setText(QString("%1").arg(value.toFloat()));
		}
		hmiDlg->dataInPlcChanged(ip,table,index,value);
	}
}
