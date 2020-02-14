#ifndef APPITEM_H
#define APPITEM_H

#include "dockitem.h"
#include "controller/dockitemmanager.h"
#include "controller/appwindowmanager.h"

class AppItem : public DockItem
{
    Q_OBJECT

public:
    explicit AppItem(DockEntry *entry, QWidget *parent = nullptr);

    quint64 windowId() const { return m_id; };

    inline ItemType itemType() const override { return DockItem::App; }

    void closeWindow();

private:
    void refreshIcon();
    void updateWindowIconGeometries();

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    quint64 m_id;
    QPixmap m_iconPixmap;
    DockEntry *m_entry;
    QMenu m_contextMenu;
    QTimer *m_updateIconGeometryTimer;
};

#endif // APPITEM_H
