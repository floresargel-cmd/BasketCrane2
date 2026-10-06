#ifndef XMLREADER_H
#define XMLREADER_H

#include <QtGui>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QList.h>
#include <QDomDocument>

///////////////////////////////////////////////rungClass/////////////////////////////////
class xmlRungClass
{
	QString number;
	QString type;
	QString comment;
	QString data;
public:
	xmlRungClass(QString number_,QString type_,QString comment_,QString data_)
	{
		number=number_;
		type=type_;
		comment=comment_;
		data=data_;
	}
	QString getNumber()
	{
		return number;
	}
	QString getType()
	{
		return type;
	}
	QString getData()
	{
		return data;
	}
	QString getComment()
	{
		return comment;
	}
};
///////////////////////////////////////////////routineClass/////////////////////////////////
class xmlRoutineClass
{
	QString												name;
	QString												type;
	QVector<xmlRungClass*>				rungVector;

public:
	xmlRoutineClass(QString name_,QString type_)
	{
		name=name_;
		type=type_;
	}
	~xmlRoutineClass()
	{
		for (int i=0;i<rungVector.count();i++)
			delete rungVector[i];
		rungVector.clear();
	}
	QString getName()
	{
		return name;
	}
	QString getType()
	{
		return type;
	}
	bool addRung(QString name_,QString type_,QString number,QString rungType,QString comment,QString data)
	{
		if ((name==name_)&&(type==type_))
		{
			rungVector<<new xmlRungClass(number,rungType,comment,data);
			return true;
		}
		return false;
	}
	int getNoOfRungs()
	{
		return rungVector.count();
	}
	QString getNumberOfRung(int i)
	{
		if (i<rungVector.count())
			return rungVector[i]->getNumber();
		return QString();
	}
	QString getCommentOfRung(int i)
	{
		if (i<rungVector.count())
			return rungVector[i]->getComment();
		return QString();
	}
	QString getDataOfRung(int i)
	{
		if (i<rungVector.count())
			return rungVector[i]->getData();
		return QString();
	}
};
///////////////////////////////////////////////tugClass/////////////////////////////////
class xmlTugClass
{
	QString index;//this is [x] or [x].x
	QString comment;
public:
	xmlTugClass(QString index_,QString comment_)
	{
		index=index_;
		comment=comment_;
	}
	QString getIndex()
	{
		return index;
	}
	QString getComment()
	{
		return comment;
	}
};
///////////////////////////////////////////////tugClass/////////////////////////////////
class xmlTugVectorClass
{
	QString									address;
	QString									type;
	int											plcSize;
	QVector<xmlTugClass*>		tugVector;
public:
	xmlTugVectorClass(QString address_,QString type_,int plcSize_)
	{
		address=address_;
		type=type_;
		plcSize=plcSize_;
	}
	~xmlTugVectorClass()
	{
		for (int i=0;i<tugVector.count();i++)
			delete tugVector[i];
		tugVector.clear();
	}
	QString getAdress()
	{
		return address;
	}
	QString getType()
	{
		return type;
	}
	int getPlcSize()
	{
		return plcSize;
	}
	int getNoOfElements()
	{
		return tugVector.count();
	}
	bool addTag(QString type_,QString address_,QString operand,QString comment)
	{
		if ((address==address_)&&(type==type_))
		{
			tugVector<<new xmlTugClass(operand,comment);
			return true;
		}
		return false;
	}
	QString getAdressOfTag(int i)
	{
		if (i<tugVector.count())
			return address+tugVector[i]->getIndex();
		return QString();
	}
	QString getOperandOfTag(int i)
	{
		if (i<tugVector.count())
			return tugVector[i]->getIndex();
		return QString();
	}	
	QString getCommentOfTag(int i)
	{
		if (i<tugVector.count())
			return tugVector[i]->getComment();
		return QString();
	}
	QString getCommentOfTag(QString add,QString index)
	{
		QString ans;
		if (address==add)
		{
			for (int i=0;i<tugVector.count();i++)
			{
				if (index==tugVector[i]->getIndex())
					return tugVector[i]->getComment();
			}
		}
		return QString();
	}
	QString getCommentOfTag(QString add,int t);
};
///////////////////////////////////////////////xmlTreeWidgetClass/////////////////////////////////

class xmlReaderClass:public QWidget
{
	int														createTree;
	QTreeWidget										*allTreeWidget;
	QVector<xmlRoutineClass*>			routinesVector;
	QVector<xmlTugVectorClass*>		tugsVector;
	QString												ipAdress;
public:
	xmlReaderClass(QString fileName,int createTree_,QWidget *p);
	~xmlReaderClass();
	void readXml(QString fileName);
	void printNodes(QDomNode dNode,QTreeWidgetItem *item);
	QVector<xmlRoutineClass*> getXmlRoutinesVector();
	QVector<xmlTugVectorClass*> getXmlTugsVector();
	QString getCommentOfTag(QString address,QString index);
	QTreeWidget *getXMLTree();
	QString getIpAdress();
};
#endif
