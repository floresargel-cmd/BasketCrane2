#ifndef conPos_H
#define conPos_H

#include <QtGui>
#include <QWidget>
class convPositionClass;
#include "serv.h"
#include "conMove.h"
#include "conPs.h"
#include "conBtn.h"
#include "conPosDlg.h"
class convPositionClass : public QWidget				
{
protected:
	tuxipClass														*tuxip;
	QGraphicsScene												*myScene;
	//
	int																		globalId;
	QString																description;
	float																	gXPos;
	float																	gYPos;
	float																	angle;
	float																	rotateXoffset;
	float																	rotateYoffset;
	gItemClass														*mouseOver;
	gItemClass														*conveyor;
	gItemClass														*basket;
	gItemClass														*profiles;
	myGraphicsTextItem										*graphicsText;
	int																		currentBasketDestination;
	//
	QString																plcIp;
	QString																plcBasketNumberTable;
	QString																plcStateTable;
	QString																plcDataTable;
	int																		plcIndex;
	//
	int																		basketNumber;
	int																		basketDestination;
	QVector<gProximitySwitchClass*>				proximitySwitchVector;
	QVector<buttonClass*>									buttonsVector;
	QVector<moveClass*>										movesVector;	//
	bool																	inMaintenanceMode;
	cPosDlgClass													*posDlg;
	Q_OBJECT
public:
	convPositionClass(tuxipClass *tuxip_,QGraphicsScene *myScene_,
		int globalId_,QString description_,QString plcIp_,QString plcBasketNumberTable_,QString plcStateTable_,QString plcDataTable_,int plcIndex_,
		gItemClass *posTopMouseOver_,gItemClass *conveyorItem_,gItemClass *basketTop_,gItemClass *profileTop_,
		float dx,float dy,QWidget *p);
	~convPositionClass();
	void myRotate(float rotateXoffset_,float rotateYoffset_,float angle_);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setBasketId(int b);
	void setBasketDestination(int b);
	void addMoveClass(moveClass* m);
	void updateGui();
	int getGlobalId();
	bool getHasBasket();
	int getBasket();
	float getGX();
	float getGY();
	QString getDescription();
	void setPos(float x,float y,bool transformText=true);
	void addProximitySwitch(int bit,float offset,QString txt="Proximimity");
	void addProximitySwitch(QString plcIpAdress_,QString plcTable_,int plcIndex_,int plcBit_,QString txt,float offset);
	void addButton(QString plcIpAdress_,QString plcCommandTable_,int plcCommandIndex_,int plcCommandNumber_,QString plcActiveTable_,int plcActiveIndex_,int plcActiveBit_,float offset,QString label,int type=buttonClass::typeNormal);
	void setMaintenanceMode(bool m);
public slots:
	void hoverEnterSlot();
	void hoverLeaveSlot();
	void mouseReleasedSlot(QGraphicsSceneMouseEvent*);
};											
#endif
