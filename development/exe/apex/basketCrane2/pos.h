#ifndef POS_H
#define POS_H

class posClass;
#include <QtGui>

#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "allPos.h"
#include "posDlg.h"
class posClass : public QWidget				
{
protected:
	tuxipClass										*tuxip;
	QString												plcTable;
	int														posNumber;
	int														posIndex;
	int														plcDestination;
	QString												description;
	float													plcXPos;
	float													plcYPos;
	float													plcZPos;
	float													gXPos;
	float													gYPos;
	float													gXOffset;
	float													gYOffset;
	allPossClass									*allPos;
	QGraphicsScene								*scene;
	gItemClass										*posItem;
	gItemClass										*mouseOverItem;
	gItemClass										*basketItem;
	gItemClass										*profilesItem;
	gItemClass										*lockedItem;
	gItemClass										*fromItem;
	gItemClass										*toItem;
	QTransform										txtTransformation;
	QGraphicsSimpleTextItem 			*posItemText;
	QVector<QGraphicsSimpleTextItem*> profileTxts;
	QGraphicsSimpleTextItem 			*exportItemText;
	//
	posClass											*abovePos;
	posClass											*belowPos;
	QVector<posClass*>						obstaclePosV;
	//
	int														basketNum;
	QVector<QVector<QVariant>>		contentsVV;
	bool													basketWithContents;
	bool													isLocked;
	bool													isFrom;
	bool													isTo;
	//plc
	bool													isCranePlcPos;//the basket number is controled by plc
	bool													isCranePos;
	int														plcIndex;
	int														displayMode;
	int														destination;
	int														exportGroup;//
	int														exportGroupQueue;//
	bool													isOnExportList;
public:
	QString												debugStr;
public:
	enum{//displayMode
		displayNone=0,
		displayNormal,
		displayToModify,
		displayToExport,
		displayToSelectFrom,
		displayToSelectTo,
	};
	enum{//destination
		destinationNone=0,
		destinationPacking=1,
		destinationDestacker=2,
	};
	Q_OBJECT
public:
	posClass(allPossClass *allPos_,int posNumber_,int posIndex_,float plcXPos_,float plcYPos_,float plcZPos_,float gXOffset_,float gYOffset_,
		QGraphicsScene *scene_,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_
		,bool handleMousePressed,QWidget *p);
	~posClass();
	QString getDescription();
    QRectF displayFootprint() const { return posItem->sceneBoundingRect(); }
    QString displayBasketDetails() const;
    QString displayHoverDetails() const;
    QString displayDestination() const;
    QStringList displayBasketRows() const;
    bool displayIsTarget() const { return isTo; }
    QList<QPolygonF> displaySourcePaths() const { return fromItem->mapToScene(fromItem->displayPath()).toSubpathPolygons(); }
    QList<QPolygonF> displayTargetPaths() const { return toItem->mapToScene(toItem->displayPath()).toSubpathPolygons(); }
	int getPositionNumber();
	int getPositionIndex();
	void setDisplayMode(int m);
	QString getPosIndexDisplayString();
	int getBasketNumber();
	bool getIsLocked();
	float getPlcX();
	float getPlcY();
	float getPlcZ();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setPlcAdress(tuxipClass *tuxip,QString plcTable_,int index_);
	int getIdForPlc();
	void setAboveBelowPos(posClass *a,posClass *b);
	void setObstaclePos(QVector<posClass*> obstaclePosV_);
	void setPlcPos(float x,float y,float z);
	void updateTxt();
	void setBasketId(int b,bool user=false,QString logInfo="",int doNotCheckPosition=0,int doNotCheckIndex=0);
	bool getHasBasket();
	bool getHasFullBasket();
	void loadFromDatabase();
	void loadFromAllData(QVector<QVector<QVariant>> allPosVV,QVector<QVector<QVariant>> allPontentsVV);
	void updateFromData(QVector<QVector<QVariant>> posVV,QVector<QVector<QVariant>> contentsVV_);
	void setIsFrom(bool s);
	bool getIsFrom();
	void setIsTo(bool s);
	void updateGui();
	void showPositionDlg(int activeTab,int type=-1,bool checkPass=true);
	void updateItemsVisibility();
	bool getIsFreeToMove();
	int getBasketForInFromOut();
	int getBasketForInFromDestacker();
	//
	bool getCanGetBasketInPos(posClass *p);
	int getMaximumLength();
	void setDestination(int d);
	int getDestination();
	void setExportGroup(int g);
	int getExportGroup();
	void setExportGroupQueue(int q);
	int getExportGroupQueue();
	void updateExportText();
	QVector<int> getProfiles();
	int getTopProfile();
	int getBottomProfile();
	bool getIsProfileStacked(int p);
	void setIsOnExportList(bool is);
	bool getIsOnExportList();
	QVector<posClass*> getPosThatPreventYouToMove();
signals:
	void posChangedSignal();
	void basketChangedSignal(int oldBasket,int newBasket);
	void posMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*);
	void userChangedBasketSignal(posClass *pos);
public slots:
	void showPositionDlgSlotSlot();		
	void mousePressedSlot(QGraphicsSceneMouseEvent*);
	void loadFromDatabaseSlot();
};											
#endif
