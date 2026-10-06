#ifndef DXFDATACLASSES_H
#define DXFDATACLASSES_H
#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QGraphicsScene>
////
#include "dl_dxf.h"
///////////////////////////////////////////////////point///////////////////////////////////////////////////
class pointGraphicsItemClass:public QVector<QLineF*>
{
public:
	pointGraphicsItemClass();
	pointGraphicsItemClass(double x,double y);
	~pointGraphicsItemClass();
};
///////////////////////////////////////////////////point///////////////////////////////////////////////////
///////////////////////////////////////////////////line////////////////////////////////////////////////////
class lineGraphicsItemClass:public QVector<QLineF*>
{
public:
	lineGraphicsItemClass();
	lineGraphicsItemClass(double x1,double y1,double x2,double y2);
	~lineGraphicsItemClass();
};
///////////////////////////////////////////////////ellipse/////////////////////////////////////////////////
class ellipseGraphicsItemClass:public QVector<QLineF*>
{
public:
	ellipseGraphicsItemClass();
	ellipseGraphicsItemClass(double cx,double cy,double mx,double my,double ratio,double angle1,double angle2);
	~ellipseGraphicsItemClass();
};
///////////////////////////////////////////////////ellipse/////////////////////////////////////////////////
#endif
