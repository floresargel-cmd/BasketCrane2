#ifndef BTNS_H
#define BTNS_H

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QDialog>
#include <QPushButton>
#include <QtNetwork>
#include <QRadioButton>
#include "../common/tcpTux.h"
class myQPushButton: public QPushButton
{	
	tcpServerClass								*tcpServer;
	QString												sendCommandToServerString;
	QString												sendZeroToServerString;
	bool													isPressed;
	Q_OBJECT								
public:
	myQPushButton(tcpServerClass *tcpServer_,QString sendCommandToServerString_,QString sendZeroToServerString_,QString txt,QWidget *p):QPushButton(txt,p)
	{
		isPressed=false;
		tcpServer=tcpServer_;
		sendCommandToServerString=sendCommandToServerString_;
		sendZeroToServerString=sendZeroToServerString_;
	}
	void setSendCommandToServerString(QString cmd);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void leaveEvent(QEvent *ev);
	void setEnabled(bool e,bool sendZeroToPlc=true);
	void setDisabled(bool e,bool sendZeroToPlc=true);
};
class myQRadioButton:public QRadioButton
{	
	tcpServerClass								*tcpServer;
	QString												sendCommandToServerString;
	QString												sendZeroToServerString;
	bool													isPressed;
	Q_OBJECT								
public:
	myQRadioButton(tcpServerClass *tcpServer_,QString sendCommandToServerString_,QString sendZeroToServerString_,QString txt,QWidget *p):QRadioButton(txt,p){isPressed=false;tcpServer=tcpServer_;sendCommandToServerString=sendCommandToServerString_;sendZeroToServerString=sendZeroToServerString_;}
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void leaveEvent(QEvent *ev);
	void setEnabled(bool e,bool sendZeroToPlc=true);
	void setDisabled(bool e,bool sendZeroToPlc=true);
};
#endif

