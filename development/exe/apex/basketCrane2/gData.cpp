#include "gData.h"
double accur=0.00001;
bool areEqual(QPointF a,QPointF b) 
{
	if (fabs(a.x()-b.x())>accur)
		return false;
	if (fabs(a.y()-b.y())>accur)
		return false;
	return true;
}
////////////////////////////////////////////////////////////groupOfLinesClass/////////////////////////////////////////////////////////////
groupOfLinesClass::groupOfLinesClass()
{
	dxfColorIndex=0;
}
groupOfLinesClass::groupOfLinesClass(int dxfColorIndex_)
{
	dxfColorIndex=dxfColorIndex_;
}
groupOfLinesClass::~groupOfLinesClass()
{
	
}
bool groupOfLinesClass::append(QLineF tl)
{
	double accur=0.01;
	if (lines.count()>0)
	{
		if (areEqual(lines.last().p2(),tl.p1()))
		{
			lines.append(tl);
			return true;
		}
		else if (areEqual(lines.last().p2(),tl.p2()))
		{
			tl.setP2(tl.p1());
			tl.setP1(lines.last().p2());
			lines.append(tl);
			return true;
		}
		else if (areEqual(lines.first().p1(),tl.p1()))
		{
			tl.setP1(tl.p2());
			tl.setP2(lines.first().p1());
			lines.prepend(tl);
			return true;
		}
		else if (areEqual(lines.first().p1(),tl.p2()))
		{
			lines.prepend(tl);
			return true;
		}
		else
			return false;
	}
	lines.append(tl);
	return true;
}
QRectF groupOfLinesClass::calculateRect()
{
	QRectF ans;
	for (int i=0;i<lines.count();i++)
		ans=ans.united(QRectF(lines[i].p1(),lines[i].p2()));
	return ans;
}
QPainterPath groupOfLinesClass::calculatePath(bool closePath)
{
	QPainterPath ans;
	if (lines.count()>0)
	{
		ans.moveTo(lines[0].p1());
		for (int i=0;i<lines.count();i++)
			ans.lineTo(lines[i].p2());
		if (closePath)
			ans.closeSubpath();
	}
	return ans;
}
////////////////////////////////////////////////////////////dxfDataClass/////////////////////////////////////////////////////////////
dxfDataClass::dxfDataClass()
{
}
dxfDataClass::~dxfDataClass()
{
}
void dxfDataClass::calculate()
{
}
void dxfDataClass::addLayer(const DL_LayerData& data) 
{
}
void dxfDataClass::addPoint(const DL_PointData& data) 
{
	int color=attributes.getColor();
	pointGraphicsItemClass *point=new pointGraphicsItemClass(data.x,data.y);
	for (int i=0;i<point->count();i++)
		allLines.append(*point->at(i));
	delete point;
}
void dxfDataClass::addLine(const DL_LineData& data) 
{
	double dist=fabs(data.x1-data.x2)+fabs(data.y1-data.y2);
	if (dist<FLT_EPSILON)
		return;
	int color=attributes.getColor();
	lineGraphicsItemClass *line=new lineGraphicsItemClass(data.x1,data.y1,data.x2,data.y2);
	for (int i=0;i<line->count();i++)
		allLines.append(*line->at(i));
	delete line;
}
void dxfDataClass::addArc(const DL_ArcData& data) 
{
	double angle2=data.angle2;
	if (angle2<data.angle1)
		angle2+=360.;
	int color=attributes.getColor();
	ellipseGraphicsItemClass *arc=new ellipseGraphicsItemClass(data.cx,data.cy,data.radius,0.0,1.0,data.angle1*M_PI/180.,angle2*M_PI/180.);
	for (int i=0;i<arc->count();i++)
		allLines.append(*arc->at(i));
	delete arc;
}
void dxfDataClass::addCircle(const DL_CircleData& data) 
{
	int color=attributes.getColor();
	ellipseGraphicsItemClass *cicle=new ellipseGraphicsItemClass(data.cx,data.cy,data.radius,0.0,1.0,0.0,2.0*M_PI);
	for (int i=0;i<cicle->count();i++)
		allLines.append(*cicle->at(i));
	delete cicle;
}
void dxfDataClass::addEllipse(const DL_EllipseData& data) 
{
	int color=attributes.getColor();
	ellipseGraphicsItemClass *ellipse=new ellipseGraphicsItemClass(data.cx,data.cy,data.mx,data.my,data.ratio,data.angle1,data.angle2);
	for (int i=0;i<ellipse->count();i++)
		allLines.append(*ellipse->at(i));
	delete ellipse;
}
void dxfDataClass::addText(const DL_TextData& data)
{
	stringsVector<<dxfStringClass(QString(data.text.c_str()),QPointF(data.ipx,data.ipy),QFont("Tahoma",data.height));
}

