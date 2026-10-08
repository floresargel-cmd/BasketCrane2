#include "exportQueues.h"
#include "allPos.h"
#include "observerDisplay.h"

//#define notAllPos
allPossClass::allPossClass(tuxipClass *craneTux_,tuxipClass *oldOvenTux_,tuxipClass *newOvenTux,QGraphicsScene *layoutScene,QWidget *p):QWidget(p)
{
	oldOvenTux=oldOvenTux_;
	craneTux=craneTux_;
	noOfRows=9;
	selectMode=selectForMission;
	setVisible(false);
	gItemClass *posItem=new gItemClass(":dxfs/basketRect.dxf",QPen(QColor(200,200,250)),false,false,QColor(),this);
	gItemClass *mouseOverItem=new gItemClass(":dxfs/basketRect.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	gItemClass *basketItem=new gItemClass(":dxfs/basket.dxf",QPen(QColor(100,200,100)),false,false,QColor(),this);
	gItemClass *profilesItem=new gItemClass(":dxfs/profiles.dxf",QPen(QColor(150,150,100)),false,false,QColor(),this);profilesItem->setFillBrush(QBrush(QColor(150,150,100,50)));
	gItemClass *lockedItem=new gItemClass(":dxfs/basketLocked.dxf",QPen(QColor(255,100,100)),false,false,QColor(),this);
	gItemClass *basketRectFrom=new gItemClass(":dxfs/basketRectFrom.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	gItemClass *basketRectTo=new gItemClass(":dxfs/basketRectTo.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	manualSelectedFromPos=NULL;
	crane=NULL;
	basket::StartupProgress::ShowMessage(tr("Loading position data..."));
	//old oven
	posV<<new posClass(this,660,1,200.f,7808.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(oldOvenTux,"N11",15);
	posV<<new posClass(this,660,2,200.f,7808.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,660,3,200.f,7808.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,660,4,200.f,7808.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(660);
	//posWithBasketsForExportV<<getPositionsOfColumn(660);

	posV<<new posClass(this,670,1,7351.f,7808.f,0.f,-2100.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(oldOvenTux,"N11",16);
	posV<<new posClass(this,670,2,7351.f,7808.f,0.f,-2100.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,670,3,7351.f,7808.f,0.f,-2100.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,670,4,7351.f,7808.f,0.f,-2100.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(670);
	posWithBasketsForExportV<<getPositionsOfColumn(670);

	posV<<new posClass(this,680,1,200.,9445.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(oldOvenTux,"N11",7);
	posV<<new posClass(this,680,2,200.,9445.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,680,3,200.,9445.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,680,4,200.,9445.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(680);
	//posWithBasketsForExportV<<getPositionsOfColumn(680);

	posV<<new posClass(this,690,1,7351.,9445.f,0.f,-2100.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(oldOvenTux,"N11",8);
	posV<<new posClass(this,690,2,7351.,9445.f,0.f,-2100.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,690,3,7351.,9445.f,0.f,-2100.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,690,4,7351.,9445.f,0.f,-2100.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(690);
	posWithBasketsForExportV<<getPositionsOfColumn(690);

	//new oven
	float newOvenYOffset=187.f;
	posV<<new posClass(this,1040,1,1043.f,3143.f+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(newOvenTux,"N11",7);
	posV<<new posClass(this,1040,2,1043.f,3143.f+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,1040,3,1043.f,3143.f+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,1040,4,1043.f,3143.f+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(1040);
	posWithBasketsForExportV<<getPositionsOfColumn(1040);

	posV<<new posClass(this,1050,1,1043.f,1572.+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(newOvenTux,"N11",8);
	posV<<new posClass(this,1050,2,1043.f,1572.+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,1050,3,1043.f,1572.+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,1050,4,1043.f,1572.+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(1050);
	posWithBasketsForExportV<<getPositionsOfColumn(1050);

	posV<<new posClass(this,1060,1,1043.f,0.f+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(newOvenTux,"N11",9);
	posV<<new posClass(this,1060,2,1043.f,0.f+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,1060,3,1043.f,0.f+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,1060,4,1043.f,0.f+newOvenYOffset,0.f,-3593.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(1060);
	posWithBasketsForExportV<<getPositionsOfColumn(1060);

	//returning conveyor
	posV<<new posClass(this,700,1,200.f,5290.f,0.f,-2575.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);returningPos1=posV.last();
	posV<<new posClass(this,700,2,200.f,5290.f,0.f,-2575.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);returningPos2=posV.last();
	posV<<new posClass(this,700,3,200.f,5290.f,0.f,-2575.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);returningPos3=posV.last();
//	posV<<new posClass(this,700,4,200.f,5290.f,0.f,-2575.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(700);

	//buffer
	posV<<new posClass(this,bufferNoprthPlcId,1,11050.f,1572.,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferNoprthPlcId,2,11050.f,1572.,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferNoprthPlcId,3,11050.f,1572.,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
//	posV<<new posClass(this,bufferNoprthPlcId,4,11050.f,1572.,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(bufferNoprthPlcId);
	posWithBasketsForExportV<<getPositionsOfColumn(bufferNoprthPlcId);

	posV<<new posClass(this,bufferSouthPlcId,1,11050.f,0.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferSouthPlcId,2,11050.f,0.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferSouthPlcId,3,11050.f,0.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
//	posV<<new posClass(this,bufferSouthPlcId,4,11050.f,0.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(bufferSouthPlcId);
	posWithBasketsForExportV<<getPositionsOfColumn(bufferSouthPlcId);

	posV<<new posClass(this,bufferEastOfOldOvenPlcId,1,14980.f,8600.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferEastOfOldOvenPlcId,2,14980.f,8600.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	posV<<new posClass(this,bufferEastOfOldOvenPlcId,3,14980.f,8600.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
//	posV<<new posClass(this,bufferEastOfOldOvenPlcId,4,14980.f,8600.f,0.f,-2750.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);
	setUpDownPositionsOfColumn(bufferEastOfOldOvenPlcId);
	posWithBasketsForExportV<<getPositionsOfColumn(bufferEastOfOldOvenPlcId);

	posV<<new posClass(this,toRotatingSouthPlcId,1,15224.f,3202.f,0.f,-1500.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(craneTux,"bConveyorBasket",7);
	posV<<new posClass(this,toRotatingNorthPlcId,1,15224.f,5002.f,0.f,-1500.f,0.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(craneTux,"bConveyorBasket",6);
	posV<<new posClass(this,rotatingSouthPlcId,1,0.f,0.f,0.f,19180.f,3200.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(craneTux,"bConveyorBasket",4);
	posV<<new posClass(this,rotatingNorthPlcId,1,0.f,0.f,0.f,19180.f,5001.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(craneTux,"bConveyorBasket",3);

	posV<<new posClass(this,stackerWorkingingPlcId,1,0.f,0.f,0.f,19567.f-1500.f,13433.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(craneTux,"bConveyorBasket",9);
	posV<<new posClass(this,stackerWaitingPlcId,1,0.f,0.f,0.f,21368.f-1500.f,13433.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(craneTux,"bConveyorBasket",8);

	posV<<new posClass(this,exit1PlcId,1,0.f,0.f,0.f,21370.f-1500.f,-4159.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(craneTux,"bConveyorBasket",2);
	posV<<new posClass(this,exit2PlcId,1,0.f,0.f,0.f,21370.f-1500.f,-12330.f,layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,true,this);posV.last()->setPlcAdress(craneTux,"bConveyorBasket",1);

	setUpObstacleOfColumn(680,QVector<posClass*>()<<getPosWithPosIndex(660,4,false));
	setUpObstacleOfColumn(690,QVector<posClass*>()<<getPosWithPosIndex(670,4,false));
	setUpObstacleOfColumn(1050,QVector<posClass*>()<<getPosWithPosIndex(1040,4,false));
	setUpObstacleOfColumn(1060,QVector<posClass*>()<<getPosWithPosIndex(1040,4,false)<<getPosWithPosIndex(1050,4,false));
//	setUpObstacleOfColumn(1090,QVector<posClass*>()<<getPosWithPosIndex(1080,4,false));
	for (int i=0;i<posV.count();i++)
	{
		connect(posV[i],SIGNAL(posMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
		connect(posV[i],SIGNAL(userChangedBasketSignal(posClass*)),this,SLOT(userChangedBasketSlot(posClass*)));
	}

	QList <QGraphicsView*> gv=layoutScene->views();
	if (gv.count()!=0)
		connect(gv[0],SIGNAL(keyPressedSignal(QKeyEvent*)),this,SLOT(graphicsViewKeyPressedSlot(QKeyEvent*)));

	QTimer *mainTimer=new QTimer(this);
	connect(mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer->start(basketCraneConfig().number("Timers/PositionsMs"));

	loadFromDatabase();
	setDisplayMode(posClass::displayToSelectFrom);
	exportListChangedSlot();
}
allPossClass::~allPossClass()
{
}
void allPossClass::mainTimerSlot()
{
	loadFromDatabase();
	//if there is a basket on 1070 and length on rotating >7820 do not turn
	int basket=getPosWithPosIndex(1070,1,false)->getBasketNumber();
	{ if (!basketCraneConfig().observer()) craneTux->writeInteger("toPcI",45,basket); }
}
void allPossClass::loadFromDatabase()
{
	QVector<QVector<QVariant>> allPosVV=execTableQuery(QString("select posNumber,posIndex,description,basketTableId,locked from positions"),bDb);
    QVector<QVector<QVariant>> allContentsVV=basketCraneConfig().observer()
        ? execTableQuery(observerContentsSql(),aDb2)
        : execTableQuery(QString("select basketnum,dienum,Pc,PcLen,temper from basket_details where Pc<>0"),aDb);
	for (int i=0;i<posV.count();i++)
		posV[i]->loadFromAllData(allPosVV,allContentsVV);
}
QVector<posClass*> allPossClass::getPositionsOfColumn(int col)
{
	QVector<posClass*> ans;
	for (int i=0;i<posV.count();i++)
	{
		if (posV[i]->getPositionNumber()==col)
			ans<<posV[i];
	}
	return ans;
}
QVector<posClass*> allPossClass::getPosWithBasketsForExport()
{
	return posWithBasketsForExportV;
}
void allPossClass::setDisplayMode(int m)
{
	if (crane)
		crane->setDisplayMode(m);
	if ((m==posClass::displayToModify)&&(!crane->canGetNewMission()))
		return;
	for (int i=0;i<posV.count();i++)
		posV[i]->setDisplayMode(m);
}
void allPossClass::clearIsFromPos()
{
	for (int i=0;i<posV.count();i++)
		posV[i]->setIsFrom(false);
}
void allPossClass::clearIsToPos()
{
	for (int i=0;i<posV.count();i++)
		posV[i]->setIsTo(false);
}

void allPossClass::setUpDownPositionsOfColumn(int column)
{
	QVector<posClass*> pV;
	posClass* p1=getPosWithPosIndex(column,1,false);
	posClass* p2=getPosWithPosIndex(column,2,false);
	posClass* p3=getPosWithPosIndex(column,3,false);
	posClass* p4=getPosWithPosIndex(column,4,false);
	p1->setAboveBelowPos(p2,NULL);
	p2->setAboveBelowPos(p3,p1);
	p3->setAboveBelowPos(p4,p2);
	if (p4)
		p4->setAboveBelowPos(NULL,p3);
}
void allPossClass::setUpObstacleOfColumn(int column,QVector<posClass*> v)
{
	posClass* p1=getPosWithPosIndex(column,1,false);
	posClass* p2=getPosWithPosIndex(column,2,false);
	posClass* p3=getPosWithPosIndex(column,3,false);
	posClass* p4=getPosWithPosIndex(column,4,false);
	p1->setObstaclePos(v);
	p2->setObstaclePos(v);
	p3->setObstaclePos(v);
	p4->setObstaclePos(v);
}
void allPossClass::setCraneClass(craneClass *c)
{
	crane=c;
}
posClass* allPossClass::getPosWithPosIndex(int pos,int index,bool unLockedOnly)
{
	posClass* ans=NULL;
	for (int i=0;i<posV.count();i++)
	{
		if ( (posV[i]->getPositionNumber()==pos)&&(posV[i]->getPositionIndex()==index)  )
		{
			if (!unLockedOnly)
				ans=posV[i];
			else if (!posV[i]->getIsLocked())
				ans=posV[i];
			break;
		}
	}
	return ans;
}
posClass* allPossClass::getFreeReturningPosition()
{
	if (!returningPos1->getHasBasket())
	{
		if ((!returningPos2->getHasBasket())&&(!returningPos3->getHasBasket()))
			return returningPos1;
	}
	if (!returningPos2->getHasBasket())
	{
		if ((returningPos1->getHasBasket())&&(!returningPos3->getHasBasket()))
			return returningPos2;
	}
	if (!returningPos3->getHasBasket())
	{
		if ((returningPos1->getHasBasket())&&(returningPos2->getHasBasket()))
			return returningPos3;
	}
	return NULL;
}
posClass* allPossClass::getPosWithBasket(int b,bool unLockedOnly)
{
	posClass* ans=NULL;
	for (int i=0;i<posV.count();i++)
	{
		if (posV[i]->getBasketNumber()==b)
		{
			if (!unLockedOnly)
				ans=posV[i];
			else if (!posV[i]->getIsLocked())
				ans=posV[i];
			break;
		}
	}
	return ans;
}
posClass* allPossClass::getPosWithBasketToMoveInAuto(int b)
{
	if (b==0)
		return NULL;
	for (int i=0;i<posV.count();i++)
	{
		if (
			(posV[i]->getPositionNumber()==670)
			||
			(posV[i]->getPositionNumber()==690)
			||
			(posV[i]->getPositionNumber()==1040)
			||
			(posV[i]->getPositionNumber()==1050)
			||
			(posV[i]->getPositionNumber()==1060)
			||
			(posV[i]->getPositionNumber()==700)
			||
			(posV[i]->getPositionNumber()==1080)
			||
			(posV[i]->getPositionNumber()==1090)
			||
			(posV[i]->getPositionNumber()==1070)
			)
		{
			if (posV[i]->getBasketNumber()==b)
			{
				if (!posV[i]->getIsLocked())
					return posV[i];
			}
		}
	}
	return NULL;
}
int allPossClass::moveBasket(posClass *from,posClass *to)
{
	if ((!from)||(!to))
	{
		const QString message=QString("moveBasket from==%1 to==%2")
			.arg(from ? from->getPosIndexDisplayString() : "missing")
			.arg(to ? to->getPosIndexDisplayString() : "missing");
		log(message,errorStr);
		uExit(errorStr,message);
	}
	int fromBasket=from->getBasketNumber();
	int toBasket=to->getBasketNumber();
	if ((fromBasket==0)||(toBasket!=0))
	{
		log(QString("moveBasket from==%1 to==%2, fromBasket:%3 toBasket:%4").arg(from->getPosIndexDisplayString()).arg(to->getPosIndexDisplayString()).arg(fromBasket).arg(toBasket),errorStr);
		uExit(errorStr,QString("moveBasket from==%1 to==%2, fromBasket:%3 toBasket:%4").arg(from->getPosIndexDisplayString()).arg(to->getPosIndexDisplayString()).arg(fromBasket).arg(toBasket));
	}
	logV(QString("moveBasket b=%1 f=%2 t=%3 ").arg(fromBasket).arg(from->getPosIndexDisplayString()).arg(to->getPosIndexDisplayString()));
	to->setBasketId(fromBasket,false,"move basket",from->getPositionNumber(),from->getPositionIndex());
	from->setBasketId(0);
	basketExecQuery(QString("insert into c2logPos (pos,basket,info,time) values(%1,%2,'from %3','%4')").arg(to->getPosIndexDisplayString()).arg(fromBasket).arg(from->getPosIndexDisplayString()).arg(timeForLog),bDb);
	return fromBasket;
}
void allPossClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((ip==crane2Ip)&&((table=="bConveyorBasket")||(table=="bConveyorData")))
	{
		for (int i=0;i<posV.count();i++)
			posV[i]->dataInPlcChanged(ip,table,index,value);
	}
	if ((ip==crane2Ip)&&(table=="toPcI"))
	{
		if (index==10)//from
		{
			int from=value.toInt();
			if (from)
			{
				int col=floor(from/10)*10;
				int index=from-col;
				posClass *pos=getPosWithPosIndex(col,index);
				if (pos)
					pos->setIsFrom(true);
			}
			else
				clearIsFromPos();
		}
		if (index==11)//from
		{
			int to=value.toInt();
			if (to)
			{
				int col=floor(to/10)*10;
				int index=to-col;
				posClass *pos=getPosWithPosIndex(col,index);
				if (pos)
					pos->setIsTo(true);
			}
			else
				clearIsToPos();
		}
	}
}
void allPossClass::graphicsViewKeyPressedSlot(QKeyEvent *ev)
{
	if (ev->key()==Qt::Key_Escape)
	{
		if (manualSelectedFromPos)
		{
			manualSelectedFromPos->setIsFrom(false);
			manualSelectedFromPos=NULL;
			setDisplayMode(posClass::displayToSelectFrom);
		}
	}
}
void allPossClass::setSelectMode(int m)
{
	selectMode=m;
	if (selectMode==selectForMission)
		setDisplayMode(posClass::displayToSelectFrom);
	else if (selectMode==selectToModify)
		setDisplayMode(posClass::displayToModify);
	else if ( (selectMode>=selectToExportDestacker)&&(selectMode<=selectToExportPackingA2)  )
		setDisplayMode(posClass::displayToExport);
}
void allPossClass::posMouseReleasedSlot(posClass *p,QGraphicsSceneMouseEvent *ev)
{
	if (selectMode==selectForMission)
	{
		if (!crane->canGetNewMission())
			return;
		if (p->getIsLocked())
			return;
		if (!manualSelectedFromPos)
		{
			if (p->getBasketNumber()>0)
			{
				manualSelectedFromPos=p;
				manualSelectedFromPos->setIsFrom(true);
				setDisplayMode(posClass::displayToSelectTo);
			}
		}
		else
		{
			if ((manualSelectedFromPos==p)||(p->getBasketNumber()>0))
			{
				manualSelectedFromPos->setIsFrom(false);
				manualSelectedFromPos=NULL;
				return;
			}
			crane->sendManualMission(manualSelectedFromPos,p);
			manualSelectedFromPos->setIsFrom(false);
			manualSelectedFromPos=NULL;
			setDisplayMode(posClass::displayToSelectFrom);
		}
	}
    if (selectMode>=selectToExportDestacker && selectMode<=selectToExportPackingA2) {
        if (!basketCraneConfig().databaseWritesAllowed() || p->getBasketNumber()<=0) return;
        if (selectMode>=selectToExportPackingA1 && p->getMaximumLength()>7660) return;
        removeBasketFromExports(p->getBasketNumber());
        addBasketToExports(p->getBasketNumber(),basketExportQueues()[selectMode-selectToExportDestacker].table);
    }
}
void allPossClass::removeBasketFromExports(int basket)
{
    if (!basketCraneConfig().databaseWritesAllowed() || basket<=0) return;
    QVector<basket::SqlCommand> commands;
    foreach (const BasketExportQueue& queue, basketExportQueues())
        commands.append(basket::SqlCommand(QString("delete from [%1] where basket=?").arg(queue.table),
            QVector<QVariant>() << basket));
    basketExecTransaction(commands, bDb);
    emit exportListChangedSignal();
	posClass *pos=getPosWithBasket(basket);
	if (pos)
		pos->setIsOnExportList(false);
}
void allPossClass::addBasketToExports(int basket,QString table)
{
    if (!basketCraneConfig().databaseWritesAllowed() || basket<=0) return;
    bool knownTable=false;
    foreach (const BasketExportQueue& queue,basketExportQueues()) knownTable |= queue.table==table;
    if (!knownTable) return;
	if (getFirst(QString("select tId from %1 where basket=%2").arg(table).arg(basket),bDb).toInt()==0)
	{
		basketExecQuery(QString("insert into %1 (basket,priority) values(%2,%3)").arg(table).arg(basket).arg(getFirst(QString("select top 1 priority from %1 order by priority desc").arg(table),bDb).toInt()+1),bDb);
		exportListChangedSlot();
		posClass *pos=getPosWithBasket(basket);
		if (pos)
			pos->setIsOnExportList(true);
		emit exportListChangedSignal();
	}
}
void allPossClass::exportListChangedSlot()
{
    QVector<QVariant> baskets;
    foreach (const BasketExportQueue& queue,basketExportQueues())
        baskets += basketExecQuery(QString("select basket from %1 order by priority asc").arg(queue.table),bDb);
    for (int i=0;i<posV.count();i++)
        posV[i]->setIsOnExportList(baskets.contains(posV[i]->getBasketNumber()));
}

void allPossClass::userChangedBasketSlot(posClass *pos)
{
	if (selectMode==selectToModify)
	{
		setDisplayMode(posClass::displayToModify);
	}
}
void allPossClass::checkReturningConveyor()
{
	int no=0;
	if (returningPos1->getHasBasket())no++;
	if (returningPos2->getHasBasket())no++;
	if (returningPos3->getHasBasket())no++;
	if (no==3)
		writeBasketToReturningConveyor();
}
void allPossClass::writeBasketToReturningConveyor()
{
	int basket=getPosWithPosIndex(700,1)->getBasketNumber();
	{ if (!basketCraneConfig().observer()) oldOvenTux->writeInteger("N11",17,basket); }
	emit sendBasketsToCrane1Signal();
}
