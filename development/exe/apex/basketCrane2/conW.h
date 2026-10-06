#ifndef conW_H
#define conW_H

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
#include "conAllPos.h"
class conveyorsWidgetClass : public QWidget				
{
	QVector<bitClass*>										bitVector;
	allConvPositionsClass									*allConvPositions;
	QVector<hmiPushButtonWidgetClass*>		pushButtonsVector;
	QVector<bitLogVectorClass*>						bitLogVectors;

public:
	Q_OBJECT								
public:										
	conveyorsWidgetClass(tuxipClass *tuxip,QWidget *p);
	~conveyorsWidgetClass();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};											
#endif
