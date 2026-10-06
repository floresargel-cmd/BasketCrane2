#ifndef PROGB_H
#define PROGB_H
#include <QtGui>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include "Ghmi.h"
#include "plot.h"
class progressClass:public QWidget		
{
protected:
	tuxipClass											*tuxip;
	QString													ip;
	QString													plcAdress;
	int															plcIndex;
	QString													title;
	QString													unit;
	int															width;
	int															height;
	int															noOfDecimals;
	QColor													color1;
	QColor													color2;
	QString													dbConnectionName;
	QString													databaseType;
	QString													logTableName;
	int															type;
	float														minValue;
	float														maxValue;
	float														stepValue;

	int															firstUpdate;
	float														value;
	//arc
	float														xCenter;
	float														yCenter;
	float														smallRadius;
	float														bigRadius;
	float														startAngle;
	float														totalAngle;
	QPainterPath										outlinePath;
	QPainterPath										marksPath;
	plotClass												*plot;
public:										
enum{//type
	TYPE_HORIZONTAL=       0x0001,
	TYPE_VERTICAL=         0x0002,
	TYPE_ARC=              0x0004,
	TYPE_INT=              0x0008,
	TYPE_FLOAT=            0x0020,
	TYPE_USER_INPUT=       0x0040,
};
	//
Q_OBJECT								
public:										
  progressClass(tuxipClass *tuxip_,QString ip_,QString plcAdress_,int plcIndex_,QString title_,QString unit_,float minValue_,float maxValue_,float stepValue_,int width_,int height_,QColor color1_,QColor color2_,int type_,QString dbConnectionName_,QString databaseType_,QString logTableName_,QString databaseDescription_,int displayAngle,int noOfDecimals_,QWidget *p);
	~progressClass();
	void logToDatabase(QString connectionName,QString table,QString type,QString plcAddress,int plcIndex,float value,bool status,QString description);
	void updateValue(QString ip_,QString plcAdress_,int plcIndex_,float v);
	virtual void	mouseReleaseEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *e);
	QPainterPath createPie(float startA,float totalA);
	void setPlotClass(plotClass *plot_);
};

#endif