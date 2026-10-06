#include "gView.h"

myQGraphicsViewClass::myQGraphicsViewClass(bool isZoomable_,QWidget* parent) : QGraphicsView(parent) 
{
	isZoomable=isZoomable_;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	setRenderHints(QPainter::Qt4CompatiblePainting);
#else
	setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
#endif
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	resetView();
	//
	readyToPan=isPanning=false;
}
void myQGraphicsViewClass::setScene(QGraphicsScene *s)
{
	QGraphicsView::setScene(s);
}
void myQGraphicsViewClass::resetView()
{
		resetTransform();
		setTransform(QTransform(1.,0.,0.,-1.,0.,0.),false);
}
void myQGraphicsViewClass::keyPressEvent(QKeyEvent *ev)
{
	if (scene()->focusItem())
		scene()->sendEvent(scene()->focusItem(),ev);
	emit keyPressedSignal(ev);
	ev->setAccepted(true);
}
void myQGraphicsViewClass::mousePressEvent(QMouseEvent* ev) 
{
    if (ev->button()==Qt::MidButton)
    {
		 readyToPan=true;
		 isPanning=false;
		 panningStart.setX(ev->x());
		 panningStart.setY(ev->y());
    }
	QGraphicsView::mousePressEvent(ev);
}
void myQGraphicsViewClass::mouseMoveEvent(QMouseEvent* ev)
{
	//pan
	if (readyToPan)
	{
		isPanning=true;
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (ev->x() - panningStart.x()));
		verticalScrollBar()->setValue(verticalScrollBar()->value() - (ev->y() -  panningStart.y()));
		panningStart.setX(ev->x());
		panningStart.setY(ev->y());
	}
	QGraphicsView::mouseMoveEvent(ev);
}
void myQGraphicsViewClass::mouseReleaseEvent(QMouseEvent *ev)
{
	if (readyToPan)
		readyToPan=false;
	QGraphicsView::mouseReleaseEvent(ev);
}
void myQGraphicsViewClass::scaleView(double scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
#ifndef _DEBUG
	if (factor<0.01||factor>1.0)
		return;
#endif
	scale(scaleFactor, scaleFactor);
}
void myQGraphicsViewClass::wheelEvent(QWheelEvent* ev)
{
	if (isZoomable)
		scaleView(pow((double)1.25,-ev->delta()/240.0));
}