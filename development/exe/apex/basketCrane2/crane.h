#include <QVector3D>
#ifndef CRANE_H
#define CRANE_H

class craneClass;
#include <QtGui>
#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "carriage.h"
#include "craneWdg.h"
#include "allPos.h"
class groupOfPosToExportsClass
{
public:
	int												destination;
	int												index;
	QVector<posClass*>				posV;
	QVector<posClass*>				posThatPreventBasketsToMove;
	groupOfPosToExportsClass();
	groupOfPosToExportsClass &operator=(const groupOfPosToExportsClass& p)
	{
		posV=p.posV;
		posThatPreventBasketsToMove=p.posThatPreventBasketsToMove;
		return *this;
	}
	bool addPos(posClass*);
	void setIndex(int index_);
	void updatePos();
	void calculatePosThatPreventBasketsToMove(QVector<posClass*> posToIgnore);
	QVector<posClass*> getPosThatPreventBasketsToMove();
	int getTopProfile();
};
class craneClass : public QWidget				
{
	QTimer															*missionsTimer;
	tuxipClass													*tuxip;
	QGraphicsScene											*layoutScene;
	allPossClass												*allPos;
	carriageClass												*carriage;
	gItemClass													*craneXItem;
	gItemClass													*craneYItem;
	craneWidgetClass										*craneWidget;
	posClass														*toRotatingNorth;
	posClass														*toRotatingSouth;
	posClass														*rotatingNorth;
	posClass														*exit1;
	posClass														*exit2;
	posClass														*destackerBuffer;
	posClass														*destacker;
	QVector<posClass*>									posWithBasketsForExportV;
	QVector<groupOfPosToExportsClass*>	exportGroups;
	//
	//
	float												plcX;
	float												plcY;
	float												plcZ;
	int													autoMode;
    int displayAxes=0;
    qint64 displayAxisTimes[3]={0,0,0};
	Q_OBJECT								
public:
	enum {//autoMode
		autoNone											=0x00000000,
		autoExportsToDestaker					=0x00000001,
		autoExportsToPacking					=0x00000002, // anodizing
        autoExportsToPackingDestacker1 = 0x20,
        autoExportsToPackingDestacker2 = 0x40,
		autoImportsFromNorth					=0x00000004,
		autoImportsFromSouth					=0x00000008,
		autoExports										=0x00000080, // EPICS queue population
	};
public:										
	craneClass(tuxipServerClass *tuxipServer,tuxipClass *tuxipConnection,allPossClass *allPos_,QGraphicsScene *layoutScene_,QWidget *p=NULL);
	~craneClass();
	QWidget* getCraneWidget();
    QVector3D displayCoordinates() const { return QVector3D(plcX,plcY,plcZ); }
    bool displayTelemetryValid() const { return displayAxes==7; }
    qint64 displayTelemetryTime() const { return qMin(displayAxisTimes[0],qMin(displayAxisTimes[1],displayAxisTimes[2])); }
	void setStations();
	void loadFromDatabase();
	carriageClass *getCarriage();
	void checkForNextMissionInDatabase();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void sendManualMission(posClass *from,posClass *to);
	void addToAutoMode(int mode);
	void removeFromAutoMode(int mode);
	bool canGetNewMission();
	void setDisplayMode(int m);
	void uncoverBasketForExport(int basket);
    void addExportsFromEpicsData();
	posClass *getFreePosForBasketInPos(posClass *p);
public slots:								
	void missionsTimerSlot();
	void calculateExportGroupsSLot();
};											
#endif
