#ifndef SLCGUI_H
#define SLCGUI_H

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTimer>
#include <QLabel>
#include <QSplitter>
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include "utilities.h"
#include "slcVar.h"
class slcVarGuiClass : public QWidget
{
	QString							driverName;
	int									station;
	slcVarClass					*slcVar;
	QTableWidget				*tableWidget;
	//
	QTimer							*readFromPlcTimer;
	int									readInterval;
Q_OBJECT
public:
	slcVarGuiClass(QString ipName,QString plcTable,int plcFirstIndex,int dataType,int noOfVars,QString driverName_,int station_,int readInterval_,QWidget *p);
	slcVarGuiClass::~slcVarGuiClass();
public:
	void startUpdate();
	bool writeValue(QString ip,QString table,int index,QVariant val);
signals:
	void dataInPlcChangedSignal(QString ip,QString plcTable,int plcIndex,QVariant value);
public slots:
	void onReadFromPlcTimerSlot();
	void tableWidgetCellDoubleClickedSlot(int row,int col);
	void dataInPlcChangedSlot(QString ip,QString plcTable,int plcIndex,QVariant value);
};
#endif
