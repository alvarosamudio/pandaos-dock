#ifndef PLUGINITEM_H
#define PLUGINITEM_H

#include "dockitem.h"
#include "Interfaces/pluginsiteminterface.h"

class PluginItem : public DockItem
{
    Q_OBJECT

public:
    explicit PluginItem(QWidget *parent = nullptr);

    inline ItemType itemType() const override { return DockItem::Plugins; }

private:
    PluginsItemInterface *m_interface;
};

#endif // PLUGINITEM_H
