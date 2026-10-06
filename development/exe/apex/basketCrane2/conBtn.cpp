#include "conBtn.h"
buttonClass::buttonClass(tuxipClass *tuxip_,QString plcIpAddress_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,float offset_,QGraphicsScene *myScene,QString label_,int type_)
{
	tuxip=tuxip_;
	plcIpAddress=plcIpAddress_;
	plcCommandTable=plcCommandTable_;
	plcCommandIndex=plcCommandIndex_;
	plcCommandNumber=plcCommandNumber_;
	plcActiveTable=plcActiveTable_;
	plcActiveIndex=plcActiveIndex_;
	plcActiveBit=plcActiveBit_;
	offset=offset_;
	inActivePen=QPen(QColor(0,0,175),10);
	activePen=QPen(QColor(255,50,50),10);
	isActive=false;
	isMouseOver=false;
	inMaintenanceMode=false;
	isEnabled=true;
	label=label_;
	type=type_;
	float size=250.;
	if (offset>0)
	{
		myPath.moveTo(2*size,0.f);
		myPath.lineTo(0.f,+size);
		myPath.lineTo(0.f,+0.5*size);
		myPath.lineTo(-0.5*size,+0.5*size);
		myPath.lineTo(-0.5*size,-0.5*size);
		myPath.lineTo(0.f,-0.5*size);
		myPath.lineTo(0.f,-size);
		myPath.closeSubpath();
		if (type==typeConveyorDouble)
		{
			//myPath.moveTo(0.f,500.f);
			//myPath.lineTo(+125.f,+250.f);
			//myPath.lineTo(-125.f,250.f);
			//myPath.closeSubpath();
		}
	}
	else
	{
		myPath.moveTo(-2*size,0.f);
		myPath.lineTo(0.f,+size);
		myPath.lineTo(0.f,+0.5*size);
		myPath.lineTo(+0.5*size,+0.5*size);
		myPath.lineTo(+0.5*size,-0.5*size);
		myPath.lineTo(0.,-0.5*size);
		myPath.lineTo(0.f,-size);
		myPath.closeSubpath();
		if (type==typeConveyorDouble)
		{
			//myPath.moveTo(0.f,-500.f);
			//myPath.lineTo(+125.f,-250.f);
			//myPath.lineTo(-125.f,-250.f);
			//myPath.closeSubpath();
		}
	}
	if (isEnabled&&(plcCommandTable.length()>0))
	{
		setAcceptHoverEvents(true);
		setFlag(QGraphicsItem::ItemIsFocusable,true);
		setFlag(QGraphicsItem::ItemIsSelectable,true);
		QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	}
	else
	{
		setAcceptHoverEvents(false);
		setFlag(QGraphicsItem::ItemIsFocusable,false);
		setFlag(QGraphicsItem::ItemIsSelectable,false);
		QGraphicsItem::setCursor(Qt::ArrowCursor);
	}
	myBoundRect=myPath.boundingRect();
	setToolTip(tr("%1\nSends %2 to %3[%4]\nConnected to %5[%6].%7\nip: %8").arg(label).arg(plcCommandNumber).arg(plcCommandTable).arg(plcCommandIndex).arg(plcActiveTable).arg(plcActiveIndex).arg(plcActiveBit).arg(plcIpAddress));
	setZValue(1);
	myScene->addItem(this);
	setMaintenanceMode(false);
}
buttonClass::~buttonClass()
{
}
void buttonClass::mySetIsVisible(bool is)
{
	setVisible(is);
	if (is&&(plcCommandTable.length()>0))
	{
		setAcceptHoverEvents(true);
		setFlag(QGraphicsItem::ItemIsFocusable,true);
		setFlag(QGraphicsItem::ItemIsSelectable,true);
		QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	}
	else
	{
		setAcceptHoverEvents(false);
		setFlag(QGraphicsItem::ItemIsFocusable,false);
		setFlag(QGraphicsItem::ItemIsSelectable,false);
		QGraphicsItem::setCursor(Qt::ArrowCursor);
	}
}
void buttonClass::setIsEnabled(bool e)
{
	isEnabled=e;
	mySetIsVisible(isEnabled);
}
QRectF buttonClass::boundingRect() const
{
	return myBoundRect;
}
void buttonClass::setTransform(QTransform tr)
{
	tr.translate(offset,0.f);
	QGraphicsItem::setTransform(tr);
}
void buttonClass::setMaintenanceMode(bool m)
{
	inMaintenanceMode=m;
	setVisible(inMaintenanceMode);
}
void buttonClass::setActvePen(QPen ap)
{
	activePen=ap;
}
void buttonClass::setInactvePen(QPen ip)
{
	inActivePen=ip;
}

void buttonClass::myRotate(float x,float y,float angle)
{
	QTransform  transformation;
	if (fabs(angle)>0.01)
		transformation.rotate(angle);
	transformation.translate(x*qCos(-uPi*angle/180)-y*qSin(-uPi*angle/180),x*qSin(-uPi*angle/180)+y*qCos(-uPi*angle/180));
	setTransform(transformation);
}
void buttonClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if (isActive)
	{
		//if (isMouseOver)
			painter->fillPath(myPath,QBrush(QColor(50,250,0)));
		painter->setPen(activePen);
		painter->drawPath(myPath);
	}
	else if (inMaintenanceMode)
	{
		if (isMouseOver)
			painter->fillPath(myPath,QBrush(QColor(150,150,255)));
		else
			painter->fillPath(myPath,QBrush(QColor(0,0,150)));
		painter->setPen(inActivePen);
		painter->drawPath(myPath);
	}
}
void buttonClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((plcIpAddress==ip)&&(plcActiveTable==table)&&(plcActiveIndex==index))
	{
		bool b=HAVECOMMONBIT(value.toInt(),BITS[plcActiveBit]);
		if (b!=isActive)
		{
			isActive=b;
			update();
		}
	}
}
void buttonClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=true;
	QGraphicsItem::update();
	QGraphicsItem::hoverEnterEvent(ev);
}
void buttonClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=false;
	QGraphicsItem::update();
	QGraphicsItem::hoverLeaveEvent(ev);
}
void buttonClass::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
	if (!isEnabled)
		return;
	if (plcCommandTable.length()>0)
	{
		if (inMaintenanceMode)
			{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcCommandTable,plcCommandIndex,plcCommandNumber); }
	}
	//QGraphicsItem::mousePressEvent(ev);
}
void buttonClass::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
	if (!isEnabled)
		return;
	if (plcCommandTable.length()>0)
	{
		{ if (!basketCraneConfig().observer()) tuxip->writeInteger(plcCommandTable,plcCommandIndex,0); }
	}
	//QGraphicsItem::mouseReleaseEvent(ev);
}