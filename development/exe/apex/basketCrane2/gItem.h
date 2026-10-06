#ifndef G_ITEM_H
#define G_ITEM_H

#include <QtGui>
#include <QMainWindow>
#include <QAction>
#include <QList>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include "Gb2.h"
#include "gData.h"
class gItemClass : public QObject,public QGraphicsItem
{
protected:
	QPen												pen;
	QColor											mouseOverFillColor;
	bool												mouseOverFlag;
	bool												hideWhenMouseNotOver;

	QBrush											fillBrush;
	//	
	QRectF											rectOfAll;
	QPainterPath								pathOfAll;
	//
	QPainterPath								boundingRectPath;
	//
	bool												isMouseOver;
	bool												myIsVisible;
	//
Q_OBJECT
public:
	gItemClass();
	gItemClass(gItemClass *p,float dx,float dy,QObject* o);
	gItemClass(QString fileName,QPen pen_,bool mouseOverFlag_,bool hideWhenMouseNotOver_,QColor mouseOverFillColor_,QObject* o=NULL);
	~gItemClass();
	void myRotate(float xCenter,float yCenter,float angle);
	void loadFromDxfFile(QString fileName);
	void setPen(QPen p);
	void setFillBrush(QBrush b);
	void setMouseOverFillColor(QColor c);
	//
	QPainterPath shape() const;
    QPainterPath displayPath() const { return pathOfAll; }
	QRectF boundingRect() const;
	void setMouseOverFlag(bool f);
	//
	void showGraphics(bool s);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *ev);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *ev);
signals:
	void hoverLeaveSignal();
	void hoverEnterSignal();
	void mousePressedSignal(QGraphicsSceneMouseEvent*);
	void mouseReleasedSignal(QGraphicsSceneMouseEvent*);
};											
#endif
