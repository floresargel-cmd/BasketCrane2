#ifndef HMIDLG_H
#define HMIDLG_H

class hmiDlgClass;
#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include "gView.h"
#include "Gb2.h"
#include "serv.h"
#include "hmiSett.h"
#include "crane.h"
#include "carriage.h"
#include "carriageW.h"
class hmiDlgClass : public QDialog				
{												
	tuxipClass													*tuxip;
	craneClass													*crane;
	QVector<progressClass*>							progressVector;
	QVector<floatPlcMemoryClass*>				floatPlcMemoryVector;
	QVector<intPlcMemoryClass*>					intPlcMemoryVector;
	QVector<bitClass*>									bitVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	QLabel															*craneStepLabel;
	QLabel															*hooksStepLabel;
	bitLogVectorClass										*bitErrorsVector;
	bitLogVectorClass										*bitWarningsVector;
	bitLogVectorClass										*bitPositionsVector;
	bitLogVectorClass										*bitSignalsFromVector;
	bitLogVectorClass										*bitSignalsToVector;
	QVector<plotClass*>									plotsVector;
	hmiSettingsClass										*settings;
	carriageClass												*carriage;
	//lock/unlock
	QTabWidget													*missionsTabWidget;
	bool																areAdvancedButtonsLocked;
	QPushButton													*clearActiveMission;
	hmiPushButtonWidgetClass						*abordActiveMission;
	hmiPushButtonWidgetClass						*endActiveMission;
	Q_OBJECT								
public:										
	hmiDlgClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,craneClass *crane_);
	~hmiDlgClass();
	void setCarriageClass(carriageClass *a);
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void lockAdvancedBtns(bool l);
	void fillAxisGroup(QVBoxLayout *layout,QString title,int index);
public slots:								
	void clearNextMissionBtnClickedSlot();	
	void clearActiveMissionBtnClickedSlot();	
	void settingsBtnClickedSlot();
	void missionsTabWidgetTabBarDoubleClickedSlot(int);
	//
	void stopSlot();
};											
#endif
