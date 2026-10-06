#ifndef conBtn_H
#define conBtn_H

#include <QtGui>
#include <QGraphicsItem>
#ifdef stationsExe
#include "Gc.h"
#endif
#ifdef basketWareHouse
#include "../basketWarehouse/Gb.h"
#endif
#include "serv.h"
#include "gItem.h"
#include "gText.h"
class buttonClass :public QObject, public QGraphicsItem			
{
protected:
	tuxipClass														*tuxip;
	QString																plcIpAddress;
	QString																plcCommandTable;
	int																		plcCommandIndex;
	int																		plcCommandNumber;
	QString																plcActiveTable;
	int																		plcActiveIndex;
	int																		plcActiveBit;
	QString																label;
	bool																	isActive;
	bool																	inMaintenanceMode;
	bool																	isMouseOver;
	bool																	isEnabled;
	//
	QRectF																myBoundRect;
	float																	offset;
	QPainterPath													myPath;
	QPen																	activePen;
	QPen																	inActivePen;
	int																		type;
public:
	enum
	{
		typeNormal=1,
		typeConveyorDouble=2,
	};
Q_OBJECT
public:
	buttonClass(tuxipClass *tuxip_,QString plcIpAddress_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,float offset_,QGraphicsScene *myScene,QString label_,int type_=typeNormal);
	~buttonClass();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	QRectF boundingRect() const;
	void setTransform(QTransform tr);
	void myRotate(float x,float y,float angle);
	void setMaintenanceMode(bool m);
	void setActvePen(QPen ap);
	void setInactvePen(QPen ip);
	void setIsEnabled(bool e);
	void mySetIsVisible(bool is);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *ev);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
};
#endif

