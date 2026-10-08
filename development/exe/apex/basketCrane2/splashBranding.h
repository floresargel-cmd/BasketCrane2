#pragma once

#include <QFont>
#include <QPainter>
#include <QPixmap>
#include "lcmLogoData.h"

inline QPixmap basketCraneSplashPixmap()
{
    QPixmap pixmap(":/splash.png");
    QPainter painter(&pixmap);
    painter.fillRect(0, 0, pixmap.width(), 70, QColor("#262626"));
    QPixmap logo;
    logo.loadFromData(basketCraneLcmLogoPng, sizeof(basketCraneLcmLogoPng), "PNG");
    painter.drawPixmap(9, 9, logo.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.setPen(QColor("#f3f4f6"));
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(QRect(72, 14, pixmap.width() - 80, 28), Qt::AlignLeft | Qt::AlignVCenter,
        "Lorval Capital Management");
    painter.setPen(QColor("#b8bec8"));
    painter.setFont(QFont("Arial", 10));
    painter.drawText(QRect(72, 42, pixmap.width() - 80, 20), Qt::AlignLeft | Qt::AlignVCenter,
        "IT Software Team");
    painter.end();
    return pixmap;
}