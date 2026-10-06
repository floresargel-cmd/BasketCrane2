#ifndef dataLogingH
#define dataLogingH
#include <QtGui>
#include <QSqlQuery>
#include <QScrollBar>
#include "serv.h"
////////////////////////////////dataLogingValsClass////////////////////////////////////
class dataLogingValsClass:public QObject
{
	QString							dbVarsTable;
	QString							dbValsTable;
	int									dbVarsTableId;

	QVector<quint64>		secsVector;
	QVector<float>			valsVector;
public:
	QString							plcIp;
	QString							plcTableName;
	int									plcIndex;
Q_OBJECT
public:
	dataLogingValsClass(QString dbVarsTable_,QString dbValsTable_,int dbVarsTableId_,QString plcIp_,QString plcTableName_,int plcIndex_,QObject *p);
	~dataLogingValsClass();
	int getLength();
	void deleteOlderThan(quint64 sec);
	void deleteAll();
	void save(QSqlDatabase *logDb,QLabel *progressLabel);
	int noOfValus();
public slots:	
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
};
////////////////////////////////dataLogingVarClass////////////////////////////////////
////////////////////////////////dataPlayerClass////////////////////////////////////
struct plcVar
{
public:
	int logTableId;
	QString plcIp;
	QString plcTable;
	int plcIndex;
	QList<QVariant>			valsList;
	QList<quint64>			secsList;
	plcVar()
	{
		logTableId=plcIndex=-1;
	}
	plcVar(int logTableId_,QString ip_,QString table_,int index_)
	{
		logTableId=logTableId_;
		plcIp=ip_;
		plcTable=table_;
		plcIndex=index_;
	};
	bool addVal(int tableId,QVariant v,quint64 s)
	{
		if (tableId==logTableId)
		{
			valsList<<v;
			secsList<<s;
			return true;
		}
		return false;
	}
	QVariant getValueForMSec(quint64 mSec)
	{
		QVariant ans;
		for (int i=1;i<valsList.count();i++)
		{
			if (secsList[i]==mSec)
			{
				ans=valsList[i];
				break;
			}
			if (secsList[i]>mSec)
			{
				ans=valsList[i-1];
				break;
			}
		}
		return ans;
	}
	bool hasValueOnTime(quint64 mSec)
	{
		for (int i=0;i<valsList.count();i++)
		{
			if (secsList[i]==mSec)
				return true;
		}
		return false;
	}
};
class dataPlayerClass:public QWidget
{
	QTimer							*mainTimer;
	QString							dbVarsTable;
	QString							dbValsTable;
	QList<plcVar>				plcVarsList;
	quint64							minSec;
	quint64							maxSec;
	quint64							lastEmittedMSec;
	QString							saveAbsDir;
	//
	QLabel							*startLabel;
	QLabel							*endLabel;
	QLabel							*currentTimeLabel;
	QScrollBar					*playScrollBar;
	QPushButton					*playPushButton;
	QPushButton					*pausePushButton;
	QLabel							*leftImageLable;
	QLabel							*rightImageLable;
Q_OBJECT
public:
	dataPlayerClass(QString dbVarsTable_,QString dbValsTable_,QString saveAbsDir_,QWidget *p);
	~dataPlayerClass();
	void loadFromDatabase(QString fileName);
	void emitDataOff(quint64 s);
	void emitNextActiveTimeData();
	void emitPreviousActiveTimeData();
signals:
	void newDataSignal(QString ip,QString table,int index,QVariant value);
public slots:
	void onMainTimerSlot();
	void playPushButtonSlot();
	void pausePushButtonSlot();
	void sliderValueChangedSlot(int);
};
////////////////////////////////dataPlayerClass////////////////////////////////////
////////////////////////////////dataLogingClassClass////////////////////////////////////
class dataLogingClass:public QDialog
{
	tuxipServerClass									*tuxipServer;
	QSqlDatabase											logDb;
	QString														dbVarsTable;
	QString														dbValsTable;
	QString														saveAbsDir;

	QString														plcActivateSaveIp;
	QString														plcActivateSaveTableName;
	int																plcActivateSaveIndex;
	int																plcActivateSaveBit;

	quint64														oldestValue;
	quint64														userTimeSpan;
	quint64														currentTimeSpan;
	QList<dataLogingValsClass*>				varsList;
	bool															activateSaveBitValue;
	//
	QListWidget												*availableLogsListWidget;
	QLabel														*currentDataLabel;
	dataPlayerClass										*dataPlayer;
	bool															isSaving;
Q_OBJECT
public:
	dataLogingClass(tuxipServerClass *tuxipServer_,quint64 userTimeSpan_,QString plcActivateSaveIp_,QString plcActivateSaveTableName_,int plcActivateSaveIndex_,int plcActivateSaveBit_,QString saveAbsDir_,QWidget *p);
	~dataLogingClass();
	void updateAvailableLogsListWidget();
	void addTable(QString plcIp,QString plcTableName,int minPlcIndex,int maxPlcIndex);
	void addVariable(QString plcIp,QString plcTableName,int plcIndex);
	void saveToDatabase(QString suffix);
	dataPlayerClass *getDataPlayer();
	int exec();
	QString getAbsDir();
signals:
	void takeSnapShotSignal(QString fName);
public slots:								
	void onMainTimerSlot();						
	void savePushBtnClickedSlot();
	void availableLogsListWidgetCurrentItemChangedSlot(QListWidgetItem *current,QListWidgetItem *previous);
 	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
};
////////////////////////////////dataLogingClassClass////////////////////////////////////
#endif