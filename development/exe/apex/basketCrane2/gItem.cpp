#include "gItem.h"
#include "plantAppearance.h"
gItemClass::gItemClass():QGraphicsItem()
{
	myIsVisible=true;
	isMouseOver=false;
}
gItemClass::gItemClass(gItemClass *p,float dx,float dy,QObject* o):QGraphicsItem(),QObject(o)
{
	pen=plantDrawingPen(p->pen);
	mouseOverFillColor=p->mouseOverFillColor;
	hideWhenMouseNotOver=p->hideWhenMouseNotOver;
	mouseOverFlag=p->mouseOverFlag;
	rectOfAll=p->rectOfAll;
	pathOfAll=p->pathOfAll;
	myIsVisible=p->myIsVisible;
	fillBrush=p->fillBrush;
	showGraphics(myIsVisible);
	boundingRectPath=p->boundingRectPath;
	QTransform transform=QTransform().translate(dx,dy);
	setTransform(transform);
	setMouseOverFlag(mouseOverFlag);
	isMouseOver=false;
}
gItemClass::gItemClass(QString fileName,QPen pen_,bool mouseOverFlag_,bool hideWhenMouseNotOver_,QColor mouseOverFillColor_,QObject* o):QGraphicsItem(),QObject(o)
{
	//isMouseOver=false;
	//xPos=0.0;
	//yPos=0.0;
	//color=color_;
	////
	//normalPen=QPen(color,0.0);
	//mouseOverPen=QPen(QColor(100,100,200),0.0);
	//mouseOverFlag=mouseOverFlag_;

	pen=plantDrawingPen(pen_);
	mouseOverFillColor=mouseOverFillColor_;
	mouseOverFlag=mouseOverFlag_;
	hideWhenMouseNotOver=hideWhenMouseNotOver_;
	fillBrush=Qt::NoBrush;
	isMouseOver=false;
	myIsVisible=true;
	setMouseOverFlag(mouseOverFlag);
	loadFromDxfFile(fileName);
}
gItemClass::~gItemClass()
{
}
void gItemClass::showGraphics(bool s)
{
	myIsVisible=s;
	QGraphicsItem::setVisible(s);
}
void gItemClass::setMouseOverFlag(bool f)
{
	mouseOverFlag=f;
	setAcceptHoverEvents(mouseOverFlag);
	setFlag(QGraphicsItem::ItemIsFocusable,mouseOverFlag);
	setFlag(QGraphicsItem::ItemIsSelectable,mouseOverFlag);
	if (mouseOverFlag)
		QGraphicsItem::setCursor(Qt::ClosedHandCursor);
	else
		QGraphicsItem::setCursor(Qt::ArrowCursor);
}
QPainterPath gItemClass::shape() const
{
	return boundingRectPath;
	//return pathOfAll;
}
QRectF gItemClass::boundingRect() const
{
	return rectOfAll; 
}
void gItemClass::myRotate(float xCenter,float yCenter,float angle)
{
	QTransform  transformation;
	transformation.rotate(angle);
	transformation.translate(xCenter+QGraphicsItem::x()*qCos(-uPi*angle/180)-QGraphicsItem::y()*qSin(-uPi*angle/180),yCenter+QGraphicsItem::x()*qSin(-uPi*angle/180)+QGraphicsItem::y()*qCos(-uPi*angle/180));
	QGraphicsItem::setTransform(transformation);
}
void gItemClass::setPen(QPen p)
{
	pen=plantDrawingPen(p);
	update();
}
void gItemClass::setFillBrush(QBrush b)
{
	fillBrush=b;
}
void gItemClass::setMouseOverFillColor(QColor c)
{
	mouseOverFillColor=c;
	update();
}
void gItemClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if (!myIsVisible)
		return;
	if ((mouseOverFlag)&&(hideWhenMouseNotOver)&&(!isMouseOver))
		return;
	if (fillBrush!=Qt::NoBrush)
		painter->fillPath(pathOfAll,fillBrush);
	if (isMouseOver)
	{
		//painter->fillRect(boundingRect(),QBrush(mouseOverFillColor));
		painter->fillPath(pathOfAll,QBrush(mouseOverFillColor));
		painter->drawPath(pathOfAll);
	}
	painter->setPen(pen);
	painter->drawPath(pathOfAll);
}
void gItemClass::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=true;
	emit hoverEnterSignal();
	QGraphicsItem::update();
	QGraphicsItem::hoverEnterEvent(ev);
}
void gItemClass::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
	isMouseOver=false;
	emit hoverLeaveSignal();
	QGraphicsItem::update();
	QGraphicsItem::hoverLeaveEvent(ev);
}
void gItemClass::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
	emit mousePressedSignal(ev);
	QGraphicsItem::mousePressEvent(ev);
}
void gItemClass::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
	emit mouseReleasedSignal(ev);
	QGraphicsItem::mouseReleaseEvent(ev);
}
void gItemClass::loadFromDxfFile(QString fileName)
{
	QFileInfo fInfo(fileName);
	if (!fInfo.exists())
		uExit(tr("error"),tr("Could not find %1 file").arg(fileName));
	dxfDataClass *dxfData=new dxfDataClass();//dxf reader
	DL_Dxf* dxf=new DL_Dxf();
	if (!dxf->qFileIn(fileName,dxfData))
	{
		delete dxf;
		delete dxfData;
		uExit(tr("error"),tr("Could read %1 file").arg(fileName));
	}
	dxfData->calculate();
	QPointF lastPoint;
	for (int i=0;i<dxfData->getNoOfLines();i++)
	{
		//if (!areSamePoints(dxfData->getLine(i).p1(),lastPoint))
		if (dxfData->getLine(i).p1()!=lastPoint)
			pathOfAll.moveTo(dxfData->getLine(i).p1());
		pathOfAll.lineTo(dxfData->getLine(i).p2());
		lastPoint=dxfData->getLine(i).p2();
		rectOfAll=rectOfAll.united(QRectF(dxfData->getLine(i).p1(),dxfData->getLine(i).p2()));
	}
	for (int i=0;i<dxfData->getNoOfStrings();i++)
	{
		QMatrix  mirrorMatrix;
		mirrorMatrix.scale(1,-1);
		QPainterPath txtPath;txtPath.addText(dxfData->getString(i).position,dxfData->getString(i).font,dxfData->getString(i));
		pathOfAll.addPath(mirrorMatrix.map(txtPath).translated(0,2*dxfData->getString(i).position.y()));
		//
	}
	boundingRectPath.addRect(rectOfAll);
	delete dxf;
	delete dxfData;
}
