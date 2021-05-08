#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include <qqml.h>

#include "constants.h"

class Obstacle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
    QML_ELEMENT

public:
    Obstacle(qreal width, qreal height);
};

#endif // OBSTACLE_H
