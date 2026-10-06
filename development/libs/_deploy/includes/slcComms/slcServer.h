#ifndef SLCSERVER_H
#define SLCSERVER_H

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
#include "slcGui.h"

class slcServerDlgClass : public QDialog
{
	QHBoxLayout											*mainLayout;
	QVector<slcVarGuiClass*>				vars;
Q_OBJECT
public:
	slcServerDlgClass(int varsToInitialize,QWidget *p);
	slcServerDlgClass::~slcServerDlgClass();
	void showDialog();
	void startUpdate();
	void addVar(QString ipName,QString plcTable,int plcFirstIndex,int dataType,int noOfVars,QString driverName,int station,int readInterval);
public:
	void writeValue(QString ip,QString table,int index,QVariant val);
signals:
	void dataInPlcChangedSignal(QString ip,QString plcTable,int plcIndex,QVariant value);
public slots:
	void dataInPlcChangedSlot(QString ip,QString plcTable,int plcIndex,QVariant value);
};
#endif
