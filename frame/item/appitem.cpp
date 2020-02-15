#include "appitem.h"
#include "utils/utils.h"
#include "utils/themeappicon.h"
#include "xcb/xcbmisc.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QToolTip>

AppItem::AppItem(DockEntry *entry, QWidget *parent)
    : DockItem(parent),
      m_entry(entry),
      m_updateIconGeometryTimer(new QTimer(this))
{
    m_updateIconGeometryTimer->setInterval(500);
    m_updateIconGeometryTimer->setSingleShot(true);

    m_id = m_entry->windowID;

    QAction *dockAction = new QAction("Dock");
    QAction *closeAction = new QAction("Close All");
    m_contextMenu.addAction(dockAction);
    m_contextMenu.addAction(closeAction);

    refreshIcon();

    connect(m_updateIconGeometryTimer, &QTimer::timeout, this, &AppItem::updateWindowIconGeometries, Qt::QueuedConnection);
    connect(closeAction, &QAction::triggered, this, &AppItem::closeWindow);
}

void AppItem::closeWindow()
{
    AppWindowManager::instance()->closeWindow(m_id);
}

void AppItem::refreshIcon()
{
    const int iconSize = qMin(width(), height());
    const QString iconName = m_entry->icon;

    m_iconPixmap = ThemeAppIcon::getIcon(iconName, m_id, iconSize * 0.8, devicePixelRatioF());

    QWidget::update();

    m_updateIconGeometryTimer->start();
}

void AppItem::updateWindowIconGeometries()
{
    const QRect r(mapToGlobal(QPoint(0, 0)),
                  mapToGlobal(QPoint(width(), height())));
    auto *xcb_misc = XcbMisc::instance();

//    for (auto it(m_windowInfos.cbegin()); it != m_windowInfos.cend(); ++it)
//        xcb_misc->set_window_icon_geometry(it.key(), r);
    xcb_misc->set_window_icon_geometry(m_id, r);
}

void AppItem::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // draw background
    const QRectF itemRect = rect();
    qreal min = qMin(itemRect.width(), itemRect.height());
    QRectF backgroundRect = QRectF(itemRect.x(), itemRect.y(), min, min);
    backgroundRect = backgroundRect.marginsRemoved(QMargins(3, 3, 3, 3));
    backgroundRect.moveCenter(itemRect.center());
    QPainterPath path;
    path.addRoundedRect(backgroundRect, 8, 8);
    if (m_entry->isActive) {
        painter.fillPath(path, QColor(0, 0, 0, 255 * 0.8));
    } else {
        painter.fillPath(path, QColor(0, 0, 0, 255 * 0.3));
    }

    const auto ratio = devicePixelRatioF();
    const int iconX = rect().center().x() - m_iconPixmap.rect().center().x() / ratio;
    const int iconY = rect().center().y() - m_iconPixmap.rect().center().y() / ratio;

    painter.drawPixmap(iconX, iconY, m_iconPixmap);
}

void AppItem::mousePressEvent(QMouseEvent *e)
{
    m_updateIconGeometryTimer->stop();

    if (e->button() == Qt::RightButton) {
        m_contextMenu.popup(QCursor::pos());
    }

    DockItem::mousePressEvent(e);
}

void AppItem::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        AppWindowManager::instance()->triggerWindow(m_id);
    }
}

void AppItem::resizeEvent(QResizeEvent *e)
{
    DockItem::resizeEvent(e);

    refreshIcon();
}