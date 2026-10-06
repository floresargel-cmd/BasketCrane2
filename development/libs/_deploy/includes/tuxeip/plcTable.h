#ifndef PLCTABLE_H
#define PLCTABLE_H

#include <QtGui>
#include <QSpinBox>
//#include <ti_me.h> 
#include "tuxG.h"
#include "plcVar.h"
////////////////////////////////readFloatsClass////////////////////////////////////
class readFloatsClass:public QObject
{
	tuxipClass								*tuxip;
	QString										tableName;
Q_OBJECT
public:
	readFloatsClass(tuxipClass *tuxip_,QString tableName_);
	QVector<float> read(int minIndex,int maxIndex);
};
////////////////////////////////plcVar////////////////////////////////////
class plcTableClass:public QWidget
{
public:
	QTimer										mainTimer;
	QTimer										delayTimer;
	Q_OBJECT
	tuxipClass								*tuxip;
	readFloatsClass						*readFloats;
	QSpinBox									*updateFromPlcTimerSpinBox;
	QString										tableName;
	int												minIndex;
	int												maxIndex;
	int												updateInterval;//millisecond
	int												type;
	QVector<intClass*>				intClassesVector;
	QVector<floatClass*>			floatClassesVector;
	bool											emulateMode;
	bool											isAutoUpdatingFromPlc;
public:
	enum{
	TYPE_INT=1,
	TYPE_FLOAT,
	};
	plcTableClass &operator=(const plcTableClass& v)
	{
		tuxip=v.tuxip;
		tableName=v.tableName;
		intClassesVector=v.intClassesVector;
		floatClassesVector=v.floatClassesVector;
		minIndex=v.minIndex;
		maxIndex=v.maxIndex;
		updateInterval=v.updateInterval;
		return *this;
	};
	//
public:
	plcTableClass(tuxipClass *tuxip_,int type_,QString tableName_,int minIndex_,int maxIndex_,bool emulate_,int updateInterval_,QWidget *p);
	~plcTableClass();
	void setUpdatePlcReadInterval(int ms);
	QString getName();
	void setVarComment(int index,QString comment);
	void setBitComment(int index,int bit,QString comment);
	int getType();
	void setValueOfVariable(int index,QVariant val);
	int getOldIntValue(int index);
	int getOldIntValue();
	QVector<int> getOldIntValues(int index,int numberOfValues);
	float getOldFloatValue(int index);
	QVector<float> getOldFloatValues(int index,int numberOfValues);
	void setIsAutoUpdatingFromPlc(bool u);
	void updateFromPlc(int delay);
signals:
	void dataInPlcChangedSignal(QString table,int index,QVariant value);
public slots:								
	void dataSendToPlcSlot();
	void dataInPlcChangedSlot(int index,QVariant value);
	void onMainTimerSlot();
	void updateFromPlcTimerSpinBoxChangedSlot(int ms);
};
#endif