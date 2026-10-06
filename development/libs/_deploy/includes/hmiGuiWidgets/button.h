#ifndef BUTTON_H
#define BUTTON_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "Ghmi.h"
class myPushButtonClass;
class sendDialogClass:public QDialog
{
	myPushButtonClass								*myPushButton;
public:
	sendDialogClass(myPushButtonClass *b,QString txt);
	void setPlcActive(bool a);
};
class myPushButtonClass:public QPushButton		
{
	QString													buttonText;
	QString													buttonIcon;
	QString													commandPlcAdress;
	int															plcIndexToSendTheCommand;
	int															valueToPlcOnPress;
	int															valueToPlcOnRelease;
	int															bitIndexToActivateOnPress;
	QString													dbConnectionName;
	QString													databaseType;
	QString													databaseDescription;
	QString													logTableName;
	//
	QString													plcInactiveStyle;
	QString													plcActiveStyle;
	QString													plcInactiveText;
	QString													userPromptText;
	sendDialogClass									*sendDialog;
	bool														isToogle;
	bool														isPlcActive;
	//
	tuxipClass											*tuxip;
	//
	myPushButtonClass &operator=(const myPushButtonClass& p)
	{
		buttonText=p.buttonText;
		buttonIcon=p.buttonIcon;
		commandPlcAdress=p.commandPlcAdress;
		plcIndexToSendTheCommand=p.plcIndexToSendTheCommand;
		valueToPlcOnPress=p.valueToPlcOnPress;
		valueToPlcOnRelease=p.valueToPlcOnRelease;
		bitIndexToActivateOnPress=p.bitIndexToActivateOnPress;
		dbConnectionName=p.dbConnectionName;
		databaseType=p.databaseType;
		databaseDescription=p.databaseDescription;
		logTableName=p.logTableName;
		plcInactiveStyle=p.plcInactiveStyle;
		plcActiveStyle=p.plcActiveStyle;
		plcInactiveText=p.plcInactiveText;
		userPromptText=p.userPromptText;
		tuxip=p.tuxip;
		isToogle=p.isToogle;
		return *this;
	}
Q_OBJECT								
public:
	myPushButtonClass(myPushButtonClass *m);
	myPushButtonClass(QString buttonText_,QString buttonIcon_,tuxipClass *tuxip_,QString commandPlcAdress_,int plcIndexToSendTheCommand_,int valueToPlcOnPress_,int valueToPlcOnRelease_,
		QString dbConnectionName_,QString databaseType_,QString databaseDescription_,QString logTableName_,
		QString plcInactiveStyle_,QString plcActiveStyle_,QString plcInactiveText_,QWidget *p,int bitIndexToActivateOnPress_=-1,bool isToogle_=false);
	~myPushButtonClass();
	void create();
	//
	void mousePressEvent(QMouseEvent * e);
	void mouseReleaseEvent(QMouseEvent * e);
	void mouseMoveEvent(QMouseEvent *e);
	void resizeEvent(QResizeEvent *e);
	void hideEvent(QHideEvent *e);
	void setPlcActive(bool a);
	void setUserPromptText(QString str);
	void setSendDialogText(QString str);
	void sendCommand();
	void clearCommand();
	//
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,int value,QString description);
	void setEnabled(bool e);
	void setDisabled(bool e);
};
class hmiPushButtonWidgetClass:public QWidget		
{
	QString													buttonText;
	QString													commandPlcAdress;
	int															plcIndexToSendTheCommand;
	int															valueToPlcOnPress;
	int															valueToPlcOnRelease;
	QString													dbConnectionName;
	QString													databaseType;
	QString													logTableName;

	QString													activePlcAdress;
	int															activePlcIndex;
	int															activePlcBit;
	QString													activeBitDescription;
	bool														isActive;
	bool														firstTimeUpdatedFromPLC;
	//
	myPushButtonClass								*pushButton;
	QLabel													*activeLabel;
	QString													plcInactiveText;
	//
Q_OBJECT								
public:										
	hmiPushButtonWidgetClass(QString buttonText_,QString buttonIcon,tuxipClass *tuxip_,
		QString commandPlcAdress,int plcIndexToSendTheCommand,int valueToPlcOnPress,int valueToPlcOnRelease,
		QString activeIcon,QString activePlcAdress_,int activePlcIndex_,int activePlcBit_,QString activeBitDescription_,
		QString dbConnectionName_,QString databaseType_,QString databaseDescription,QString logTableName_,
		QString plcInactiveStyle_,QString plcActiveStyle_,QString plcInactiveText_,QWidget *p,int bitIndexToActivateOnPress=-1,bool isToogle=false);
	~hmiPushButtonWidgetClass();
	void setUserPromptText(QString str);
	void setSendDialogText(QString str);
	void setIconSize(QSize s);
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,int value,bool status,QString description);
	void updateValue(QString plcAdress,int plcIndex,int intValue);
};
#endif

