#ifndef RUNG_H
#define RUNG_H

#include <QtGui>
#include <QMainWindow>
#include <QTreeWidget>
#include <QList.h>
#include <QDomDocument>
#include "serv.h"
class rungClass;
#include "tug.h"
class lineOfTugsClass:public QWidget//one line of tugs
{
	QHBoxLayout								*mainLayout;
	QVector<tugClass*>				tugsVector;
	QStringList								usedAdresses;
	int												status;
public:
	lineOfTugsClass()
	{
		mainLayout=new QHBoxLayout();mainLayout->setMargin(2);QWidget::setLayout(mainLayout);
		status=tugClass::STATUS_OFF;
	}
	~lineOfTugsClass()
	{
		for (int i=0;i<tugsVector.count();i++)
			delete tugsVector[i];
		tugsVector.clear();
	}
	QStringList getUsedAdresses()
	{
		return usedAdresses;
	}
	void setTuxIpClass(tuxipClass *t)
	{
		for (int i=0;i<tugsVector.count();i++)
			tugsVector[i]->setTuxIpClass(t);
	}
	void addTug(tugClass* t)
	{
		usedAdresses<<t->getUsedAdresses();
		tugsVector<<t;
		mainLayout->addWidget(t);
	}
	bool dataInPlcChanged(QString address,int index,QVariant value)
	{
		bool ans=false;
		for (int i=0;i<tugsVector.count();i++)
		{
			if (tugsVector[i]->dataInPlcChanged(address,index,value))
				ans=true;
		}
		if (ans)
		{
			status=tugClass::STATUS_ON;
			for (int i=0;i<tugsVector.count();i++)
			{
				if (tugsVector[i]->getStatus()==tugClass::STATUS_OFF)
				{
					status=tugClass::STATUS_OFF;
					break;
				}
			}
		}
		return ans;
	}
	int getStatus()
	{
		return status;
	}
	int getFirstTugStatus()
	{
		if (tugsVector.count()>0)
			return tugsVector[0]->getStatus();
		else return 0;
	}
};
///////////////////////////////////////////////singleClass/////////////////////////////////
class singleRungClass:public QGroupBox//only tags no []
{
	tuxipClass								*tuxip;
	xmlReaderClass						*xmlReader;
	QVector<lineOfTugsClass*>	lineOftugsVector;
	int												status;
Q_OBJECT
public:		
	singleRungClass(tuxipClass *tuxip_,xmlReaderClass *xmlReader_,QString data,QWidget *p);
	~singleRungClass();
	//
	void setTuxIpClass(tuxipClass *t);
	QStringList getUsedAdresses();
	bool dataInPlcChanged(QString address,int index,QVariant value);
	int getStatus();
	int getFirstTugStatus();
};
///////////////////////////////////////////////rungClass/////////////////////////////////
class rungClass:public QGroupBox//only singleClass
{
	tuxipClass									*tuxip;
	xmlReaderClass							*xmlReader;
	QVector<singleRungClass*>		subRungsVector;
	int													showAllRungsMode;
public:
	enum
	{
		MODE_SHOW_ALL=1,
		MODE_SHOW_ACTIVE_ONLY,
	};
public:
Q_OBJECT
public:
	rungClass(tuxipClass *tuxip_,xmlReaderClass *xmlReader_,QString routineName,QString number,QString comment,QString data,QWidget *p);
	~rungClass();
	//
	void setTuxIpClass(tuxipClass *t);
	QStringList getUsedAdresses();
	void setShowAllRungs(bool v);
	bool dataInPlcChanged(QString address,int index,QVariant value);
	void checkStatus();
};
#endif
