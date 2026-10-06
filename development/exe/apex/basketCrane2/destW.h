#ifndef destW_H
#define destW_H

#include <QtGui>
#include <QGroupBox>
#ifdef stationsExe
#include "Gc.h"
#endif
#ifdef basketWareHouse
#include "../basketWarehouse/Gb.h"
#endif
#include "serv.h"
#include "conPos.h"
#include "conMove.h"
#include "hmiGuiWidgetsLib.h"
class destackerWidgetClass : public QWidget				
{
	QVector<bitClass*>										bitVector;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	QVector<bitLogVectorClass*>						bitLogVectors;
public:
	Q_OBJECT								
public:										
	destackerWidgetClass(tuxipClass *tuxip,QWidget *p);
	~destackerWidgetClass();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};											
#endif
