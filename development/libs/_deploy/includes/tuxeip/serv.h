#ifndef SERV_H
#define SERV_H

#include <QtGui>
#include <QTabWidget>
#include "tuxG.h"
#include "tuxClass.h"
#include "ping.h"

class tuxipServerClass : public QWidget				
{
//	QWidget															*parrent;
	//QTimer															mainTimer;
	QVector<tuxipClass*>								tuxipClassVector;
	QHBoxLayout													*mainDlgLayout;
	QTabWidget													*tabWidget;
	QDialog															*serverDialog;
	Q_OBJECT								
public:										
	EXPORT tuxipServerClass(QWidget *p=NULL);							
	~tuxipServerClass();							
	
	EXPORT tuxipClass* addConnection(QString ip,QString comment,bool noPlc=false,bool useSimplePing=false);
	void addConnection(tuxipClass *tClass);
	EXPORT void addTable(QString tableName,int type,int minIndex,int maxIndex,int updateInterval,QString ip);
	void setVarComment(QString ip,QString table,int index,QString comment);
	void setBitComment(QString ip,QString table,int index,int bit,QString comment);
	void setUpdatePlcReadIntervalForAllTables(int ms);
	tuxipClass* getTuxipClassWithIp(QString ip=QString());
	tuxipClass* getTuxipClassWithIndex(int index);
	void setIsAutoUpdatingFromPlc(bool u);
	void createDialog();
	EXPORT void showDialog();
	void closeDialog();
	//bool getIsOnEmulationMode();
	bool getHasError();
	void writeBit(QString table,int tableIndex,int bitIndex,bool value,QString ip=QString());
	void writeInteger(QString table,int index,int value,QString ip=QString());
	void writeIntegers(QString table,int index,QVector<int> values,QString ip=QString());
	int readIntegerValue(QString table,int index,QString ip=QString());
	QVector<int> readIntegerValues(QString table,int index,int numberOfValues,QString ip=QString());
	//int getOldIntegerValue(QString table,int index,QString ip=QString());
	//QVector<int> getOldIntegerValues(QString table,int index,int numberOfValues,QString ip=QString());

	void writeFloat(QString table,int index,float value,QString ip=QString());
	void writeFloats(QString table,int index,QVector<float> values,QString ip=QString());
	double readFloatValue(QString table,int index,QString ip=QString());
	QVector<float> readFloatValues(QString table,int index,int numberOfValues,QString ip=QString());
	//double getOldFloatValue(QString table,int index,QString ip=QString());
	//QVector<float> getOldFloatValues(QString table,int index,int numberOfValues,QString ip=QString());
private:
signals:
	void dataInPlcChangedSignal(QString ip,QString table,int index,QVariant value);
public slots:
	void dataInPlcChangedSlot(QString ip,QString table,int index,QVariant value);
};											
#endif
