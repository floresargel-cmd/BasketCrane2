#ifndef LADER_H
#define LADER_H

#include <QtGui>
#include <QMainWindow>
#include <QTreeWidget>
#include <QScrollArea>
#include <QList>
#include <QDomDocument>
#include "xmlReader.h"
#include "serv.h"
#include "rung.h"
class laderClass:public QScrollArea
{
	tuxipClass								*tuxip;
	xmlReaderClass						*xmlReader;
	QVector<rungClass*>				rungsVector;
public:
	laderClass(tuxipClass *tuxip_,xmlReaderClass *xmlReader_,QString rungComment,bool addTablesToMonitor,int updateFromPlcInterval,QWidget *p);
	~laderClass();
	xmlReaderClass* getXmlReader();
	QVector<xmlRoutineClass*> getXmlRoutinesVector();
	QVector<xmlTugVectorClass*> getXmlTugsVector();
	//
	void dataInPlcChanged(QString address,int index,QVariant value);
	void setShowAllRungs(bool v);
};
#endif
