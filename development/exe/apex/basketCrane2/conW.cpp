#include "conW.h"
conveyorsWidgetClass::conveyorsWidgetClass(tuxipClass *tuxip,QWidget *p):QWidget(p)
{
	setWindowTitle(tr("Conveyors HMI"));
	resize(1800,900);
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);mainLayout->setMargin(2);
	QSplitter *mainSplitter=new QSplitter(Qt::Horizontal,this);mainLayout->addWidget(mainSplitter);
	mainSplitter->setStyleSheet("QSplitter::handle:Horizontal{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:Horizontal:hover{height:3px;background-color:#339966;}");
	mainLayout->addWidget(mainSplitter);

	QScrollArea *leftScrollArea=new QScrollArea();leftScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);mainSplitter->addWidget(leftScrollArea);
	leftScrollArea->setWidgetResizable(true);
	QWidget *leftScrollAreaWidget=new QWidget();leftScrollArea->setWidget(leftScrollAreaWidget);
	QVBoxLayout *leftScrollAreaWidgetL=new QVBoxLayout();leftScrollAreaWidgetL->setMargin(0);leftScrollAreaWidget->setLayout(leftScrollAreaWidgetL);
	//mainLayout->addStretch(1); 
	basket::StartupProgress::ShowMessage(tr("Creating positions..."));
	allConvPositions=new allConvPositionsClass(tuxip,this);mainSplitter->addWidget(allConvPositions);
	basket::StartupProgress::ShowMessage(tr("Creating mode gui..."));

	QGroupBox *modeGroupBox=new QGroupBox(tr("Mode"));leftScrollAreaWidgetL->addWidget(modeGroupBox);
	QVBoxLayout *modeGroupBoxL=new QVBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"PcCom",9,1,0,"","bConveyorState",1,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"PcCom",9,2,0,"","bConveyorState",1,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("No alarms"),"",tuxip,"PcCom",9,3,0,"","bConveyorState",1,6,"","","","","",buttonStyleRed,buttonStyleActive,tr("Reset alarms"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	bitLogVectors<<new bitLogVectorClass("PcCom",QString("conveyorCommands.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelG,styleLabelGr,"","Commands","","",true,this);leftScrollAreaWidgetL->addWidget(bitLogVectors.last());
	bitLogVectors<<new bitLogVectorClass("PcCom",QString("conveyorAlarms.txt"),bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,"","Errors","","",true,this);leftScrollAreaWidgetL->addWidget(bitLogVectors.last());
	//bitVector<<new bitClass("PcCom",hmiStatusIndex,3,bitClass::TYPE_CHANGE_COLOR,tr("Active stop"),tr("No active stop"),styleLabelR,styleLabelG,"","","","",this);modeGroupBoxL->addWidget(bitVector.last());
	//bitErrorsVector->setBitLogHistoryClassDatabaseEnum(bitLogHisoryClass::typeMySql);
	//bitWarningsVector->setBitLogHistoryClassDatabaseEnum(bitLogHisoryClass::typeMySql);
	mainSplitter->setSizes(QList<int>()<<500<<1300);
}
conveyorsWidgetClass::~conveyorsWidgetClass()
{
}
void conveyorsWidgetClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		for (int i=0;i<bitVector.count();i++)
			bitVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<pushButtonsVector.count();i++)
			pushButtonsVector[i]->updateValue(table,index,value.toInt());
		for (int i=0;i<bitLogVectors.count();i++)
			bitLogVectors[i]->updateValue(table,index,value.toInt());
		allConvPositions->dataInPlcChanged(ip,table,index,value);
	}
}