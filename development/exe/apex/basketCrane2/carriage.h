#ifndef carriage_H
#define carriage_H

#include <QtGui>

class carriageClass;
#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "allPos.h"
#include "carriageW.h"
class carriageClass : public QWidget				
{
	tuxipClass								*tuxip;
	allPossClass							*allPos;
	posClass									*pos;
	QString										plcIp;
	QString										plcMissionTable;
	int												plcNextMissionFromIndex;
	int												plcNextMissionToIndex;
	int												plcNextMissionStatusIndex;
	int												plcCurrentMissionFromIndex;
	int												plcCurrentMissionToIndex;
	int												plcCurrentMissionPcStatusIndex;
	int												plcCurrentMissionPLcStatusIndex;
	int												plcCurrentCranePlcStepIndex;
	int												plcCurrentHooksPlcStepIndex;
	//
	int												nextMissionFrom;
	int												nextMissionTo;
	int												nextMissionStatus;
	int												currentMissionFrom;
	int												currentMissionTo;
	int												currentMissionPcStatus;
	int												currentMissionPLcStatus;
	//
	posClass									*nextFromPos;
	posClass									*nextToPos;
	posClass									*activeFromPos;
	posClass									*activeToPos;
	int												activeMissionStatus;
	int												activeCraneStep;
	carriageWidgetClass				*carriageWidget;
public:										
	enum//mission status
	{
		plcStatusIdle=0,
		plcStatusStart=1,
		plcStatusBascketOnConv=2,
		plcStatusFinished=3,
		plcStatusAborted=4,
		plcStatusStartDoubleMission=10,
	};
	enum//
	{
		plcStepIdle=0,
		plcStepStart=1,
		plcStepGoToXYLoad=2,
		plcStepGoToZLoad=3,
		plcStepPinsGetBasket=4,
		plcStepGoTo0Z=5,
		plcStepGoToXYUnLoad=6,
		plcStepGoToZUnload=7,
		plcStepPinsReleaseBasket=8,
		plcStepGoToZeroZ=9,
		plcStepFinished=10,
	};
	Q_OBJECT
public:										
	carriageClass(tuxipClass *tuxipConnection,allPossClass *allPos_,QString description,QGraphicsScene *scene,gItemClass *posItem_,gItemClass *mouseOverItem_,gItemClass *basketItem_,gItemClass *profilesItem_,gItemClass *lockedItem_,gItemClass *fromItem_,gItemClass *toItem_,QWidget *p);
	~carriageClass();
	QWidget* getCarriageWidget();
    int displayBasketNumber() const;
    QStringList displayBasketRows() const;
    QString displayDestination() const;
	void setPlcMissionTable(QString plcIp_,QString plcMissionTable_,int plcNextMissionFromIndex_,int plcNextMissionToIndex_,int plcNextMissionStatusIndex_,int plcCurrentMissionFromIndex_,int plcCurrentMissionToIndex_,int plcCurrentMissionPcStatusIndex_,int plcCurrentMissionPLcStatusIndex_,int plcCurrentCranePlcStepIndex_);
	void checkForNextMissionInDatabase();
	void loadFromDatabase();
	bool getIsIdle();
	void setNextFromPos(posClass *p);
	void setNextToPos(posClass *p);
	void setActiveFromPos(posClass *p);
	void setActiveToPos(posClass *p);
	bool isReadyForNewMission();
	bool getCanStartNewMission(posClass* f,posClass* t);
	void plcStatusChanged(int s);
	void missionStart(posClass* f,posClass*t);
	void missionSendToPlc(bool force=false);
	void missionBasketOnConveyor();
	void missionFinished();
	void missionAborted();
	int moveBasket(posClass *from,posClass *to);
	bool moveNextToActive();
	void clearMissionInDb(QString user=QString());
	void clearNextMission();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	QString getCraneStepText(int s);
	QString getCraneStepDescription(int s);
	void setPlcPos(float x,float y,float z);
	void setDisplayMode(int m);
signals:
	void exportListChangedSignal();
public slots:								
	void checkStatusTimerSlot();						
};											
#endif
