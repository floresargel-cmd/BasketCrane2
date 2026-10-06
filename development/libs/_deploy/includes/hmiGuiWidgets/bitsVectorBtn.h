#ifndef	BITSVECTORDLG_H
#define BITSVECTORDLG_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include "hmiGuiWidgetsLib.h"
///////////////////////////////////////////bitsVectorsDialogClass/////////////////////////////////////////////////////
class bitsVectorsDialogClass : public QDialog
{
protected:
	bitLogVectorClass								*bitLogVector;
	QString													dbConnection;
Q_OBJECT
public:		
  bitsVectorsDialogClass(QString title,QString fileName,QString plcAddress,int type,QString onLabelStyle,QString offLabelStyle,QString errorsLogTableName,QString plcLogTableName,QString dbConnection_,bool useTranslation,QWidget *p);
	~bitsVectorsDialogClass();
	void dataInPlcChanged(QString table,int index,QVariant value);
	void setBlinkingStyle(QString blinkingStyle,int type);
	void setTypeOfBit(int ind,int bit,QString onStyle,QString offStyle,QString blinkingStyle,int blinkingType);
};
///////////////////////////////////////////bitsVectorsPushButtonClass/////////////////////////////////////////////////////
class bitsVectorBtnClass : public QPushButton
{
	QString											plcIp;
	QString											plcTable;			 //this activates the red state of the button
	int													plcIndex;			 //this activates the red state of the button
	int													plcBit;				 //this activates the red state of the button
	bitsVectorsDialogClass			*bitsVectorsDialog;
	QString											dbConnection;
	QString											onStyle;
	QString											offStyle;
Q_OBJECT
public:		
  bitsVectorBtnClass(QString title,QString plcIp_,QString plcTable_,int plcIndex_,int plcBit_,QString fileName,QString listPlcTable,int type,QString onBtnStyle_,QString offBtnStyle_,QString onLabelStyle,QString offLabelStyle,QString errorsLogTableName,QString plcLogTableName,QString dbConnection_,bool useTranslation,QWidget *p);
	~bitsVectorBtnClass();
	void setOnOffStyles(QString on,QString off);
	void dataInPlcChanged(QString ip,QString table,int index,QVariant value);
	void setBlinkingStyle(QString blinkingStyle,int type);
	void setTypeOfBit(int ind,int bit,QString onStyle,QString offStyle,QString blinkingStyle,int blinkingType);
public slots:								
	void onClickedSlot();

}; 
#endif