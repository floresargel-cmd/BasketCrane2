#ifndef TUXG_H
#define TUXG_H
#include <QtGui>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#define timeForLog QDateTime::currentDateTime().toString("yyyy/MM/dd")+QTime::currentTime().toString(" hh:mm:ss")
//
#define USE_OLDVALUES
#define REFRESH_DELAY_AFTER_WHRITE_TO_PLC 100
#define tuxLogFileName QString("txuipLog.txt")
#define timeForFile QDateTime::currentDateTime().toString("yyyy.MM.dd")+QTime::currentTime().toString(".hh.mm.ss")
#define timeForLog QDateTime::currentDateTime().toString("yyyy/MM/dd")+QTime::currentTime().toString(" hh:mm:ss")
#define timeVerbose QDateTime::currentDateTime().toString("yyyy/MMMM/dd")+QTime::currentTime().toString(" hh:mm:ss")
//
//#define isDebugLogActive
#undef isDebugLogActive
#ifdef isDebugLogActive
static void writeDebugLog(QString mess)
{
	QFile file("debugLog.txt");
	if (!file.open(QIODevice::Append|QIODevice::Text))
		return;
	QTextStream out(&file);
	out<<QDateTime::currentDateTime().toString("yyyy//MM//dd")+QTime::currentTime().toString(" hh:mm:ss\t")<<"-\t -"+mess+"\n";
	file.close();
}
#define debugLog(m) writeDebugLog(m);
#else
#define writeDebugLog(m) ;
#define debugLog(m) ;
#endif
static void tuxLog(QString title,QString mess)
{
	QFile file(tuxLogFileName);
	if (!file.open(QIODevice::Append|QIODevice::Text))
		return;
	QTextStream out(&file);
	out<<timeForLog<<"-"+title+"-\t -"+mess+"\n";
	file.close();
}
static void tuxExit(QString title,QString mess)
{
	tuxLog(title,mess);
	QDesktopServices::openUrl(tuxLogFileName);
	exit(0); 
}
#define TUX_EXIT(title,mess)       (tuxExit(title,mess))


static signed int BITS[32]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,INT_MIN};
#define ADDBIT(a,b) (a|b)
#define REMOVEBIT(a,b) (a^(a&b))
#define HAVECOMMONBIT(a,b) (bool)(a&b)
#define getValueOfBit(integer,bit)  HAVECOMMONBIT(integer,BITS[bit])
static QVector<bool> deprecatedCalculateBits(signed int n)
{
	QVector<bool> ans(32);
	signed int mask=1;
	for (int i=0;i<32;i++)
	{
		ans[i]=static_cast<bool>(n&mask);
		mask<<=1;
	}
	return ans;
}
static int deprecatedCalculateInt(QVector<bool> bits)
{
	int ans=0;
	unsigned int bitValue=0;
	bitValue=(int)bits[0];
	ans|=bitValue;
	for(int i=1;i<bits.count();i++)
	{
		bitValue=(int)bits[i];
		bitValue<<=i;
		ans|=bitValue;
	}
	return ans;
}
#endif
