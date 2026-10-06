#ifndef LBLONOFF_H
#define LBLONOFF_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "Ghmi.h"
#include "logHistory.h"

////////////////////////////////////////////////////////////labelOnOffClass////////////////////////////////////////////////////////////
class labelOnOffClass:public QWidget
{
	QString			dbConnectionName;
	QString			databaseType;
	QString			logTableName;
	QString			plcAdress;
	QString			description;
	int					plcIndex;
	int					plcBit;
	bool				value;
	int					width;
	int					height;
	bool				firstUpdate;
	//
	QLabel			*imageLabel;
	QLabel			*txtLabel;
	QString			colorOn;
	QString			colorOff;
	QString			iconOn;
	QString			iconOff;
		
	QTimer			*blinkingTimer;
	QString			blinkingPlcAdress;
	int					blinkingPlcIndex;
	int					blinkingPlcBit;
	int					blinkingInterval;
	bool				blinkValue;
	bool				blinkIsOnOnGui;
	public:
	Q_OBJECT								
	public:
	labelOnOffClass(QString plcAdress_,int plcIndex_,int plcBit_,int width_,int height_,QString description_,QString colorOn_,QString colorOff_,QString IconOn_,QString IconOff_,QString dbConnectionName_,QString databaseType_,QString logTableName_,QWidget* p);
	void updateValue(QString plcAdress_,int plcIndex_,int intValue);
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,int plcBit,bool status,QString bitDescription);
	void setBlinking(QString blinkingPlcAdress_,int blinkingPlcIndex_,int blinkingPlcBit_,int interval);
	void updateGui();
public slots:
	void blinkingTimerTimeOutSlot();
};
#endif
