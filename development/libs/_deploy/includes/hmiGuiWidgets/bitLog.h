#ifndef BITLOG_H
#define BITLOG_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QGroupBox>
#include "Ghmi.h"
#include "logHistory.h"
//////////////////////////////////////////////////////////bitClass////////////////////////////////////////////////////////////
class bitClass:public QLabel///one bit
{
	QTimer			*mainTimer;
	QString			dbConnectionName;
	QString			databaseType;
	QString			primaryLogTableName;
	QString			secondaryLogTableName;
//	QString			plcIp;
	QString			plcAdress;
	int					plcIndex;
	int					plcBit;
	bool				value;
	bool				firstUpdate;
	QString			bitOnDescription;
	QString			bitOffDescription;
	int					type;
	QString			onStyle;
	QString			offStyle;
	int					blinkingType;
	bool				blinkBit;
	QString			blinkingStyle;
public:
//enum{//styles
//	STYLE_R=1,
//	STYLE_G,
//	STYLE_B,
//	STYLE_O,
//	STYLE_GRAY,
//	};
enum{//type
	TYPE_SHOW_HIDE=1,
	TYPE_CHANGE_COLOR=2,
	};
enum{//blinkingType
	TYPE_BLINKING_NONE=1,
	TYPE_BLINKING_IN_ON,
	TYPE_BLINKING_IN_OFF,
	};
Q_OBJECT								
public:
	bitClass(QString plcAdress_,int plcIndex_,int plcBit_,int type_,QString bitOnDescription_,QString bitOffDescription_,QString onStyle_,QString offStyle_,QString dbConnectionName_,QString databaseType_,QString primaryLogTableName_,QString secondaryLogTableName_,QWidget *p);
	~bitClass();
	void updateValue(QString plcAdress_,int plcIndex_,int value_,bool forceLog=false);
	bool getBitBalue();
	void setType(int t);
	void setTypeOfBit(int ind,int bit,QString onStyle,QString offStyle,QString blinkingStyle,int blinkingType);
	void setBlinkingStyle(QString blinkingStyle_,int type_,int blinkRate=1000);
	void updateGui();
	QString getBitOnDescription();
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,int plcBit,bool status,QString bitDescription);
	void forceSetValue(bool v);
	bool getValue();
	void setBitDescriptions(QString on,QString off);
	//void resizeEvent(QResizeEvent *ev);
protected:
	void updateValue(int bit_,bool value_,bool forceLog);
public slots:
	void onMainTimerSlot();
};
//////////////////////////////////////////////////////////multyStatebitClass////////////////////////////////////////////////////////////
class multiStatebitClass:public QWidget///multyStatebitClass
{
	QVBoxLayout									*mainLayout;
	QLabel*											nothingBitOnLabel;
	QVector<bitClass*>					bitLabelsVector;
	bool												firstTimeUpdatedFromPLC;
Q_OBJECT								
public:
	multiStatebitClass(QString txt,QString style,QWidget *p);
	~multiStatebitClass();
	void addBitLabel(bitClass *b);
	void updateValue(QString plcAdress,int plcIndex,int value);
	void setTypeOfBit(int ind,int bit,QString onStyle,QString offStyle,QString blinkingStyle,int blinkingType);
	void setBlinkingStyle(QString blinkingStyle,int type);
	void updateGui();
};
//////////////////////////////////////////////////////////////bitLogIntClass////////////////////////////////////////////////////////////
class bitLogIntClass : public QVector<bitClass*>,public QWidget//one integer
{
	QString							plcAdress;
	int									plcIndex;
	QString							dbConnectionName;
	QString							databaseType;
	QString							primaryLogTableName;
	QString							secondaryLogTableName;
	bool								firstTimeUpdatedFromPLC;
	int									type;
public:										
	bitLogIntClass(QString plcAdress_,int plcIndex_,int type_,QString dbConnectionName_,QString databaseType_,QString primaryLogTableName_,QString secondaryLogTableName_,QWidget *p);
	~bitLogIntClass();
	QWidget* appendBit(int bit,QString bitOnDescription,QString bitOffDescription,QString onStyle,QString offStyle);
	int getPlcIndex();
	bool getHasActive();
	QString getDescriptionOfFirstActiveBit();
	bool getValueOfFirstBit();
	void setType(int t);
	void updateValue(QString plcAdress_,int plcIndex_,int value);
	void setBlinkingStyle(QString blinkingStyle,int type_);
	void setTypeOfBit(int ind,int bit,QString onStyle,QString offStyle,QString blinkingStyle,int blinkingType);
};
//////////////////////////////////////////////////////////////bitLogVectorClass////////////////////////////////////////////////////////////
class bitLogVectorClass : public QGroupBox		
{
	QString													dbConnectionName;
	QString													databaseType;
	QString													primaryLogTableName;//for errors loging 
	QString													secondaryLogTableName;//for warning logging
	QString													plcAdress;
	int															bitType;
	QString													onStyle;
	QString													offStyle;
	//
	QVector<bitLogIntClass*>				bitLogIntsVector;
	//
	QHBoxLayout											*bitsLayout;
	QVBoxLayout											*currentLayout;
	int															noOfColumns;
	int															type;
	QWidget													*parrent;
	int															historyClassDatabaseEnum;
public:										
enum{
	TYPE_ALWAYS_SHOW=1,
	TYPE_SHOW_ONLY_IF_FIRST_BIT_IS_ACTIVE=2,
	};
public:										
Q_OBJECT								
public:										
	bitLogVectorClass(QString plcAdress_,QString bitsDescriptionfileName,int bitType_,QString onStyle_,QString offStyle_,QString dbConnectionName_,QString databaseType_,QString primaryLogTableName_,QString secondaryLogTableName_,bool useTrnaslation,QWidget *p);
	bitLogVectorClass(QString title,QString plcAdress_,QString onStyle_,QString offStyle_,QWidget *p);//this is for insight
	~bitLogVectorClass();
	void readBitsDescriptionfile(QString fileName,bool useTrnaslation=false);
	void readBitsDescriptions(QTextStream &fileStream,bool useTrnaslation=false);
	void addLayout();
	int getNumberOfColumns();
	void close();
	void addBit(int index,int bit,QString onDescription,QString offDescription);
	void addBitsEnded();
	void updateValue(QString plcAdress_,int plcIndex,int value);
	QString getDescriptionOfFirstActiveBit();
	bool getValueOfFirstBit();
	void setType(int t);
	void setBitType(int t);
	void updateGui();
	void setBlinkingStyle(QString blinkingStyle,int type_);
	void setTypeOfBit(int ind,int bit,QString onStyle,QString offStyle,QString blinkingStyle,int blinkingType);
	void setBitLogHistoryClassDatabaseEnum(int en);
public slots:
	void historyBtnClickedSlot();
};
#endif
