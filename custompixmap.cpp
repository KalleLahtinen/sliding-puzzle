#include "custompixmap.hpp"
#include <QPainter>

CustomPixmap::CustomPixmap(const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
    animation = new QPropertyAnimation(this, "pos");
}

QRectF CustomPixmap::boundingRect() const
{
    return QRectF(0, 0, pixmap().width(), pixmap().height());
}

void CustomPixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(boundingRect(), pixmap(), boundingRect());
}
