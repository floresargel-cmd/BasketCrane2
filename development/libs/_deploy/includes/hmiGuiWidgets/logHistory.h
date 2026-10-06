#ifndef LOGHISTORY_H
#define LOGHISTORY_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QComboBox>
#include <QHeaderView>
#include "Ghmi.h"
//////////////////////////////////////////////////////////////logHisoryClass////////////////////////////////////////////////////////////
class bitLogHisoryClass:public QDialog		
{
	QString													dbConnectionName;
	QString													databaseType;
	QString													logTableName;
	QComboBox												*databaseTypesComboBox;
	QComboBox												*trueFalseComboBox;
	QTableWidget										*logTableWidget;
	QSpinBox												*plcIndexSpinBox;
	QSpinBox												*plcBitSpinBox;
	QSpinBox												*topNSpinBox;
	//
	QString													clipboardString;
	int															databaseEnum;
public:										
enum{//databaseEnum
	typeNone=0,
	typeMsSql=1,
	typeMySql=2,
	typeSqlLite=3,
	};
Q_OBJECT								
public:										
  bitLogHisoryClass(QString dbConnectionName_,QString databaseType_,QString logTableName_,int plcIndexValue=-1,int databaseEnum_=typeNone);
public slots:
	void copyDataSlot();
	void updateFromDbSlot();
};
#endif
