#ifndef SLCVAR_H
#define SLCVAR_H

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTimer>
#include "utilities.h"
#include "dtl.h"

class slcVarClass: public QObject
{
public:
	QString							nameIp;//for search only
	QString							plcTable;
	int									plcFirstIndex;
	unsigned long				varId;
	int									dataType;
	int									plcCommunicationTimeOut;
	QVector<QVariant>		values;
	//
public:
	enum {//dataType
		TYPE_INTEGER=1,
		TYPE_FLOAT,
	};
Q_OBJECT
public:
	slcVarClass(QString nameIp_,QString plcTable_,int plcFirstIndex_,int dataType_,int noOfVars,QString driverName,int station);
	slcVarClass::~slcVarClass();
	QVector<QVariant> readValues();
	QVariant getValue(int index);
	void write(QVariant val);
	//
	bool checkError(int ans,unsigned long io_stat=-1);
signals:
	void dataInPlcChangedSignal(QString ip,QString plcTable,int plcIndex,QVariant value);
};
#endif
