#ifndef PLCVAR_H
#define PLCVAR_H

#include <QtGui>
#include <QHBoxLayout>
#include <QListWidget>
#include <QApplication>
#include <QPushButton>
//#include <tim_e.h> 
#include "tuxG.h"
class tuxipClass;
class tuxipClassSLC;
class floatClass;
class intClass;
#include "tuxClass.h"
#include "lineEdits.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////varClass///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class varClass:public QWidget		
{
#ifdef USE_OLDVALUES
protected:
	QVector<QVariant>		oldValuesVector;
#endif
protected:
	tuxipClass					*tuxip;
	QString							tableName;
	int									index;
	bool								emulateMode;
	bool								hasLastValueBeenEmited;
	QHBoxLayout					*mainLayout;
	QVariant						lastValue;
	bool								firstUpdate;
	//
	myLabel							*adressLabel;
public:
Q_OBJECT
public:
	varClass(tuxipClass *tuxip_,QString tableName_,int index_,bool emulate_,QWidget *p=NULL);
	~varClass();
	int getIndex();
	void emitChangeSignal();
	void setComment(QString comment);
signals:
	void dataSendToPlcSignal();
	void dataInPlcChangedSignal(int index,QVariant value);
public slots:
	void adressLabelClickedSlot();
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////intClass///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class intClass:public varClass		
{
	intLineEditClass		*intLineEdit;
public:
Q_OBJECT
public:
	intClass(tuxipClass *tuxip_,QString tableName_,int index_,bool emulateMode,QWidget *p=NULL);
	~intClass();
	void setBitComment(int bit,QString comment);
	int getOldValue();
	void setDataFromPlc(int v);
public slots:
	void intLineEditEnterSlot(int val);
	void intLineFocusOutSlot();
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////floatClass///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class floatClass:public varClass		
{
	floatLineEditClass	*floatLineEdit;
public:
Q_OBJECT
public:
	floatClass(tuxipClass *tuxip_,QString tableName_,int index_,bool emulateMode,QWidget *p=NULL);
	~floatClass();
	float getOldValue();
	void setDataFromPlc(float v);
public slots:
	void floatLineEditEnterSlot(float val);
	void floatLineFocusOutSlot();
};
#endif