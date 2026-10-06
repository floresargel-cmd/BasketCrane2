#ifndef APEXG_H
#define APEXG_H
#include"..\G.h"
#define smallLength 6900.
#define smallLengthForEvenWoods 5330.
#define bigLength 7660.

#define BASKET_BASKET_CONNECTION_ID 1
#define BASKET_AGING_CONNECTION_ID 2
#define AGING_AGING_CONNECTION_ID 3
#define EXPORTS_CONV_CONNECTION_ID 4
//#define EXPORTS_HMI_PINCER_CONNECTION_ID 5
//#define EXPORTS_HMI_PACKING_CONNECTION_ID 6
#define STRAPING_HMI_FORK_CONNECTION_ID 7
#define WAREHOUSE_FORK_CONNECTION_ID 8
#define WAREHOUSE_CRANE_CONNECTION_ID 9

#define AGEING_OVEN_READ_PORT  1001
#define AGEING_OVEN_WRITE_PORT 1002
#define AGEING_OVEN_READ_PORT2  1003
#define AGEING_OVEN_WRITE_PORT2 1004
#define PINCER_READ_PORT  2001
#define PINCER_WRITE_PORT 2002
#define CRANE_READ_PORT  3001
#define CRANE_WRITE_PORT 3002
#define PACKING_READ_PORT  4001
#define PACKING_WRITE_PORT 4002
#define STRAPING_READ_PORT_FOR_STRAPING_PC  5001
#define STRAPING_WRITE_PORT_FOR_STRAPING_PC 5002
#define STRAPING_READ_PORT_FOR_WARE_HOUSE_PC  5003
#define STRAPING_WRITE_PORT_FOR_WARE_HOUSE_PC 5004
#define PACKET_CRANE_READ_PORT  6001
#define PACKET_CRANE_WRITE_PORT 6002
#define STACKER_READ_PORT  7001
#define STACKER_WRITE_PORT 7002
//type for baskets table
#define BASKET_NONE 0
#define BASKET_NORMAL 1
#define BASKET_SCRAP 2
//contentsType for baskets table
#define BASKET_CONTENS_NONE 0
#define BASKET_CONTENS_LESS_THAN_20 1								//6096mm
#define BASKET_CONTENS_LESS_THAN_24 2							//7315mm
#define BASKET_CONTENS_LESS_THAN_30 3							//9144mm
#define BASKET_CONTENS_MORE_THAN_30 4
#define BASKET_CONTENS_SCRAP 5

#define WHITE_TEXTURE_TRIA 1
#define WHITE_TEXTURE_QUAD 2
#define ORANGE_TEXTURE_TRIA 3
#define ORANGE_TEXTURE_QUAD 4

#define TEXTURE_FOR_24_TRIA 5
#define TEXTURE_FOR_24_QUAD 6
#define TEXTURE_FOR_30_TRIA 7
#define TEXTURE_FOR_30_QUAD 8

#define ARROW_TEXTURE 9
#define MAX_LAYERS_IN_A_BASKET 20
//
#define PACKET_WITHOUT_SB 1
#define PACKET_WITH_SB 2

#define PACKET_DESTINATION_OUT 1
#define PACKET_DESTINATION_WAREHOUSE 2
//storagingType :packets Table/storagingType
#define STORAGE_TYPE_NULL							0
#define STORAGE_TYPE_SHORT_TERM				1
#define STORAGE_TYPE_MID_TERM					2
#define STORAGE_TYPE_LONG_TERM				3
//
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <windows.h>

#define USE_LOCAL_DB_
#undef USE_LOCAL_DB_
#ifdef USE_LOCAL_DB_
static QString craneIp="192.168.0.111";
static QString forkIp="192.168.0.111";
static QString conveyorsIp="192.168.0.111";
#else
static QString basketCraneIp="20.20.20.10";
static QString conveyorsIp="20.20.20.40";
static QString forkIp="20.20.20.21";
//static QString forkIp="20.20.20.20";
static QString packetCraneIp="20.20.20.30";
static QString newOvenIp="192.168.105.135";
#endif
//
//#ifdef USE_LOCAL_DB
//#define CRANE_IP "192.168.0.111"
//#define CONVEYORS_IP "192.168.0.111"
//#else
//#define CRANE_IP "20 .20.20.10"
//#define CONVEYORS_IP "20 .20.20.40"
//#endif
#define uCompilerData QString("msc version:%1 msc build:%2, %3 %4").arg(_MSC_FULL_VER).arg(_MSC_BUILD).arg(__DATE__).arg(__TIME__)
extern QVector<QProcess*> externalProcessesVector; 
static void closeOpenDatabases()
{
	QStringList allConnections=QSqlDatabase::connectionNames();
	for (int i=0;i<allConnections.count();i++)
	{
		QSqlDatabase::database(allConnections[i],false).close();
	}
}
static void exit(QString title,QString mess)
{
	//QMessageBox *msgBox=new QMessageBox(QMessageBox::Critical,title,mess,QMessageBox::Ok,NULL);
	//msgBox->exec();
	QString logFileName=QString("error.%1.txt").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd")+QTime::currentTime().toString(".hh.mm.ss"));
	QFile file(logFileName);
	if (!file.open(QIODevice::Append|QIODevice::Text))
		exit(0);
	QTextStream out(&file);
	out<<QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss\t");
	out<<"-"+title+"-\t -"+mess+"\n";
	file.close();
	closeOpenDatabases();
	ShellExecute(0,"open",logFileName.toLatin1(), NULL, NULL, SW_SHOWNORMAL);
	exit(0);
}
#define EXIT(title,mess)       (exit(title,mess))

////
#define MAIN_DB_CONNECTION QString("mainConnection")
#define APEX_DB_CONNECTION QString("apexConnection")
#define productionPlanDbConnection QString("aproductionPlanConnection")
static QVector<QVariant> apexExecQuery(QString stmnt,QString connection=MAIN_DB_CONNECTION)
{
	return execQuery(stmnt,connection);
	//QSqlQuery query(QSqlDatabase::database(connection));
	//query.setForwardOnly(true);
	//if (!query.exec(stmnt))
	//	EXIT(("Error"),query.lastError().text()+"\nstmnt: "+stmnt);
	////
	//QVector<QVariant> ans;
	//while (query.next())
	//{
	//	ans.append(query.value(0));
	//}
	//return ans;
}
static QVariant GETFIRST(QString query,QString connectionName=MAIN_DB_CONNECTION)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()>0)
		return resArray[0];
	else
		return 0.;
}
static QVariant GETONEATMOST(QString query,QString connectionName=MAIN_DB_CONNECTION)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()==0)
		return 0;
	if (resArray.count()==1)
		return resArray[0];
	EXIT("database error",QString("query: '%1' returned %2 results").arg(query).arg(resArray.count()));
	return -1;
}
static QVariant GETONE(QString query,QString connectionName=MAIN_DB_CONNECTION)
{
	QVector<QVariant> resArray=execQuery(query,connectionName);
	if (resArray.count()==0)
		EXIT("database error",QString("query: '%1' returned no results").arg(query));
	if (resArray.count()==1)
		return resArray[0];
	EXIT("database error",QString("query: '%1' returned %2 results").arg(query).arg(resArray.count()));
	return -1;
}
static QVariant GETONE(QString colName,QString tableName,QString idName,QString idVal,QString orderString="",QString connectionName=MAIN_DB_CONNECTION)
{
	QVector<QVariant> resArray=execQuery(QString("select %1 from %2 where %3=%4 %5").arg(colName).arg(tableName).arg(idName).arg(idVal).arg(orderString),connectionName);
	if (resArray.count()==0)
		EXIT("database error",QString("Table %1 does not have a record with %2=%3").arg(tableName).arg(idName).arg(idVal));
	if (resArray.count()==1)
		return resArray[0];
	EXIT("database error",QString("Table %1 has more than one record with %2=%3").arg(tableName).arg(idName).arg(idVal));
}
static void PUTONE(QString colName,QString colVal,QString tableName,QString idName,QString idVal,QString connectionName=MAIN_DB_CONNECTION)
{
	execQuery(QString("update %1 set %2=%3 where %4=%5").arg(tableName).arg(colName).arg(colVal).arg(idName).arg(idVal),connectionName);
}
////
///////////////////////////////////////////////////////////////////////////
static QString getPicFileNameFromProfile(int p)	
{
	return QString("C:\\profiles\\%1\\%2.jpg").arg(int(floor(double(p)/100.)*100.)).arg(p);
}

//extern QVector<QColor> colorsForPicking;
//extern int lastColorForPicking;
//static QColor getColorForPicking()
//{
//	if (lastColorForPicking<colorsForPicking.count())
//	{
//		return colorsForPicking[lastColorForPicking++];
//	}
//	QMessageBox::critical(0,"error","colorForPicking>255x255x255");
//	return QColor(0,0,0);
//}
////////////////////////////////////
#define verboseLog
#ifdef verboseLog
static void vLog(QString type,QString title,QString description)
{
	apexExecQuery(QString("insert into vLog (type,title,description,date) values('%1','%2','%3','%4')").arg(type).arg(title).arg(description).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")));
}
#else
static void vLog(QString type,QString title,QString description)
{
}
#endif


//
#define STYLE_R					 QString("QLabel{font-size:14px;background-color:#ff2a2a;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define STYLE_R_BLINKING QString("QLabel{font-size:14px;background-color:#f0f0f0;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:#ff0000;}")
#define STYLE_O					 QString("QLabel{font-size:14px;background-color:#f27a40;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define STYLE_G					 QString("QLabel{font-size:14px;background-color:#09c500;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:blue;}")
#define STYLE_B					 QString("QLabel{font-size:14px;background-color:#99ccff;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define STYLE_GRAY			 QString("QLabel{font-size:14px;background-color:#999999;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:#000077;}")

#define STYLE_G_SMALL    QString("QLabel{font-size:10px;background-color:#09c500;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:blue;}")
#define STYLE_B_SMALL    QString("QLabel{font-size:10px;background-color:#99ccff;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:white;}")
#define STYLE_GRAY_SMALL QString("QLabel{font-size:10px;background-color:#999999;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:#000077;}")

#define BUTTON_STYLE QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #8888aa,stop:0.4#DDDDDD,stop:0.5#D8D8D8,stop:1.0#88aa88);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #aa8888,stop:0.4 #DDDDDD,stop:0.5 #D8D8D8,stop:1.0 #8888aa);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define BUTTON_STYLE_ACTIVE QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define BUTTON_STYLE_RED QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #aa5555,stop:0.4#ff5555,stop:0.5#ff2222,stop:1.0#aa5555);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #aa5555,stop:0.4#ff5555,stop:0.5#ff2222,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define BUTTON_STYLE_GREEN QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define BUTTON_STYLE_O QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #f2aa40,stop:0.4#f27740,stop:0.5#f27740,stop:1.0#f2aa40);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #f27740,stop:0.4#f2aa40,stop:0.5#f2aa40,stop:1.0#f27740);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")

#define groupBoxStyle1 "QGroupBox{border:2px solid #000099;border-radius:5px;margin-top:10px;font-size: 15px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#000099;}"
#define groupBoxStyle2 "QGroupBox{border:2px solid #007700;border-radius:5px;margin-top:10px;font-size: 12px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#007700;}"
#define groupBoxStyle3 "QGroupBox{border:1px solid #227722;border-radius:5px;margin-top:10px;font-size: 11px;font-weight:normal;}QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#227722;}"

#define groupBoxUpperStyle1 "QGroupBox{border:2px solid #000099;background-color:#b8c3e6;border-radius:9px;margin-top:15px;margin-left:20px;margin-right:20px;font-size: 15px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-30px;color:#000099;}"
#define groupBoxUpperStyle2 "QGroupBox{border:2px solid #f0f0f0;background-color:#dee4f8;border-radius:5px;margin-top:10px;margin-left: 5px;margin-right: 5px;font-size: 12px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-22px;color:#000077;}"
#define groupBoxUpperStyle3 "QGroupBox{border:1px solid #227722;background-color:#dee4f8;border-radius:5px;margin-top:10px;margin-left: 5px;margin-right: 5px;font-size: 11px;font-weight:normal;}QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#227722;}"
#define groupBoxLowerStyle1 "QGroupBox{border:2px solid #009900;background-color:#9bc59e;border-radius:9px;margin-top:15px;margin-left:20px;margin-right:20px;font-size: 15px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-30px;color:#009900;}"
#define groupBoxLowerStyle2 "QGroupBox{border:2px solid #f0f0f0;background-color:#bfe5c3;border-radius:5px;margin-top:10px;margin-left: 5px;margin-right: 5px;font-size: 12px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-22px;color:#007700;}"
#define groupBoxLowerStyle3 "QGroupBox{border:1px solid #227722;background-color:#bfe5c3;border-radius:5px;margin-top:10px;margin-left: 5px;margin-right: 5px;font-size: 11px;font-weight:normal;}QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#227722;}"


#define labelStyleNormalBlack	QString("QLabel{font-size:15px;padding:2px;color:#000000;}")
#define labelStyleBigBlack	QString("QLabel{font-size:20px;padding:1px;color:#000000;}")
#define labelStyleBigRed		QString("QLabel{font-size:20px;padding:1px;color:#aa0000;}")

#define spinStyleBigBlack	QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#000000;}")
#define spinStyleBigRed		QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#aa0000;}")
#define spinStyleBigGreen	QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#00aa00;}")
#define doubleSpinStyleBigBlack	QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#000000;}")
#define doubleSpinStyleBigRed		QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#aa0000;}")
#define doubleSpinStyleBigGreen	QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#00aa00;}")

#define splitterHorizontalStyle	QString("QSplitter::handle:horizontal{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:horizontal:hover{height:3px;background-color:#339966;}")
#define splitterVerticalStyle	QString("QSplitter::handle:vertical{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:vertical:hover{height:3px;background-color:#339966;}")
#endif
