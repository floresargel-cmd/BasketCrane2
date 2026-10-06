#ifndef MEM_H
#define MEM_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include "Ghmi.h"
#include "mySpin.h"
class plcMemoryClass:public QWidget		
{
protected:
	tuxipClass											*tuxip;
	QString													ip;
	QString													plcAdress;
	int															plcIndex;
	QString													title;
	QString													suffix;
	QString													dbConnectionName;
	QString													databaseType;
	QString													databaseDescription;
	QString													logTableName;
	QLabel													*titleLabel;
	
	QLabel													*valueLabel;
	mySpinBoxWidgetClass						*intValueSpinBox;
	myFloatSpinBoxWidgetClass				*floatValueSpinBox;

	int															firstUpdate;
	int															type;
	int															dataType;
	QString													styleSheetDisplayOnly;
	QString													styleSheetOutOfLimits;
	QString													styleSheetInLimits;
	QString													styleForTitle;
	//
public:
	enum{//type
		TYPE_USER_CAN_CHANGE_VALUE_POPUP=1,
		TYPE_USER_CAN_CHANGE_VALUE_INLINE,
		TYPE_USER_CANNOT_CHANGE_VALUE,
	};
	enum{//dataType
		DATA_TYPE_INT=1,
		DATA_TYPE_FLOAT,
	};
Q_OBJECT								
public:										
	plcMemoryClass(tuxipClass *tuxip_,QString ip_,QString plcAdress_,int plcIndex_,QString title_,QString dbConnectionName_,QString databaseType_,QString logTableName_,QString databaseDescription_,int type_,int datatType_,int minWidth,QString suffix_=QString(),QWidget *p=NULL);
	~plcMemoryClass();
	virtual void	mouseReleaseEvent(QMouseEvent *e);
	void setStyleSheet(QString styleSheetDisplayOnly_,QString styleSheetOutOfLimits_,QString styleSheetInLimits_,QString styleForTitle_=QString());
	void setBtnSize(QSize s);
};
//////////////////////////////////////////////////////////////intPlcMemoryClass//////////////////////////////////////////
class intPlcMemoryClass:public plcMemoryClass		
{
	int															value;
	int															minValue;
	int															maxValue;
	int															spinStep;
	//
Q_OBJECT								
public:										
	intPlcMemoryClass(tuxipClass *tuxip,QString ip_,QString plcAdress,int plcIndex,int minValue_,int maxValue_,QString title,QString dbConnectionName,QString databaseType,QString logTableName,QString databaseDescription,int minWidth,int type,QString suffix=QString(),QWidget *p=NULL);
	~intPlcMemoryClass();
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,int value,bool status,QString description);
	void updateValue(QString ip_,QString plcAdress_,int plcIndex_,int v);
	virtual void	mouseReleaseEvent(QMouseEvent *e);
	void setSpinStep(int step);
	void setMinValue(int v);
	void setMaxValue(int v);
public slots:
	void intValueSpinBoxValueReadySlot(int value);
};
//////////////////////////////////////////////////////////////floatPlcMemoryClass//////////////////////////////////////////
class floatPlcMemoryClass:public plcMemoryClass		
{
	float															value;
	float															minValue;
	float															maxValue;
	int																noOfDecimals;
	//
Q_OBJECT								
public:										
	floatPlcMemoryClass(tuxipClass *tuxip,QString ip_,QString plcAdress,int plcIndex,float minValue_,float maxValue_,QString title,QString dbConnectionName,QString databaseType,QString logTableName,QString databaseDescription,int minWidth,int type,int noOfDecimals_,QString suffix=QString(),QWidget *p=NULL);
	~floatPlcMemoryClass();
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,float value,bool status,QString description);
	void updateValue(QString ip_,QString plcAdress_,int plcIndex_,float v);
	virtual void	mouseReleaseEvent(QMouseEvent *e);
	void setSpinStep(double step);
	void sendValueToPlc(float v);
	float getValue();
	void setMinValue(float v);
	void setMaxValue(float v);
public slots:
	void floatValueSpinBoxValueReadySlot(float value);
};
#endif

