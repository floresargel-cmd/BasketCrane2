#ifndef USAGEDLG_H
#define USAGEDLG_H

#include <QtGui>
#include <QMainWindow>
#include <QSplitter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>

#include "Gb2.h"
#include "plot.h"
class missionClass
{
public:
	int					fromPos;
	int					toPos;
	int					start;
	int					end;
	missionClass()
	{
		fromPos=toPos=start=end=0;
	}
	missionClass(int fromPos_,int toPos_,int start_,int end_)
	{
		fromPos=fromPos_;toPos=toPos_;start=start_;end=end_;
		if ((end-start)>200)//mission overtime
			end=start+200;
	}
	int getUsage(int f,int t)
	{
		if ((f>end)&&(t<start))
			return end-start;
		else if ((f<end)&&(t>start))
			return f-t;
		else if ((f>start)&&(f<end))
			return f-start;
		else if ((t<end)&&(t>start))
			return end-t;
		else 
			return 0;
	}
};
class usageDialogClass : public QDialog		
{
	QCustomPlot								*plot;
	QSpinBox									*calculationIntervalSpinBox;
	qint64										maxT;
	qint64										minT;
	//
	QCPGraph									*totalUsage;
	QCPGraph									*pickingExportsUsage;
	QCPGraph									*pickingImportsUsage;
	QCPGraph									*gate1Usage;
	QTableWidget							*importsExportsTable;
Q_OBJECT								
public:		
	usageDialogClass(QWidget *p);
	~usageDialogClass();
	void update();
	void fillGraph(QCPGraph *g,QVector<QVector<QVariant>> dataVV);
public slots:								
	void calculationIntervalSpinBoxValueChangedSlot(int);
};     

#endif 