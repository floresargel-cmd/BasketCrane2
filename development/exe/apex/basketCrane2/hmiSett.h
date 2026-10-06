#ifndef ISETT_H
#define ISETT_H

#include <QtGui>
#include <QMainWindow>
#include <QList>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>
#include <QMessageBox>
#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class hmiSettingsClass: public QDialog
{
	tuxipClass													*tuxip;
	QVector<floatPlcMemoryClass*>				floatPlcMemoryVector;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
public:
	Q_OBJECT
public:
	hmiSettingsClass(tuxipClass *tux,QWidget *p);
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
private slots:
	void reject();
	void accept();
};
#endif
