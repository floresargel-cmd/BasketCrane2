#ifndef EXPWDG_H
#define EXPWDG_H

class exportStationWidgetClass;
#include <QtGui>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QToolButton>
#include <QToolBox>
#include "Gb2.h"
class exportStationWidgetClass:public QWidget
{
	QString								dbTable;
	QString								title;
	QListWidget						*exportsList;
	QToolButton						*basketUpBtn;
	QToolButton						*basketDownBtn;
	QToolButton						*basketRemoveBtn;
	int										selectedBasket;
    QToolBox *queueToolBox = nullptr;
    int queueToolBoxIndex = -1;
	Q_OBJECT								
public:										
	exportStationWidgetClass(QString dbTable_,QString title_,QWidget *p=NULL);
	~exportStationWidgetClass();
	void updateFromDatabase();
    void attachToToolBox(QToolBox *box, int index);
public slots:
	void dbUpdateTimerSlot();
	void exportsListSelectionChangedSlot(QListWidgetItem* selected,QListWidgetItem* deselected);
	void basketUpSlot();
	void basketDownSlot();
	void basketRemoveSlot();
	void resizeEvent(QResizeEvent *e);
signals:
	void listChangedSignal();
};											
#endif
