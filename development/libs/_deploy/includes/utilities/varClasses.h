#ifndef isExeRuningH
#define isExeRuningH
#include "Gu.h"
///////////////////////////////////////////////isExeRuningClass//////////////////////////////////////////
class isExeRuningClass
{
public:
	isExeRuningClass();
	~isExeRuningClass();
	static bool isRunning(QString proccesName);
};
///////////////////////////////////////////////isExeRuningClass//////////////////////////////////////////
/////////////////////////////////////////////////print screen log////////////////////////////////////////
class savePixmapClass:public QObject
{
	QString dir;
public:
Q_OBJECT
public:
	savePixmapClass(QString dir_);
signals:
	void doneSignal();
public slots:								
	void saveSlot(QPixmap pixmap);
};
class saveWindowClass:public QObject
{
	ulong									lastTimeStamp;
	QScreen								*screen;
	QVector<QPixmap>			pixMapV;
	savePixmapClass				*savePixmap;
	bool									isThreadSaving;
	QTimer								*delayTimer;
	bool									isLogging;
public:
Q_OBJECT
public:
	saveWindowClass(QScreen *screen_,QString dir);
	void grab(bool drawMouse,int mouseX,int mouseY,QString text);
	void newEvent(QObject *obj, QEvent *event);
	void togleLogging(bool displayMsg);
	bool getIsLogging();
signals:
	void savePixmapSignal(QPixmap pixmap);
public slots:								
	void onMainTimerSlot();
	void onDelayTimerSlot();
	void saveDoneSlot();
};
/////////////////////////////////////////////////print screen log////////////////////////////////////////
#endif