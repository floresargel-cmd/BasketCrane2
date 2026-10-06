#ifndef GAW1_H
#define GAW1_H
#include "G.h"
#include "config.h"
#include "../Gs.h"
#include "environmentIo.h"
#define toRotatingSouthPlcId 1100
#define toRotatingNorthPlcId 1110
#define bufferNoprthPlcId 1080
#define bufferSouthPlcId 1090
#define bufferEastOfOldOvenPlcId 1070

#define rotatingNorthPlcId 1200
#define rotatingSouthPlcId 1210
#define exit1PlcId 1220
#define exit2PlcId 1230
#define stackerWaitingPlcId 1240
#define stackerWorkingingPlcId 1250
#define crane2PlcId 1300


#include "startupProgress.h"
#include "destinationCatalog.h"
#define exportsDestacker QString("c2exportsDestacker")
#define exportsPacking QString("c2exportsPacking")

#define styleLabelR						QString("QLabel{font-size:14px;background-color:#ff2a2a;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define styleLabelO						QString("QLabel{font-size:14px;background-color:#f27a40;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define styleLabelG						QString("QLabel{font-size:14px;background-color:#09c500;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:blue;}")
#define styleLabelB						QString("QLabel{font-size:14px;background-color:#99ccff;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define styleLabelGr					QString("QLabel{font-size:14px;background-color:#999999;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:#000077;}")

//#define STYLE_G_SMALL    QString("QLabel{font-size:10px;background-color:#09c500;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:blue;}")
//#define STYLE_B_SMALL    QString("QLabel{font-size:10px;background-color:#99ccff;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:white;}")
//#define STYLE_GRAY_SMALL QString("QLabel{font-size:10px;background-color:#999999;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:#000077;}")

#define buttonStyle QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #8888aa,stop:0.4#DDDDDD,stop:0.5#D8D8D8,stop:1.0#88aa88);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #aa8888,stop:0.4 #DDDDDD,stop:0.5 #D8D8D8,stop:1.0 #8888aa);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define buttonStyleActive QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define buttonStyleRed QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #aa5555,stop:0.4#ff5555,stop:0.5#ff2222,stop:1.0#aa5555);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #aa5555,stop:0.4#ff5555,stop:0.5#ff2222,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define buttonStyleG QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define buttonStyleO QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #f2aa40,stop:0.4#f27740,stop:0.5#f27740,stop:1.0#f2aa40);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #f27740,stop:0.4#f2aa40,stop:0.5#f2aa40,stop:1.0#f27740);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")

extern QString passWeak;
extern QString passStrong;
void logV(QString description,QString type=infoStr);
void log(QString description,QString type=infoStr);
void logGui(QString mess,QString description,QString from,QString type=infoStr);
bool getPassword(QString title="Password is needed to open this window.",QString text="Please give the password",QString pass=QString(),QWidget *p=NULL);
int getBasketNumberInPos(int p);
QString	getDescriptionOfPosWithBasket(int b,QVector<int> ignorePos,QVector<int> ignoreIndex);
bool isPosActiveG(int posNumber);

#endif