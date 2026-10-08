#include "crane.h"
#include "automaticMissions.h"
#include "observerDisplay.h"

craneClass::craneClass(tuxipServerClass *tuxipServer,tuxipClass *tuxipConnection,allPossClass *allPos_,QGraphicsScene *layoutScene_,QWidget *p):QWidget(p)
{
	basket::StartupProgress::ShowMessage(tr("Initializing crane..."));
	tuxip=tuxipConnection;
    plcX=plcY=plcZ=0;
	allPos=allPos_;
	setVisible(false);
	layoutScene=layoutScene_;

	gItemClass *posItem=new gItemClass(":dxfs/basketRect.dxf",QPen(QColor(200,200,250)),false,false,QColor(),this);
	gItemClass *mouseOverItem=new gItemClass(":dxfs/basketRect.dxf",QPen(QColor(255,100,100)),true,true,QColor(255,100,100,150),this);
	gItemClass *basketItem=new gItemClass(":dxfs/basket.dxf",QPen(QColor(100,200,100)),false,false,QColor(),this);
	gItemClass *profilesItem=new gItemClass(":dxfs/profiles.dxf",QPen(QColor(150,150,100)),false,false,QColor(),this);profilesItem->setFillBrush(QBrush(QColor(150,150,100,50)));
	gItemClass *lockedItem=new gItemClass(":dxfs/basketLocked.dxf",QPen(QColor(255,100,100)),false,false,QColor(),this);
	gItemClass *basketRectFrom=new gItemClass(":dxfs/basketRectFrom.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);
	gItemClass *basketRectTo=new gItemClass(":dxfs/basketRectTo.dxf",QPen(QColor(255,100,100)),false,false,QColor(255,100,100,150),this);

	int basketNum=getOne(QString("select basketTableId from positions where posNumber=%1").arg(crane2PlcId),bDb).toInt();
	carriage=new carriageClass(tuxip,allPos,"Crane 2",layoutScene,posItem,mouseOverItem,basketItem,profilesItem,lockedItem,basketRectFrom,basketRectTo,p);

	carriage->setPlcMissionTable(crane2Ip,"toPcI",0,1,2,10,11,12,13,18);
	carriage->loadFromDatabase();

	QColor craneColor=QColor(150,150,175);
	// Draw the beam and carriage above baskets without blocking position clicks.
	craneXItem=new gItemClass(":dxfs/craneX.dxf",QPen(QColor(200,225,200)),false,false,QColor(),this);  layoutScene->addItem(craneXItem);craneXItem->setZValue(20);craneXItem->setAcceptedMouseButtons(Qt::NoButton);
	craneYItem=new gItemClass(":dxfs/craneY.dxf",QPen(QColor(150,150,175)),false,false,QColor(),this);  layoutScene->addItem(craneYItem);craneYItem->setZValue(21);craneYItem->setAcceptedMouseButtons(Qt::NoButton);
	craneWidget=new craneWidgetClass(tuxipServer,tuxip,carriage->getCarriageWidget(),this);
	craneWidget->setCarriageClass(carriage);
	//
	posWithBasketsForExportV=allPos->getPosWithBasketsForExport();
	missionsTimer=new QTimer(this);
	connect(missionsTimer,SIGNAL(timeout()),this,SLOT(missionsTimerSlot()));
	autoMode=0;
}
craneClass::~craneClass()
{
}
void craneClass::addToAutoMode(int mode)
{
	autoMode=autoMode|mode;
	missionsTimerSlot();
}
void craneClass::removeFromAutoMode(int mode)
{
	autoMode &= ~mode;
}
QWidget* craneClass::getCraneWidget()
{
	return craneWidget;
}
carriageClass *craneClass::getCarriage()
{
	return carriage;
}
void craneClass::setStations()
{
	toRotatingNorth=allPos->getPosWithPosIndex(toRotatingNorthPlcId,1);
	toRotatingSouth=allPos->getPosWithPosIndex(toRotatingSouthPlcId,1);
	rotatingNorth=allPos->getPosWithPosIndex(rotatingNorthPlcId,1);
	destackerBuffer=allPos->getPosWithPosIndex(stackerWaitingPlcId,1);
	destacker=allPos->getPosWithPosIndex(stackerWorkingingPlcId,1);
	exit1=allPos->getPosWithPosIndex(exit1PlcId,1);
	exit2=allPos->getPosWithPosIndex(exit2PlcId,1);

	missionsTimer->start(basketCraneConfig().number("Timers/MissionsMs"));
}

void craneClass::loadFromDatabase()
{
	carriage->loadFromDatabase();
}
bool craneClass::canGetNewMission()
{
	return carriage->getIsIdle()&&carriage->isReadyForNewMission();
}
void craneClass::sendManualMission(posClass *from,posClass *to)
{
	if (canGetNewMission())
		carriage->missionStart(from,to);
}
void craneClass::checkForNextMissionInDatabase()
{
	carriage->checkForNextMissionInDatabase();
}
void craneClass::setDisplayMode(int m)
{
	carriage->setDisplayMode(m);
}
void craneClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((ip==crane2Ip)&&(table=="toPcF"))
	{
		if (index==0)
			plcX=value.toFloat();
		if (index==1)
			plcZ=value.toFloat();
		if (index==6)
			plcY=value.toFloat();
		// Drawing telemetry follows the HMI's actual Z channel. Keep legacy
		// carriage/control inputs unchanged; this only feeds the overview.
		if (index==12) displayZ=value.toFloat();
		if (index==13 && std::isfinite(value.toFloat()) && value.toFloat()>100)
			displayLoweredZ=value.toFloat();
		if ((index==0)||(index==6)||(index==12)) {
            displayAxes |= index==0?1:(index==6?2:4);
            displayAxisTimes[index==0?0:(index==6?1:2)]=QDateTime::currentMSecsSinceEpoch();
		}
		if ((index==0)||(index==1)||(index==6))
		{
			craneXItem->setPos(plcX,0.);
			craneYItem->setPos(plcX,plcY);
			carriage->setPlcPos(plcX,plcY,plcZ);
		}
	}
	if ((ip==crane2Ip)&&(table=="toPcI"))
	{
		carriage->dataInPlcChanged(ip,table,index,value);
	}
	craneWidget->dataInPlcChanged(ip,table,index,value);
}
void craneClass::missionsTimerSlot()
{
    if (!basketCraneConfig().databaseWritesAllowed() || !autoMode || !carriage->getIsIdle()) return;
    const bool northFree = !toRotatingNorth->getHasBasket() && !rotatingNorth->getHasBasket();
    const bool canSendB = northFree && !destackerBuffer->getHasBasket();
    // Keep the south conveyor path clear before admitting another export.
    const bool canSendSouth = northFree && !exit1->getHasBasket() && !exit2->getHasBasket();
    struct Candidate { QString queue; bool available; int basket; posClass* position; };
    QVector<Candidate> candidates;
    // Production priority: B, anodizing, A1, A2, then imports.
    const int optionOrder[] = {0, 3, 1, 2};
    for (int index : optionOrder) {
        const auto& option = basket::AutomaticMissionOptions()[index];
        if (!(autoMode & option.flag)) continue;
        bool available = index==0 ? canSendB : canSendSouth;
        if (available && (index==1 || index==2)) {
            const int downstream = index==1 ? 1520 : 1530;
            const QString downstreamQueue = index==1 ? "c3exportsDestackerPosition1" : "c3exportsDestackerPosition2";
            const int count = getFirst(QString("select (select count(tId) from %1)+(select count(pid) from positions where basketTableId<>0 and posNumber=%2)")
                .arg(downstreamQueue).arg(downstream), bDb).toInt();
            available = count <= 2;
        }
        const int basketNumber = available ? getFirst(QString("select top 1 basket from %1 order by priority asc").arg(option.queue), bDb).toInt() : 0;
        posClass* position = allPos->getPosWithBasketToMoveInAuto(basketNumber);
        if (position && (!position->getIsFreeToMove() || isPosActiveG(position->getPositionNumber()))) position = nullptr;
        candidates.append(Candidate{option.queue, available, basketNumber, position});
    }
    if (carriage->isReadyForNewMission()) {
        for (const Candidate& candidate : candidates) {
            if (candidate.position) {
                carriage->missionStart(candidate.position, toRotatingNorth);
                break;
            }
        }
        posClass* returning = allPos->getFreeReturningPosition();
        if (carriage->isReadyForNewMission() && returning) {
            if ((autoMode & autoImportsFromNorth) && toRotatingNorth->getBasketForInFromOut() && !isPosActiveG(toRotatingNorth->getPositionNumber()))
                carriage->missionStart(toRotatingNorth, returning);
            else if ((autoMode & autoImportsFromSouth) && toRotatingSouth->getBasketForInFromDestacker() && !isPosActiveG(toRotatingSouth->getPositionNumber()))
                carriage->missionStart(toRotatingSouth, returning);
        }
        if (carriage->isReadyForNewMission()) {
            for (const Candidate& candidate : candidates) {
                if (candidate.available && candidate.basket) uncoverBasketForExport(candidate.basket);
                if (!carriage->isReadyForNewMission()) return;
            }
        }
    }
    if (autoMode & autoExports) addExportsFromEpicsData();
}

void craneClass::addExportsFromEpicsData()
{
    if (!basketCraneConfig().databaseWritesAllowed()) return;
    // EPICS layer order and classifier match the live display. Include order/line
    // keys to keep successive baskets for a destacker on the same sales order.
    const auto contents = [](int basketNumber) {
        QString sql = observerContentsSql(basketNumber);
        sql.replace("r.NextDeptNum ", "r.NextDeptNum,r.SONum,r.SOItemNum ");
        return execTableQuery(sql, aDb2);
    };
    const auto matches = [&contents](int queuedBasket, const QVector<QVariant>& top) {
        const auto rows = contents(queuedBasket);
        return !rows.isEmpty() && rows.first().size()>=11 && top.size()>=11
            && rows.first()[9]==top[9] && rows.first()[10]==top[10];
    };
    const int columns[] = {670,690,1040,1050,1060,1090,1080};
    for (int column : columns) {
        posClass* top = nullptr;
        for (posClass* position : allPos->displayPositions()) {
            if (position->getPositionNumber()==column && position->getHasBasket()
                && (!top || position->getPositionIndex()>top->getPositionIndex())) top=position;
        }
        if (!top || top->getIsOnExportList()) continue;
        const auto rows = contents(top->getBasketNumber());
        if (rows.isEmpty()) continue;
        const auto& topContents = rows.last();
        const QString destination = observerDestination(topContents);
        if (destination=="HCB") {
            QVector<QVariant> queued;
            if (destacker->getHasBasket()) queued << destacker->getBasketNumber();
            if (destackerBuffer->getHasBasket()) queued << destackerBuffer->getBasketNumber();
            foreach (posClass* conveyor, (QVector<posClass*>() << rotatingNorth << toRotatingNorth)) {
                if (conveyor->getHasBasket() && tuxip->readIntegerValue("bConveyorData", conveyor==rotatingNorth?3:6)
                    ==basket::DestinationCatalog::PlcValue(basket::DestinationId::Destacker))
                    queued << conveyor->getBasketNumber();
            }
            queued += execQuery("select basket from c2exportsDestacker order by priority", bDb);
            if (queued.size()<=2 && (queued.isEmpty() || matches(queued.first().toInt(),topContents)))
                allPos->addBasketToExports(top->getBasketNumber(),"c2exportsDestacker");
        } else if (destination=="HCA") {
            for (int index=1; index<=2; ++index) {
                const QString queue = basket::AutomaticMissionOptions()[index].queue;
                const QString downstreamQueue = index==1 ? "c3exportsDestackerPosition1" : "c3exportsDestackerPosition2";
                const auto queued = execQuery(QString("select basket,priority,1 as source from %1 union all select basket,priority,2 as source from %2 union all select basketTableId,3 as priority,3 as source from positions where basketTableId<>0 and posNumber=%3 order by source,priority")
                    .arg(queue).arg(downstreamQueue).arg(index==1?1520:1530), bDb);
                if (queued.size()<=2 && (queued.isEmpty() || matches(queued.first().toInt(),topContents))) {
                    allPos->addBasketToExports(top->getBasketNumber(),queue);
                    break;
                }
            }
        }
    }
}

posClass *craneClass::getFreePosForBasketInPos(posClass *p)
{
	if ( (p->getPositionNumber()==1040)||(p->getPositionNumber()==1050)||(p->getPositionNumber()==1060)  )
	{
		if (allPos->getPosWithPosIndex(1080,1)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,1);
		if (allPos->getPosWithPosIndex(1090,1)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,1);
		if (allPos->getPosWithPosIndex(1080,2)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,2);
		if (allPos->getPosWithPosIndex(1090,2)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,2);
		if (allPos->getPosWithPosIndex(1080,3)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,3);
		if (allPos->getPosWithPosIndex(1090,3)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,3);
		if (allPos->getPosWithPosIndex(1070,1)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1070,1);
		if (allPos->getPosWithPosIndex(1070,2)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1070,2);
		if (allPos->getPosWithPosIndex(1070,3)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1070,3);
	}
	if (p->getPositionNumber()==1080)
	{
		if (allPos->getPosWithPosIndex(1090,1)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,1);
		if (allPos->getPosWithPosIndex(1090,2)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,2);
		if (allPos->getPosWithPosIndex(1090,3)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,3);
	}
	if (p->getPositionNumber()==1090)
	{
		if (allPos->getPosWithPosIndex(1080,1)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,1);
		if (allPos->getPosWithPosIndex(1080,2)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,2);
		if (allPos->getPosWithPosIndex(1080,3)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,3);
	}

	if ( (p->getPositionNumber()==670)||(p->getPositionNumber()==690)  )
	{
		if (allPos->getPosWithPosIndex(1070,1)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1070,1);
		if (allPos->getPosWithPosIndex(1070,2)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1070,2);
		if (allPos->getPosWithPosIndex(1070,3)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1070,3);
		if (allPos->getPosWithPosIndex(1080,1)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,1);
		if (allPos->getPosWithPosIndex(1090,1)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,1);
		if (allPos->getPosWithPosIndex(1080,2)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,2);
		if (allPos->getPosWithPosIndex(1090,2)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,2);
		if (allPos->getPosWithPosIndex(1080,3)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1080,3);
		if (allPos->getPosWithPosIndex(1090,3)->getCanGetBasketInPos(p))return allPos->getPosWithPosIndex(1090,3);
	}
	return NULL;
}
void craneClass::uncoverBasketForExport(int basket)
{
	if (basketCraneConfig().databaseWritesAllowed())
	{
		posClass *basketPos=allPos->getPosWithBasket(basket);
		if (!basketPos)
			return;
		QVector<posClass*> preventMovePos=basketPos->getPosThatPreventYouToMove();
		if (preventMovePos.count()==0)
			return;
		posClass *fromPos=preventMovePos.last();
		posClass *toPos=getFreePosForBasketInPos(fromPos);
		if (fromPos&&toPos)
			carriage->missionStart(fromPos,toPos);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
groupOfPosToExportsClass::groupOfPosToExportsClass()
{
	index=-1;
	destination=posClass::destinationNone;
}
bool groupOfPosToExportsClass::addPos(posClass *p)
{
	if (posV.count())
	{
		int prNew=p->getTopProfile();
		int prbottom=posV.last()->getBottomProfile();
		if(
			(posV.last()->getDestination()==p->getDestination())
			&&
			(posV.last()->getBottomProfile()==p->getTopProfile())
			)
		{
		if (prNew==3457)
			double a=0.;
			posV<<p;
			return true;
		}
		else
			return false;
	}
	else
	{
		posV<<p;
		destination=p->getDestination();
		return true;
	}
}
void groupOfPosToExportsClass::setIndex(int index_)
{
	index=index_;
}
void groupOfPosToExportsClass::updatePos()
{
	QVector<posClass*> unSorted=posV;
	qSort(posV.begin(),posV.end(),[](posClass* a,posClass* b)
		{
			if (a->getBottomProfile()==b->getTopProfile())
				return a->getPosThatPreventYouToMove().count()<b->getPosThatPreventYouToMove().count();
			else
				return false;
		}
	);
	for (int i=0;i<posV.count();i++)
	{
		posV[i]->setExportGroup(index+1);
		posV[i]->setExportGroupQueue(i+1);
	}
}
void groupOfPosToExportsClass::calculatePosThatPreventBasketsToMove(QVector<posClass*> posToIgnore)
{
	posThatPreventBasketsToMove.clear();
	for (int i=0;i<posV.count();i++)
	{
		QVector<posClass*> pp=posV[i]->getPosThatPreventYouToMove();
		for (int j=0;j<pp.count();j++)
		{
			if ( (!posV.contains(pp[j]))&&(!posThatPreventBasketsToMove.contains(pp[j])) )
				posThatPreventBasketsToMove<<pp[j];
		}
	}
	for (int i=0;i<posToIgnore.count();i++)
		posThatPreventBasketsToMove.removeAll(posToIgnore[i]);
}
QVector<posClass*> groupOfPosToExportsClass::getPosThatPreventBasketsToMove()
{
	return posThatPreventBasketsToMove;
}
int groupOfPosToExportsClass::getTopProfile()
{
	if (posV.count())
		return posV.first()->getTopProfile();
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool getIsProfileStacked(int profile,QVector<posClass*> posV)
{
	for (int i=0;i<posV.count();i++)
	{
		if (!posV[i]->getIsProfileStacked(profile))
			return true;
	}
	return false;
}
void craneClass::calculateExportGroupsSLot()
{
	//destinations
	QVector<posClass*> posToExport;
	for (int i=0;i<posWithBasketsForExportV.count();i++)
	{
		if (posWithBasketsForExportV[i]->getHasFullBasket())
		{
			int basket=posWithBasketsForExportV[i]->getBasketNumber();
			int dest=getFirst(QString("select min(Destination) as Destination from (select RackID, SONum, SOItemNum, Destination = dbo.ufn_ApexGetHCDestination(SONum, SOITemNum) from foy.RackDetail where RackID = cast(%1 as varchar(12)))x").arg(basket),aDb2).toInt();
			posWithBasketsForExportV[i]->setDestination(dest);
			posToExport<<posWithBasketsForExportV[i];
		}
		else
			posWithBasketsForExportV[i]->setDestination(posClass::destinationNone);
	}
	//order by basket on top
	QVector<int> topProfiles;
	QVector<groupOfPosToExportsClass*> exportGroupsBulk;
	for (int i=0;i<posToExport.count();i++)
	{
		topProfiles<<posToExport[i]->getTopProfile();
	}
	//
	for (int i=0;i<topProfiles.count();i++)
	{
		if (!getIsProfileStacked(topProfiles[i],posToExport))
		{
			bool added=false;
			for (int j=0;j<exportGroupsBulk.count();j++)
			{
				if (exportGroupsBulk[j]->addPos(posToExport[i]))
				{
					added=true;
					break;
				}
			}
			if (!added)
			{
				exportGroupsBulk<<new groupOfPosToExportsClass();
				exportGroupsBulk.last()->addPos(posToExport[i]);
			}
			topProfiles.remove(i);
			posToExport.remove(i);
			i=-1;
		}
	}
	if (exportGroupsBulk.count()==0)
		return;
	//
	QVector<posClass*> posToIgnor;
	exportGroups.clear();
	for (int i=0;i<exportGroupsBulk.count();i++)
	{
		if (destackerBuffer->getBottomProfile()==exportGroupsBulk[i]->getTopProfile())
		{
			exportGroups<<exportGroupsBulk[i];
			posToIgnor<<exportGroupsBulk[i]->posV;
			exportGroupsBulk.remove(i);
			i--;
		}
		if ((i>=0)&&(destacker->getBottomProfile()==exportGroupsBulk[i]->getTopProfile()))
		{
			exportGroups<<exportGroupsBulk[i];
			posToIgnor<<exportGroupsBulk[i]->posV;
			exportGroupsBulk.remove(i);
			i--;
		}
	}
	while (exportGroupsBulk.count())
	{
		for (int i=0;i<exportGroupsBulk.count();i++)
			exportGroupsBulk[i]->calculatePosThatPreventBasketsToMove(posToIgnor);
		qSort(exportGroupsBulk.begin(),exportGroupsBulk.end(),[](groupOfPosToExportsClass* a,groupOfPosToExportsClass* b){return a->getPosThatPreventBasketsToMove().count()<b->getPosThatPreventBasketsToMove().count();});
		exportGroups<<exportGroupsBulk[0];
		posToIgnor<<exportGroupsBulk[0]->posV;
		exportGroupsBulk.remove(0);
	}
	for (int i=0;i<exportGroups.count();i++)
	{
		exportGroups[i]->setIndex(i);
		exportGroups[i]->updatePos();
	}
}

