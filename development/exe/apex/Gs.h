#ifndef Gs_H
#define Gs_H
#include "utilities.h"

extern QString crane2Ip;			 //plc
extern QString sbWarehouseIp;	 //plc
extern QString oldOvenIp;			 //plc
extern QString newOvenIp;			 //plc

extern QString sbDbIp;			 //db
#define bDb QString("bDb")//basket crane database
#define aDb QString("aDb")//apex database
#define aDb2 QString("aDb2")//apex database2


constexpr auto activeStr="active";
constexpr auto nextStr="next";

constexpr auto exportsFrontTable="exportsFront";
constexpr auto exportsTriple1Table="exportsTriple1";
constexpr auto exportsTriple2Table="exportsTriple2";
constexpr auto exportsDoubleTable="exportsDouble";


//crane
constexpr int craneId=10000;											//warehouse crane

constexpr int conveyor__1_id=10000+1;								//warehouse exit destucker conveyor
constexpr int conveyor__2_id=10000+2;								//warehouse exit destucker conveyor
constexpr int conveyor__4_id=10000+4;								//warehouse exit destucker conveyor
constexpr int conveyor__5_id=10000+5;								//warehouse exit destucker conveyor
constexpr int conveyor__6_id=10000+6;								//warehouse exit destucker conveyor

constexpr int cassetteTripleExit__8_id=10000+8;			//warehouse triple exit	conveyor
constexpr int cassetteTripleExit__9_id=10000+9;			//warehouse triple exit	conveyor
constexpr int cassetteTripleExit_10_id=10000+10;		//warehouse triple exit	conveyor
constexpr int cassetteTripleExit_11_id=10000+11;		//warehouse triple exit	conveyor
constexpr int cassetteTripleExit_12_id=10000+12;		//warehouse triple exit	conveyor
constexpr int cassetteTripleExit_13_id=10000+13;		//warehouse triple exit	conveyor
																																						
constexpr int cassetteDoubleExit_15_id=10000+15;		//warehouse double exit	conveyor
constexpr int cassetteDoubleExit_16_id=10000+16;		//warehouse double exit	conveyor
constexpr int cassetteDoubleExit_17_id=10000+17;		//warehouse double exit	conveyor
constexpr int cassetteDoubleExit_18_id=10000+18;		//warehouse double exit	conveyor
//pincer 1
constexpr int pincer1Id=10000+40;								//pincer 1 (old)

constexpr int pincer1_41_id=10000+41;							//pincer 1 sb position buffer
constexpr int pincer1_42_id=10000+42;							//pincer 1 sb position buffer
constexpr int pincer1_43_id=10000+43;							//pincer 1 sb position buffer
constexpr int pincer1_44_id=10000+44;							//pincer 1 sb position buffer

constexpr int pincer1_45_id=10000+45;							//pincer 1 sb position buffer
constexpr int pincer1_46_id=10000+46;							//pincer 1 sb position buffer
constexpr int pincer1_47_id=10000+47;							//pincer 1 sb position buffer
constexpr int pincer1_48_id=10000+48;							//pincer 1 sb position buffer
																										
constexpr int pincer1_49_id=10000+49;								//pincer 1 sb conveyor position destacker buffer
constexpr int pincer1_50_id=10000+50;								//pincer 1 sb conveyor position destacker working
constexpr int pincer1_51_id=10000+51;								//pincer 1 sb conveyor position scrap
constexpr int pincer1_52_id=10000+52;								//pincer 1 sb conveyor position to warehouse
constexpr int pincer1_53_id=10000+53;								//pincer 1 sb conveyor position system import/export
//pincer 2
constexpr int pincer2Id=10000+30;								//pincer 2 (new)

constexpr int pincer2_21_id=10000+21;							//pincer 2 sb position conveyor		//import
constexpr int pincer2_24_id=10000+24;							//pincer 2 sb position conveyor		//import
constexpr int pincer2_25_id=10000+25;							//pincer 2 sb position conveyor		//import
constexpr int pincer2_26_id=10000+26;							//pincer 2 sb position conveyor		//import

constexpr int pincer2_22_id=10000+22;							//pincer 2 sb position conveyor		//import
constexpr int pincer2_27_id=10000+27;							//pincer 2 sb position conveyor		//import
constexpr int pincer2_28_id=10000+28;							//pincer 2 sb position conveyor		//import
constexpr int pincer2_29_id=10000+29;							//pincer 2 sb position conveyor		//import

constexpr int pincer2_20_id=10000+20;							//pincer 2 sb position conveyor		//unload
constexpr int pincer2_31_id=10000+31;							//pincer 2 sb position conveyor		//unload
constexpr int pincer2_32_id=10000+32;							//pincer 2 sb position conveyor		//unload
constexpr int pincer2_33_id=10000+33;							//pincer 2 sb position conveyor		//unload

//
constexpr int maxMessageLength=75;
constexpr int maxDescriptionLength=200;


#define styleLabelR						QString("QLabel{font-size:14px;background-color:#ff2a2a;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define styleLabelO						QString("QLabel{font-size:14px;background-color:#f27a40;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define styleLabelG						QString("QLabel{font-size:14px;background-color:#09c500;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:blue;}")
#define styleLabelB						QString("QLabel{font-size:14px;background-color:#99ccff;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define styleLabelGr					QString("QLabel{font-size:14px;background-color:#999999;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:#000077;}")

#define labelStyleNormalBlack	QString("QLabel{font-size:15px;padding:2px;color:#000000;}")
#define labelStyleBigBlack	QString("QLabel{font-size:20px;padding:1px;color:#000000;}")
#define labelStyleBigRed		QString("QLabel{font-size:20px;padding:1px;color:#aa0000;}")

#define buttonStyle				QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #8888aa,stop:0.4#DDDDDD,stop:0.5#D8D8D8,stop:1.0#88aa88);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #aa8888,stop:0.4 #DDDDDD,stop:0.5 #D8D8D8,stop:1.0 #8888aa);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define buttonStyleActive QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define buttonStyleRed		QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #aa5555,stop:0.4#ff5555,stop:0.5#ff2222,stop:1.0#aa5555);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #aa5555,stop:0.4#ff5555,stop:0.5#ff2222,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define buttonStyleGreen	QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define buttonStyleOrange	QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #f2aa40,stop:0.4#f27740,stop:0.5#f27740,stop:1.0#f2aa40);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #f27740,stop:0.4#f2aa40,stop:0.5#f2aa40,stop:1.0#f27740);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")


#define spinStyleBigBlack	QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#000000;}")
#define spinStyleBigRed		QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#aa0000;}")
#define spinStyleBigGreen	QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#00aa00;}")
#define doubleSpinStyleBigBlack	QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#000000;}")
#define doubleSpinStyleBigRed		QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#aa0000;}")
#define doubleSpinStyleBigGreen	QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#00aa00;}")

#define splitterHorizontalStyle	QString("QSplitter::handle:horizontal{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:horizontal:hover{height:3px;background-color:#339966;}")
#define splitterVerticalStyle	QString("QSplitter::handle:vertical{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:vertical:hover{height:3px;background-color:#339966;}")
//
int getPosIdOfCassetteGlobal(int cassette,QString dbs);
int getPosIdOfSbGlobal(int ticket,QString dbs);
int getPosIdOfTicketGlobal(int sb,QString dbs);
QString getDescriptionOfPosIdGlobal(int posId,QString dbs);
void setCassetteSbTicketInPosG(int globalPosId,int cassette,int sbNumber,int ticket,QString info,bool isPlcChange,bool isMove,bool isSbPos,bool isUserChange,QString mDb);
int movePosDataGlobal(int from,int to,QString mDb);

#define logImportTrigger 1
#define logExportTrigger 2
void logImportExportGlobal(int ticket,int sbNumber,QString importLocation,QString exportLocation,int triggerStatus,QString mDb);

void log(QString description,QString type,QString mDb);
void logV(QString description,QString type,QString mDb);

#endif