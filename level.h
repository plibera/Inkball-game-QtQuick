#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <vector>

#include <QObject>
#include <QPointF>
#include <qqml.h>
#include <QLineF>

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


class Segment : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF p1 READ p1 WRITE setP1 NOTIFY p1Changed)
    Q_PROPERTY(QPointF p2 READ p2 WRITE setP2 NOTIFY p2Changed)
    Q_PROPERTY(qreal angle READ angle NOTIFY angleChanged)
    Q_PROPERTY(qreal length READ length NOTIFY lengthChanged)

    QPointF m_p1, m_p2;
    qreal m_angle, m_length;

public:
    Segment(QObject *parent = nullptr) : QObject(parent) {}

    bool operator==(Segment& other);

    QPointF p1() {return m_p1;}
    QPointF p2() {return m_p2;}
    qreal angle() {return m_angle;}
    qreal length() {return m_length;}

    void setP1(QPointF p1);
    void setP2(QPointF p2);

signals:
    void p1Changed();
    void p2Changed();
    void angleChanged();
    void lengthChanged();

};

class Level : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPair<int, int> gridSize READ gridSize WRITE setGridSize NOTIFY gridSizeChanged)
    Q_PROPERTY(int levelId READ levelId WRITE setLevelId NOTIFY levelIdChanged)
    //Q_PROPERTY(QList<int> scores READ scores WRITE setScores NOTIFY scoredChanges)
    Q_PROPERTY(QList<bool> obstacles READ obstacles WRITE setObstacles NOTIFY obstaclesChanged)
    //Q_PROPERTY(QPair<int, int> source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QList<GoalInfo*> goals READ goals WRITE setGoals NOTIFY goalsChanged)
    Q_PROPERTY(QList<BallInfo*> balls READ balls WRITE setBalls NOTIFY ballsChanged)
    Q_PROPERTY(Segment* activeSegment READ activeSegment WRITE setActiveSegment NOTIFY activeSegmentChanged)
    Q_PROPERTY(bool segmentStarted READ segmentStarted WRITE setSegmentStarted NOTIFY segmentStartedChanged)
    Q_PROPERTY(QList<Segment*> segments READ segments WRITE setSegments NOTIFY segmentsChanged)
    //0 - Idle; 1 - Playing; 2 - Game won; 3 - Game over
    Q_PROPERTY(int gameState READ gameState WRITE setGameState NOTIFY gameStateChanged)

private:
    QList<bool> m_obstacles;
    QList<BallInfo*> m_balls;
    QList<GoalInfo*> m_goals;
    QList<Segment*> m_segments;
    QPair<int, int> m_gridSize;
    int m_levelId;
    Segment* m_activeSegment;
    bool m_segmentStarted;
    QStringList levels;
    int m_gameState;

public:

    Level();
    std::string toString();
    void fromString(std::string levelInfo);
    //void addScore(int score);

    QList<bool> obstacles() {return m_obstacles;}
    QList<BallInfo*> balls() {return m_balls;}
    QList<GoalInfo*> goals() {return m_goals;}
    QList<Segment*> segments() { return m_segments;}
    QPair<int, int> gridSize() {return m_gridSize;}
    Segment* activeSegment() {return m_activeSegment;}
    bool segmentStarted() { return m_segmentStarted;}
    int levelId() {return m_levelId;}
    int gameState() { return m_gameState;}

public slots:
    void setObstacles(QList<bool> obstacles);
    void setBalls(QList<BallInfo*> balls);
    void setGoals(QList<GoalInfo*> goals);
    void setSegments(QList<Segment*> segments);
    void setGridSize(QPair<int, int> gridSize);
    void setActiveSegment(Segment* activeSegment);
    void setSegmentStarted(bool segmentStarted);
    void setLevelId(int levelId);
    void setGameState(int gameState);
    void updatePositions();
    void nextLevel();
    void prevLevel();
    void startActiveSegment(QPointF point);
    void updateActiveSegment(QPointF point);
    void finishActiveSegment();

signals:
    void obstaclesChanged();
    void ballsChanged();
    void goalsChanged();
    void segmentsChanged();
    void gridSizeChanged();
    void levelIdChanged();
    void activeSegmentChanged();
    void segmentStartedChanged();
    void gameStateChanged();
};

#endif // LEVEL_H
