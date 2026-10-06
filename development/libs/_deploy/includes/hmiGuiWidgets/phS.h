#ifndef PHS_H
#define PHS_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "Ghmi.h"
#include "logHistory.h"
//////////////////////////////////////////////////////////photoSensorClass////////////////////////////////////////////////////////////
class photoSensorClass:public QWidget///one bit
{
	QString			dbConnectionName;
	QString			databaseType;
	QString			logTableName;
	QString			plcAdress;
	QString			description;
	QColor			color;
	int					plcIndex;
	int					plcBit;
	bool				value;
	int					width;
	int					height;
	int					firstUpdate;
	//
public:
	int type;
	enum{
		TYPE_HORIZONTAL=1,
		TYPE_VERTICAL=2,
	};
public:
Q_OBJECT								
public:
	photoSensorClass(QString plcAdress_,int plcIndex_,int plcBit_,int width_,int height_,int type_,QString description_,QColor color_,QString dbConnectionName_,QString databaseType_,QString logTableName_,QWidget* p);
	void updateValue(QString plcAdress_,int plcIndex_,int intValue);
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,int plcBit,bool status,QString bitDescription);
	void paintEvent(QPaintEvent *e);
};
	#endif
