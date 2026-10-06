#ifndef GHMI_H
#define GHMI_H
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "serv.h"
#include "logHistory.h"
#include <windows.h>
//#if !defined(MS_SQL) && !defined(SQL_LITE)
//#error hmiGuiWidgetsLib no database type defined
//#endif
#define LOG_TABLE_NAME QString("log")
#define PI 3.1415926535897
#ifndef max
#define max(a,b) (a>b?a:b)
#endif
#ifndef min
#define min(a,b) (a<b?a:b)
#endif
static QVector<QVariant> hmiExecQuery(QString stmnt,QString connection)
{
	QSqlQuery query(QSqlDatabase::database(connection));
	if (!query.exec(stmnt))
	{
		QString logFileName=QString("error.%1.txt").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd")+QTime::currentTime().toString(".hh.mm.ss"));
		QFile file(logFileName);
		if (!file.open(QIODevice::Append|QIODevice::Text))
			exit(0);
		QTextStream out(&file);
		out<<QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss\n");
		out<<"\terror:"+query.lastError().text()+"\n";
		out<<"\tcannot execute query:"+stmnt+"\n";
		file.close();
		ShellExecute(0,"open",logFileName.toLatin1(), NULL, NULL, SW_SHOWNORMAL);
		exit(0);
	}
	//
	QVector<QVariant> ans;
	while (query.next()) 
	{
		ans.append(query.value(0));
	}
	return ans;
}
//static QVector<bool> hmiCalculateBits(signed int n)
//{
//	QVector<bool> ans(32);
//	signed int mask=1;
//	for (int i=0;i<32;i++)
//	{
//		ans[i]=static_cast<bool>(n&mask);
//		mask<<=1;
//	}
//	return ans;
//}
//static int hmiCalculateInt(QVector<bool> bits)
//{
//	int ans=0;
//	unsigned int bitValue=0;
//	bitValue=(int)bits[0];
//	ans|=bitValue;
//	for(int i=1;i<bits.count();i++)
//	{
//		bitValue=(int)bits[i];
//		bitValue<<=i;
//		ans|=bitValue;
//	}
//	return ans;
//}
#endif
