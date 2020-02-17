#include "blurhelper.h"
#include <QWidget>
#include <KWindowEffects>
#include <QVariant>
#include <QEvent>
#include <QIcon>

#include <QDebug>

BlurHelper::BlurHelper(QObject *parent) : QObject(parent)
{

}

bool BlurHelper::eventFilter(QObject *obj, QEvent *e)
{
    //FIXME:
    //qDebug()<<e->type()<<obj;
    //qDebug()<<KWindowEffects::isEffectAvailable(KWindowEffects::BlurBehind);
    switch (e->type()) {
        case QEvent::Hide:
        case QEvent::UpdateRequest:
        case QEvent::Show:
        case QEvent::Resize:
        {
            // cast to widget and check
            QWidget* widget(qobject_cast<QWidget*>(obj));
            //KWindowEffects::enableBlurBehind(widget->winId(), false);

            if (!widget)
                break;

            QVariant regionValue = widget->property("blurRegion");
            QRegion region = qvariant_cast<QRegion>(regionValue);

            if (widget->inherits("QMenu")) {
                QPainterPath path;
                path.addRoundedRect(widget->rect().adjusted(1, 1, -1, -1), 10, 10);
                KWindowEffects::enableBlurBehind(widget->winId(), true, path.toFillPolygon().toPolygon());
                widget->update();
                break;
            }

            //qDebug()<<regionValue<<region;
            //qDebug()<<widget->metaObject()->className()<<widget->geometry()<<widget->mask();
            if (!region.isEmpty()) {
                //qDebug()<<"blur region"<<region;
                KWindowEffects::enableBlurBehind(widget->winId(), true, region);
                widget->update();
            } else {
                //qDebug()<<widget->mask();
                KWindowEffects::enableBlurBehind(widget->winId(), true, widget->mask());
                widget->update(widget->mask());
            }

            //NOTE: we can not setAttribute Qt::WA_TranslucentBackground here,
            //because the window is about to be shown.
            //widget->setAttribute(Qt::WA_TranslucentBackground);
            //KWindowEffects::enableBlurBehind(widget->winId(), true);
            //widget->update();
            break;
        }

        default: break;
    }
    return false;
}

/*!
 * \brief BlurHelper::registerWidget
 * \param widget
 * \bug
 * I want to use mask for blur a specific region, but
 * it seems that when window update, mask will be cleared.
 * That makes event filter can not handle the right region to blur.
 */
void BlurHelper::registerWidget(QWidget *widget)
{
    if (!m_blur_widgets.contains(widget)) {
        m_blur_widgets<<widget;
        //qDebug()<<KWindowEffects::isEffectAvailable(KWindowEffects::BlurBehind);
        if (!widget->mask().isEmpty()) {
            KWindowEffects::enableBlurBehind(widget->winId(), true, widget->mask());
        } else {
            KWindowEffects::enableBlurBehind(widget->winId(), true);
        }

        connect(widget, &QWidget::destroyed, this, [=](){
            this->onWidgetDestroyed(widget);
        });
    }
    widget->removeEventFilter(this);
    widget->installEventFilter(this);

    if (!widget->mask().isEmpty()) {
        widget->update(widget->mask());
    } else {
        widget->update();
    }
}

void BlurHelper::unregisterWidget(QWidget *widget)
{
    m_blur_widgets.removeOne(widget);
    widget->removeEventFilter(this);
    KWindowEffects::enableBlurBehind(widget->effectiveWinId(), false);
}

void BlurHelper::onBlurEnableChanged(bool enable)
{
    for (auto widget : m_blur_widgets) {
        KWindowEffects::enableBlurBehind(widget->effectiveWinId(), enable);
        if (widget->isVisible())
            widget->update();
    }
}

void BlurHelper::onWidgetDestroyed(QWidget *widget)
{
    unregisterWidget(widget);
}
