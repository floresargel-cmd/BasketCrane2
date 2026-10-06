#ifndef conPosDlg_H
#define conPosDlg_H

//class cPosDlgClass
#include <QtGui>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QDialog>
#include <QComboBox>
#include "Gb2.h"
#include "hmiGuiWidgetsLib.h"
class cPosDlgClass : public QDialog				
{												
	int													posId;
	int													plcId;
	QString											description;
	int													basketNum;
	int													basketDestination;
	//
	QSpinBox										*basketSpinBox;
	int													basket;
	QPushButton									*okBtn;
	QLabel											*basketInfoLabel;
	QVector<bitClass*>					bitVector;
	QComboBox										*destinationsCombo;
Q_OBJECT								
public:										
	cPosDlgClass(int posId_,int plcId_,QString posDescription,QWidget *p);
	~cPosDlgClass();
	int exec();
	int getBasketNum();
	int getBasketDestination();
	void accept();
	void reject();
	void setData(int b,int d);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:
	void basketSpinBoxChangedSlot(int b);
};											
#endif
