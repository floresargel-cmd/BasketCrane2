#include "conAllPos.h"
allConvPositionsClass::allConvPositionsClass(tuxipClass *tuxip_,QWidget *p):QWidget(p)
{
	tuxip=tuxip_;
	inMaintenanceMode=false;
	QVBoxLayout *mainLayout=new QVBoxLayout();mainLayout->setMargin(0);setLayout(mainLayout);

	QScrollArea *scrollArea=new QScrollArea();scrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);mainLayout->addWidget(scrollArea);
	scrollArea->setWidgetResizable(true);
	QWidget *scrollAreaWidget=new QWidget();
	QVBoxLayout *scrollAreaWidgetLayout=new QVBoxLayout();scrollAreaWidgetLayout->setMargin(0);scrollAreaWidget->setLayout(scrollAreaWidgetLayout);
	layoutGraphicsView=new myQGraphicsViewClass(true,this);scrollAreaWidgetLayout->addWidget(layoutGraphicsView);
	layoutGraphicsView->scale(0.03,0.03);
	layoutScene=new QGraphicsScene(this);
	scrollArea->setWidget(scrollAreaWidget);
	layoutGraphicsView->setScene(layoutScene);//setScene
	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr(""),this);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
//	return;
	//zoomExtentsBtn->setMaximumWidth(250);
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));
	//
	gItemClass *layoutGraphicsItem=new gItemClass(":dxfs/conLayout.dxf",QPen(QColor(200,200,200),0),false,false,Qt::color0,this);

	layoutGraphicsItem->setZValue(-10.);
	layoutScene->addItem(layoutGraphicsItem);
	gItemClass *posTopMouseOver=new gItemClass(":dxfs/conPosTopMouseOver.dxf",QPen(QColor(200,200,250)),true,true,QColor(150,225,150,150),this);
	gItemClass *conveyor=new gItemClass(":dxfs/conPosTop.dxf",QPen(QColor(225,200,200),0.0),false,false,Qt::color0,this);
	gItemClass *basketTop=new gItemClass(":dxfs/conBasketTop.dxf",QPen(QColor(100,200,100)),false,false,Qt::color0,this);
	gItemClass *profileTop=new gItemClass(":dxfs/conProfileTop.dxf",Qt::NoPen,false,false,Qt::color0,this);

	gItemClass *proxOnItem=new gItemClass(":dxfs/proximitySwitchOn.dxf",QPen(QColor(75,150,75),25),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *proxOffItem=new gItemClass(":dxfs/proximitySwitchOff.dxf",QPen(QColor(150,150,150),10),this,Qt::NoBrush,Qt::NoBrush);
	QVector<QVector<QVariant>> dataVV;
	positionsVector<<new convPositionClass(tuxip,layoutScene,exit2PlcId,"Exit 2",												crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",1,posTopMouseOver,conveyor,basketTop,profileTop,0.f,0.f,this);
	positionsVector<<new convPositionClass(tuxip,layoutScene,exit1PlcId,"Exit 1",												crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",2,posTopMouseOver,conveyor,basketTop,profileTop,0.,8170.,this);

	positionsVector<<new convPositionClass(tuxip,layoutScene,rotatingSouthPlcId,"Rotating south",				crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",4,posTopMouseOver,conveyor,basketTop,profileTop,-900.f,15460.f,this);
	positionsVector<<new convPositionClass(tuxip,layoutScene,rotatingNorthPlcId,"Rotating north",				crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",3,posTopMouseOver,conveyor,basketTop,profileTop,-900.f,17265.f,this);

	positionsVector<<new convPositionClass(tuxip,layoutScene,toRotatingSouthPlcId,"To rotating south",	crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",7,posTopMouseOver,conveyor,basketTop,profileTop,-6145.f-3000.f,15460.f,this);
	positionsVector<<new convPositionClass(tuxip,layoutScene,toRotatingNorthPlcId,"To rotating north",	crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",6,posTopMouseOver,conveyor,basketTop,profileTop,-6145.f-3000.f,17265.f,this);

	positionsVector<<new convPositionClass(tuxip,layoutScene,stackerWaitingPlcId,"Stacker waiting",			crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",8,posTopMouseOver,conveyor,basketTop,profileTop,0.f,24194.f,this);
	positionsVector<<new convPositionClass(tuxip,layoutScene,stackerWorkingingPlcId,"Stacker working",	crane2Ip,"bConveyorBasket","bConveyorState","bConveyorData",9,posTopMouseOver,conveyor,basketTop,profileTop,-1801.f,24194.f,this);

	//btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,503,"bConveyorState",5,17,+500.,layoutScene,tr("Conveyor left"),buttonClass::typeNormal);  btnsVector.last()->myRotate(-900.f,16365.f,+135.f);btnsVector.last()->setMaintenanceMode(true);
	//btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,502,"bConveyorState",5,16,-500.,layoutScene,tr("Conveyor right"),buttonClass::typeNormal); btnsVector.last()->myRotate(-900.f,16365.f,+45.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,503,"bConveyorState",5,5,+1.,layoutScene,tr("Conveyor left"),buttonClass::typeNormal);  btnsVector.last()->myRotate(-870.f,18365.f,+180.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,502,"bConveyorState",5,4,-1.,layoutScene,tr("Conveyor right"),buttonClass::typeNormal); btnsVector.last()->myRotate(1098.f,16365.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,136,"",-1,-1,250,layoutScene,tr("Conveyors 1+2 north"),buttonClass::typeNormal);btnsVector.last()->myRotate(1725.f,4085.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,137,"",-1,-1,-250,layoutScene,tr("Conveyor 1+2 south"),buttonClass::typeNormal);btnsVector.last()->myRotate(1725.f,4085.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,236,"",-1,-1,250,layoutScene,tr("Conveyors 2+3 north"),buttonClass::typeNormal);btnsVector.last()->myRotate(1725.f,12125.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,237,"",-1,-1,-250,layoutScene,tr("Conveyor 2+3 south"),buttonClass::typeNormal);btnsVector.last()->myRotate(1725.f,12125.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,637,"",-1,-1,250,layoutScene,tr("Conveyors 6+3 west"),buttonClass::typeNormal);btnsVector.last()->myRotate(-4025.f-1500.f,19000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,636,"",-1,-1,-250,layoutScene,tr("Conveyor 6+3 east"),buttonClass::typeNormal);btnsVector.last()->myRotate(-4025.f-1500.f,19000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,737,"",-1,-1,250,layoutScene,tr("Conveyors 7+4 west"),buttonClass::typeNormal);btnsVector.last()->myRotate(-4025.f-1500.f,13775.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,736,"",-1,-1,-250,layoutScene,tr("Conveyors 7+4 east"),buttonClass::typeNormal);btnsVector.last()->myRotate(-4025.f-1500.f,13775.f,+0.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,336,"",-1,-1,250,layoutScene,tr("Conveyors 3+8 north"),buttonClass::typeNormal);btnsVector.last()->myRotate(+1000.f,19000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,337,"",-1,-1,-250,layoutScene,tr("Conveyor 3+8 south"),buttonClass::typeNormal);btnsVector.last()->myRotate(+1000.f,19000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,936,"",-1,-1,250,layoutScene,tr("Conveyors 4+9 north"),buttonClass::typeNormal);btnsVector.last()->myRotate(-2600.f,19000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,937,"",-1,-1,-250,layoutScene,tr("Conveyors 4+9 south"),buttonClass::typeNormal);btnsVector.last()->myRotate(-2600.f,19000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);

	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,826,"bConveyorState",8,14,250, layoutScene,tr("Conveyor 8 up"),  buttonClass::typeNormal);btnsVector.last()->myRotate(+1000.f,24000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,827,"bConveyorState",8,11,-250,layoutScene,tr("Conveyor 8 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(+1000.f,24000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,1003,"",-1,-1,250,layoutScene,tr("Side move west"),buttonClass::typeNormal);btnsVector.last()->myRotate (-900.f,21000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,1002,"",-1,-1,-250,layoutScene,tr("Side move east"),buttonClass::typeNormal);btnsVector.last()->myRotate(-900.f,21000.f,+0.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,926,"bConveyorState",9,14,250,layoutScene,tr ("Conveyor 9 up"),  buttonClass::typeNormal);btnsVector.last()->myRotate(-2600.f,24000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	btnsVector<<new buttonClass(tuxip,crane2Ip,"PcCom",9,927,"bConveyorState",9,11,-250,layoutScene,tr("Conveyor 9 down"),buttonClass::typeNormal);btnsVector.last()->myRotate(-2600.f,24000.f,+90.f);btnsVector.last()->setMaintenanceMode(true);
	
	getPosWithId(exit2PlcId)->addProximitySwitch(4,+2500.);
	getPosWithId(exit2PlcId)->addProximitySwitch(5,-2000.);
	getPosWithId(exit1PlcId)->addProximitySwitch(4,+2500.);
	getPosWithId(exit1PlcId)->addProximitySwitch(5,-2000.);
	getPosWithId(rotatingNorthPlcId)->addProximitySwitch(4,-2500.);
	getPosWithId(rotatingNorthPlcId)->addProximitySwitch(5,+2000.);
	getPosWithId(rotatingSouthPlcId)->addProximitySwitch(4,+2500.);
	getPosWithId(rotatingSouthPlcId)->addProximitySwitch(5,-2000.);

	getPosWithId(stackerWaitingPlcId)->addProximitySwitch(4,+2500.);
	getPosWithId(stackerWaitingPlcId)->addProximitySwitch(5,+3000.);

	getPosWithId(stackerWorkingingPlcId)->addProximitySwitch(5,+2000.);

	getPosWithId(toRotatingSouthPlcId)->addProximitySwitch(5,-1000.);
	getPosWithId(toRotatingNorthPlcId)->addProximitySwitch(5,-1000.);

	getPosWithId(exit2PlcId)->addButton(crane2Ip,"PcCom",9,102,"bConveyorState",1,16,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(exit2PlcId)->addButton(crane2Ip,"PcCom",9,103,"bConveyorState",1,17,-1000.,tr("BWD"),buttonClass::typeNormal);
	
	getPosWithId(exit1PlcId)->addButton(crane2Ip,"PcCom",9,202,"bConveyorState",2,16,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(exit1PlcId)->addButton(crane2Ip,"PcCom",9,203,"bConveyorState",2,17,-1000.,tr("BWD"),buttonClass::typeNormal);

	getPosWithId(rotatingSouthPlcId)->addButton(crane2Ip,"PcCom",9,403,"bConveyorState",4,17,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(rotatingSouthPlcId)->addButton(crane2Ip,"PcCom",9,402,"bConveyorState",4,16,-1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(rotatingNorthPlcId)->addButton(crane2Ip,"PcCom",9,303,"bConveyorState",3,17,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(rotatingNorthPlcId)->addButton(crane2Ip,"PcCom",9,302,"bConveyorState",3,16,-1000.,tr("BWD"),buttonClass::typeNormal);

	getPosWithId(toRotatingSouthPlcId)->addButton(crane2Ip,"PcCom",9,703,"bConveyorState",7,17,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(toRotatingSouthPlcId)->addButton(crane2Ip,"PcCom",9,702,"bConveyorState",7,16,-1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(toRotatingNorthPlcId)->addButton(crane2Ip,"PcCom",9,603,"bConveyorState",6,17,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(toRotatingNorthPlcId)->addButton(crane2Ip,"PcCom",9,602,"bConveyorState",6,16,-1000.,tr("BWD"),buttonClass::typeNormal);

	getPosWithId(stackerWaitingPlcId)->addButton(crane2Ip		,"PcCom",9,802,"bConveyorState",8,16,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(stackerWaitingPlcId)->addButton(crane2Ip		,"PcCom",9,803,"bConveyorState",8,17,-1000.,tr("BWD"),buttonClass::typeNormal);
	getPosWithId(stackerWorkingingPlcId)->addButton(crane2Ip,"PcCom",9,902,"bConveyorState",9,16,+1000.,tr("FWD"),buttonClass::typeNormal);
	getPosWithId(stackerWorkingingPlcId)->addButton(crane2Ip,"PcCom",9,903,"bConveyorState",9,17,-1000.,tr("BWD"),buttonClass::typeNormal);

	getPosWithId(exit2PlcId)->myRotate(0.,0.,90.);
	getPosWithId(exit1PlcId)->myRotate(0.,0.,90.);
	getPosWithId(stackerWaitingPlcId)->myRotate(0.,0.,90.);
	getPosWithId(stackerWorkingingPlcId)->myRotate(0.,0.,90.);
	//getPosWithId(rotatingSouthPlcId)->myRotate(0.,0.,0.);
	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(onMainTimerSlot()));
	mainTimer->start(basketCraneConfig().number("Timers/ConveyorsMs"));
}
allConvPositionsClass::~allConvPositionsClass()
{
	//delete cMOver;
	//delete c;
	//delete cWBasket;
	//delete cWFull;
	//delete triGraphicsItem;
}
void allConvPositionsClass::resetGraphicsViewZoom()
{
	layoutGraphicsView->resetView();
	layoutGraphicsView->fitInView(10000.f,-3000.f,13000.f,32000.f,Qt::KeepAspectRatio);
}
convPositionClass* allConvPositionsClass::getPosWithId(int id)
{
	convPositionClass *ans=NULL;
	for (int i=0;i<positionsVector.count();i++)
	{
		if (positionsVector[i]->getGlobalId()==id)
		{
			ans=positionsVector[i];
			break;
		}
	}
	return ans;
}
void allConvPositionsClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==crane2Ip)
	{
		for (int i=0;i<positionsVector.count();i++)
			positionsVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<movesVector.count();i++)
			movesVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<btnsVector.count();i++)
			btnsVector[i]->dataInPlcChanged(ip,table,index,value);
		for (int i=0;i<proxVector.count();i++)
			proxVector[i]->dataInPlcChanged(ip,table,index,value);
		if ( (table=="bConveyorState")&&(index==14) )
		{
			bool m=getValueOfBit(value.toInt(),1);
			m=true;
			for (int i=0;i<positionsVector.count();i++)
				positionsVector[i]->setMaintenanceMode(m);
			for (int i=0;i<btnsVector.count();i++)
				btnsVector[i]->setMaintenanceMode(m);
		}
		if ( (table=="bConveyorState")&&(index==5) )
		{
			bool isHorizontal=false;
			bool isVertical=false;
			if (isHorizontal)
			{
			}
			else if (isVertical)
			{
			}
			else
			{
				//getPosWithId(rotatingSouthPlcId)->myRotate(0.f,+900.f,-45.);
				//getPosWithId(rotatingNorthPlcId)->myRotate(0.f,-900.f,-45.);
			}
		}
	}	 
}
//int angle=0.;
void allConvPositionsClass::onMainTimerSlot()
{
	//if (angle==0)
	//{
	//	getPosWithId(rotatingNorthPlcId)->myRotate(-900.f,-900.f,0.);
	//	getPosWithId(rotatingSouthPlcId)->myRotate(-900.f,+900.f,0.);
	//}
	//if (angle==1)
	//{
	//	getPosWithId(rotatingNorthPlcId)->myRotate(-900.f,-900.f,-45.);
	//	getPosWithId(rotatingSouthPlcId)->myRotate(-900.f,+900.f,-45.);
	//}
	//if (angle==2)
	//{
	//	getPosWithId(rotatingNorthPlcId)->myRotate(-900.f,-900.f,-90.);
	//	getPosWithId(rotatingSouthPlcId)->myRotate(-900.f,+900.f,-90.);
	//	angle=-1;
	//}
	//angle++;
}


