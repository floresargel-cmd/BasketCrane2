#ifndef BASKET_CRANE_PLANT_APPEARANCE_H
#define BASKET_CRANE_PLANT_APPEARANCE_H
#include <QtWidgets>

inline QColor plantCanvasColor() { return QColor(7,13,24); }
inline QPen plantDrawingPen(QPen pen) {
    const QColor c = pen.color();
    if (c.red()==200 && c.green()==200 && c.blue()==200) pen.setColor(QColor("#52647b"));
    else if (c.red()==200 && c.green()==225 && c.blue()==200) pen.setColor(QColor("#38bdf8"));
    else if (c.red()==150 && c.green()==150 && c.blue()==175) pen.setColor(QColor("#7dd3fc"));
    else if (c.red()==100 && c.green()==200 && c.blue()==100) pen.setColor(QColor("#94a3b8"));
    else if (c.red()==200 && c.green()==200 && c.blue()==250) pen.setColor(QColor("#64748b"));
    else if (c.red()==255 && c.green()==100 && c.blue()==100) pen.setColor(QColor("#fb7185"));
    pen.setCosmetic(true); pen.setWidthF(1.0);
    return pen;
}

// Draw text in device pixels inside the basket's original world-space footprint.
// Zoom reveals more contents; labels never grow across adjacent baskets.
// This item accepts no input, preserving the existing machine hit targets.
class PlantBasketCard : public QGraphicsItem {
    QRectF bounds;
    QString title, destination, exportCodes;
    QStringList contents;
    bool locked=false, source=false, target=false, hovered=false;
public:
    explicit PlantBasketCard(const QRectF& rect, QGraphicsItem *parent=nullptr) : QGraphicsItem(parent), bounds(rect) {
        setAcceptedMouseButtons(Qt::NoButton); setAcceptHoverEvents(false); setZValue(10);
    }
    QRectF boundingRect() const override { return bounds; }
    void setExportCodes(const QString& codes) { exportCodes=codes; update(); }
    void setHovered(bool value) { hovered=value; update(); }
    void setBasket(int number, const QString& dest, const QStringList& rows, bool isLocked, bool from, bool to) {
        title=QString("#%1").arg(number); destination=dest; contents=rows;
        locked=isLocked; source=from; target=to; update();
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        const QRectF deviceRect=painter->worldTransform().mapRect(bounds).adjusted(2,2,-2,-2);
        if (deviceRect.width()<8 || deviceRect.height()<8) return;
        painter->save(); painter->resetTransform(); painter->setRenderHint(QPainter::Antialiasing);
        QColor accent=(destination=="HCA" || destination=="P") ? QColor("#22d3ee") : ((destination=="HCB" || destination=="D") ? QColor("#fbbf24") : QColor("#94a3b8"));
        if (locked) accent=QColor("#ef4444"); else if (source) accent=QColor("#f472b6"); else if (target) accent=QColor("#38bdf8");
        QLinearGradient fill(deviceRect.topLeft(),deviceRect.bottomLeft());
        fill.setColorAt(0, locked ? QColor("#52212b") : QColor("#243349")); fill.setColorAt(1,QColor("#111d30"));
        painter->setBrush(fill); painter->setPen(QPen(hovered?QColor("#ffffff"):accent, hovered?2.0:1.2));
        painter->drawRoundedRect(deviceRect,4,4);
        painter->setClipRect(deviceRect.adjusted(4,3,-4,-3));
        QFont font("Segoe UI"); font.setPixelSize(deviceRect.width()>100?13:11); font.setBold(true); painter->setFont(font);
        const QRectF textRect=deviceRect.adjusted(6,3,-6,-3);
        const QString caption=title+((destination.isEmpty() || destination=="None")?QString():"  "+destination)+(exportCodes.isEmpty()?QString():":"+exportCodes)+(locked?" LOCK":"");
        painter->setPen(QColor("#f8fafc"));
        painter->drawText(textRect,Qt::AlignTop|Qt::AlignLeft,QFontMetrics(font).elidedText(caption,Qt::ElideRight,int(textRect.width())));
        font.setBold(false); font.setPixelSize(deviceRect.width()>100?12:10); painter->setFont(font); painter->setPen(QColor("#cbd5e1"));
        const int lineHeight=QFontMetrics(font).height()+1;
        int y=int(textRect.top())+16;
        const int capacity=qMax(0,int(textRect.bottom()-y)/lineHeight);
        for (int i=0; i<qMin(capacity,contents.size()); ++i) {
            QString row=contents[i];
            if (deviceRect.width()<100) row=row.section(" / ",0,0);
            if (capacity>1 && i==capacity-1 && contents.size()>capacity) row=QString("+%1 profiles (zoom in)").arg(contents.size()-i);
            painter->drawText(QRectF(textRect.left(),y,textRect.width(),lineHeight),Qt::AlignLeft|Qt::AlignTop,
                QFontMetrics(font).elidedText(row,Qt::ElideRight,int(textRect.width())));
            y+=lineHeight;
        }
        painter->restore();
    }
};

inline PlantBasketCard *plantBasketCard(QGraphicsItem *basket) {
    foreach (QGraphicsItem *child, basket->childItems()) if (PlantBasketCard *card=dynamic_cast<PlantBasketCard *>(child)) return card;
    return nullptr;
}
#endif
