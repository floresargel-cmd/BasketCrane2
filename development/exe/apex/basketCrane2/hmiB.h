#ifndef HMIB_H
#define HMIB_H

#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include "gView.h"
#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
class hmiDlgBasicClass : public QDialog				
{												
	tuxipClass													*tuxip;
	QVector<hmiPushButtonWidgetClass*>	pushButtonsVector;
	bitLogVectorClass										*bitWarningsVector;
	bitLogVectorClass										*bitErrorsVector;
	Q_OBJECT								
public:										
	hmiDlgBasicClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip_,QWidget *p);
	~hmiDlgBasicClass();
	int exec();
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
};											
#endif
