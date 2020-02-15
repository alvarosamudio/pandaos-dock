#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include "dragwidget.h"
#include "mainpanel.h"
#include "controller/dockitemmanager.h"
#include "controller/docksettings.h"
#include "xcb/xcbmisc.h"

class MainWindow : public QFrame
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initSize();

private:
    void setStrutPartial();
    void resizeMainPanelWindow();
    void onMainWindowSizeChanged(QPoint offset);
    void onDragFinished();

private:
    MainPanel *m_mainPanel;
    DragWidget *m_dragWidget;
    DockItemManager *m_itemManager;
    DockSettings *m_settings;
    XcbMisc *m_xcbMisc;
    QSize m_size;
};

#endif // MAINWINDOW_H
