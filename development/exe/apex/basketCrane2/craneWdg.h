#ifndef CRANEWDG_H
#define CRANEWDG_H

class craneWidgetClass;
#include <QtGui>
#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "crane.h"
#include "carriage.h"
#include "hmiDlg.h"
class craneWidgetClass : public QWidget
{
    bool historyLoaded = false;
	QListWidget														*logGuiList;
	QString																logGuiName;
	QVector<activeIconClass*>							activeIconsVector;
	QLabel																*xPosLabel;
	QLabel																*yPosLabel;
	QLabel																*zPosLabel;
	QLabel																*xPosTargetLabel;
	QLabel																*yPosTargetLabel;
	QLabel																*zPosTargetLabel;
	hmiDlgClass														*hmiDlg;
	Q_OBJECT								
public:										
	craneWidgetClass(tuxipServerClass *tuxipServer,tuxipClass *tuxip,QWidget *conv,craneClass *crane=NULL);
	~craneWidgetClass();
	void setCarriageClass(carriageClass *c);
	void loadFromDataBase();
	void addMessageToGui(QString mess,QString details,QString type);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
public slots:
	void loadHmiDlgSlot();
	void popUpList();						
};											
#endif
