#include "dockitemmanager.h"
#include "item/launcheritem.h"
#include <QDebug>

DockItemManager *DockItemManager::INSTANCE = nullptr;

DockItemManager *DockItemManager::instance(QObject *parent)
{
    if (!INSTANCE)
        INSTANCE = new DockItemManager(parent);

    return INSTANCE;
}

DockItemManager::DockItemManager(QObject *parent)
    : QObject(parent),
      m_windowManager(AppWindowManager::instance())
{
    m_itemList.append(new LauncherItem);

    for (DockEntry *entry : m_windowManager->dockList()) {
        m_itemList.append(new AppItem(entry));
    }

    connect(m_windowManager, &AppWindowManager::entryAdded, this, &DockItemManager::appItemAdded);
    connect(m_windowManager, &AppWindowManager::entryRemoved, this, &DockItemManager::appItemRemoved);
    connect(m_windowManager, &AppWindowManager::activeChanged, this, &DockItemManager::appItemActiveChanged);
}

const QList<QPointer<DockItem> > DockItemManager::itemList() const
{
    return m_itemList;
}

void DockItemManager::appItemAdded(DockEntry *entry)
{
    // 第一个是启动器
    int insertIndex = 1;
    // -1 表示插入最后
    int index = -1;

    // -1 for append to app list end
    if (index != -1) {
        insertIndex += index;
    } else {
        for (auto item : m_itemList)
            if (item->itemType() == DockItem::App)
                ++insertIndex;
    }

    AppItem *item = new AppItem(entry);
    m_itemList.insert(insertIndex, item);

    if (index != -1) {
        emit itemInserted(insertIndex - 1, item);
        return;
    }

    emit itemInserted(insertIndex, item);
}

void DockItemManager::appItemRemoved(DockEntry *entry)
{
    for (int i = 0; i < m_itemList.size(); ++i) {
        if (m_itemList.at(i)->itemType() != DockItem::App)
            continue;

        AppItem *app = static_cast<AppItem *>(m_itemList.at(i).data());
        if (!app)
            continue;

        if (app->windowId() == entry->windowID) {
            emit itemRemoved(app);
            m_itemList.removeOne(app);
            app->deleteLater();
        }
    }
}

void DockItemManager::appItemActiveChanged(DockEntry *entry)
{
    for (DockItem *item : m_itemList) {
        if (item->itemType() != DockItem::App)
            continue;

        AppItem *app = static_cast<AppItem *>(item);
        if (!app)
            continue;

        app->update();
    }
}
