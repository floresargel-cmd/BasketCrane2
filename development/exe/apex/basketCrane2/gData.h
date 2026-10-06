#ifndef G_DATA_H
#define G_DATA_H
#include <QtGui>
#include <QGraphicsLineItem>
//
#define DIST(a,b)       sqrt((a.x()-b.x())*(a.x()-b.x())+(a.y()-b.y())*(a.y()-b.y()))
#define ABSDIST(a,b)    fabs(a.x()-b.x())+fabs(a.y()-b.y())

#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include "gDataC.h"
////////////////////////////////////////////////////////////groupOfLinesClass/////////////////////////////////////////////////////////////
class groupOfLinesClass
{
public:
	int											dxfColorIndex;
	QVector<QLineF>					lines;
	groupOfLinesClass();
	~groupOfLinesClass();
	groupOfLinesClass(int dxfColorIndex_);
	bool append(QLineF tl);
	QPainterPath calculatePath(bool closePath);
	QRectF calculateRect();
	groupOfLinesClass &operator=(const groupOfLinesClass& p)
	{
		dxfColorIndex=p.dxfColorIndex;
		lines=p.lines;
		return *this;
	}
};
class dxfStringClass:public QString
{
public:
	QPointF										position;
	QFont											font;
	dxfStringClass():QString()
	{
		;
	}	
	dxfStringClass(QString str,QPointF pos,QFont f):QString(str)
	{
		position=pos;
		font=f;
	}
	~dxfStringClass()
	{
	}
};
////////////////////////////////////////////////////////////dxfDataClass/////////////////////////////////////////////////////////////
class dxfDataClass : public DL_CreationAdapter
{
	//QVector<groupOfLinesClass>	groupOfLines;
	QVector<QLineF>							allLines;
	//QVector<int>								allDxfColors;
	QVector<dxfStringClass>			stringsVector;
public:
	dxfDataClass();
	~dxfDataClass();
private:
	//void addLinesToGroup(QList<QLineF*> lines,int dxfColor);
	//QVector<groupOfLinesClass*> getGroupsWithColor(int dxfColorIndex);
public:
	void calculate();
	//int getNoOfGroupsOfLines(){return groupOfLines.count();}
	//groupOfLinesClass getGroupOfLines(int i){return groupOfLines[i];}
	int getNoOfLines(){return allLines.count();}
	QLineF getLine(int i){return allLines[i];}
	int getNoOfStrings(){return stringsVector.count();}
	dxfStringClass getString(int i){return stringsVector[i];}
	//
	virtual void addLayer(const DL_LayerData& data);
  virtual void addPoint(const DL_PointData& data);
  virtual void addLine(const DL_LineData& data);
  virtual void addArc(const DL_ArcData& data);
  virtual void addCircle(const DL_CircleData& data);
  virtual void addEllipse(const DL_EllipseData& data);
	virtual void addText(const DL_TextData&);
};

#endif
