#ifndef BASKET_CRANE_PLANT_3D_H
#define BASKET_CRANE_PLANT_3D_H
#include <QtWidgets>
#include <QVector3D>
#include <functional>
#include <algorithm>
#include <cmath>

// Display-only snapshots: the renderer has no DB, PLC or mission-control API.
struct Plant3DSlot {
    QRectF footprint;
    int key=0, basket=0;
    QString destination, details;
    QStringList rows;
    QList<QPolygonF> sourcePaths, targetPaths;
    bool locked=false, source=false, target=false;
};
struct Plant3DScene {
    QVector<Plant3DSlot> positions3D;
    QList<QPolygonF> floorPaths;
    QVector3D crane;
    int carriedBasket=0;
    QStringList carriedRows;
    QString carriedDestination;
    bool telemetryValid=false;
    qint64 telemetryTime=0;
};
class Plant3DView : public QWidget {
public:
    typedef std::function<Plant3DScene()> Provider;
    explicit Plant3DView(Provider provider,QWidget *parent=nullptr):QWidget(parent),provider(provider) {
        setMinimumSize(300,250); setMouseTracking(true); setFocusPolicy(Qt::StrongFocus);
        setToolTip("Left drag: orbit | Right drag: pan | Wheel: zoom | Click basket: inspect | R: reset");
        QTimer *timer=new QTimer(this);
        connect(timer,&QTimer::timeout,this,[this]() { if (isVisible()) { scene=this->provider(); update(); refreshDetails(); } });
        timer->start(150); scene=provider();
    }
    std::function<void(const QString&)> inspected;
    void setScrollBars(QScrollBar* horizontal,QScrollBar* vertical) {
        horizontalScroll=horizontal; verticalScroll=vertical;
        connect(horizontal,&QScrollBar::valueChanged,this,[this](int value){pan.setX(-value); update();});
        connect(vertical,&QScrollBar::valueChanged,this,[this](int value){pan.setY(-value); update();});
    }
    void resetView() { yaw=0; elevation=1.20; zoom=1; pan=QPointF(); update(); }
    void topView() { elevation=1.52; yaw=0; zoom=1; pan=QPointF(); update(); }
    void zoomBy(double factor) { zoom=qBound(0.35,zoom*factor,5.0); update(); }
    int visibleDetailCardCount() const { return detailCards; }
    int visibleMissionMarkerCount() const { return missionMarkers; }
    int visibleBasketCount() const { int n=0; foreach (const Plant3DSlot& slot,scene.positions3D) if (slot.basket>0) ++n; return n; }
protected:
    void mousePressEvent(QMouseEvent *e) override { start=last=e->pos(); dragged=false; }
    void mouseMoveEvent(QMouseEvent *e) override {
        QPoint delta=e->pos()-last; last=e->pos();
        if ((e->pos()-start).manhattanLength()>4) dragged=true;
        if (e->buttons()&Qt::RightButton || e->buttons()&Qt::MiddleButton) { pan+=delta; update(); }
        else if (e->buttons()&Qt::LeftButton) { yaw+=delta.x()*0.008; elevation=qBound(0.15,elevation+delta.y()*0.006,1.52); update(); }
    }
    void mouseReleaseEvent(QMouseEvent *e) override {
        if (e->button()!=Qt::LeftButton || dragged) return;
        selectedKey=-1;
        for (int i=hits.size()-1;i>=0;--i) if (hits[i].path.contains(e->pos())) { selectedKey=hits[i].key?hits[i].key:-1; break; }
        refreshDetails(); update();
    }
    void wheelEvent(QWheelEvent *e) override { zoomBy(std::pow(1.0015,e->angleDelta().y())); e->accept(); }
    void keyPressEvent(QKeyEvent *e) override { if (e->key()==Qt::Key_R) resetView(); else QWidget::keyPressEvent(e); }
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this); painter.setRenderHint(QPainter::Antialiasing); painter.fillRect(rect(),QColor("#070d18"));
        faces.clear(); hits.clear(); detailCards=0; missionMarkers=0;
        QRectF ground;
        foreach (const Plant3DSlot& slot,scene.positions3D) if (validRect(slot.footprint)) ground=ground.united(slot.footprint);
        if (ground.isEmpty()) ground=QRectF(0,0,20000,10000);
        ground.adjust(-1800,-1800,1800,1800); center=ground.center();
        QRectF fit;
        for (int z=0;z<2;++z) foreach (const QPointF& pt,QVector<QPointF>()<<ground.topLeft()<<ground.topRight()<<ground.bottomLeft()<<ground.bottomRight())
            fit=fit.united(QRectF(projectRaw(QVector3D(pt.x(),pt.y(),z*3500)),QSizeF(1,1)));
        scale=qMin((width()-70)/qMax(1.0,fit.width()),(height()-100)/qMax(1.0,fit.height()))*zoom;
        synchronizeScrollBars(fit.size()*scale);
        offset=QPointF(width()/2.0,height()/2.0+10)-fit.center()*scale+pan;
        painter.setPen(QPen(QColor("#19283d"),1));
        painter.setPen(QPen(QColor("#334155"),0.8));
        foreach (const QPolygonF& outline,scene.floorPaths) {
            QPolygonF projected;
            foreach (const QPointF& point,outline) projected<<project(QVector3D(point.x(),point.y(),0));
            painter.drawPolyline(projected);
        }
        foreach (const Plant3DSlot& slot,scene.positions3D) {
            if (!validRect(slot.footprint)) continue;
            box(slot.footprint,0,100,QColor("#182332"),0);
            if (slot.basket<=0) continue;
            QColor color=slot.locked?QColor("#f87171"):(slot.source?QColor("#f472b6"):(slot.target?QColor("#38bdf8"):
                ((slot.destination=="HCA" || slot.destination=="P")?QColor("#22d3ee"):((slot.destination=="HCB" || slot.destination=="D")?QColor("#fbbf24"):QColor("#94a3b8")))));
            QRectF basket=slot.footprint.adjusted(80,80,-80,-80);
            box(basket,100,450,color.darker(290),slot.key,QString(),slot.key==selectedKey);
        }
        const bool valid=scene.telemetryValid && finite(scene.crane);
        const bool stale=valid && QDateTime::currentMSecsSinceEpoch()-scene.telemetryTime>5000;
        const QColor craneColor=!valid?QColor("#64748b"):(stale?QColor("#fbbf24"):QColor("#38bdf8"));
        const double cx=valid?scene.crane.x():ground.center().x();
        const double cy=valid?scene.crane.y():ground.center().y();
        const double railTop=ground.top(),railBottom=ground.bottom();
        box(QRectF(ground.left(),railTop,ground.width(),180),2700,2900,QColor("#475569"),0);
        box(QRectF(ground.left(),railBottom-180,ground.width(),180),2700,2900,QColor("#475569"),0);
        foreach (double x,QVector<double>()<<ground.left()<<ground.right()-180)
            foreach (double y,QVector<double>()<<railTop<<railBottom-180) box(QRectF(x,y,180,180),0,2700,QColor("#334155"),0);
        box(QRectF(cx-160,railTop,320,ground.height()),2900,3200,craneColor,0);
        box(QRectF(cx-360,cy-480,720,960),3200,3500,craneColor.lighter(125),0);
        // Z is drawn in the same mm coordinate sense as the application, with a
        // schematic 1 m baseline; gantry height is illustrative, not calibrated.
        const double lift=valid?qBound(180.0,1000.0+double(scene.crane.z()),2700.0):1000.0;
        box(QRectF(cx-45,cy-45,90,90),lift,3200,QColor("#cbd5e1"),0);
        box(QRectF(cx-550,cy-250,1100,500),lift,lift+120,craneColor,scene.carriedBasket>0?-2:0,
            scene.carriedBasket>0?QString::number(scene.carriedBasket):QString());
        if (scene.carriedBasket>0) box(QRectF(cx-650,cy-320,1300,640),qMax(10.0,lift-550),lift,QColor("#38bdf8"),-2,QString::number(scene.carriedBasket),selectedKey==-2);
        std::sort(faces.begin(),faces.end(),[](const Face& a,const Face& b){return a.depth<b.depth;});
        foreach (const Face& face,faces) {
            painter.setPen(QPen(face.selected?QColor("#ffffff"):face.color.lighter(125),face.selected?2:0.8));
            painter.setBrush(face.color); painter.drawPolygon(face.polygon);
            { QPainterPath path; path.addPolygon(face.polygon); hits<<Hit{path,face.key}; }
        }
        // Keep contents readable in screen space while camera/model geometry moves.
        QFont labelFont=painter.font(); labelFont.setPixelSize(11); painter.setFont(labelFont);
        foreach (const Plant3DSlot& slot,scene.positions3D) {
            if (slot.basket<=0 || !validRect(slot.footprint)) continue;
            
            const QColor border=slot.locked?QColor("#f87171"):(slot.source?QColor("#f472b6"):(slot.target?QColor("#38bdf8"):
                ((slot.destination=="HCA" || slot.destination=="P")?QColor("#22d3ee"):QColor("#fbbf24"))));
            drawDetailCard(painter,slot.footprint.adjusted(80,80,-80,-80),450,slot.key,QString("%1:%2%3").arg(slot.basket).arg(slot.destination).arg(slot.locked?" LOCKED":""),slot.rows,border);
        }
        if (scene.carriedBasket>0) drawDetailCard(painter,QRectF(cx-650,cy-320,1300,640),lift,-2,
            QString("%1  /  %2  Crane").arg(scene.carriedBasket).arg(scene.carriedDestination),scene.carriedRows,QColor("#38bdf8"));
        // Draw mission arrows last so equipment cannot hide them. Empty stations
        // are mission endpoints too. Preserve the original DXF arrow orientation.
        foreach (const Plant3DSlot& slot,scene.positions3D) {
            if (!validRect(slot.footprint)) continue;
            if (slot.source) drawMissionMarker(painter,slot.footprint,slot.sourcePaths,QColor("#f472b6"),true);
            if (slot.target) drawMissionMarker(painter,slot.footprint,slot.targetPaths,QColor("#38bdf8"),false);
        }
        painter.setPen(QColor("#94a3b8"));
        painter.drawText(QRect(16,12,width()-32,22),Qt::AlignLeft,"3D OVERVIEW  /  VIEW ONLY");
        painter.drawText(QRect(16,height()-44,width()-32,20),Qt::AlignLeft,
            valid?(stale?"Crane telemetry stale - last position shown":QString("Crane X %1  Y %2  Z %3 mm").arg(scene.crane.x(),0,'f',0).arg(scene.crane.y(),0,'f',0).arg(scene.crane.z(),0,'f',0)):"Waiting for crane telemetry - crane position schematic");
        painter.drawText(QRect(16,height()-24,width()-32,20),Qt::AlignLeft,"Layout coordinates; equipment shapes and heights illustrative");
    }
private:
    struct Face { QPolygonF polygon; QColor color; double depth; int key; QString label; bool selected; };
    struct Hit { QPainterPath path; int key; };
    Provider provider; Plant3DScene scene; QVector<Face> faces; QVector<Hit> hits;
    QScrollBar* horizontalScroll=nullptr;
    QScrollBar* verticalScroll=nullptr;
    void synchronizeScrollBars(const QSizeF& contentSize) {
        if (!horizontalScroll || !verticalScroll) return;
        const int horizontalExtent=int(std::ceil(qMax(0.0,contentSize.width()-(width()-70))/2));
        const int verticalExtent=int(std::ceil(qMax(0.0,contentSize.height()-(height()-100))/2));
        const QSignalBlocker horizontalBlock(horizontalScroll),verticalBlock(verticalScroll);
        horizontalScroll->setRange(-horizontalExtent,horizontalExtent);
        verticalScroll->setRange(-verticalExtent,verticalExtent);
        horizontalScroll->setPageStep(width()); verticalScroll->setPageStep(height());
        horizontalScroll->setSingleStep(30); verticalScroll->setSingleStep(30);
        horizontalScroll->setValue(qRound(-pan.x())); verticalScroll->setValue(qRound(-pan.y()));
        pan=QPointF(-horizontalScroll->value(),-verticalScroll->value());
    }
    double yaw=0,elevation=1.20,zoom=1,scale=1; QPointF center,offset,pan; QPoint start,last;
    int selectedKey=-1; int detailCards=0,missionMarkers=0; bool dragged=false;
    static bool finite(const QVector3D& v) { return std::isfinite(v.x()) && std::isfinite(v.y()) && std::isfinite(v.z()); }
    static bool validRect(const QRectF& r) { return std::isfinite(r.x()) && std::isfinite(r.y()) && std::isfinite(r.width()) && std::isfinite(r.height()) && r.width()>160 && r.height()>160; }
    QPointF projectRaw(const QVector3D& v) const {
        double x=v.x()-center.x(),y=v.y()-center.y();
        double u=x*std::cos(yaw)-y*std::sin(yaw), d=x*std::sin(yaw)+y*std::cos(yaw);
        return QPointF(u,-d*std::sin(elevation)-v.z()*std::cos(elevation));
    }
    QPointF project(const QVector3D& v) const { return projectRaw(v)*scale+offset; }
    double depth(const QVector3D& v) const { return (v.x()*std::sin(yaw)+v.y()*std::cos(yaw))*std::cos(elevation)+v.z()*std::sin(elevation); }
    void drawMissionMarker(QPainter& painter,const QRectF& footprint,QList<QPolygonF> outlines,const QColor& color,bool source) {
        if (outlines.isEmpty()) {
            QPolygonF frame; frame<<footprint.topLeft()<<footprint.topRight()<<footprint.bottomRight()<<footprint.bottomLeft()<<footprint.topLeft();
            outlines<<frame;
            const QPointF c=footprint.center(); const double w=footprint.width()*0.2,h=footprint.height()*0.3,d=source?1:-1;
            QPolygonF arrow; arrow<<QPointF(c.x()-w/2,c.y()-d*h)<<QPointF(c.x()+w/2,c.y()-d*h)
                <<QPointF(c.x()+w/2,c.y())<<QPointF(c.x()+w,c.y())<<QPointF(c.x(),c.y()+d*h)
                <<QPointF(c.x()-w,c.y())<<QPointF(c.x()-w/2,c.y())<<arrow.first();
            outlines<<arrow;
        }
        QPainterPath path;
        foreach (const QPolygonF& outline,outlines) {
            QPolygonF projected;
            foreach (const QPointF& point,outline) projected<<project(QVector3D(point.x(),point.y(),500));
            if (!projected.isEmpty()) { path.moveTo(projected.first()); for (int i=1;i<projected.size();++i) path.lineTo(projected[i]); }
        }
        if (!rect().intersects(path.boundingRect().toAlignedRect())) return;
        ++missionMarkers;
        painter.save(); painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor("#070d18"),5,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin)); painter.drawPath(path);
        painter.setPen(QPen(color,2.5,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin)); painter.drawPath(path);
        painter.restore();
    }
    void box(const QRectF& r,double bottom,double top,const QColor& color,int key,const QString& label=QString(),bool selected=false) {
        QVector<QVector3D> v;
        for (int z=0;z<2;++z) v<<QVector3D(r.left(),r.top(),z?top:bottom)<<QVector3D(r.right(),r.top(),z?top:bottom)<<QVector3D(r.right(),r.bottom(),z?top:bottom)<<QVector3D(r.left(),r.bottom(),z?top:bottom);
        const int indexes[5][4]={{0,1,5,4},{1,2,6,5},{2,3,7,6},{3,0,4,7},{4,5,6,7}};
        for (int f=0;f<5;++f) {
            QPolygonF polygon; double distance=0;
            for (int j=0;j<4;++j) { const QVector3D pt=v[indexes[f][j]]; polygon<<project(pt); distance+=depth(pt); }
            faces<<Face{polygon,f==4?color:color.darker(150+(f%2)*25),distance/4,key,f==4?label:QString(),selected};
        }
    }
    void drawDetailCard(QPainter& painter,const QRectF& footprint,double z,int key,const QString& title,const QStringList& content,const QColor& border) {
        QPolygonF surface;
        surface<<project(QVector3D(footprint.left(),footprint.top(),z))<<project(QVector3D(footprint.right(),footprint.top(),z))
            <<project(QVector3D(footprint.right(),footprint.bottom(),z))<<project(QVector3D(footprint.left(),footprint.bottom(),z));
        const QRectF bounds=surface.boundingRect();
        if (!rect().intersects(bounds.toRect())) return;
        ++detailCards;
        const QStringList rows=content.isEmpty()?QStringList()<<"Empty basket":content;
        const QRectF textArea=bounds.adjusted(2,1,-2,-1);
        QFont font=painter.font(); font.setPixelSize(11); QFontMetrics initial(font);
        int widest=initial.width(title);
        foreach (const QString& row,rows) widest=qMax(widest,initial.width(row));
        const double fit=qMin(textArea.width()/qMax(1,widest),textArea.height()/((rows.size()+1)*13.0));
        font.setPixelSize(qBound(6,int(11*qMin(1.0,fit)),11));
        painter.save(); painter.setFont(font);
        painter.setPen(QPen(key==selectedKey?QColor("#ffffff"):border,key==selectedKey?2:1));
        painter.setBrush(QColor("#111d2b")); painter.drawPolygon(surface);
        QPainterPath clip; clip.addPolygon(surface); painter.setClipPath(clip);
        const QFontMetrics metrics(font); const double lineHeight=qMax(7,metrics.height());
        double y=textArea.center().y()-lineHeight*(rows.size()+1)/2;
        painter.setPen(border);
        painter.drawText(QRectF(textArea.left(),y,textArea.width(),lineHeight),Qt::AlignCenter,metrics.elidedText(title,Qt::ElideRight,int(textArea.width())));
        painter.setPen(QColor("#d6e0eb"));
        foreach (const QString& row,rows) { y+=lineHeight; painter.drawText(QRectF(textArea.left(),y,textArea.width(),lineHeight),Qt::AlignCenter,metrics.elidedText(row,Qt::ElideRight,int(textArea.width()))); }
        painter.restore(); hits<<Hit{clip,key};
    }
    void refreshDetails() {
        if (!inspected) return;
        if (selectedKey==-2) { inspected(QString("Crane basket: %1").arg(scene.carriedBasket)); return; }
        foreach (const Plant3DSlot& slot,scene.positions3D) if (slot.key==selectedKey) { inspected(slot.details); return; }
        inspected("Click a basket to inspect its contents. Left drag: orbit | Right drag: pan | Wheel: zoom");
    }
};
inline Plant3DView *installPlant3DView(QWidget *map,Plant3DView::Provider provider) {
    QWidget *parent=map->parentWidget(); QVBoxLayout *layout=qobject_cast<QVBoxLayout*>(parent->layout());
    if (!layout) return nullptr;
    layout->removeWidget(map);
    // Keep the original scene view alive for existing model/selection plumbing.
    // It is hidden; the plant panel presents one 3D layout directly.
    map->hide();
    QWidget *page=new QWidget; page->setObjectName("plant3DPanel"); page->setStyleSheet("QWidget#plant3DPanel { background:#0f172a; }"); QVBoxLayout *pageLayout=new QVBoxLayout(page); pageLayout->setMargin(0);
    QHBoxLayout *buttons=new QHBoxLayout; pageLayout->addLayout(buttons);
    Plant3DView *view=new Plant3DView(provider); QLabel *details=new QLabel; details->setWordWrap(true); details->setTextFormat(Qt::PlainText);
    details->setMinimumHeight(45); details->setMaximumHeight(80);
    details->setText("Click a basket to inspect its contents. Left drag: orbit | Right drag: pan | Wheel: zoom");
    view->inspected=[details](const QString& text){details->setText(text);};
    QPushButton *reset=new QPushButton("Reset view"), *top=new QPushButton("Top view"), *plus=new QPushButton("Zoom +"), *minus=new QPushButton("Zoom -");
    buttons->addWidget(reset); buttons->addWidget(top); buttons->addWidget(plus); buttons->addWidget(minus); buttons->addStretch();
    QObject::connect(reset,&QPushButton::clicked,view,[view](){view->resetView();});
    QObject::connect(top,&QPushButton::clicked,view,[view](){view->topView();});
    QObject::connect(plus,&QPushButton::clicked,view,[view](){view->zoomBy(1.2);});
    QObject::connect(minus,&QPushButton::clicked,view,[view](){view->zoomBy(1/1.2);});
    QGridLayout* viewportLayout=new QGridLayout;
    viewportLayout->setSpacing(0); viewportLayout->setContentsMargins(0,0,0,0);
    QScrollBar* horizontal=new QScrollBar(Qt::Horizontal,page);
    QScrollBar* vertical=new QScrollBar(Qt::Vertical,page);
    horizontal->setObjectName("plantHorizontalScrollBar"); vertical->setObjectName("plantVerticalScrollBar");
    horizontal->setToolTip("Pan the plant overview left or right"); vertical->setToolTip("Pan the plant overview up or down");
    view->setScrollBars(horizontal,vertical);
    viewportLayout->addWidget(view,0,0); viewportLayout->addWidget(vertical,0,1);
    viewportLayout->addWidget(horizontal,1,0);
    viewportLayout->setRowStretch(0,1); viewportLayout->setColumnStretch(0,1);
    pageLayout->addLayout(viewportLayout,1); pageLayout->addWidget(details); layout->addWidget(page,1);
    return view;
}
#endif
