#include "Gs.h"
#include "basketCrane2/environmentIo.h"
QString crane2Ip;
QString oldOvenIp;
QString newOvenIp;
QString sbWarehouseIp=QString("20.20.20.80");
QString sbDbIp="192.168.105.129";

int getPosIdOfCassetteGlobal(int cassette,QString db)
{
	return getFirst(QString("select posId from pos where cassette=%1").arg(cassette),db).toInt();
}
int getPosIdOfSbGlobal(int sb,QString db)
{
	return getFirst(QString("select posId from pos where sbNumber=%1").arg(sb),db).toInt();
}
int getPosIdOfTicketGlobal(int ticket,QString db)
{
	return getFirst(QString("select posId from pos where ticket=%1").arg(ticket),db).toInt();
}
QString getDescriptionOfPosIdGlobal(int posId,QString db)
{
	return getFirst(QString("select description from pos where posId=%1").arg(posId),db).toString().trimmed();
}
void setCassetteSbTicketInPosG(int globalPosId,int cassette,int sbNumber,int ticket,QString info,bool isPlcChange,bool isMove,bool isSbPos,bool isUserChange,QString mDb)
{
	int cassetteInPos=getFirst(QString("select cassette from pos where posId=%1").arg(globalPosId),mDb).toInt();
	int sbNumberInPos=getFirst(QString("select sbNumber from pos where posId=%1").arg(globalPosId),mDb).toBool();
	int ticketInPos=getFirst(QString("select ticket from pos where posId=%1").arg(globalPosId),mDb).toInt();
	if ((!isPlcChange)&&(!isUserChange))
	{
		if ( ((cassetteInPos!=0)&&(cassette!=0))&&(cassetteInPos!=cassette) )
			uExit("error",QString("Trying to write cassette %1 in position %2. Position %2 has cassette %3").arg(cassette).arg(globalPosId).arg(cassetteInPos));
		if ( ((ticketInPos!=0)&&(ticket!=0))&&(ticketInPos!=ticket) )
			uExit("error",QString("Trying to write ticket %1 in position %2. Position %2 has ticket %3").arg(ticket).arg(globalPosId).arg(ticketInPos));
	}
	if ( (ticket!=0)&&(sbNumber==0))
		uExit("error",QString("Trying to write ticket %1 in position %2. Position %2 has no sb").arg(ticket).arg(globalPosId));
	if (!isSbPos)
	{
		if ( (sbNumber!=0)&&(cassette==0))
			uExit("error",QString("Trying to write SB in position %1. Position %1 has no cassette").arg(globalPosId));
	}
	if ((!isPlcChange)&&(!isMove))
	{
		int otherPosOfCassette=getFirst(QString("select posId from pos where cassette=%1 and cassette<>0 and posId<>%2").arg(cassette).arg(globalPosId),mDb).toInt();
		if (otherPosOfCassette!=0)
			uExit("error",QString("Trying to write cassette %1 in position %2. Cassette %1 is in position %3").arg(cassette).arg(globalPosId).arg(otherPosOfCassette));

		int otherPosOfTicket=getFirst(QString("select posId from pos where ticket=%1 and ticket<>0 and posId<>%2").arg(ticket).arg(globalPosId),mDb).toInt();
		if (otherPosOfTicket!=0)
			uExit("error",QString("Trying to write ticket %1 in position %2. Ticket %1 is in position %3").arg(ticket).arg(globalPosId).arg(otherPosOfTicket));
	}
	if ((cassetteInPos!=cassette)||(sbNumber!=sbNumberInPos)||(ticketInPos!=ticket))
	{
		basketExecQuery(QString("insert into positionsLog (posId,cassette,sbNumber,ticket,info,time) values(%1,%2,%3,%4,'%5','%6')").arg(globalPosId).arg(cassette).arg(sbNumber).arg(ticket).arg("user:"+info).arg(timeForLog),mDb);
		basketExecQuery(QString("update pos set cassette=%1,sbNumber=%2,ticket=%3 where posId=%4").arg(cassette).arg(sbNumber).arg(ticket).arg(globalPosId),mDb);
	}
}
int movePosDataGlobal(int from,int to,QString mDb)
{
	if ((from==0)||(to==0))
	{
		log(QString("moveCassette from==%1 to==%2").arg(from).arg(to),errorStr,mDb);
		uExit(errorStr,QString("moveCassette from==%1 to==%2").arg(from).arg(to));
	}
	int fromCassette=getOne(QString("select cassette from pos where posId=%1").arg(from),mDb).toInt();
	int toCassette=getOne(QString("select cassette from pos where posId=%1").arg(to),mDb).toInt();
	int fromSbNumber=getOne(QString("select sbNumber from pos where posId=%1").arg(from),mDb).toInt();
	int toSbNumber=getOne(QString("select sbNumber from pos where posId=%1").arg(to),mDb).toInt();
	int fromTicket=getOne(QString("select ticket from pos where posId=%1").arg(from),mDb).toInt();
	int toTicket=getOne(QString("select ticket from pos where posId=%1").arg(to),mDb).toInt();
	if ( 
		( (toCassette!=0)||(toSbNumber!=0)||(toTicket!=0) )
		||
		( (fromCassette==0)||  ((fromSbNumber==0)&&(fromTicket!=0)) )
		)
	{
		log(QString("move data from==%1 to==%2, fromCassette:%3 toCassette:%4, fromSb:%5 toSb:%6, fromTicket:%7 toTicket:%8").arg(from).arg(to).arg(fromCassette).arg(toCassette).arg(fromSbNumber).arg(toSbNumber).arg(fromTicket).arg(toTicket),errorStr,mDb);
		uExit(errorStr,QString("moveCassette from==%1 to==%2, fromCassette:%3 toCassette:%4").arg(from).arg(to).arg(fromCassette).arg(toCassette));
	}
	logV(QString("moveData (p:%1 c:%2 s:%3 t:%4)->(p:%5 c:%6 s:%7 t:%8)").arg(from).arg(fromCassette).arg(fromSbNumber).arg(fromTicket).arg(to).arg(toCassette).arg(toSbNumber).arg(toTicket),infoStr,mDb);
	setCassetteSbTicketInPosG(to,fromCassette,fromSbNumber,fromTicket,"move",false,true,false,false,mDb);
	setCassetteSbTicketInPosG(from,0,0,0,"move",false,false,false,false,mDb);
	return fromCassette;
}


#define doVerboseLog
void logV(QString description,QString type,QString mDb)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
#ifdef doVerboseLog
	basketExecQuery(QString("insert into logV (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),mDb);
#endif
}
void log(QString description,QString type,QString mDb)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	basketExecQuery(QString("insert into log (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),mDb);
	logV(description,type,mDb);
}
void logImportExportGlobal(int ticket,int sbNumber,QString importLocation,QString exportLocation,int triggerStatus,QString mDb)
{
	basketExecQuery(QString("insert into importExportLog (ticket,sbNumber,dateAndTime,importLocation,exportLocation,triggerStatus) values(%1,%2,now(),'%3','%4',%5)").arg(ticket).arg(sbNumber).arg(importLocation).arg(exportLocation).arg(triggerStatus),mDb);
}