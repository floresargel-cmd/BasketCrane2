#ifndef LINEEDITS_H
#define LINEEDITS_H

#include <QtGui>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QMenu>
#include <QGridLayout>
//#include <ti_me.h> 
class myLabel;
class intLineEditClass;
class floatLineEditClass;
#include "tuxG.h"
//#include "tuxClass.h"
static QVector<bool> lineEditCalculateBits(signed int n)
{
	QVector<bool> ans(32);
	signed int mask=1;
	for (int i=0;i<32;i++)
	{
		ans[i]=static_cast<bool>(n&mask);
		mask<<=1;
	}
	return ans;
}
static int lineEditCalculateInt(QVector<bool> bits)
{
	int ans=0;
	unsigned int bitValue=0;
	bitValue=(int)bits[0];
	ans|=bitValue;
	for(int i=1;i<bits.count();i++)
	{
		bitValue=(int)bits[i];
		bitValue<<=i;
		ans|=bitValue;
	}
	return ans;
}
///////////////////////////////////myLabel//////////////////////////////////
class myLabel:public QLabel
{
Q_OBJECT
public:
	myLabel(QString str,QWidget *parent=NULL);
	void mouseReleaseEvent(QMouseEvent *ev);
signals:
	void clickedSignal();
};

///////////////////////////////////myLineEditClass//////////////////////////////////
class myLineEditClass:public QLineEdit
{
protected:
	bool isInEditingMode;
Q_OBJECT
public:
	myLineEditClass(QWidget *parent=NULL);
	void setIsInEditingMode(bool m);
	bool getIsInEditingMode();
	void focusOutEvent(QFocusEvent *e);
signals:
	void focusOutSignal();
};

////////////////////////intLineEditClass//////////////////
class intLineEditClass:public myLineEditClass
{
	QIntValidator				*validator;
	QVector<QCheckBox*> checkBtnsVector;
	QStringList					checkBtnsVectorComments;
Q_OBJECT
public:
	intLineEditClass(QWidget *parent=NULL);
	~intLineEditClass();
	void setBitComment(int bit,QString comment);
	void setValue(int v);
	void keyPressEvent(QKeyEvent *e);
	void contextMenuEvent(QContextMenuEvent *ev);
signals:
	void enterSignal(int value);
public slots:
	void checkBoxstateChangedSlot(int state);
};
////////////////////////floatLineEditClass//////////////////
class floatLineEditClass:public myLineEditClass
{
	QDoubleValidator *validator;
Q_OBJECT
public:
	floatLineEditClass(QWidget *parent=NULL);
	~floatLineEditClass();
	void setValue(float v);
	void keyPressEvent(QKeyEvent *e);
signals:
	void enterSignal(float value);
};
#endif