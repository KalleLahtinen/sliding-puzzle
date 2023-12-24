/**
 * Author: Kalle Lahtinen
 *
 * A custom implementation of the QGraphicsView class that detects
 * mouse clicks and sends a mouseClickDetected signal with clicked position
*/

#ifndef CLICKABLEVIEW_HPP
#define CLICKABLEVIEW_HPP

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

class ClickableView : public QGraphicsView
{
    Q_OBJECT

public:
    ClickableView(QGraphicsScene* scene);

signals:
    /**
     * @brief mouseClickDetected is a signal for indicating mouse clicks
     * @param clickPos is the clicked position as QPoint
     */
    void mouseClickDetected(const QPoint &clickPos);

protected:
    /**
     * @brief mousePressEvent detects mouse clicks on the object
     *        and emits mouseClickDetected signal
     * @param event is the click event
     */
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CLICKABLEVIEW_HPP
