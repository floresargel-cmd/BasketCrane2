#ifndef HAND_H
#define HAND_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "Ghmi.h"
#include "logHistory.h"
//////////////////////////////////////////////////////handShakeClass////////////////////////////////////////////////
class handShakeClass:public QObject///
{
	tuxipClass	*tuxipConnection;
	QString			plcIp;
	QString			plcToAdress;
	int					plcToIndex;
	QString			plcFromAdress;
	int					plcFromIndex;
	//
	QTimer			*mainTimer;
	int					pcValue;
	int					plcValue;
	bool				firstScan;
	bool				comsOk;
	int					type;
public:
	enum
	{
		typeWriteRead=1,//pc writes and waits to see it in plc
		typeReadOnly=2,//plc incriments and pc waits to see it change
	};
Q_OBJECT								
public:
	handShakeClass(tuxipClass	*tuxipConnection_,QString plcIp_,QString plcToAdress_,int plcToIndex_,QString plcFromAdress_,int plcFromIndex_,int timeInterval,int type_=1);
	~handShakeClass();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	bool getComsOk();
public:
signals:
	void stateChangedSignal(bool s);
public slots:								
	void onMainTimerSlot();						
};
//////////////////////////////////////////////////////handShakeLabelClass////////////////////////////////////////////////
class handShakeWidgetClass:public QWidget///
{
	handShakeClass		*handShake;
	QLabel						*txtLabel;
	QLabel						*iconLabel;
	QPixmap						okPixMap;
	QPixmap						notOkPixMap;
public:
Q_OBJECT								
public:
	handShakeWidgetClass(QString title,QPixmap okPixMap_,QPixmap notOkPixMap_,tuxipClass *tuxipConnection,QString plcIp,QString plcToAdress,int plcToIndex,QString plcFromAdress,int plcFromIndex,int timeInterval,QWidget *p);
	~handShakeWidgetClass();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:								
	void handShakeStateChangedSlot(bool s);						
};
#endif




