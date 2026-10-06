#ifndef G_TEXT_H
#define G_TEXT_H

#include <QtGui>
#include <QGraphicsItem>
class myGraphicsTextItem : public QGraphicsTextItem
{
private:
	QRectF									myBoundRect;
	QTextOption							textOp;
	QFont										font;
	QColor									color;
	QString									text;
	Q_OBJECT
protected:
public:
enum 
{ 
	Type=UserType+3 
};
public:
	myGraphicsTextItem(QString txt,QRectF r,QFont f,QColor col=QColor(),QGraphicsItem *parent=0,QGraphicsScene *scene=0);
	~myGraphicsTextItem();
	void setText(const QString &txt);
protected:
	void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget=0);
	QRectF boundingRect() const;
};											
#endif
