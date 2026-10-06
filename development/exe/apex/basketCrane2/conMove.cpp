#include "conMove.h"
moveClass::moveClass(tuxipClass *tuxip_,QGraphicsScene *myScene_,
		int type_,
		QString plcIpAddress_,
		QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,
		QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,
		convPositionClass *fromPos_,convPositionClass *toPos_,
		gItemClass *tri_,
		float offset,
		QObject *p):QObject(p)
{
	tuxip=tuxip_;
	myScene=myScene_;
	fromPos=fromPos_;
	toPos=toPos_;
	isEnabled=false;
	type=type_;
	gOffset=offset;
	plcIpAddress=plcIpAddress_;
	plcActiveTable=plcActiveTable_;
	plcActiveIndex=plcActiveIndex_;
	plcActiveBit=plcActiveBit_;
	plcCommandTable=plcCommandTable_;
	plcCommandIndex=plcCommandIndex_;
	plcCommandNumber=plcCommandNumber_;

	activePen=QPen(QColor(255,100,100),50);
	highLightedPen=QPen(QColor(100,150,100),10);
	if ((plcActiveTable=="")&&(plcCommandTable==""))
		isVirtual=true;
	else
		isVirtual=false;
	tri=new gItemClass(tri_,0.f,0.f,this);
	//tooltip
	myScene->addItem(tri);
	isHighLighted=isActive=false;
	inMaintenanceMode=true;
	connect(tri,SIGNAL(hoverEnterSignal()),this,SLOT(hoverEnterSlot()));
	connect(tri,SIGNAL(hoverLeaveSignal()),this,SLOT(hoverLeaveSlot()));
	connect(tri,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
	connect(tri,SIGNAL(mouseReleasedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mouseReleasedSlot(QGraphicsSceneMouseEvent*)));
	reposition();
	myScene->addItem(this);
	if (fromPos)
		fromPos->addMoveClass(this);
	if (toPos)
		toPos->addMoveClass(this);
}
moveClass::~moveClass()
{
}
void moveClass::setIsEnabled(bool e)
{
	isEnabled=e;
}
void moveClass::reposition()
{
	myPath=QPainterPath();//clear path
	float x=0.5*(fromPos->getGX()+toPos->getGX());
	float y=0.5*(fromPos->getGY()+toPos->getGY());
	tri->setPos(0.f,0.f);
	if (type&HORIZONTAL)
	{
		if (fromPos->getGX()>toPos->getGX())
			tri->myRotate(0,0,180);
		tri->moveBy(x,y+gOffset);
	}
	if (type&VERTICAL)
	{
		if (fromPos->getGY()<toPos->getGY())
			tri->myRotate(0,0,90);
		else
			tri->myRotate(0,0,-90);
		tri->moveBy(x+gOffset,y);
	}
	myBoundRect=QRectF(QPointF(uMin(fromPos->getGX(),toPos->getGX()),uMax(fromPos->getGY(),toPos->getGY())),QPointF(uMax(fromPos->getGX(),toPos->getGX()),uMin(fromPos->getGY(),toPos->getGY())));
	myBoundRect=myBoundRect.united(tri->boundingRect().translated(tri->x(),tri->y()));
	//
	float x0,x1,x2,x3;
	float y0,y1,y2,y3;
	if (type&HORIZONTAL)
	{
		if (fromPos->getGX()<toPos->getGX())
		{
			x0=fromPos->getGX();
			y0=fromPos->getGY();
			x3=toPos->getGX();
			y3=toPos->getGY();
		}
		else
		{
			x0=toPos->getGX();
			y0=toPos->getGY();
			x3=fromPos->getGX();
			y3=fromPos->getGY();
		}
		x1=tri->boundingRect().left()+tri->x();
		y1=tri->boundingRect().center().y()+tri->y();
		x2=tri->boundingRect().right()+tri->x();
		y2=tri->boundingRect().center().y()+tri->y();
		if (gOffset>0)
		{
			myPath.moveTo(x0,y0);
			myPath.arcTo(QRectF(QPointF(x0,y0-(y1-y0)),QPointF(x0+2*(x1-x0),y1)),180,90);
			myPath.moveTo(x2,y2);
			myPath.arcTo(QRectF(QPointF(x2-(x3-x2),y2-2*(y2-y3)),QPointF(x3,y2)),270,90);
		}
		else
		{
			myPath.moveTo(x1,y1);
			myPath.arcTo(QRectF(QPointF(x0,y0-(y0-y1)),QPointF(x0+2*(x1-x0),y0+(y0-y1))),90,90);
			myPath.moveTo(x3,y3);
			myPath.arcTo(QRectF(QPointF(x2-(x3-x2),y2),QPointF(x3,y2+2*(y3-y2))),0,90);
		}
	}
	else
	{
		if (fromPos->getGY()>toPos->getGY())
		{
			x0=fromPos->getGX();
			y0=fromPos->getGY();
			x3=toPos->getGX();
			y3=toPos->getGY();
		}
		else
		{
			x0=toPos->getGX();
			y0=toPos->getGY();
			x3=fromPos->getGX();
			y3=fromPos->getGY();
		}
		x2=tri->boundingRect().center().x()+tri->x();
		y2=tri->boundingRect().top()+tri->y();
		x1=tri->boundingRect().center().x()+tri->x();
		y1=tri->boundingRect().bottom()+tri->y();
		if (gOffset>0)
		{
			myPath.moveTo(x0,y0);
			myPath.arcTo(QRectF(QPointF(x0-(x1-x0),y0-2*(y0-y1)),QPointF(x0+(x1-x0),y0)),-90,90);
			myPath.moveTo(x2,y2);
			myPath.arcTo(QRectF(QPointF(x3-(x2-x3),y3),QPointF(x3+(x2-x3),y3+2*(y2-y3))),0,90);
		}
		else
		{
			myPath.moveTo(x0,y0);
			myPath.arcTo(QRectF(QPointF(x1,y1-(y0-y1)),QPointF(x1+2*(x0-x1),y0)),-90,-90);
			myPath.moveTo(x3,y3);
			myPath.arcTo(QRectF(QPointF(x2,y2-(y2-y3)),QPointF(x2+2*(x3-x2),y2+(y2-y3))),90,90);
		}
	}
	updateGui();
}
void moveClass::setMaintenanceMode(bool m)
{
	inMaintenanceMode=m;
	updateGui();
}
int moveClass::getFromPosId()
{
	return fromPos->getGlobalId();
}
int moveClass::getToPosId()
{
	return toPos->getGlobalId();
}

QRectF moveClass::boundingRect() const
{
	return myBoundRect;
}
void moveClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if (isActive)
	{
		painter->setPen(activePen);
		painter->drawPath(myPath);
	}
	if (isHighLighted)
	{
		painter->setPen(highLightedPen);
		painter->drawPath(myPath);
	}

}
void moveClass::setIsHighLighted(bool h)
{
	if (isHighLighted!=h)
	{
		isHighLighted=h;
		update();
		updateGui();
	}
}
bool moveClass::setIsActive(bool activate)
{
	if (isActive!=activate)
	{
		isActive=activate;
		updateGui();
		update();
	}
	return isActive;
}
bool moveClass::getIsActive()
{
	return isActive;
}
void moveClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if (!isEnabled)
		return;
	if (inMaintenanceMode)
		{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcCommandTable,plcCommandIndex,plcCommandNumber); }
	//setIsActive(!isActive);
}
void moveClass::mouseReleasedSlot(QGraphicsSceneMouseEvent *ev)
{
	if (!isEnabled)
		return;
	{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcCommandTable,plcCommandIndex,0); }
	//setIsActive(!isActive);
}
void moveClass::hoverEnterSlot()
{
	setIsHighLighted(true);
}
void moveClass::hoverLeaveSlot()
{
	setIsHighLighted(false);
}
void moveClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((plcIpAddress==ip)&&(plcActiveTable==table)&&(plcActiveIndex==index))
		setIsActive(HAVECOMMONBIT(value.toInt(),BITS[plcActiveBit]));
}
void moveClass::updateGui()
{
	QString toolTipTxt;
	toolTipTxt+="<font style='color:#333333'>"+tr("From %1 to %2:").arg(fromPos->getGlobalId()).arg(toPos->getGlobalId())+"</font><br>";
	if (plcActiveBit>0)
		toolTipTxt+=tr("Active bit: %1[%2].%3").arg(plcActiveTable).arg(plcActiveIndex).arg(plcActiveBit)+"<br>";
	toolTipTxt+=tr("Sends %1 to %2[%3]").arg(plcCommandNumber).arg(plcCommandTable).arg(plcCommandIndex)+"<br>";
	toolTipTxt+=tr("Plc Ip: %1").arg(plcIpAddress);
	tri->setToolTip(toolTipTxt);
	//
	//if ((inMaintenanceMode&&(!isVirtual))||(isHighLighted)||(isActive))
	//{
	//	tri->showGraphics(true);
	//	tri->setHideWhenMouseNotOver(false);
	//	if (isActive)
	//	{
	//		tri->setPen(activePen);
	//		tri->setMouseOverFillColor(QColor(255,150,150));
	//	}
	//	else if (isHighLighted)
	//	{
	//		tri->setPen(highLightedPen);
	//		tri->setMouseOverFillColor(QColor(150,255,150));
	//	}
	//	else
	//		tri->setPen(QPen(QColor(100,100,255),0));

	//}
	//else
	//{
	//	tri->showGraphics(false);
	//	tri->setHideWhenMouseNotOver(true);
	//}
}
