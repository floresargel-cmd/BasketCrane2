#ifndef SQL_H
#define SQL_H
#include "Gu.h"
//#define logQuerys
#ifdef logQuerys
static void logExeQuery(QString a)
{
	QString logFileName=QString("logExeQuery.txt");
	QFile file(logFileName);
	if (!file.open(QIODevice::Append|QIODevice::Text))
		exit(0);
	QTextStream out(&file);
	out<<timeForLog<<" "<<a+"\n";
	file.close();
}
#endif
static QVector<QVector<QVariant>> execTableQuery(QString stmnt,QString connection)
{
	QSqlQuery query(QSqlDatabase::database(connection));
	query.setForwardOnly(true);
	if (!query.exec(stmnt))
		uExit(errorStr,QString("Cannot execQuery:%1/nsql server:%2").arg(stmnt).arg(query.lastError().text()));
	QVector<QVector<QVariant>> ans;
	while (query.next())
	{
		QVector<QVariant> a;
		QSqlRecord r=query.record();
		for (int i=0;i<r.count();i++)
			a.append(r.value(i));
		ans<<a;
	}
	if (query.lastError().isValid())
		uExit(errorStr,QString("Cannot execQuery:%1/nsql server:%2").arg(stmnt).arg(query.lastError().text()));
#ifdef logQuerys
	logExeQuery(stmnt);
#endif
	return ans;
}
static QVector<QVariant> execQuery(QString stmnt,QString connection)
{
	QSqlQuery query(QSqlDatabase::database(connection));
	query.setForwardOnly(true);
	if (!query.exec(stmnt))
		uExit(errorStr,QString("Cannot execQuery:%1/nsql server:%2").arg(stmnt).arg(query.lastError().text()));
	QVector<QVariant> ans;
	while (query.next()) 
		ans.append(query.value(0));
	if (query.lastError().isValid())
		uExit(errorStr,QString("Cannot execQuery:%1/nsql server:%2").arg(stmnt).arg(query.lastError().text()));
#ifdef logQuerys
	logExeQuery(stmnt);
#endif
	return ans;
}
static QVariant getFirst(QString query,QString connectionName)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()>0)
		return resArray[0];
	else
		return 0.;
}
static QVariant getOneAtMost(QString query,QString connectionName)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()==0)
		return 0;
	if (resArray.count()==1)
		return resArray[0];
	uExit("database error",QString("query: '%1' returned %2 results").arg(query).arg(resArray.count()));
	return -1;
}
static QVariant getOne(QString query,QString connectionName)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()==0)
		uExit("database error",QString("query: '%1' returned no results").arg(query));
	if (resArray.count()==1)
		return resArray[0];
	uExit("database error",QString("query: '%1' returned %2 results").arg(query).arg(resArray.count()));
	return -1;
}
#endif