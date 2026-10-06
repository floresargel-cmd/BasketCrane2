#ifndef BASKET_CRANE_MODERN_UI_H
#define BASKET_CRANE_MODERN_UI_H
#include <QtWidgets>
#include <QHostInfo>
#include <windows.h>
#include "crane1Theme.h"
#include "plantAppearance.h"
#include "version.h"

inline void applyModernCranePalette() {
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#111827"));
    palette.setColor(QPalette::WindowText, QColor("#e2e8f0"));
    palette.setColor(QPalette::Base, QColor("#111827"));
    palette.setColor(QPalette::AlternateBase, QColor("#1e293b"));
    palette.setColor(QPalette::Text, QColor("#e2e8f0"));
    palette.setColor(QPalette::Button, QColor("#334155"));
    palette.setColor(QPalette::ButtonText, QColor("#e2e8f0"));
    palette.setColor(QPalette::Highlight, QColor("#075985"));
    palette.setColor(QPalette::HighlightedText, QColor("#ffffff"));
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#94a3b8"));
    palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#94a3b8"));
    qApp->setPalette(palette);
}

inline QString modernCraneTheme() {
    return crane1Theme() + QStringLiteral(R"CSS(
QMainWindow, QWidget#workspace, QWidget#sidePanel { background:#0b1220; }
QWidget#workspaceHeader, QToolBar { background:#111827; border:0; border-bottom:1px solid #263449; }
QToolBar { spacing:7px; padding:8px 12px; }
QToolBar::separator { background:#334155; width:1px; margin:7px 6px; }
QToolBox { background:#111827; border:1px solid #2d3b50; border-radius:8px; }
QToolBox::tab { background:#1e293b; color:#94a3b8; min-height:24px; padding:4px; border:0; border-bottom:1px solid #334155; font-weight:600; }
QToolBox::tab:selected { background:#075985; color:#e0f2fe; }
QToolBox::tab:hover { background:#334155; color:#ffffff; }
QSplitter::handle { background:#0b1220; }
QSplitter::handle:hover { background:#334155; }
QLabel { background:transparent; }
QListWidget, QTextEdit, QPlainTextEdit { background:#111827; color:#cbd5e1; border:1px solid #334155; border-radius:5px; padding:6px; selection-background-color:#075985; }
QListWidget::item { padding:4px; }
QListWidget::item:selected { background:#075985; color:#ffffff; }
QScrollBar:vertical { width:12px; background:#0f172a; }
QScrollBar::handle:vertical { min-height:24px; background:#475569; border-radius:6px; }
QScrollBar:horizontal { height:12px; background:#0f172a; }
QScrollBar::handle:horizontal { min-width:24px; background:#475569; border-radius:6px; }
QScrollBar::add-line, QScrollBar::sub-line { width:0; height:0; }
QStatusBar { background:#0b1220; color:#94a3b8; border-top:1px solid #263449; padding:4px 10px; }
QStatusBar::item { border:0; }
QGraphicsView#plantMap { background:#070d18; border:1px solid #334155; border-radius:6px; }
QLabel#panelTitle { color:#f8fafc; font-size:13px; font-weight:600; padding:3px 0; }
QLabel#panelHint { color:#94a3b8; font-size:11px; }
QWidget#mapCard { background:#111827; border:1px solid #263449; border-radius:8px; }
)CSS");
}

inline QWidget *modernCraneHeader(const QString& mode, bool preview = false) {
    QWidget *header = new QWidget;
    header->setObjectName("appHeader");
    header->setFixedHeight(64);
    QHBoxLayout *layout = new QHBoxLayout(header);
    layout->setContentsMargins(18, 7, 18, 7); layout->setSpacing(14);
    QLabel *brand = new QLabel("APEX"); brand->setObjectName("brandLogo"); brand->setFixedSize(116,42);
    const HRSRC resource = FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(201), RT_RCDATA);
    if (resource) {
        HGLOBAL data = LoadResource(GetModuleHandle(NULL), resource);
        QPixmap logo;
        if (data && logo.loadFromData(static_cast<const uchar*>(LockResource(data)), SizeofResource(GetModuleHandle(NULL), resource), "PNG"))
            brand->setPixmap(logo.scaled(112,40,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
    brand->setAlignment(Qt::AlignCenter); layout->addWidget(brand);
    QFrame *divider = new QFrame; divider->setFrameShape(QFrame::VLine); divider->setStyleSheet("color:#334155;"); layout->addWidget(divider);
    QVBoxLayout *titles = new QVBoxLayout; titles->setSpacing(2);
    QLabel *title = new QLabel("Basket Crane 2 v" BASKET_VERSION_STRING); title->setObjectName("applicationTitle"); titles->addWidget(title);
    QLabel *subtitle = new QLabel(preview ? "Offline UI preview - sample values" : "Material flow, missions and exports");
    subtitle->setObjectName("applicationSubtitle"); titles->addWidget(subtitle); layout->addLayout(titles);
    layout->addStretch();
    QLabel *host = new QLabel("HOST  " + QHostInfo::localHostName().toUpper()); host->setObjectName("hostIdentity"); layout->addWidget(host);
    QLabel *badge = new QLabel(mode == "Live" ? "LIVE SYSTEM" : (mode == "LiveObserver" ? "LIVE OBSERVER" : "TEST SYSTEM"));
    badge->setObjectName("systemMode"); badge->setProperty("mode", mode == "Live" ? "live" : (mode == "LiveObserver" ? "observer" : "test"));
    badge->setAlignment(Qt::AlignCenter); layout->addWidget(badge);
    badge->setMaximumHeight(28);
    badge->setToolTip(mode == "Live" ? "Live database writes and PLC control" : "Database writes blocked; live PLC outputs blocked");
    return header;
}

inline QGroupBox *modernCraneLegend(bool observer) {
    QGroupBox *legend = new QGroupBox("Basket legend");
    QVBoxLayout *layout = new QVBoxLayout(legend); layout->setSpacing(5);
    QLabel *destinations = new QLabel(observer ? "HCA / HCB   Destination" : "D / P   Destacker / Packing"); destinations->setObjectName("panelHint"); layout->addWidget(destinations);
    QLabel *locked = new QLabel("Red outline   Locked basket"); locked->setObjectName("panelHint"); layout->addWidget(locked);
    QLabel *selection = new QLabel("Pink / Blue   Source / Target"); selection->setObjectName("panelHint"); layout->addWidget(selection);
    return legend;
}

inline void modernizeCraneWorkspace(QMainWindow *window, QToolBar *toolbar, QSplitter *splitter, QWidget *map,
    QWidget *operations, const QString& mode, bool preview = false) {
    window->setStyleSheet(modernCraneTheme());
    window->setMinimumSize(1280,800);
    window->removeToolBar(toolbar);
    toolbar->setMovable(false); toolbar->setFloatable(false); toolbar->setIconSize(QSize(18,18));
    QWidget *workspace = new QWidget; workspace->setObjectName("workspace");
    QVBoxLayout *root = new QVBoxLayout(workspace); root->setMargin(0); root->setSpacing(0);
    root->addWidget(modernCraneHeader(mode, preview)); root->addWidget(toolbar);
    QWidget *body = new QWidget; QVBoxLayout *bodyLayout = new QVBoxLayout(body); bodyLayout->setContentsMargins(12,12,12,12);
    // Detach the existing splitter before replacing the central widget.
    if (window->centralWidget() == splitter) window->takeCentralWidget();
    window->setCentralWidget(workspace); root->addWidget(body,1); bodyLayout->addWidget(splitter);
    QWidget *mapCard = new QWidget; mapCard->setObjectName("mapCard");
    QVBoxLayout *mapLayout = new QVBoxLayout(mapCard); mapLayout->setContentsMargins(10,8,10,10); mapLayout->setSpacing(8);
    QHBoxLayout *mapTitle = new QHBoxLayout;
    QLabel *title = new QLabel("PLANT OVERVIEW"); title->setObjectName("panelTitle"); mapTitle->addWidget(title); mapTitle->addStretch();
    QLabel *hint = new QLabel("Drag to orbit | Wheel to zoom"); hint->setObjectName("panelHint"); mapTitle->addWidget(hint);
    mapLayout->addLayout(mapTitle); map->setObjectName("plantMap"); mapLayout->addWidget(map,1);
    if (QGraphicsView *view = qobject_cast<QGraphicsView *>(map)) {
        view->setBackgroundBrush(plantCanvasColor());
        view->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform);
    }
    splitter->insertWidget(1,mapCard); splitter->setChildrenCollapsible(false); splitter->setHandleWidth(10);
    operations->setMinimumWidth(240); operations->setObjectName("sidePanel");
    if (operations->layout()) operations->layout()->setContentsMargins(0,0,0,0);
    if (operations->layout()) operations->layout()->addWidget(modernCraneLegend(mode == "LiveObserver"));
    splitter->setStretchFactor(0,0); splitter->setStretchFactor(1,1); splitter->setStretchFactor(2,0);
    splitter->setSizes(QList<int>() << 270 << 1050 << 320);
    window->statusBar()->setSizeGripEnabled(false);
    toolbar->show();
}
#endif
