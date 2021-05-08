#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <vector>

#include <QObject>
#include <QPointF>
#include <qqml.h>

#include "constants.h"

class BallInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    QML_ELEMENT

    QPointF m_pos;
    //QPointF m_dst;
    qreal m_angle;
    QColor m_color;

public:
    BallInfo(QObject *parent = nullptr) : QObject(parent)
    {}

    bool operator==(BallInfo& other);
    qreal stepX, stepY;

    QPointF pos() {return m_pos;}
    qreal angle() {return m_angle;}
    QColor color() {return m_color;}

    void setPos(QPointF pos);
    void setAngle(qreal angle);
    void setColor(QColor color);

signals:
    void posChanged();
    void angleChanged();
    void colorChanged();

};

class GoalInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    QPointF m_pos;
    QColor m_color;

public:
    GoalInfo(QObject *parent = nullptr) : QObject(parent)
    {}

    QPointF pos() {return m_pos;}
    QColor color() {return m_color;}

    void setPos(QPointF pos);
    void setColor(QColor color);

signals:
    void posChanged();
    void colorChanged();
};

class Level : public QObject
{
public:
    Q_OBJECT
    Q_PROPERTY(QPair<int, int> gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)
    Q_PROPERTY(int levelId READ levelId WRITE setLevelId NOTIFY levelIdChanged)
    //Q_PROPERTY(QList<int> scores READ scores WRITE setScores NOTIFY scoredChanges)
    Q_PROPERTY(QList<bool> obstacles READ obstacles WRITE setObstacles NOTIFY obstaclesChanged)
    //Q_PROPERTY(QPair<int, int> source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QList<GoalInfo*> goals READ goals WRITE setGoals NOTIFY goalsChanged)
    Q_PROPERTY(QList<BallInfo*> balls READ balls WRITE setBalls NOTIFY ballsChanged)

private:
    QList<bool> m_obstacles;
    QList<BallInfo*> m_balls;
    QList<GoalInfo*> m_goals;
    QPair<int, int> m_gridSize;
    int m_levelId;
    QStringList levels;

public:

    Level();
    std::string toString();
    void fromString(std::string levelInfo);
    //void addScore(int score);

    QList<bool> obstacles() {return m_obstacles;}
    QList<BallInfo*> balls() {return m_balls;}
    QList<GoalInfo*> goals() { return m_goals;}
    QPair<int, int> gridSize() {return m_gridSize;}
    int levelId() {return m_levelId;}

    void setObstacles(QList<bool> obstacles);
    void setBalls(QList<BallInfo*> balls);
    void setGoals(QList<GoalInfo*> goals);
    void setGridSize(QPair<int, int> gridSize);
    void setLevelId(int levelId);

public slots:
    void updatePositions();
    void nextLevel();
    void prevLevel();

signals:
    void obstaclesChanged();
    void ballsChanged();
    void goalsChanged();
    void gridSizeChanged();
    void levelIdChanged();
};

#endif // LEVEL_H
