#include "obstacle.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QRandomGenerator>
#include <QStyleOption>
#include <QtMath>


Obstacle::Obstacle(qreal width, qreal height)
    :w(width), h(height)
{

}

QRectF Obstacle::boundingRect() const
{
    return QRectF(-w/2, -h/2, w, h);
}


QPainterPath Obstacle::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(obstacleOutlineColor);
    painter->drawRect(boundingRect());
    painter->setBrush(obstacleColor);
    painter->drawRect(QRectF(-w*0.4, -h*0.4, 0.8*w, 0.8*h));
}
