#include "hoverhighlighteffect.h"
#include "utils/imagefactory.h"

#include <QPainter>
#include <QDebug>

HoverHighlightEffect::HoverHighlightEffect(QObject *parent)
    : QGraphicsEffect(parent),
      m_highlighting(false)
{

}

void HoverHighlightEffect::draw(QPainter *painter)
{
    const QPixmap pix = sourcePixmap(Qt::DeviceCoordinates);

    if (m_highlighting) {
        painter->drawPixmap(0, 0, ImageFactory::lighterEffect(pix));
    } else {
        painter->drawPixmap(0, 0, pix);
    }
}
