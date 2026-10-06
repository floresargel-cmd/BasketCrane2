#ifndef ACT_H
#define ACT_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsColorizeEffect>
#include "Ghmi.h"
#include "logHistory.h"
//////////////////////////////////////////////////////////activeIconClass////////////////////////////////////////////////////////////
class activeIconClass:public QLabel///one bit
{
	QTimer			*animateTimer;
	QString			dbConnectionName;
	QString			databaseType;
	QString			logTableName;
	QString			plcAdress;
	QString			description;
	int					plcIndex;
	int					plcBit;
	bool				value;
	bool				firstUpdate;
	QPixmap			onPixMap;
	QPixmap			offPixMap;
	//
	QGraphicsColorizeEffect *effect;

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
	activeIconClass(QString plcAdress_,int plcIndex_,int plcBit_,QPixmap onPixMap_,QPixmap offPixMap_,QString description_,QString dbConnectionName_,QString databaseType_,QString logTableName_,QWidget* p);
	~activeIconClass();
	void updateValue(QString plcAdress_,int plcIndex_,int intValue);
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,int plcBit,bool status,QString bitDescription);
	void setBlinking(QString blinkingPlcAdress_,int blinkingPlcIndex_,int blinkingPlcBit_,int interval);
	void updateGui();
public slots:								
	void onAnimateTimerSlot();
	void blinkingTimerTimeOutSlot();
};
	#endif
