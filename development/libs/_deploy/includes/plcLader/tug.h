#ifndef TUG_H
#define TUG_H

#include <QtGui>
#include <QMainWindow>
#include <QGroupBox>
#include <QTreeWidget>
#include <QList.h>
#include <QDomDocument>
#include "serv.h"
#include "xmlReader.h"
#include "tugVar.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////tugClass///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class tugClass:public QGroupBox
{
	tuxipClass								*tuxip;
	xmlReaderClass						*xmlReader;
protected:
	QString										functionName;
	QStringList								functionArgs;
	QVector<tugVarClass*>			tugVars;
	//
	int												status;
public:		
	enum
	{
		STATUS_NONE=1,
		STATUS_ON,
		STATUS_OFF,
	};
public:		
Q_OBJECT
public:		
	tugClass(xmlReaderClass *xmlReader_,QString name,QString data,QWidget *p);
	~tugClass();
	//
	void setTuxIpClass(tuxipClass *t);
	bool dataInPlcChanged(QString address,int index,QVariant value);
	virtual void updateStatus();
	QStringList getUsedAdresses();
	void updateGui();
	int getStatus();
};
//////////////////////////////////////////////xicClass///////////////////////////////////////////////////////////
class xicClass:public tugClass
{
public:		
	xicClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////xicClass///////////////////////////////////////////////////////////
class xioClass:public tugClass
{
public:		
	xioClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////oteClass///////////////////////////////////////////////////////////
class oteClass:public tugClass
{
public:		
	oteClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////otlClass///////////////////////////////////////////////////////////
class otlClass:public tugClass
{
public:		
	otlClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////otuClass///////////////////////////////////////////////////////////
class otuClass:public tugClass
{
public:		
	otuClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////osrClass///////////////////////////////////////////////////////////
class osrClass:public tugClass
{
public:		
	osrClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////onsClass///////////////////////////////////////////////////////////
class onsClass:public tugClass
{
public:		
	onsClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////onfClass///////////////////////////////////////////////////////////
class onfClass:public tugClass
{
public:		
	onfClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////equClass///////////////////////////////////////////////////////////
class equClass:public tugClass
{
public:		
	equClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////limClass///////////////////////////////////////////////////////////
class limClass:public tugClass
{
public:		
	limClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////grtClass///////////////////////////////////////////////////////////
class grtClass:public tugClass
{
public:		
	grtClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////geqClass///////////////////////////////////////////////////////////
class geqClass:public tugClass
{
public:		
	geqClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////lesClass///////////////////////////////////////////////////////////
class lesClass:public tugClass
{
public:		
	lesClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////leqClass///////////////////////////////////////////////////////////
class leqClass:public tugClass
{
public:		
	leqClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////movClass///////////////////////////////////////////////////////////
class movClass:public tugClass
{
public:		
	movClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////addClass///////////////////////////////////////////////////////////
class addClass:public tugClass
{
public:		
	addClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////subClass///////////////////////////////////////////////////////////
class subClass:public tugClass
{
public:		
	subClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////mulClass///////////////////////////////////////////////////////////
class mulClass:public tugClass
{
public:		
	mulClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////divClass///////////////////////////////////////////////////////////
class divClass:public tugClass
{
public:		
	divClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////sqrClass///////////////////////////////////////////////////////////
class sqrClass:public tugClass
{
public:		
	sqrClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////sinClass///////////////////////////////////////////////////////////
class sinClass:public tugClass
{
public:		
	sinClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////cosClass///////////////////////////////////////////////////////////
class cosClass:public tugClass
{
public:		
	cosClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////tanClass///////////////////////////////////////////////////////////
class tanClass:public tugClass
{
public:		
	tanClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////atnClass///////////////////////////////////////////////////////////
class atnClass:public tugClass
{
public:		
	atnClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////asnClass///////////////////////////////////////////////////////////
class asnClass:public tugClass
{
public:		
	asnClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////acsClass///////////////////////////////////////////////////////////
class acsClass:public tugClass
{
public:		
	acsClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////lnClass///////////////////////////////////////////////////////////
class lnClass:public tugClass
{
public:		
	lnClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////logClass///////////////////////////////////////////////////////////
class logClass:public tugClass
{
public:		
	logClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
//////////////////////////////////////////////xpyClass///////////////////////////////////////////////////////////
class xpyClass:public tugClass
{
public:		
	xpyClass(xmlReaderClass *xmlReader,QString name,QString strData,QWidget *p);
	void updateStatus();
};
#endif
