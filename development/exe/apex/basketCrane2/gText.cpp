#include "gText.h"
myGraphicsTextItem::myGraphicsTextItem(QString txt,QRectF r,QFont f,QColor col,QGraphicsItem *parent,QGraphicsScene *scene):QGraphicsTextItem(parent)
{
	text=txt;
	font=f;
	color=col;
	myBoundRect=r;
	textOp.setAlignment(Qt::AlignCenter);
	textOp.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
}
myGraphicsTextItem::~myGraphicsTextItem()
{
}
void myGraphicsTextItem::setText(const QString &txt)
{
	text=txt;
	update();
}
void myGraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(color);
	painter->setFont(font);
	painter->drawText(boundingRect(),text,textOp);
	QGraphicsTextItem::paint(painter,option,widget);
}
 
QRectF myGraphicsTextItem::boundingRect() const
{
 	return myBoundRect;
}