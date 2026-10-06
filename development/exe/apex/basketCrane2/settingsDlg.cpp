#include "settingsDlg.h"
settingsDlgClass::settingsDlgClass(QString odbcName,QString odbcUser,QString odbcPassword,QString basketIp,QString autoIp,QWidget *p):QDialog(p)
{
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	QGridLayout *dataLayout=new QGridLayout();mainLayout->addLayout(dataLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);
	//
	dataLayout->addWidget(new QLabel(QString("Odbc name:")),0,0);
	odbNameLineEdit=new QLineEdit(odbcName);dataLayout->addWidget(odbNameLineEdit,0,1);
	dataLayout->addWidget(new QLabel(QString("Odbc user:")),0,2);
	odbcUserLineEdit=new QLineEdit(odbcUser);dataLayout->addWidget(odbcUserLineEdit,0,3);
	dataLayout->addWidget(new QLabel(QString("Odbc password:")),0,4);
	odbcPasswordLineEdit=new QLineEdit(odbcPassword);dataLayout->addWidget(odbcPasswordLineEdit,0,5);
	dataLayout->addWidget(new QLabel(QString("Basket crane plc Ip:")),1,0);
	basketCranePlcIpLineEdit=new QLineEdit(basketIp);dataLayout->addWidget(basketCranePlcIpLineEdit,1,1,1,6);
	dataLayout->addWidget(new QLabel(QString("Auto crane plc Ip:")),2,0);
	autoCranePlcIpLineEdit=new QLineEdit(autoIp);dataLayout->addWidget(basketCranePlcIpLineEdit,2,1,1,6);
	//
	QPushButton *cancelBtn=new QPushButton(tr("Cancel"),this);
	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	btnsLayout->addWidget(cancelBtn);
	QPushButton *okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	btnsLayout->addWidget(okBtn);

}
settingsDlgClass::~settingsDlgClass()
{
}
int settingsDlgClass::exec()
{
	return QDialog::exec();
}
QString settingsDlgClass::getOdbcName()
{
	return odbNameLineEdit->text();
}
QString settingsDlgClass::getOdbcUser()
{
	return odbcUserLineEdit->text();
}
QString settingsDlgClass::getOdbcPassword()
{
	return odbcPasswordLineEdit->text();
}
QString settingsDlgClass::getBasketCranePlcIp()
{
	return basketCranePlcIpLineEdit->text();
}
QString settingsDlgClass::getAutoCranePlcIp()
{
	return autoCranePlcIpLineEdit->text();
}