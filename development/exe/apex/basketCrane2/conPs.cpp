#include "conPs.h"
gProximitySwitchClass::gProximitySwitchClass(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset_,QGraphicsScene *myScene)
{
	plcIpAdress=plcIpAdress_;
	plcTable=plcTable_;
	plcIndex=plcIndex_;
	plcBit=plcBit_;
	offset=offset_;
	isMouseOver=false;
	inActivePen=QPen(QColor(150,150,150),10,Qt::DotLine);
	activePen=QPen(QColor(100,255,100),100);
	isActive=false;
	myPath.moveTo(0.f,-500.f);
	myPath.lineTo(0.f,+500.f);
	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsFocusable,true);
	setFlag(QGraphicsItem::ItemIsSelectable,true);
	QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	myBoundRect=QRectF(QPointF(-100.f,-600.f),QPointF(100.f,600.f));
	setToolTip(tr("Proximity Switch\nConnected to %1[%2].%3\nip: %4%5").arg(plcTable).arg(plcIndex).arg(plcBit).arg(plcIpAdress).arg((txt.length()>0?"\n"+txt:"")));
	setZValue(1);
	myScene->addItem(this);
}
gProximitySwitchClass::~gProximitySwitchClass()
{
}
QRectF gProximitySwitchClass::boundingRect() const
{
 	return myBoundRect;
}
void gProximitySwitchClass::setTransform(QTransform tr)
{
	tr.translate(offset,0.f);
	QGraphicsItem::setTransform(tr);
}
void gProximitySwitchClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if (isMouseOver)
	{
		//painter->fillRect(myBoundRect,QBrush(QColor(240,225,225)));
		painter->setPen(QPen(QColor(150,150,200)));
		painter->drawRect(myBoundRect);
	}
	if (isActive)
	{
		painter->setPen(activePen);
		painter->drawPath(myPath);
	}
	else 
	{
		painter->setPen(inActivePen);
		painter->drawPath(myPath);
	}
}
void gProximitySwitchClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=true;
	QGraphicsItem::update();
	QGraphicsItem::hoverEnterEvent(ev);
}
void gProximitySwitchClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=false;
	QGraphicsItem::update();
	QGraphicsItem::hoverLeaveEvent(ev);
}
void gProximitySwitchClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((plcIpAdress==ip)&&(plcTable==table)&&(plcIndex==index))
	{
		bool b=HAVECOMMONBIT(value.toInt(),BITS[plcBit]);
		if (b!=isActive)
		{
			isActive=b;
			update();
		}
	}
}