#include "automaticMissions.h"
#include "exportQueues.h"
#include "pos.h"
#include "allPos.h"
#include "observerDisplay.h"
#include "plantAppearance.h"
double textScale=15.;
double exportTextScale=25.;
posClass::posClass(allPossClass *allPos_,int posNumber_,int posIndex_,float plcXPos_,float plcYPos_,float plcZPos_,float gXOffset_,float gYOffset_,
	QGraphicsScene *scene_,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_
	,bool handleMousePressed,QWidget *p)
{
	tuxip=NULL;
	plcIndex=-1;
	destination=destinationNone;
	exportGroup=0;
	exportGroupQueue=0;
	isOnExportList=false;
	setVisible(false);
	allPos=allPos_;
	posNumber=posNumber_;
	posIndex=posIndex_;
	plcXPos=plcXPos_;
	plcYPos=plcYPos_;
	plcZPos=plcZPos_;
	gXOffset=gXOffset_;
	gYOffset=gYOffset_;
	abovePos=belowPos=NULL;
	displayMode=displayToSelectFrom;
	scene=scene_;
	isFrom=isTo=false;
	basketNum=-1;
    isLocked=false;
    basketWithContents=false;
	plcDestination=0;
	if (posNumber==1070)
		double a=0.;
	isCranePlcPos=(posNumber==toRotatingSouthPlcId)||(posNumber==toRotatingNorthPlcId)||(posNumber==rotatingNorthPlcId)||(posNumber==rotatingSouthPlcId)
		||(posNumber==exit1PlcId)||(posNumber==exit2PlcId)||(posNumber==stackerWaitingPlcId);
	isCranePos=!((posNumber==rotatingNorthPlcId)||(posNumber==rotatingSouthPlcId)||(posNumber==exit1PlcId)||(posNumber==exit2PlcId)
		||(posNumber==stackerWaitingPlcId)||(posNumber==stackerWorkingingPlcId));
	//
	//isLocked=isPlcPos=(plcId>craneBPlcId);//=getFirst(QString("select tid from pos where plcId=%1").arg(plcId),bDb).toInt()==0;
	//if (    ((plcId<0)&&(plcId<cranePlcId))   ||   ((plcId==station6PlcId)||(plcId==station7PlcId)||(plcId==station8PlcId) )  )
	//	yOffset=gYMinusOffset; 
	//else if ((plcId>0)&&(plcId<cranePlcId))   
	//	yOffset=gYPlussOffset;
	fromItem=new gItemClass(fromItem_,0.,0.,this);scene->addItem(fromItem);
	toItem=new gItemClass(toItem_,0.,0.,this);scene->addItem(toItem);
    fromItem->setPen(QPen(QColor("#f472b6")));
    toItem->setPen(QPen(QColor("#38bdf8")));
	basketItem=new gItemClass(basketItem_,0.,0.,this);scene->addItem(basketItem);
	profilesItem=new gItemClass(profilesItem_,0.,0.,this);scene->addItem(profilesItem);
	lockedItem=new gItemClass(lockedItem_,0.,0.,this);scene->addItem(lockedItem);
	mouseOverItem=new gItemClass(mouseOverItem_,0.,0.,this);scene->addItem(mouseOverItem);mouseOverItem->setCursor(Qt::ClosedHandCursor);
	posItem=new gItemClass(posItem_,0.,0.,this);scene->addItem(posItem);
    new PlantBasketCard(posItem->boundingRect(),basketItem);
    connect(mouseOverItem,&gItemClass::hoverEnterSignal,this,[this]() { if (PlantBasketCard *card=plantBasketCard(basketItem)) card->setHovered(true); });
    connect(mouseOverItem,&gItemClass::hoverLeaveSignal,this,[this]() { if (PlantBasketCard *card=plantBasketCard(basketItem)) card->setHovered(false); });

	posItemText=new QGraphicsSimpleTextItem();
	scene->addItem(posItemText);
	posItemText->setZValue(10);
	txtTransformation.scale(textScale,-textScale);
	posItemText->setTransform(txtTransformation,false);

	exportItemText=new QGraphicsSimpleTextItem();
	scene->addItem(exportItemText);
	exportItemText->setZValue(10);
	exportItemText->setBrush(Qt::NoBrush);
	txtTransformation.reset();
	txtTransformation.scale(exportTextScale,-exportTextScale);
	exportItemText->setTransform(txtTransformation,false);

	txtTransformation.reset();
	txtTransformation.scale(textScale,-textScale);
	for (int i=0;i<4;i++)
	{
		profileTxts<<new QGraphicsSimpleTextItem();
		profileTxts.last()->setPen(QPen(QColor(0,200,200)));
		profileTxts[i]->setBrush(Qt::NoBrush);
		scene->addItem(profileTxts.last());
		profileTxts.last()->setZValue(10);
		profileTxts.last()->setTransform(txtTransformation,false);
	}
	setPlcPos(plcXPos,plcYPos,plcZPos);
	if (handleMousePressed)
		connect(mouseOverItem,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
}
posClass::~posClass()
{
}
bool posClass::getIsFreeToMove()
{
	for (int i=0;i<obstaclePosV.count();i++)
	{
		if (obstaclePosV[i]->getHasBasket())
			return false;
	}
	if (
		((abovePos)&&(abovePos->getHasBasket()))
		||
		((belowPos)&&(!belowPos->getHasBasket()))
		)
		return false;
	return true;
}
void posClass::setDisplayMode(int m)
{
	if (isCranePos)
	{
		displayMode=m;
		updateItemsVisibility();
        updateTxt();
	}
}
void posClass::setAboveBelowPos(posClass *a,posClass *b)
{
	abovePos=a;
	belowPos=b;
}
void posClass::setObstaclePos(QVector<posClass*> obstaclePosV_)
{
	obstaclePosV=obstaclePosV_;
}
void posClass::setPlcPos(float x,float y,float z)
{
	float bWidth=1500.f;
	plcXPos=x;
	plcYPos=y;
	plcZPos=z;
	gXPos=gXOffset+plcXPos+posIndex*bWidth;
	gYPos=plcYPos+gYOffset;
	posItem->setPos(gXPos,gYPos);
	mouseOverItem->setPos(gXPos,gYPos);
	basketItem->setPos(gXPos,gYPos);
	profilesItem->setPos(gXPos,gYPos);
	lockedItem->setPos(gXPos,gYPos);
	fromItem->setPos(gXPos,gYPos);
	toItem->setPos(gXPos,gYPos);
	updateTxt();
}
void posClass::setPlcAdress(tuxipClass *tuxip_,QString plcTable_,int index_)
{
	tuxip=tuxip_;
	plcTable=plcTable_;
	plcIndex=index_;
}
void posClass::updateTxt()
{
    // The card shares the basket's geometry; existing selection items own input.
    posItemText->setText(""); posItemText->setVisible(false);
    exportItemText->setText(""); exportItemText->setVisible(false);
    foreach (QGraphicsSimpleTextItem *text, profileTxts) { text->setText(""); text->setVisible(false); }
    PlantBasketCard *card=plantBasketCard(basketItem);
    if (!card) return;
    QString destinationText;
    if (basketCraneConfig().observer()) destinationText=contentsVV.isEmpty()?"None":observerDestination(contentsVV.last());
    else if (destination==destinationDestacker) destinationText="D";
    else if (destination==destinationPacking) destinationText="P";
    QStringList rows;
    foreach (const QVector<QVariant>& row, contentsVV) {
        QString text=QString("%1:%2").arg(row[1].toString().trimmed()).arg(row[2].toInt());
        if (basketCraneConfig().observer()) text += " / " + observerDestination(row);
        rows << text;
    }
    if (rows.isEmpty()) rows << "Empty basket";
    card->setBasket(basketNum,destinationText,rows,isLocked,isFrom,isTo);
    card->setVisible(basketNum>0 && displayMode!=displayNone);
    card->setToolTip(QString("Basket %1 | %2\nDestination: %3%4\n%5").arg(basketNum).arg(description).arg(destinationText)
        .arg(isLocked?" | LOCKED":QString()).arg(rows.join("\n")));
}
void posClass::setBasketId(int b,bool user,QString logInfo,int doNotCheckPosition,int doNotCheckIndex)
{
	if ((basketNum!=0)&&(b!=0)&&(!user))
	{
		if (posNumber!=doNotCheckPosition)
			uExit(errorStr,QString("trying to declare basket %1 in position %2.%3, position %2.%3 allready has basket %4 declared").arg(b).arg(posNumber).arg(posIndex).arg(basketNum));
	}
	{
		QString posOfBasket=getDescriptionOfPosWithBasket(b,QVector<int>()<<doNotCheckPosition<<posNumber,QVector<int>()<<doNotCheckIndex<<posIndex);
		if (posOfBasket.isEmpty())
		{
			basketExecQuery(QString("update positions set basketTableId=%1 where posNumber=%2 and posIndex=%3").arg(b).arg(posNumber).arg(posIndex),bDb);
			basketExecQuery(QString("insert into c2logPos (pos,basket,info,time) values(%1,%2,'%3','%4')").arg(getPosIndexDisplayString()).arg(b).arg(logInfo).arg(timeForLog),bDb);
			if (tuxip)
			{
				{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcTable,plcIndex,b); }
                int dest = 0;
                if (b > 0) {
                    foreach (const BasketExportQueue& queue, basketExportQueues()) {
                        if (getFirst(QString("select count(tId) from %1 where basket=%2").arg(queue.table).arg(b), bDb).toInt()) {
                            dest = basket::ExportPlcDestination(queue.table, getMaximumLength());
                            if (posNumber==toRotatingNorthPlcId && dest==basket::DestinationCatalog::PlcValue(basket::DestinationId::PackingDestacker)) {
                                const QString downstreamQueue = queue.table=="c2exportsPackingDestacker1"
                                    ? "c3exportsDestackerPosition1" : "c3exportsDestackerPosition2";
                                basketExecTransaction(QVector<basket::SqlCommand>() << basket::SqlCommand(
                                    QString("insert into %1 (basket,priority) select ?,isnull((select max(priority) from %1),0)+1 where not exists (select 1 from %1 where basket=?)").arg(downstreamQueue),
                                    QVector<QVariant>() << b << b), bDb);
                            }
                            break;
                        }
                    }
                }
				if (tuxip->getIp()==crane2Ip)
				{
					{ if (!basketCraneConfig().observer()) tuxip->writeInteger("bConveyorData",plcIndex,dest); }
					plcDestination=dest;
				}
			}
			loadFromDatabase();
		}
	}
	emit posChangedSignal();
}
void posClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)//for plc driver positions:isPlcPos==true
{
	if ((tuxip)&&(!tuxip->getHasError())&&(tuxip->getIp()==ip)&&(index==plcIndex))
	{
		if (table==plcTable)
		{
			basketExecQuery(QString("update positions set basketTableId=%1 where posNumber=%2 and posIndex=%3").arg(value.toInt()).arg(posNumber).arg(posIndex),bDb);
			basketExecQuery(QString("insert into c2logPos (pos,basket,info,time) values(%1,%2,'%3','%4')").arg(getPosIndexDisplayString()).arg(value.toInt()).arg("plc").arg(timeForLog),bDb);
			loadFromDatabase();
		}
		if (table=="bConveyorData")
			plcDestination=value.toInt();
	}
}
int posClass::getBasketForInFromOut()
{
	if (getHasBasket())
	{
		if (plcDestination==basket::DestinationCatalog::PlcValue(basket::DestinationId::InFromOut))
			return getBasketNumber();
	}
	return 0;
}
int posClass::getBasketForInFromDestacker()
{
	if (getHasBasket())
	{
		if (plcDestination==basket::DestinationCatalog::PlcValue(basket::DestinationId::InFromDestacker))
			return getBasketNumber();
	}
	return 0;
}
int posClass::getPositionNumber()
{
	return posNumber;
}
int posClass::getPositionIndex()
{
	return posIndex;
}
QString posClass::getPosIndexDisplayString()
{
	return QString("%1.%2").arg(posNumber).arg(posIndex);
}
void posClass::loadFromDatabaseSlot()
{
	loadFromDatabase();
}
void posClass::loadFromDatabase()
{
	if (posNumber==700)
		double a=0.;
	QVector<QVector<QVariant>> posVV=execTableQuery(QString("select posNumber,posIndex,description,basketTableId,locked from positions where posNumber=%1 and posIndex=%3").arg(posNumber).arg(posIndex),bDb);
	if (posVV.count()==1)
	{
		int b=posVV[0][3].toInt();
		if (b!=0)
            contentsVV= basketCraneConfig().observer()
                ? execTableQuery(observerContentsSql(b),aDb2)
                : execTableQuery(QString("select basketnum,dienum,Pc,PcLen,temper from basket_details where basketnum=%1 and Pc<>0").arg(b),aDb);
		else
		{
			contentsVV.clear();
			destination=destinationNone;
			exportGroup=0;
			exportGroupQueue=0;
		}
		updateFromData(posVV,contentsVV);
	}
}
void posClass::loadFromAllData(QVector<QVector<QVariant>> allPosVV,QVector<QVector<QVariant>> allContentsVV)
{
	QVector<QVector<QVariant>> myPosVV;
	QVector<QVector<QVariant>> myContentsVV;
	for (int i=0;i<allPosVV.count();i++)
	{
		if (  (allPosVV[i][0].toInt()==posNumber)&&(allPosVV[i][1].toInt()==posIndex)  )
			myPosVV<<allPosVV[i];
	}
	if (myPosVV.count()==1)
	{
		int b=myPosVV[0][3].toInt();
		for (int i=0;i<allContentsVV.count();i++)
		{
			if (allContentsVV[i][0].toInt()==b)
				myContentsVV<<allContentsVV[i];
		}
	}
	updateFromData(myPosVV,myContentsVV);
}
void posClass::updateFromData(QVector<QVector<QVariant>> posVV,QVector<QVector<QVariant>> contentsVV_)
{
	contentsVV=contentsVV_;
	if (posVV.count()==1)
	{
		int b=posVV[0][3].toInt();
		if (basketNum!=b)
		{
			basketNum=b;
			basketWithContents=contentsVV.count()>0?true:false;
		}
		description=posVV[0][2].toString().trimmed();
		isLocked=(posVV[0][4].toInt()==2);
		updateGui();
	}
}
void posClass::updateItemsVisibility()
{
	if ((posNumber+posIndex)==1221)
		double a=0.;
	fromItem->setVisible(isFrom);
	toItem->setVisible(isTo);
	bool hasObstacle=false;
	for (int i=0;i<obstaclePosV.count();i++)
	{
		if (obstaclePosV[i]->getHasBasket())
			hasObstacle=true;
	}
	if (displayMode==displayNone)
	{
		posItem->setVisible(false);
		mouseOverItem->setVisible(false);
		basketItem->setVisible(false);
		profilesItem->setVisible(false);
		lockedItem->setVisible(false);
		posItemText->setVisible(false);
		for (int i=0;i<profileTxts.count();i++)
			profileTxts[i]->setVisible(false);
	}
	if (displayMode==displayNormal)
	{
		posItem->setVisible(false);
		mouseOverItem->setVisible(false);
		posItemText->setVisible(true);
		for (int i=0;i<profileTxts.count();i++)
			profileTxts[i]->setVisible(true);
	}
	if (displayMode==displayToModify)
	{
		bool canBeSelected=false;
		if (
			(isCranePos)
			&&
			((!abovePos)||(!abovePos->getHasBasket()))
			&&
			((!belowPos)||(belowPos->getHasBasket()))
			)
			canBeSelected=true;
		posItem->setVisible(canBeSelected);
		mouseOverItem->setVisible(canBeSelected);
		posItemText->setVisible(true);	
		for (int i=0;i<profileTxts.count();i++)
			profileTxts[i]->setVisible(true);
	}
	if (displayMode==displayToSelectFrom)
	{
		bool canBeSelected=false;
		if ( 
			(isCranePos)
			&&
			((getHasBasket())&&(getIsFreeToMove()))
			&&
			((!abovePos)||(!abovePos->getHasBasket()))
			&&
			((!belowPos)||((belowPos->getHasBasket())&&(!belowPos->getIsFrom())))
			&&
			( (posNumber!=660)&&(posNumber!=680))
			)
		{
			canBeSelected=true;
		}
		posItem->setVisible(canBeSelected);
		mouseOverItem->setVisible(canBeSelected);
		posItemText->setVisible(true);
		for (int i=0;i<profileTxts.count();i++)
			profileTxts[i]->setVisible(true);
	}
	if (displayMode==displayToSelectTo)
	{
		bool canBeSelected=false;
		if ( 
			(isCranePos)
			&&
			((!getHasBasket())&&(getIsFreeToMove()) )
			&&
			((!abovePos)||(!abovePos->getHasBasket()))
			&&
			((!belowPos)||(((belowPos->getHasBasket())&&(!belowPos->getIsFrom()))))
			&&
			( (posNumber!=660)&&(posNumber!=680))
			)
		{
			canBeSelected=true;
		}
		mouseOverItem->setVisible(canBeSelected);
		posItem->setVisible(canBeSelected);
		basketItem->setVisible(getHasBasket());
		posItemText->setVisible(true);
		for (int i=0;i<profileTxts.count();i++)
			profileTxts[i]->setVisible(true);
	}
	if (displayMode==displayToExport)
	{
		bool canBeSelected=false;
		if (
			(isCranePos)
			&&
			(getHasBasket())
			)			
		{
			canBeSelected=true;
		}
		mouseOverItem->setVisible(canBeSelected);
		posItem->setVisible(canBeSelected);
		posItemText->setVisible(true);
		for (int i=0;i<profileTxts.count();i++)
			profileTxts[i]->setVisible(true);
	}
}
void setIsDestackerExportBasket(bool v)
{
}
void setIsPackingExportBasket(bool v)
{
}
void posClass::updateGui()
{
	QString toolTipTxt=QString("%1<br>Position:%2<br>x:%3mm,y:%4mm,z:%5mm<br>").arg(description).arg(posNumber+posIndex).arg(plcXPos).arg(plcYPos).arg(plcZPos);
	updateItemsVisibility();
	updateExportText();
	basketItem->setVisible(basketNum!=0);
	profilesItem->setVisible(basketWithContents);
	lockedItem->setVisible(isLocked);
	if (basketNum!=0)
	{//			contentsVV=execTableQuery(QString("select basketnum,dienum,Pc,PcLen,temper from basket_details where basketnum=%1 and Pc<>0").arg(b),aDb);
		toolTipTxt+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Basket number:")+"</font><font style='color:#000077'>"+QString("%1").arg(basketNum)+"</font><br>";
		if (basketWithContents)
		{
			toolTipTxt+=QString(QString("<table><tr>"));
			for (int i=(contentsVV.count()-1);i>=0;i--)
			{
				toolTipTxt+=QString(QString("<td>"))+
					QString("%1<br>").arg(contentsVV[i][1].toString().trimmed())+
					tr("pieces:%1<br>").arg(contentsVV[i][2].toInt())+
					tr("length:%1mm<br>").arg(contentsVV[i][3].toInt())+
					tr("temper:%1<br>").arg(contentsVV[i][4].toString())
					;
				toolTipTxt+=QString(QString("</td>"));
			}
			toolTipTxt+=QString(QString("</tr></table><br><br>"));
		}
		else
		{
			toolTipTxt+=QString(tr("The basket is empty.<br>"));
		}
	}
	else
	{
		toolTipTxt+=QString("<font style='color:#000077'>")+tr("No basket")+"</font><br>";
	}
	toolTipTxt+=QString(QString("</body></html>"));
	posItem->setToolTip(toolTipTxt);
	updateTxt();
	scene->update();
}

void posClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if ((ev->modifiers()==Qt::ShiftModifier)&&(ev->button()==Qt::LeftButton))
	{
//		basketExecQuery(QString("update pos set locked=%1 where plcId=%2").arg(isLocked?0:1).arg(plcId),bDb);
		loadFromDatabase();
		return;
	}
	if ((ev->button()==Qt::LeftButton)||(ev->button()==Qt::RightButton))
	{
		if (ev->modifiers()==Qt::ControlModifier)
		{
			showPositionDlgSlotSlot();
			emit userChangedBasketSignal(this);
		}
		else
			emit posMouseReleasedSignal(this,ev);
	}
}
void posClass::showPositionDlgSlotSlot()
{
	showPositionDlg(0);
}
void posClass::showPositionDlg(int activeTab,int type,bool checkPass)
{
	if (!isCranePlcPos)
	{
		posDlgClass *posDlg=new posDlgClass(allPos,this,"user",this);
		if (posDlg->myExec(false))
		{
			bool isActive=getFirst(QString("select count(tId) from c2missions where (fromPosNumber=%1 and fromPosIndex=%2) or (toPosNumber=%1 and toPosIndex=%2)").arg(posNumber).arg(posIndex),bDb).toInt()!=0;
			if (!isActive)
				setBasketId(posDlg->getBasketNum(),true,"user");
			loadFromDatabase();
		}
		delete posDlg;
	}
	else
	{
		QMessageBox::information(this,tr("Information"),tr("This is a PLC controlled position.\nUse the conveyors program to access the position's data."));
	}
}
void posClass::setIsFrom(bool s)
{
	isFrom=s;
	updateItemsVisibility();
    updateTxt();
}
bool posClass::getIsFrom()
{
	return isFrom;
}
void posClass::setIsTo(bool s)
{
	isTo=s;
	updateItemsVisibility();
    updateTxt();
}
int posClass::getIdForPlc()
{
	return posNumber+posIndex;
}
//
int posClass::getBasketNumber()
{
	return basketNum;
}
bool posClass::getHasBasket()
{
	return basketNum!=0;
}
bool posClass::getHasFullBasket()
{
	return (basketNum!=0)&&(contentsVV.count()!=0);
}
bool posClass::getIsLocked()
{
	return isLocked;
}
float posClass::getPlcX()
{
	return plcXPos;
}
float posClass::getPlcY()
{
	return plcYPos;
}
float posClass::getPlcZ()
{
	return plcZPos;
}
QString posClass::getDescription()
{
	return description;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool posClass::getCanGetBasketInPos(posClass *p)
{
	if (posNumber==bufferEastOfOldOvenPlcId)
	{
		if (p->getMaximumLength()>6000.)
			return false;
	}
	if (getHasBasket())
		return false;
	if (!getIsFreeToMove())
		return false;
	if (abovePos&&abovePos->getHasBasket())
		return false;
	if (belowPos&&(!belowPos->getHasBasket()))
		return false;
	return true;
}
int posClass::getMaximumLength()
{
	int maxLength=0;
	for (int i=0;i<contentsVV.count();i++)
	{
		if (contentsVV[i][3].toInt()>maxLength)
			maxLength=contentsVV[i][3].toInt();
	}
	return maxLength;
}
void posClass::setDestination(int d)
{
	destination=d;
	updateExportText();
}
int posClass::getDestination()
{
	return destination;
}
void posClass::setExportGroup(int g)
{
	exportGroup=g;
	updateExportText();
}
int posClass::getExportGroup()
{
	return exportGroupQueue;
}
void posClass::setExportGroupQueue(int q)
{
	exportGroupQueue=q;
	updateExportText();
}
int posClass::getExportGroupQueue()
{
	return exportGroupQueue;
}
void posClass::updateExportText()
{
    if (basketCraneConfig().observer()) { updateTxt(); return; }
	if (destination==destinationDestacker)
	{
		exportItemText->setText(QString("D%1%2").arg(exportGroup?tr(":%1.%2").arg(exportGroup).arg(exportGroupQueue):"").arg(debugStr));
		exportItemText->setPen(QPen(QColor(0,200,0)));
		for (int i=0;i<profileTxts.count();i++)
			profileTxts[i]->setPen(QPen(QColor(0,200,0)));
	}
	else if (destination==destinationPacking)
	{
		exportItemText->setText(QString("P%1%2").arg(exportGroup?tr(":%1.%2").arg(exportGroup).arg(exportGroupQueue):"").arg(debugStr));
		exportItemText->setPen(QPen(QColor(0,0,200)));
		for (int i=0;i<profileTxts.count();i++)
			profileTxts[i]->setPen(QPen(QColor(0,0,200)));
	}
	else
		exportItemText->setText("");
    updateTxt();
}
QVector<int> posClass::getProfiles()
{
	QVector<int> ans;
	for (int i=0;i<contentsVV.count();i++)
		ans<<contentsVV[i][1].toInt();
	return ans;
}
int posClass::getTopProfile()
{
	if (contentsVV.count())
		return contentsVV.last()[1].toInt();
	return -1;
}
int posClass::getBottomProfile()
{
	if (contentsVV.count())
		return contentsVV[0][1].toInt();
	return -1;
}
bool posClass::getIsProfileStacked(int p)
{
	QVector<int> profiles=getProfiles();
	if (!profiles.contains(p))
		return true;
	if (p==getTopProfile())
		return true;
	return false;
}
void posClass::setIsOnExportList(bool is)
{
	isOnExportList=is;
    if (basketCraneConfig().observer()) { updateTxt(); return; }
	QBrush brush=Qt::NoBrush;
	if ((destination==destinationPacking)&&(isOnExportList))
		brush=QBrush(QColor(0,0,200));
	else if ((destination==destinationDestacker)&&(isOnExportList))
		brush=QBrush(QColor(0,200,0));

	for (int i=0;i<profileTxts.count();i++)
		profileTxts[i]->setBrush(brush);
	exportItemText->setBrush(brush);
}
bool posClass::getIsOnExportList()
{
	return isOnExportList;
}
QVector<posClass*> posClass::getPosThatPreventYouToMove()
{
	QVector<posClass*> ans;
	if (abovePos)
	{
		if (abovePos->getHasBasket())
		{
			if (!ans.contains(abovePos))
				ans<<abovePos;
		}
		ans<<abovePos->getPosThatPreventYouToMove();
	}
	for (int i=0;i<obstaclePosV.count();i++)
	{
		if (obstaclePosV[i]->getHasBasket())
		{
			if (!ans.contains(obstaclePosV[i]))
				ans<<obstaclePosV[i];
		}
		ans<<obstaclePosV[i]->getPosThatPreventYouToMove();
	}
	return ans;
}

QString posClass::displayDestination() const {
    if (basketCraneConfig().observer()) return contentsVV.isEmpty()?"None":observerDestination(contentsVV.last());
    return destination==destinationDestacker?"D":(destination==destinationPacking?"P":"None");
}
QString posClass::displayBasketDetails() const {
    QStringList rows;
    foreach (const QVector<QVariant>& row,contentsVV) {
        if (row.size()<4) continue;
        rows << QString("Profile %1: %2 pieces, %3 mm").arg(row[1].toString().trimmed()).arg(row[2].toInt()).arg(row[3].toInt());
    }
    return QString("Basket %1 | Position %2.%3 | %4%5\n%6").arg(basketNum).arg(posNumber).arg(posIndex)
        .arg(displayDestination()).arg(isLocked?" | LOCKED":"").arg(rows.isEmpty()?QString("Empty basket"):rows.join("; "));
}
QString posClass::displayHoverDetails() const {
    QStringList lines;
    lines << description << QString("Position: %1.%2").arg(posNumber).arg(posIndex)
        << QString("Basket: %1 | Destination: %2%3").arg(basketNum).arg(displayDestination()).arg(isLocked?" | LOCKED":"");
    foreach (const QVector<QVariant>& row,contentsVV) {
        if (row.size()<4) continue;
        lines << QString("\nProfile %1\nPieces: %2 | Length: %3 mm%4")
            .arg(row[1].toString().trimmed()).arg(row[2].toInt()).arg(row[3].toInt())
            .arg(row.size()>4?QString(" | Temper: %1").arg(row[4].toString().trimmed()):QString());
        if (row.size()>5) lines << "Current finish code: " + row[5].toString().trimmed();
        if (row.size()>6) lines << "Standard practice: " + row[6].toString().trimmed();
        if (row.size()>7) lines << "Customer: " + row[7].toString().trimmed();
        if (row.size()>8) lines << "Next department: " + row[8].toString().trimmed();
    }
    if (contentsVV.isEmpty()) lines << "Empty basket";
    return lines.join("\n");
}

QStringList posClass::displayBasketRows() const {
    QStringList rows;
    foreach (const QVector<QVariant>& row,contentsVV) {
        if (row.size()<3) continue;
        const QString destinationText=basketCraneConfig().observer()?observerDestination(row):displayDestination();
        rows << QString("%1:%2:%3").arg(row[1].toString().trimmed()).arg(row[2].toInt()).arg(destinationText);
    }
    if (rows.isEmpty()) rows << "Empty basket";
    return rows;
}
