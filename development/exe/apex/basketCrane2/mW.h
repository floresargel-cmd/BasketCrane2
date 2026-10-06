#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QHostInfo>

#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "allPos.h"
#include "crane.h"
#include "usageDlg.h"
#include "autoWdg.h"

#include "settingsDlg.h"
#include "search.h"
#include "hmiB.h"
#include "conW.h"
#include "destW.h"
#include "expWdg.h"
class mainWindowClass : public QMainWindow				
{												
	tuxipServerClass											*tuxipServer;
	allPossClass													*allPos;
	craneClass														*crane;
	exportStationWidgetClass							*exportStationWidgetDestacker;
	exportStationWidgetClass							*exportStationWidgetPacking;
	myQGraphicsViewClass									*layoutGraphicsView;
	autoWidgetClass												*autoWidget;
	conveyorsWidgetClass									*conveyorsWidget;
	destackerWidgetClass									*destackerWidget;
	QDialog																*conveyorsDlg;
	QDialog																*destackerDlg;
	//
	hmiDlgBasicClass											*hmiDlgBasic;
	QAction																*showTuxServerAction;
	handShakeClass												*handShakeCrane2;

	QToolBox															*toolBox;
	QRadioButton													*missionsRadioButton;
	QRadioButton													*modifyRadioButton;
	QRadioButton													*exportsRadioButton;

	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	Q_OBJECT								
public:										
	mainWindowClass();
	~mainWindowClass();
private:									
public slots:								
	void mainTimerSlot();						
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
	void showServerSlot();						
	void showUsageDialogSlot();
	void showConVDlgActionTriggeredSlot();
	void showDestackerDlgActionTriggeredSlot();
	void qtAboutSlot();
	void stopSlot();
	void resetGraphicsViewZoom();
	void hmiDlgBasicButtonClickedSlot();
	void handShakeCrane2ChangedSlot(bool s);

	void missionsRadioButtonClickedSlot(bool s);
	void modifyRadioButtonClickedSlot(bool s);
	void exportsRadioButtonClickedSlot(bool s);
	void toolBoxCurrentChangedSlot(int index);

	void writeBasketToReturningConveyorActionSlot();
};											
#endif
