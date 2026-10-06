#include "carriage.h"
carriageClass::carriageClass(tuxipClass *tuxipConnection,allPossClass *allPos_,QString description,QGraphicsScene *scene,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_,QWidget *p):QWidget(p)
{
	basket::StartupProgress::ShowMessage(tr("Initializing carriage..."));
	pos=new posClass(allPos_,crane2PlcId,0,0.,0.,0.,0.,0.,scene,posItem_,mouseOverItem_,basketItem_,profilesItem_,lockedItem_,fromItem_,toItem_,true,p);
	pos->loadFromDatabase();
	pos->setDisplayMode(posClass::displayNormal);
	allPos=allPos_;
	tuxip=tuxipConnection;
	carriageWidget=new carriageWidgetClass(this);
	nextFromPos=nextToPos=NULL;
	activeFromPos=activeToPos=NULL;
	activeMissionStatus=plcStatusIdle;
	activeCraneStep=plcStepIdle;
	carriageWidget->addMessageToGui(QString("..."),QString("Program started"),infoStr);

	QTimer *checkStatusTimer=new QTimer(this);
	connect(checkStatusTimer,SIGNAL(timeout()),this,SLOT(checkStatusTimerSlot()));
	checkStatusTimer->start(basketCraneConfig().number("Timers/CarriageStatusMs"));

}
carriageClass::~carriageClass()
{
}
QString carriageClass::getCraneStepText(int s)
{
	return carriageWidget->getCraneStepText(s);
}
QString carriageClass::getCraneStepDescription(int s)
{
	return carriageWidget->getCraneStepDescription(s);
}
QWidget *carriageClass::getCarriageWidget()
{
	return carriageWidget;
}
void carriageClass::setPlcPos(float x,float y,float z)
{
	pos->setPlcPos(x,y,z);
}
void carriageClass::setDisplayMode(int m)
{
	if (m==posClass::displayToModify)
		pos->setDisplayMode(m);
	else
		pos->setDisplayMode(posClass::displayNormal);
}
void carriageClass::setNextFromPos(posClass *p)
{
	if ((nextFromPos)&&(p))
	{
		carriageWidget->addMessageToGui(QString("internal error"),QString("setNextFromPos, p!=null while nextFromPos!=null"),errorStr);
		return;
	}
	nextFromPos=p;
	carriageWidget->setNextBasket(p ? p->getBasketNumber() : 0);
	if (nextFromPos)
		carriageWidget->setNextFrom(nextFromPos->getDescription());
	else
	{
		carriageWidget->setNextFrom("");
		basketExecQuery(QString("update c2missions set fromPosNumber=0,fromPosIndex=0 where status='%1'").arg(nextStr),bDb);
	}
}
void carriageClass::setNextToPos(posClass *p)
{
	if ((nextToPos)&&(p))
	{
		carriageWidget->addMessageToGui(QString("internal error"),QString("setNextToPos, p!=null while nextToPos!=null"),errorStr);
		return;
	}
	nextToPos=p;
	if (nextToPos)
		carriageWidget->setNextTo(nextToPos->getDescription());
	else
	{
		carriageWidget->setNextTo("");
		basketExecQuery(QString("update c2missions set toPosNumber=0,toPosIndex=0 where status='%1'").arg(nextStr),bDb);
	}
}
void carriageClass::setActiveFromPos(posClass *p)
{
	if ((activeFromPos)&&(p))
	{
		carriageWidget->addMessageToGui(QString("internal error"),QString("setActiveFromPos, p!=null while activeFromPos!=null"),errorStr);
		return;
	}
	activeFromPos=p;
	// Recovery can resume with the basket already on the hooks.
	carriageWidget->setActiveBasket(p ? (pos->getBasketNumber() ? pos->getBasketNumber() : p->getBasketNumber()) : 0);
	if (activeFromPos)
		carriageWidget->setActiveFrom(activeFromPos->getDescription());
	else
		carriageWidget->setActiveFrom("");
}
void carriageClass::setActiveToPos(posClass *p)
{
	if ((activeToPos)&&(p))
	{
		carriageWidget->addMessageToGui(QString("internal error"),QString("setActiveToPos, p!=null while activeToPos!=null"),errorStr);
		return;
	}
	activeToPos=p;
	if (activeToPos)
		carriageWidget->setActiveTo(activeToPos->getDescription());
	else
		carriageWidget->setActiveTo("");
}
bool carriageClass::moveNextToActive()
{
	if ((nextFromPos)&&(nextToPos)&&(!activeFromPos)&&(!activeToPos))
	{
		carriageWidget->addMessageToGui(QString("Making next mission active for basket #%1").arg(carriageWidget->nextBasket()),"Moving data from of next mission to active mission",infoStr);
        // Both mission rows move together; retries never replay widget/PLC changes.
        basketExecTransaction(QVector<basket::SqlCommand>()
            << basket::SqlCommand("update c2missions set fromPosNumber=?,fromPosIndex=?,toPosNumber=?,toPosIndex=? where status=?",
                QVector<QVariant>() << nextFromPos->getPositionNumber() << nextFromPos->getPositionIndex()
                    << nextToPos->getPositionNumber() << nextToPos->getPositionIndex() << activeStr)
            << basket::SqlCommand("update c2missions set fromPosNumber=0,fromPosIndex=0,toPosNumber=0,toPosIndex=0 where status=?",
                QVector<QVariant>() << nextStr), bDb);
		setActiveFromPos(nextFromPos);
		carriageWidget->setActiveBasket(carriageWidget->nextBasket());
		setActiveToPos(nextToPos);
		setNextFromPos(NULL);
		setNextToPos(NULL);
		return true;
	}
	else if ((nextFromPos)&&(nextToPos)&&(activeFromPos)&&(activeToPos))
	{
		if ( 
			(nextFromPos->getPositionNumber()==activeFromPos->getPositionNumber())&&(nextFromPos->getPositionIndex()==activeFromPos->getPositionIndex())  
			&&  
			(nextToPos->getPositionNumber()==activeToPos->getPositionNumber())&&(nextToPos->getPositionIndex()==activeToPos->getPositionIndex())
			)
			return true;
		else
			carriageWidget->addMessageToGui("Tried to move next to active","Internal error, active pos is not empty and next not equal to active",errorStr);
		return true;
	}
	//else if ((!nextFromPos)&&(!nextToPos)&&(activeFromPos)&&(activeToPos))
	//{
	//	carriageWidget->addMessageToGui("Move next to active","Move next mission to active, mission allready moved",infoStr);
	//	return true;
	//}
	return false;
}
void carriageClass::setPlcMissionTable(QString plcIp_,QString plcMissionTable_,int plcNextMissionFromIndex_,int plcNextMissionToIndex_,int plcNextMissionStatusIndex_,int plcCurrentMissionFromIndex_,int plcCurrentMissionToIndex_,int plcCurrentMissionPcStatusIndex_,int plcCurrentMissionPLcStatusIndex_,int plcCurrentCranePlcStepIndex_)
{
	plcIp=plcIp_;
	plcMissionTable=plcMissionTable_;
	plcNextMissionFromIndex=plcNextMissionFromIndex_;
	plcNextMissionToIndex=plcNextMissionToIndex_;
	plcNextMissionStatusIndex=plcNextMissionStatusIndex_;
	plcCurrentMissionFromIndex=plcCurrentMissionFromIndex_;
	plcCurrentMissionToIndex=plcCurrentMissionToIndex_;
	plcCurrentMissionPcStatusIndex=plcCurrentMissionPcStatusIndex_;
	plcCurrentMissionPLcStatusIndex=plcCurrentMissionPLcStatusIndex_;
	plcCurrentCranePlcStepIndex=plcCurrentCranePlcStepIndex_;
}
void carriageClass::checkForNextMissionInDatabase()
{
	if (!basketCraneConfig().databaseWritesAllowed()) return;
	carriageWidget->addMessageToGui(QString("Load from database"),QString("Loading data from database"),infoStr);
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c2missions where status='%1'").arg(nextStr),bDb);
	if (dataVV.count()==1)
	{
		int fromPosNumber=dataVV[0][0].toInt();
		int fromPosIndex=dataVV[0][1].toInt();
		int toPosNumber=dataVV[0][2].toInt();
		int toPosIndex=dataVV[0][3].toInt();
		if ((fromPosNumber!=0)&&(fromPosIndex!=0)&&(toPosNumber!=0)&&(toPosIndex!=0))
		{
			posClass *f=allPos->getPosWithPosIndex(fromPosNumber,fromPosIndex);
			posClass *t=allPos->getPosWithPosIndex(toPosNumber,toPosIndex);
			carriageWidget->addMessageToGui(QString("Found next mission"),QString("from %1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
			missionStart(f,t);
		}
		if ((fromPosNumber==0)&&(fromPosIndex==0)&&(toPosNumber==0)&&(toPosIndex==0))
			carriageWidget->addMessageToGui(QString("No next missions"),QString("No next missions found in database"),infoStr);
	}
}
void carriageClass::loadFromDatabase()
{
    if (!basketCraneConfig().databaseWritesAllowed()) {
        // Compare database missions without running recovery prompts or transitions.
        pos->loadFromDatabase();
        carriageWidget->setActiveBasket(0); carriageWidget->setActiveFrom(""); carriageWidget->setActiveTo("");
        carriageWidget->setNextBasket(0); carriageWidget->setNextFrom(""); carriageWidget->setNextTo("");
        // These flags only update the display; observer mode never starts missions.
        foreach (posClass *position, allPos->displayPositions()) {
            if (position->getIsFrom()) position->setIsFrom(false);
            if (position->displayIsTarget()) position->setIsTo(false);
        }
        const QVector<QVector<QVariant>> rows = execTableQuery(
            "select status,fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c2missions", bDb);
        foreach (const QVector<QVariant> &row, rows) {
            posClass *from = allPos->getPosWithPosIndex(row[1].toInt(), row[2].toInt(), false);
            posClass *to = allPos->getPosWithPosIndex(row[3].toInt(), row[4].toInt(), false);
            const QString fromText = from ? from->getDescription() : QString();
            const QString toText = to ? to->getDescription() : QString();
            if (row[0].toString().trimmed() == activeStr) {
                // The source empties at pickup and the hooks empty at delivery.
                const int basket = !from || !to ? 0 : pos->getBasketNumber() ? pos->getBasketNumber()
                    : (from && from->getBasketNumber() ? from->getBasketNumber() : (to ? to->getBasketNumber() : 0));
                carriageWidget->setActiveBasket(basket);
                carriageWidget->setActiveFrom(fromText); carriageWidget->setActiveTo(toText);
                if (from) from->setIsFrom(true);
                if (to) to->setIsTo(true);
            } else if (row[0].toString().trimmed() == nextStr) {
                carriageWidget->setNextBasket(from ? from->getBasketNumber() : 0);
                carriageWidget->setNextFrom(fromText); carriageWidget->setNextTo(toText);
            }
        }
        return;
    }
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c2missions where status='%1'").arg(activeStr),bDb);
	if (dataVV.count()==1)
	{
		int fromPosNumber=dataVV[0][0].toInt();
		int fromPosIndex=dataVV[0][1].toInt();
		int toPosNumber=dataVV[0][2].toInt();
		int toPosIndex=dataVV[0][3].toInt();
		if ((fromPosNumber!=0)&&(fromPosIndex!=0)&&(toPosNumber!=0)&&(toPosIndex!=0))
		{
			QMessageBox::critical(this,"Mission",tr("Found mission from %1 to %2.").arg(fromPosNumber+fromPosIndex).arg(toPosNumber+toPosIndex));
			if (QMessageBox::question(this,tr("Found mission"),tr("If the programm does not start select yes to clear the mission. You have to correct the baskets numbers in the programm"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes)
			{
				clearMissionInDb("user");
				return;
			}			
			posClass *f=allPos->getPosWithPosIndex(fromPosNumber,fromPosIndex);
			posClass *t=allPos->getPosWithPosIndex(toPosNumber,toPosIndex);
			setActiveFromPos(f);
			setActiveToPos(t);
			carriageWidget->addMessageToGui(QString("Found active mission"),QString("from %1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
		}
		else
			carriageWidget->addMessageToGui(QString("No missions"),QString("No active missions found in database"),infoStr);
	}
}
bool carriageClass::getIsIdle()
{
	return currentMissionPLcStatus==0;
}
bool carriageClass::isReadyForNewMission()
{
	return (nextMissionFrom==0)&&(nextMissionTo==0)&&(nextMissionStatus==plcStatusIdle)&&(nextFromPos==NULL)&&(nextToPos==NULL)&&(!pos->getHasBasket())/*&&(basketNum==0)*/;
}
bool carriageClass::getCanStartNewMission(posClass* f,posClass* t)
{
	if (!isReadyForNewMission())
		return false;
	if (tuxip->getHasError())
		return false;
	if ((!f)||(!t))
		return false;
	if (!f->getHasBasket())
		return false;
	if (t->getHasBasket())
		return false;
	if (f->getIsLocked())
	{
		carriageWidget->addMessageToGui(QString("Can not start new mission from:%1").arg(f->getPosIndexDisplayString()),"Position is locked",infoStr);
		log(QString("Can not start new mission from:%1, position is locked").arg(f->getPosIndexDisplayString()),errorStr);
		return false;
	}
	if (t->getIsLocked())
	{
		carriageWidget->addMessageToGui(QString("Can not start new mission to:%1").arg(t->getPosIndexDisplayString()),"Position is locked",infoStr);
		log(QString("Can not start new mission to:%1, position is locked").arg(t->getPosIndexDisplayString()),errorStr);
		return false;
	}
	if (!t->getCanGetBasketInPos(f))
		return false;
	if (!f->getIsFreeToMove())
		return false;
	if (!t->getIsFreeToMove())
		return false;
	if (isPosActiveG(f->getPositionNumber()))
		return false;
	if (isPosActiveG(t->getPositionNumber()))
		return false;
	if (!isReadyForNewMission())
	{
		carriageWidget->addMessageToGui(QString("Can not start new mission"),"It is not ready to start a new mission",infoStr);
		log(QString("startNewMission f=%1 t=%2 s=%3 ").arg(f->getPosIndexDisplayString()).arg(t->getPosIndexDisplayString()).arg(currentMissionPLcStatus),errorStr);
		return false;
	}
	QVector<QVector<QVariant>> nDataVV=execTableQuery(QString("select fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c2missions where status='%1'").arg(nextStr),bDb);
	int nextFromPosNumber= nDataVV[0][0].toInt();
	int nextFromPosIndex= nDataVV[0][1].toInt();
	int nextToPosNumber= nDataVV[0][2].toInt();
	int nextToPosIndex= nDataVV[0][3].toInt();
	if (  
		(nextFromPosNumber==f->getPositionNumber())&&(nextFromPosIndex=f->getPositionIndex())
		&&
		(nextToPosNumber==t->getPositionNumber())&&(nextToPosIndex=t->getPositionIndex())
		)
	{
		log(QString("missionStart f:%1 t:%2 allready next").arg(f->getPosIndexDisplayString()).arg(t->getPosIndexDisplayString()),infoStr);
		return false;
	}
	QVector<QVector<QVariant>> aDataVV=execTableQuery(QString("select fromPosNumber,fromPosIndex,toPosNumber,toPosIndex from c2missions where status='%1'").arg(activeStr),bDb);
	int activeFromPosNumber= aDataVV[0][0].toInt();
	int activeFromPosIndex= aDataVV[0][1].toInt();
	int activeToPosNumber= aDataVV[0][2].toInt();
	int activeToPosIndex= aDataVV[0][3].toInt();
	if (  
		(activeFromPosNumber==f->getPositionNumber())&&(activeFromPosIndex=f->getPositionIndex())
		&&
		(activeToPosNumber==t->getPositionNumber())&&(activeToPosIndex=t->getPositionIndex())
		)
	{
		log(QString("missionStart f:%1 t:%2 allready active").arg(f->getPosIndexDisplayString()).arg(t->getPosIndexDisplayString()),infoStr);
		return false;
	}
	return true;
}
void carriageClass::missionStart(posClass* f,posClass*t)
{	
	if (!getCanStartNewMission(f,t))
		return;
	setNextFromPos(f);
	setNextToPos(t);
	logV(QString("startNewMission f=%1 t=%2").arg(nextFromPos->getPosIndexDisplayString()).arg(nextToPos->getPosIndexDisplayString()));
	basketExecQuery(QString("update c2missions set fromPosNumber=%1,fromPosIndex=%2,toPosNumber=%3,toPosIndex=%4 where status='%5'").arg(nextFromPos->getPositionNumber()).arg(nextFromPos->getPositionIndex()).arg(nextToPos->getPositionNumber()).arg(nextToPos->getPositionIndex()).arg(nextStr),bDb);
	carriageWidget->addMessageToGui(QString("New mission for basket #%1: %2 to %3").arg(carriageWidget->nextBasket()).arg(f->getDescription()).arg(t->getDescription()),"Starting new mission",infoStr);
	if ((!nextFromPos)||(!nextToPos))
	{
		log(QString("missionStart ((!fromPos)||(!toPos))"),errorStr);
		return;
	}
	if (nextFromPos->getBasketNumber()==0)
	{
		log(QString("missionStart fromPos->getBasketNumber()==0 from:%1").arg(nextFromPos->getPosIndexDisplayString()),errorStr);
		return;
	}
	carriageWidget->addMessageToGui(QString("Basket #%1: %2 to %3").arg(carriageWidget->nextBasket()).arg(f->getDescription()).arg(t->getDescription()),QString("Starting mission:%1 to %2").arg(f->getDescription()).arg(t->getDescription()),infoStr);
	missionSendToPlc();
}
void carriageClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((plcIp==ip)&&(plcMissionTable==table))
	{
		if (index==plcNextMissionFromIndex)nextMissionFrom=value.toInt();
		if (index==plcNextMissionToIndex)nextMissionTo=value.toInt();
		if (index==plcNextMissionStatusIndex)nextMissionStatus=value.toInt();
		if (index==plcCurrentMissionFromIndex)currentMissionFrom=value.toInt();
		if (index==plcCurrentMissionToIndex)currentMissionTo=value.toInt();
		if (index==plcCurrentMissionPcStatusIndex)currentMissionPcStatus=value.toInt();
		if (index==plcCurrentMissionPLcStatusIndex){plcStatusChanged(value.toInt());}
		if (index==plcCurrentCranePlcStepIndex){carriageWidget->setActiveCraneStep(value.toInt());}
	}
}
void carriageClass::checkStatusTimerSlot()
{
	if (!basketCraneConfig().databaseWritesAllowed()) { loadFromDatabase(); carriageWidget->loadFromDataBase(); return; }
	plcStatusChanged(currentMissionPLcStatus);
	carriageWidget->loadFromDataBase();
}
void carriageClass::plcStatusChanged(int s)
{
	currentMissionPLcStatus=s;
    if (basketCraneConfig().observer()) return;
	if (currentMissionPLcStatus==plcStatusIdle)
	{
		if (activeFromPos!=NULL)
			log(QString("carriageClass::setStatus plcStatus:%1 and activeFromPos!=NULL").arg(currentMissionPLcStatus),errorStr);
		if (activeToPos!=NULL)
			log(QString("carriageClass::setStatus plcStatus:%1 and activeToPos!=NULL").arg(currentMissionPLcStatus),errorStr);
		if ((activeFromPos!=NULL)||(activeToPos!=NULL))
		{
			carriageWidget->addMessageToGui(QString("Plc status 0"),"Pc has active mission and plc is with status 0",errorStr);
			log(QString("plcStatus==0 and pc in a mission"),errorStr);
		}
	}
	else if (currentMissionPLcStatus==plcStatusStart)
	{
		if (moveNextToActive())
		{
#ifdef logMissionsTimes
		basketExecQuery(QString("insert into missionsTimeLog (fromPos,toPos,startSec,endSec,time) values(%1,%2,%3,0,'%4')").arg(activeFromPos->getPlcId()).arg(activeToPos->getPlcId()).arg(QDateTime::currentDateTime().toTime_t()).arg(timeForLog),bDb);
#endif
		{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusStart); }
		}
	}
	else if (currentMissionPLcStatus==plcStatusBascketOnConv)
		missionBasketOnConveyor();
	else if (currentMissionPLcStatus==plcStatusFinished)
		missionFinished();
	else if (currentMissionPLcStatus==plcStatusAborted)
		missionAborted();
}

void carriageClass::missionSendToPlc(bool force)
{
	if ((!nextFromPos)||(!nextToPos))
	{
		log(QString("sendMissionToPlc ((!fromPos)||(!toPos))"),errorStr);
		return;
	}
	carriageWidget->addMessageToGui(QString("Send basket #%1 to plc: %2 to %3").arg(carriageWidget->nextBasket()).arg(nextFromPos->getPosIndexDisplayString()).arg(nextToPos->getPosIndexDisplayString()),QString("Sending data to plc"),infoStr);
	//if (nextMissionFrom!=nextFromPos->getIdForPlc())
	{
		{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcMissionTable,plcNextMissionFromIndex,nextFromPos->getIdForPlc()); }
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionFromIndex).arg(nextFromPos->getIdForPlc()));
	}
	//if (nextMissionTo!=nextToPos->getIdForPlc())
	{
		{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcMissionTable,plcNextMissionToIndex,nextToPos->getIdForPlc()); }
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionToIndex).arg(nextToPos->getIdForPlc()));
	}
	//status allways last to send
	//if (nextMissionStatus!=plcStatusStart)
	{
		{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcMissionTable,plcNextMissionStatusIndex,plcStatusStart); }
		logV(QString("missionSendToToPlc.writeInteger %1[%2]=%3").arg(plcMissionTable).arg(plcNextMissionStatusIndex).arg(plcStatusStart));
	}	
}
int carriageClass::moveBasket(posClass *from,posClass *to)
{
	if ((from==NULL)||(to==NULL))
	{
		log(QString("moveBasket ((from==NULL)||(to==NULL))"),errorStr);
		uExit(errorStr,QString("moveBasket ((from==NULL)||(to==NULL))"));
	}
	int basket=allPos->moveBasket(from,to);
	carriageWidget->addMessageToGui(QString("Move basket #%1 from %2 to %3").arg(basket).arg(from->getIdForPlc()).arg(to->getIdForPlc()),QString("Moving basket #%1").arg(basket),infoStr);
	if (from==pos)
		pos->loadFromDatabase();
	else
		from->loadFromDatabase();
	if (to==pos)
		pos->loadFromDatabase();
	else
		to->loadFromDatabase();
	return basket;
}
void carriageClass::clearNextMission()
{
	setNextFromPos(0);
	setNextToPos(0);
	{ if (!basketCraneConfig().observer()) tuxip->writeInteger("toPcI",0,0); }
	{ if (!basketCraneConfig().observer()) tuxip->writeInteger("toPcI",1,0); }
	{ if (!basketCraneConfig().observer()) tuxip->writeInteger("toPcI",2,0); }
}
void carriageClass::clearMissionInDb(QString user)
{
	carriageWidget->addMessageToGui(QString("Clear mission"),QString("Mission has ended from %1, clearing data from database").arg(user.length()>0?user:"plc"),infoStr);
	basketExecQuery(QString("update c2missions set fromPosNumber=0,fromPosIndex=0,toPosNumber=0,toPosIndex=0 where status='%1'").arg(nextStr),bDb);
	basketExecQuery(QString("update c2missions set fromPosNumber=0,fromPosIndex=0,toPosNumber=0,toPosIndex=0 where status='%1'").arg(activeStr),bDb);

	setActiveFromPos(NULL);
	setActiveToPos(NULL);
	{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusFinished); }
}
void carriageClass::missionBasketOnConveyor()
{
	if (activeFromPos)
	{
		if ((activeFromPos->getBasketNumber()!=0) && (pos->getBasketNumber()==0))
		{
			bool isToPosEmpty=activeToPos->getBasketNumber()==0;
			if (isToPosEmpty)
			{
				int b=moveBasket(activeFromPos,pos);
				{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusBascketOnConv); }
			}
		}
		else
		{
			//if (activeFromPos->getBasketNumber()==0)
			//	carriageWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket on conveyor and pc has no basket in source position."),errorStr);
			//if (pos->getBasketNumber()!=0)
			//	carriageWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket on conveyor and pc allready has a basket on conveyor."),errorStr);
		}
	}
	else
		carriageWidget->addMessageToGui(QString("Basket on conveyor"),QString("Plc moved basket on conveyor and pc has no active mission."),errorStr);
}
void carriageClass::missionFinished()
{
	carriageWidget->addMessageToGui(QString("Plc: mission finished"),QString("Mission has ended from plc"),infoStr);
	if (activeToPos&&(activeToPos->getBasketNumber()==0) && (pos->getBasketNumber()!=0))
	{
#ifdef logMissionsTimes
		basketExecQuery(QString("update missionsTimeLog set endSec=%1 where fromPos=%2 and toPos=%3 and endSec=0").arg(QDateTime::currentDateTime().toTime_t()).arg(activeFromPos->getPlcId()).arg(activeToPos->getPlcId()),bDb);
#endif
		int b=moveBasket(pos,activeToPos);
        // Export queues survive internal uncovering moves. Clear them only
        // after delivery to the shared export conveyor (after the A1/A2 handoff).
        if (activeToPos->getPositionNumber()==toRotatingNorthPlcId)
            allPos->removeBasketFromExports(b);
		emit exportListChangedSignal();
		if (activeToPos->getPositionNumber()==700)
			allPos->checkReturningConveyor();
		clearMissionInDb();
		{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusFinished); }
		logV(QString("missionFinished "));
	}
	else if (activeFromPos&&activeToPos)
	{
		if (activeToPos&&(activeToPos->getBasketNumber()!=0))
			carriageWidget->addMessageToGui(QString("Basket move"),QString("Plc moved basket to destination and pc has allready a basket there."),errorStr);
		if (pos->getBasketNumber()==0)
			carriageWidget->addMessageToGui(QString("Basket move"),QString("Plc moved basket to destination and pc has no basket on hooks."),errorStr);
	}
}
void carriageClass::missionAborted()
{
	carriageWidget->addMessageToGui(QString("Mission was aborted"),QString("Mission has aborted by plc"),infoStr);
	int movedBasket=0;
	if (activeToPos&&activeToPos->getBasketNumber()!=0)
	{
		movedBasket=moveBasket(activeToPos,activeFromPos);
	}
	if ((pos->getBasketNumber()!=0)&&(activeFromPos))
	{
		movedBasket=moveBasket(pos,activeFromPos);
	}
	if (movedBasket==0)
	{
		clearMissionInDb();
		{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcMissionTable,plcCurrentMissionPcStatusIndex,plcStatusAborted); }
		logV(QString("missionAborted"));
	}
	else
		carriageWidget->addMessageToGui(QString("Mission aborted"),QString("Mission was aborted from plc and there is no basket on conveyor or the source position."),errorStr);
}




int carriageClass::displayBasketNumber() const { return pos->getBasketNumber(); }

QStringList carriageClass::displayBasketRows() const { return pos->displayBasketRows(); }
QString carriageClass::displayDestination() const { return pos->displayDestination(); }
