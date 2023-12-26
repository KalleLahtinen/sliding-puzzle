/**
 * A custom QGraphicsPixmapItem class that enables animating the position of the item
*/

#ifndef CUSTOMPIXMAP_H
#define CUSTOMPIXMAP_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class CustomPixmap : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    explicit CustomPixmap(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget) override;

private:
    QPropertyAnimation *animation;
};

#endif // CUSTOMPIXMAP_H
