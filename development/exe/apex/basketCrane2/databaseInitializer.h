#pragma once
#include "Gb2.h"
#include "exportQueues.h"
#include <utility>
namespace basket {
class DatabaseInitializer final {
public:
    explicit DatabaseInitializer(QString connection) : connection_(std::move(connection)) {}
    void Initialize(const BasketCraneConfig& config)
    {
	//c2logGui
	CreateTable("c2logGui",QStringList()<<"tId int primary key identity"<<QString("mess char(%1)").arg(maxMessageLength)<<QString("details char(%1)").arg(maxDescriptionLength)<<"messenger char(10)"<<"type char(10)"<<"time char(50)",connection_);
	basketExecQuery(QString("delete from c2logGui where tId not in(select top %1 tId from c2logGui order by tId desc)").arg(config.number("Logging/RetainedRows")),connection_);
	//"c2logV"
	CreateTable("c2logV",QStringList()<<"tId int primary key identity"<<QString("type char(%1)").arg(maxMessageLength)<<QString("description char(%1)").arg(maxDescriptionLength)<<"time char(50)",connection_);
	basketExecQuery(QString("delete from c2logV where tId not in(select top %1 tId from c2logV order by tId desc)").arg(config.number("Logging/RetainedRows")),connection_);
	//"c2log"
	CreateTable("c2log",QStringList()<<"tId int primary key identity"<<QString("type char(%1)").arg(maxMessageLength)<<QString("description char(%1)").arg(maxDescriptionLength)<<"time char(20)",connection_);
	basketExecQuery(QString("delete from c2log where tId not in(select top %1 tId from c2log order by tId desc)").arg(config.number("Logging/RetainedRows")),connection_);
	//"c2logPos"
	CreateTable("c2logPos",QStringList()<<"tId int primary key identity"<<"pos char(10)"<<"basket int"<<"info char(50)"<<"time char(20)",connection_);
	basketExecQuery(QString("delete from c2logPos where tId not in(select top %1 tId from c2logPos order by tId desc)").arg(config.number("Logging/RetainedRows")),connection_);
	//"plcMessages"
	CreateTable("c2plcMessages",QStringList()<<"tId int primary key identity"<<"plcAddress char(15)"<<"plcIndex int"<<"plcBit int"<<"bitStatus int"<<"type char(100)"<<"description char(300)"<<"time char(50)",connection_);
	basketExecQuery(QString("delete from c2plcMessages where tId not in(select top %1 tId from c2plcMessages order by tId desc)").arg(config.number("Logging/RetainedRows")),connection_);
	//c2missions
	CreateTable("c2missions",QStringList()<<"tId int primary key identity"<<"fromPosNumber int"<<"fromPosIndex int"<<"toPosNumber int"<<"toPosIndex int"<<"status char(30)",connection_);
	if (getFirst(QString("select count(tId) from c2missions"),connection_).toInt()==0)
	{
		basketExecQuery(QString("insert into c2missions (status,fromPosNumber,fromPosIndex,toPosNumber,toPosIndex) values('%1',0,0,0,0)").arg(nextStr),connection_);
		basketExecQuery(QString("insert into c2missions (status,fromPosNumber,fromPosIndex,toPosNumber,toPosIndex) values('%1',0,0,0,0)").arg(activeStr),connection_);
	}
    foreach (const BasketExportQueue& queue, basketExportQueues())
        CreateTable(queue.table,QStringList()<<"tId int primary key identity"<<"basket int unique"<<"priority int unique",connection_);

    }
private:
void CreateTable(const QString& tableName, const QStringList& colNamesTypes, const QString& connectionName)
{
	QString stmnt=QString("if not exists (select [name] from sys.tables where [name] ='%1') create table %1 (").arg(tableName);
	for (int i=0;i<colNamesTypes.length();i++)
	{
		stmnt+=colNamesTypes[i];
		if (i!=(colNamesTypes.length()-1))
			stmnt+=",";
	}
	stmnt+=")";
	basketExecQuery(stmnt,connectionName);
};
    QString connection_;
};
} // namespace basket
