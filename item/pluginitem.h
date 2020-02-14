#ifndef PLUGINITEM_H
#define PLUGINITEM_H

#include "dockitem.h"

class PluginItem : public DockItem
{
    Q_OBJECT

public:
    explicit PluginItem(QWidget *parent = nullptr);

    inline ItemType itemType() const override { return DockItem::Launcher; }
};

#endif // PLUGINITEM_H
