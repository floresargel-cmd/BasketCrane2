#ifndef PRS_H
#define PRS_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "Ghmi.h"
#include "logHistory.h"
#include "lbOnOff.h"
//////////////////////////////////////////////////////////proximitySwitchClass////////////////////////////////////////////////////////////
class proximitySwitchClass:public labelOnOffClass///one bit
{
public:
Q_OBJECT								
public:
	proximitySwitchClass(QString plcAdress_,int plcIndex_,int plcBit_,int width_,int height_,QString description_,QString colorOn_,QString colorOff_,QString dbConnectionName_,QString databaseType_,QString logTableName_,QWidget* p);
};
	#endif
