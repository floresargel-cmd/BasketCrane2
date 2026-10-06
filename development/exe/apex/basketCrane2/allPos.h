#ifndef ALLPOS_H
#define ALLPOS_H

class allPossClass;
#include <QtGui>
#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "gView.h"
#include "pos.h"
#include "crane.h"
class posCorrectionClass
{
public:
	int				col;
	int				row;
	float     x;
	float     y;
	float     z;
public:
	posCorrectionClass()
	{
		col=row=0;x=y=z=0.f;
	}
	posCorrectionClass(int col_,int row_,float x_,float y_,float z_)
	{
		col=col_;row=row_;x=x_;y=y_;z=z_;
	}
};
class allPossClass : public QWidget				
{
	tuxipClass															*oldOvenTux;
	tuxipClass															*craneTux;
	QVector<posClass*>											posV;
	QVector<posClass*>											posWithBasketsForExportV;
	craneClass															*crane;
	//
	posClass 																*manualSelectedFromPos;
	int																			noOfRows;

	posClass																*returningPos1;	
	posClass																*returningPos2;	
	posClass																*returningPos3;	
	Q_OBJECT								
	int																			selectMode;
public:										
	enum{//selectMode
		selectForMission=0,
		selectToModify,
		selectToExportDestacker,
		selectToExportPacking,
        selectToExportPackingA1,
        selectToExportPackingA2,
	};
public:										
	allPossClass(tuxipClass *craneTux_,tuxipClass *oldOvenTux_,tuxipClass *newOvenTux,QGraphicsScene *layoutScene,QWidget *p=NULL);
	~allPossClass();
	void loadFromDatabase();
    QVector<posClass*> displayPositions() const { return posV; }
	void setSelectMode(int m);
	void setCraneClass(craneClass *c);
	posClass* getPosWithPosIndex(int pos,int index,bool unLockedOnly=true);
	posClass* getPosWithBasket(int b,bool unLockedOnly=true);
	posClass* getPosWithBasketToMoveInAuto(int b);
	QVector<posClass*> getPositionsOfColumn(int col);
	QVector<posClass*> getPosWithBasketsForExport();
	void setUpDownPositionsOfColumn(int column);
	void setUpObstacleOfColumn(int column,QVector<posClass*> v);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	int moveBasket(posClass *from,posClass *to);
	void setDisplayMode(int m);
	void clearIsFromPos();
	void clearIsToPos();
	posClass* getFreeReturningPosition();
	void checkReturningConveyor();
	void writeBasketToReturningConveyor();
	void removeBasketFromExports(int basket);
	void addBasketToExports(int basket,QString table);
signals:
	void exportListChangedSignal();
	void sendBasketsToCrane1Signal();
public slots:
	void mainTimerSlot();
	void posMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*);
	void userChangedBasketSlot(posClass *pos);
	void graphicsViewKeyPressedSlot(QKeyEvent *ev);		
	void exportListChangedSlot();		
};											
#endif
