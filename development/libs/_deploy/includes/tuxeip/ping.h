#ifndef PING_H
#define PING_H

#include <QtGui>
#include <QTabWidget>
#include <QListWidget>
//#include <ti_me.h> 
#include "tuxG.h"
#include "tuxClass.h"
class pingClass : public QDialog				
{
	QString										ip;
	QListWidget								*resultsListWidget;
	QProcess									*pingProcess;
	Q_OBJECT								
public:										
  pingClass(QString ip_,QWidget *p=NULL);							
  ~pingClass();
	void reject();
	void accept();
private:
public slots:
	void cancelPushButtonSlot();
	void resultsSlot();
};											
#endif
