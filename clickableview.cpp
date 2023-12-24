#include "clickableview.hpp"

ClickableView::ClickableView(QGraphicsScene* scene)
    : QGraphicsView(scene)
{
}

void ClickableView::mousePressEvent(QMouseEvent *event)
{
    emit mouseClickDetected(event->pos());
}
