#ifndef conMove_H
#define conMove_H

#include <QtGui>
#include <QGraphicsItem>
#include <QHeaderView>
class moveClass;
#include "Gb2.h"
#include "serv.h"
#include "gItem.h"
#include "gText.h"
#include "conPos.h"
class moveClass : public QObject,public QGraphicsItem			
{
protected:
	tuxipClass														*tuxip;
	QGraphicsScene												*myScene;
	QString																plcIpAddress;
	QString																plcActiveTable;
	int																		plcActiveIndex;
	int																		plcActiveBit;
	QString																plcCommandTable;
	int																		plcCommandIndex;
	int																		plcCommandNumber;
	bool																	isEnabled;
	//
	convPositionClass											*fromPos;
	convPositionClass											*toPos;
	gItemClass														*tri;
	float																	gOffset;

	QPen																	activePen;
	QPen																	highLightedPen;
	QRectF																myBoundRect;
	QPainterPath													myPath;
	bool																	isVirtual;//no commands are sending to plc
	bool																	isActive;//the command is send to plc
	bool																	isHighLighted;//the command will be send to plc
	bool																	inMaintenanceMode;
	int																		type;
public:
	enum{//type
	HORIZONTAL=					0x0000001,
	VERTICAL  =					0x0000002,
	};
	Q_OBJECT
public:
	moveClass(tuxipClass *tuxip_,QGraphicsScene *myScene_,
		int type_,
		QString plcIpAddress_,
		QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,
		QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,
		convPositionClass *fromPos_,convPositionClass *toPos_,
		gItemClass *tri_,
		float offset,
		QObject *p=0);
	~moveClass();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	QRectF boundingRect() const;
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	int getFromPosId();
	int getToPosId();
	void setIsHighLighted(bool h);
	bool setIsActive(bool activate);
	bool getIsActive();
	void setIsEnabled(bool e);
	void updateGui();
	void reposition();
	void setMaintenanceMode(bool m);
public slots:
	void hoverEnterSlot();
	void hoverLeaveSlot();
	void mouseReleasedSlot(QGraphicsSceneMouseEvent*);
	void mousePressedSlot(QGraphicsSceneMouseEvent*);
};											
#endif

