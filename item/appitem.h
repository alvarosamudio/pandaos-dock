#ifndef APPITEM_H
#define APPITEM_H

#include "dockitem.h"
#include "controller/dockitemmanager.h"
#include "controller/appwindowmanager.h"

class AppItem : public DockItem
{
    Q_OBJECT

public:
    explicit AppItem(const DockEntry &entry, QWidget *parent = nullptr);

    quint64 windowId() const { return m_id; };
    bool isActive() const { return m_isActive; };
    void setActive(bool active);

    inline ItemType itemType() const override { return DockItem::App; }

private:
    void refreshIcon();

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    quint64 m_id;
    bool m_isActive;
    QPixmap m_iconPixmap;
    DockEntry m_entry;
    QMenu m_contextMenu;
};

#endif // APPITEM_H
