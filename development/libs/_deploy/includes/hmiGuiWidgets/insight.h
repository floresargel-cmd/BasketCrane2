#ifndef INSIGHT_H
#define INSIGHT_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QRadioButton>
#include <QSplitter>
#include "bitLog.h"
#include "Ghmi.h"
///////////////////////////////////////////////////plcInsightDialogClass/////////////////////////
class plcInsightClass:public QWidget
{
	QString														plcAdress;
	QString														onStyle;
	QString														offStyle;
	QSplitter													*listsSplitter;
	QVector<bitLogVectorClass*>				bitLogVectors;
public:
Q_OBJECT								
public:
	plcInsightClass(QString bitsDescriptionfileName,QString plcAdress_,QString onStyle_,QString offStyle_,QWidget* p);
	~plcInsightClass();
	void readDescriptionFile(QString bitsDescriptionfileName);
	void plcIntMemoryChanged(QString plcAdress_,int index,int value);
public slots:
	void showAllListsSlot(bool checked);
	void showActiveOnlyListsSlot(bool checked);
	void showAllBitsSlot(bool checked);
	void showActiveOnlyBitsSlot(bool checked);

};
///////////////////////////////////////////////////plcInsightPushButtonClass/////////////////////////
class plcInsightPushButtonClass:public QPushButton
{
	QDialog										*dlg;
	plcInsightClass						*plcInsight;
public:
Q_OBJECT								
public:
	plcInsightPushButtonClass(QString iconFile,QString bitsDescriptionfileName,QString plcAdress,QString onStyle,QString offStyle,QWidget* p);
	~plcInsightPushButtonClass();
	void	mouseReleaseEvent(QMouseEvent *e);
	void plcIntMemoryChanged(QString plcAdress,int index,int value);
};
	#endif
