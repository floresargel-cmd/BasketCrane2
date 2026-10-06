#ifndef MYSPIN_H
#define MYSPIN_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include "Ghmi.h"
///////////////////////////////////////////////////////mySpinBoxClass
class mySpinBoxClass:public QSpinBox		
{
	int															lastValue;
public:
Q_OBJECT								
public:										
  mySpinBoxClass(QWidget *p=NULL);
	~mySpinBoxClass();
	void mySetValue(int value);
	void loadOldValue();
	virtual void	keyPressEvent(QKeyEvent * ev);
	virtual void	focusOutEvent(QFocusEvent * ev);
signals:
	void focusLostSignal();
	void valueReadySignal();
};
///////////////////////////////////////////////////////mySpinBoxWidgetClass
class mySpinBoxWidgetClass:public QWidget		
{
	mySpinBoxClass									*spinBox;
	QPushButton											*readyPushButton;
public:
Q_OBJECT								
public:										
  mySpinBoxWidgetClass(QWidget *p=NULL);
	~mySpinBoxWidgetClass();
	void setValue(int v);
	void setMinimum(int v);
	void setMaximum(int v);
	void setSuffix(QString s);
	void setSpinStep(int step);
	void setBtnSize(QSize s);
	virtual void	focusOutEvent(QFocusEvent * ev);
signals:
	void valueReadySignal(int v);
public slots:
	void spinBoxLostFocusSlot();
	void valueReadySlot();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////myFloatSpinBoxClass///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
class myFloatSpinBoxClass:public QDoubleSpinBox		
{
	float															lastValue;
public:
Q_OBJECT								
public:										
  myFloatSpinBoxClass(QWidget *p=NULL);
	~myFloatSpinBoxClass();
	void mySetValue(float value);
	void loadOldValue();
	virtual void	keyPressEvent(QKeyEvent * ev);
	virtual void	focusOutEvent(QFocusEvent * ev);
signals:
	void focusLostSignal();
	void valueReadySignal();
};
///////////////////////////////////////////////////////myFloatSpinBoxWidgetClass
class myFloatSpinBoxWidgetClass:public QWidget		
{
	myFloatSpinBoxClass							*floatSpinBox;
	QPushButton											*readyPushButton;
public:
Q_OBJECT								
public:										
  myFloatSpinBoxWidgetClass(QWidget *p=NULL);
	~myFloatSpinBoxWidgetClass();
	void setValue(float v);
	void setMinimum(float v);
	void setMaximum(float v);
	void setSpinStep(double step);
	void setDecimals(int d);
	void setSuffix(QString s);
	void setBtnSize(QSize s);
	virtual void	focusOutEvent(QFocusEvent * ev);
signals:
	void valueReadySignal(float v);
public slots:
	void spinBoxLostFocusSlot();
	void valueReadySlot();
};
#endif

