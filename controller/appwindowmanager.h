#ifndef APPWINDOWMANAGER_H
#define APPWINDOWMANAGER_H

#include <QObject>
#include <QSettings>
#include <QDataStream>
#include <KF5/KWindowSystem/KWindowSystem>
#include <KF5/KWindowSystem/KWindowInfo>
#include <KF5/KWindowSystem/NETWM>

//typedef QList<DockEntry> DockItemList;

//Q_DECLARE_METATYPE(DockEntry)
//Q_DECLARE_METATYPE(DockEntryList)

struct DockEntry
{
    bool isActive = false;
    bool isDocked = false;
    QString desktopFile;
    QString icon;
    QString name;
    QString id;
    quint64 windowID;
    quint32 currentWindow = 0;

    bool operator==(DockEntry &a) {
        return id == a.id;
    }

    friend QDataStream &operator<<(QDataStream &argument, const DockEntry &entry) {
        argument << entry.isActive << entry.isDocked << entry.desktopFile
                 << entry.icon << entry.name << entry.id << entry.windowID
                 << entry.currentWindow;

        return argument;
    }

    friend QDataStream &operator>>(QDataStream &argument, DockEntry &entry) {
        argument >> entry.isActive >> entry.isDocked >> entry.desktopFile
                 >> entry.icon >> entry.name >> entry.id >> entry.windowID
                 >> entry.currentWindow;

        return argument;
    }
};
Q_DECLARE_METATYPE(DockEntry)

class AppWindowManager : public QObject
{
    Q_OBJECT

public:
    static AppWindowManager *instance();
    explicit AppWindowManager(QObject *parent = nullptr);

    QList<DockEntry> dockList() const { return m_dockList; }
    bool contains(quint64 id) const;
    bool isAcceptWindow(quint64 id) const;

    void triggerWindow(quint64 id);
    void minimizeWindow(quint64 id);
    void raiseWindow(quint64 id);

signals:
    void entryAdded(const DockEntry &entry);
    void entryRemoved(const DockEntry &entry);
    void activeChanged(const DockEntry &entry);

private:
    void initDockList();
    void refreshWindowList();
    void onWindowAdded(quint64 id);
    void onWindowRemoved(quint64 id);
    void onActiveWindowChanged(quint64 id);
    void onWindowChanged(WId id, NET::Properties properties, NET::Properties2 properties2);
    void saveDockList();

private:
    static AppWindowManager *INSTANCE;
    QSettings *m_settings;
    QList<DockEntry> m_dockList;
    QList<DockEntry> m_dockedList;
};

#endif // APPWINDOWMANAGER_H
