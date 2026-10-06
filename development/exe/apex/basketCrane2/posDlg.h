#ifndef POSDLG_H
#define POSDLG_H

class posDlgClass;
#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include "Gb2.h"
#include "pos.h"
#include "allPos.h"
#include "bCont.h"
class posDlgClass : public QDialog				
{												
	posClass										*parrentPos;
	allPossClass								*allPoss;
	//
	QTabWidget									*mainTabWidget;
	QSpinBox										*basketSpinBox;
	QLabel											*basketInfoLabel;
	QCheckBox										*lockedCheckBox;
	basketContentsClass					*basketContents;
	QTableWidget								*basketLogTable;
	QPushButton									*okBtn;
public:										
	Q_OBJECT								
public:										
	posDlgClass(allPossClass *allPoss_,posClass *parrentPos_,QString user,QWidget *p);
	~posDlgClass();
	int myExec(bool checkPassword);
	int getBasketNum();
	void setBasketNum(int b);
	void updateBasketData();
	void accept();
	void reject();
private:										
	int exec();
public slots:								
	void basketSpinBoxValueChangedSlot(int);
};											
#endif
