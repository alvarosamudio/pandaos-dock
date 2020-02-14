#include "mainwindow.h"
#include "utils/utils.h"
#include <QApplication>
#include <QScreen>

#define MAINWINDOW_MAX_SIZE       100
#define MAINWINDOW_MIN_SIZE       (40)
#define DRAG_AREA_SIZE (5)

const QPoint rawXPosition(const QPoint &scaledPos)
{
    QScreen const *screen = Utils::screenAtByScaled(scaledPos);

    return screen ? screen->geometry().topLeft() +
           (scaledPos - screen->geometry().topLeft()) *
           screen->devicePixelRatio()
           : scaledPos;
}

MainWindow::MainWindow(QWidget *parent)
    : QFrame(parent),
      m_mainPanel(new MainPanel(this)),
      m_dragWidget(new DragWidget(this)),
      m_itemManager(DockItemManager::instance()),
      m_settings(DockSettings::instance()),
      m_xcbMisc(XcbMisc::instance()),
      m_desktopWidget(QApplication::desktop())
{
    m_dragWidget->setMouseTracking(true);
    m_dragWidget->setFocusPolicy(Qt::NoFocus);

    m_dragWidget->setCursor(Qt::SizeVerCursor);
    // m_dragWidget->setCursor(Qt::SizeHorCursor);

    m_xcbMisc->set_window_type(winId(), XcbMisc::Dock);

    for (auto item : m_itemManager->itemList())
        m_mainPanel->insertItem(-1, item);

    QWidget::show();

    initSize();
    resizeMainPanelWindow();

    connect(m_dragWidget, &DragWidget::dragPointOffset, this, &MainWindow::onMainWindowSizeChanged);
    connect(m_dragWidget, &DragWidget::dragFinished, this, &MainWindow::onDragFinished);
}

MainWindow::~MainWindow()
{

}

void MainWindow::initSize()
{
    QRect windowRect = m_settings->windowRect();

    // set size.
    QWidget::move(windowRect.left(), windowRect.top());
    QWidget::setFixedSize(windowRect.size());

    m_size = windowRect.size();

    m_mainPanel->move(0, 0);
    m_mainPanel->setFixedSize(QWidget::size());

    setStrutPartial();
}

void MainWindow::setStrutPartial()
{
    const auto ratio = devicePixelRatioF();
    const QRect windowRect = m_settings->windowRect();
    m_xcbMisc->clear_strut_partial(winId());
    m_xcbMisc->set_strut_partial(winId(), XcbMisc::OrientationBottom, windowRect.height() * ratio, windowRect.top(), windowRect.bottom());
}

void MainWindow::resizeMainPanelWindow()
{
    m_dragWidget->setGeometry(0, 0, width(), DRAG_AREA_SIZE);
}

void MainWindow::onMainWindowSizeChanged(QPoint offset)
{
    setFixedHeight(qBound(MAINWINDOW_MIN_SIZE, m_size.height() - offset.y(), MAINWINDOW_MAX_SIZE));
    setFixedWidth(width());

    m_settings->setWindowSize(QWidget::size());
    QRect windowRect = m_settings->windowRect();
    QWidget::move(windowRect.left(), windowRect.top());

    m_mainPanel->setFixedSize(QWidget::size());

    // bottom
    m_dragWidget->setGeometry(0, 0, width(), DRAG_AREA_SIZE);

    resizeMainPanelWindow();
}

void MainWindow::onDragFinished()
{
    m_size = m_settings->windowSize();

    setStrutPartial();
}
