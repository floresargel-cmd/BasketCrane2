#ifndef GU_H
#define GU_H
#include <QtGui>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QInputDialog>
#include <QSplashScreen>
//#define buildData QString("build: %1 %2, c++:%3").arg(__DATE__).arg(__TIME__).arg(__cplusplus)
#define timeForFile QDateTime::currentDateTime().toString("yyyy.MM.dd")+QTime::currentTime().toString(".hh.mm.ss")
#define timeForLog QDateTime::currentDateTime().toString("yyyy/MM/dd")+QTime::currentTime().toString(" hh:mm:ss")
#define timeVerbose QDateTime::currentDateTime().toString("yyyy/MMMM/dd")+QTime::currentTime().toString(" hh:mm:ss")
#define dateForLog QDateTime::currentDateTime().toString("yyyy/MM/dd")
#define dateVerbose QString("yyyy/MMMM/dd")
#define sqlDateTime QString("YYYY-MM-DD HH:MI:SS")
#define qdateFormat QString("yyyy.MM.dd")
#define infoStr QString("info")
#define errorStr QString("error")

static void uWarning(QString title,QString mess)
{
	QString logFileName=QString("warning.txt");
	QFile file(logFileName);
	if (!file.open(QIODevice::Append|QIODevice::Text))
		exit(0);
	QTextStream out(&file);
	out<<timeForLog<<"-"+title+"-\t -"+mess+"\n";
	file.close();
}
static void uExit(QString title,QString mess)
{
	QString logFileName=QString("error.%1.txt").arg(timeForFile);
	QFile file(logFileName);
	if (!file.open(QIODevice::Append|QIODevice::Text))
		exit(0);
	QTextStream out(&file);
	out<<timeForLog<<"-"+title+"-\t -"+mess+"\n";
	file.close();
	QDesktopServices::openUrl(logFileName);
	exit(0);
}
static bool uCheckPassword(QString pass)
{
	if (pass.length()==3)
	{
		if ((pass[0]==110)&&(pass[1]==109)&&(pass[2]==115))
			return true;
		if ((pass[0]==78)&&(pass[1]==77)&&(pass[2]==83))
			return true;
	}		
	return false;
}
#define uIntToBool(v)       (v==1?true:false)
#define uPi 3.1415926535897932384626433
#define uMax(a,b) (a>b?a:b)
#define uMin(a,b) (a<b?a:b)
#define uCompilerData QString("msc version:%1 msc build:%2, %3 %4").arg(_MSC_FULL_VER).arg(_MSC_BUILD).arg(__DATE__).arg(__TIME__)
static bool ufEqual(float a,float b,float e=FLT_EPSILON)
{
	return (fabs(a-b)<e);
}
static bool ufNEqual(float a,float b,float e=FLT_EPSILON)
{
	return (fabs(a-b)>e);
}
static bool ufGraterThen(float a,float b,float e=FLT_EPSILON)
{
	return a>(b+e);
}
static bool ufLessThen(float a,float b,float e=FLT_EPSILON)
{
	return a<(b-e);
}
static float uFPointDist(QPointF a,QPointF b)
{
	return (float)sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()));
}
#endif