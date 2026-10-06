#ifndef conPs_H
#define conPs_H

#include <QtGui>
#include <QGraphicsItem>
#include "Gb2.h"
#include "serv.h"
#include "gItem.h"
#include "gText.h"
class gProximitySwitchClass :public QObject, public QGraphicsItem			
{
protected:
	QString																plcIpAdress;
	QString																plcTable;
	int																		plcIndex;
	int																		plcBit;
	bool																	isActive;
	bool																	isMouseOver;
	//
	QRectF																myBoundRect;
	float																	offset;
	QPainterPath													myPath;
	QPen																	activePen;
	QPen																	inActivePen;
Q_OBJECT
public:
	gProximitySwitchClass(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset_,QGraphicsScene *myScene);
	~gProximitySwitchClass();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	QRectF boundingRect() const;
	void setTransform(QTransform tr);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);
};											
#endif

