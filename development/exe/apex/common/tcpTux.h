#ifndef TCPTUX_H
#define TCPTUX_H

#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QDialog>
#include <QtNetwork>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include "serv.h"
////////////////////////////////////////////////////////////////////////tcpServerClass////////////////////////////////////////////////////////////////////////
class tcpServerClass : public QWidget
{
	tuxipClass										*tuxip;
	Q_OBJECT
public:
   tcpServerClass(tuxipClass *tuxip_,QWidget *parent=NULL);
	 tuxipClass *getTuxipClass();
public:
	void sendString(QString outgoingString);
};
#endif
