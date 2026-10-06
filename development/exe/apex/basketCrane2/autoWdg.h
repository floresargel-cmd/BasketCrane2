#ifndef AUTOWDG_H
#define AUTOWDG_H

#include <QtGui>
#include <QMainWindow>

#include "Gb2.h"
#include "crane.h"
class autoWidgetClass : public QWidget
{
	craneClass									*crane;
	QCheckBox										*exportsToDestackerCheckBtn;
	QCheckBox										*exportsToPackingCheckBtn;
	QCheckBox										*importsFromNorthCheckBtn;
	QCheckBox										*importsFromSouthCheckBtn;
	QCheckBox										*autoExportsCheckBtn;
	QPushButton									*calculateExportGroupsBtn;
	Q_OBJECT								
public:										
	autoWidgetClass(craneClass *crane_,QWidget *p=NULL);
	~autoWidgetClass();
signals:
	void calculateExportGroupsSignal();
public slots:
	void exportsToDestackerCheckBtnStateChangedSlot(int state);
	void exportsToPackingCheckBtnStateChangedSlot(int state);
	void importsFromNorthCheckBtnStateChangedSlot(int state);
	void importsFromSouthCheckBtnStateChangedSlot(int state);
	void autoExportsCheckBtnStateChangedSlot(int state);
	void sendBasketsToCrane1Slot();
	void calculateExportGroupsBtnClickedSlot();
};											
#endif
