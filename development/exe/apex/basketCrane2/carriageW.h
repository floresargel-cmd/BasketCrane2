#ifndef carriageW_H
#define carriageW_H

class carriageWidgetClass;
#include <QtGui>

#include "Gb2.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "allPos.h"
#include "carriage.h"
class intStringMapClass
{
	QVector<int>						indexV;
	QVector<QString>				textV;
	QVector<QString>				descriptionV;
public:	
	intStringMapClass(){}
	~intStringMapClass(){}
	void add(int index,QString text,QString description)
	{
		indexV<<index;
		textV<<text;
		descriptionV<<description;
	}
	QString getTextOfIndex(int id)
	{
		QString ans;
		int i=indexV.indexOf(id);
		if (i>=0)
			ans=textV[i];
		return ans;
	}
	QString getDescriptionOfIndex(int id)
	{
		QString ans;
		int i=indexV.indexOf(id);
		if (i>=0)
			ans=descriptionV[i];
		return ans;
	}
};
class carriageWidgetClass : public QGroupBox				
{
	QLabel									*activeMissionLabel;
	QLabel									*activeCraneStepLabel;
	QLabel									*activeHooksStepLabel;
	QString									activeFromDescription;
	QString									activeToDescription;

	QLabel									*nextMissionLabel;
	QString									nextFromDescription;
	QString									nextToDescription;

	QString									logGuiName;
	QListWidget							*logGuiList;
	int											activeCraneStep;
	intStringMapClass				plcCraneStepTexts;
	int activeBasketNumber = 0;
	int nextBasketNumber = 0;
	Q_OBJECT
public:										
	carriageWidgetClass(QWidget *p=NULL);
	~carriageWidgetClass();
	void loadFromDataBase();
	void addMessageToGui(QString mess,QString details,QString type);

	void setActiveBasket(int number) { activeBasketNumber = number; updateGui(); }
	void setNextBasket(int number) { nextBasketNumber = number; updateGui(); }
	int activeBasket() const { return activeBasketNumber; }
	int nextBasket() const { return nextBasketNumber; }
	void setNextFrom(QString fDescription);
	void setNextTo(QString tDescription);

	void setActiveFrom(QString fDescription);
	void setActiveTo(QString tDescription);

	void setActiveCraneStep(int s);
	QString getCraneStepText(int s);
	QString getCraneStepDescription(int s);
	
	void updateGui();
public slots:								
	void popUpList();						
};											
#endif
