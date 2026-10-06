#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include "Gb2.h"
class settingsDlgClass : public QDialog				
{
	QLineEdit						*odbNameLineEdit;
	QLineEdit						*odbcUserLineEdit;
	QLineEdit						*odbcPasswordLineEdit;
	QLineEdit						*basketCranePlcIpLineEdit;
	QLineEdit						*autoCranePlcIpLineEdit;
public:										
	Q_OBJECT								
public:										
	settingsDlgClass(QString odbcName,QString odbcUser,QString odbcPassword,QString basketIp,QString autoIp,QWidget *p);
	~settingsDlgClass();
	int exec();
	QString getOdbcName();
	QString getOdbcUser();
	QString getOdbcPassword();
	QString getBasketCranePlcIp();
	QString getAutoCranePlcIp();
};											
#endif
