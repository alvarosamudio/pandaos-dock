#include "appitem.h"
#include "utils/utils.h"
#include "utils/themeappicon.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

AppItem::AppItem(DockEntry *entry, QWidget *parent)
    : DockItem(parent),
      m_entry(entry)
{
    m_id = m_entry->windowID;

    QAction *dockAction = new QAction("Dock");
    QAction *closeAction = new QAction("Close All");
    m_contextMenu.addAction(dockAction);
    m_contextMenu.addAction(closeAction);

    refreshIcon();

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
    backgroundRect = backgroundRect.marginsRemoved(QMargins(2, 2, 2, 2));
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
