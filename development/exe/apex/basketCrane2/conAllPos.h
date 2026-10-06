#ifndef conAllPos_H
#define conAllPos_H

class proxItemClass;
#include <QtGui>
#include <QMainWindow>
#ifdef stationsExe
#include "Gc.h"
#endif
#ifdef basketWareHouse
#include "../basketWarehouse/Gb.h"
#endif
#include "serv.h"
#include "conPos.h"
#include "conMove.h"
#include "gView.h"
#include "conProxItem.h"
class allConvPositionsClass : public QWidget				
{												
	tuxipClass														*tuxip;
	myQGraphicsViewClass									*layoutGraphicsView;
	QGraphicsScene												*layoutScene;
	QVector<convPositionClass*>						positionsVector;
	QVector<moveClass*>										movesVector;
	QVector<buttonClass*>									btnsVector;
	QVector<proxItemClass*>								proxVector;
	bool																	inMaintenanceMode;
public:
	Q_OBJECT								
public:										
	allConvPositionsClass(tuxipClass *tuxip_,QWidget *p);
	~allConvPositionsClass();
	convPositionClass* getPosWithId(int id);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:								
	void onMainTimerSlot();
	void resetGraphicsViewZoom();
};											
#endif
