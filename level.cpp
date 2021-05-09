#include "level.h"
#include <sstream>
#include <QFile>

using namespace std;



void BallInfo::setPos(QPointF pos)
{
    if(m_pos == pos)
        return;
    m_pos = pos;
    emit posChanged();
}

void BallInfo::setAngle(qreal angle)
{
    if(m_angle == angle)
        return;    
    while(angle < 0)
        angle += 2*M_PI;
    while(angle >= 2*M_PI)
        angle -= 2*M_PI;
    m_angle = angle;
    if(angle < M_PI/4)
    {
        stepX = 1;
        stepY = 0;
    }
    else if(angle >= M_PI/4 && angle < 3*M_PI/4)
    {
        stepX = 0;
        stepY = 1;
    }
    else if(angle >= 3*M_PI/4 && angle < 5*M_PI/4)
    {
        stepX = -1;
        stepY = 0;
    }
    else
    {
        stepX = 0;
        stepY = -1;
    }
    emit angleChanged();
}


void BallInfo::setColor(QColor color)
{
    if(m_color == color)
        return;
    m_color = color;
    emit colorChanged();
}

bool BallInfo::operator==(BallInfo& other)
{
    return pos() == other.pos() && angle() == other.angle() && color() == other.color();
}


void GoalInfo::setPos(QPointF pos)
{
    if(pos == m_pos)
        return;
    m_pos = pos;
    emit posChanged();
}

void GoalInfo::setColor(QColor color)
{
    if(color == m_color)
        return;
    m_color = color;
    emit colorChanged();
}


void Segment::setP1(QPointF p1)
{
    if(p1 == m_p1)
        return;
    m_p1 = p1;
    QLineF line(m_p1, m_p2);
    m_angle = line.angle();
    m_length = line.length();
    emit p1Changed();
    emit angleChanged();
    emit lengthChanged();
}

void Segment::setP2(QPointF p2)
{
    if(p2 == m_p2)
        return;
    m_p2 = p2;
    QLineF line(m_p1, m_p2);
    m_angle = line.angle();
    m_length = line.length();
    emit p2Changed();
    emit angleChanged();
    emit lengthChanged();
}

bool Segment::operator==(Segment& other)
{
    QLineF line1(m_p1, m_p2);
    QLineF line2(other.p1(), other.p2());
    return line1 == line2;
}







Level::Level()
{
    QString data;
    QString fileName(":/levels.txt");
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"File not opened"<<Qt::endl;
    }
    else
    {
        qDebug()<<"File opened"<<Qt::endl;
        data = file.readAll();
    }
    file.close();
    levels = data.split('\n');
    if(levels[levels.size()-1].length() < 10)
        levels.pop_back();
    fromString(levels[0].toStdString());
    m_levelId = 0;
    m_activeSegment = new Segment(this);
}


void Level::setObstacles(QList<bool> obstacles)
{
    if(m_obstacles == obstacles)
        return;
    m_obstacles = obstacles;
    emit obstaclesChanged();
}

void Level::setBalls(QList<BallInfo*> balls)
{
    if(m_balls == balls)
        return;
    m_balls = balls;
    emit ballsChanged();
}

void Level::setGoals(QList<GoalInfo*> goals)
{
    if(m_goals == goals)
        return;
    m_goals = goals;
    emit goalsChanged();
}

void Level::setSegments(QList<Segment*> segments)
{
    if(m_segments == segments)
        return;
    m_segments = segments;
    emit segmentsChanged();
}

void Level::setLevelId(int levelId)
{
    if(levelId == m_levelId)
        return;
    if(levelId >= levels.size())
        levelId = 0;
    if(levelId < 0)
        levelId = levels.size()-1;
    m_levelId = levelId;
    fromString(levels[levelId].toStdString());
    emit levelIdChanged();
}

void Level::setGridSize(QPair<int, int> gridSize)
{
    if(m_gridSize == gridSize)
        return;
    m_gridSize = gridSize;
    emit gridSizeChanged();
}

void Level::setActiveSegment(Segment* activeSegment)
{
    if(m_activeSegment == activeSegment)
        return;
    m_activeSegment = activeSegment;
    emit activeSegmentChanged();
}

void Level::setSegmentStarted(bool segmentStarted)
{
    if(m_segmentStarted == segmentStarted)
        return;
    m_segmentStarted = segmentStarted;
    emit segmentStartedChanged();
}

void Level::setGameState(int gameState)
{
    if(gameState == m_gameState)
        return;
    m_gameState = gameState;
    emit gameStateChanged();
}

void Level::nextLevel()
{
    setLevelId(m_levelId+1);
}

void Level::prevLevel()
{
    setLevelId(m_levelId-1);
}

void Level::updateActiveSegment(QPointF point)
{
    if(!m_segmentStarted)
        return;

    QLineF line(activeSegment()->p1(), point);
    qreal angle = line.angle();
    angle = 45*round(angle/45);
    if(angle >= 360)
        angle -= 360;
    angle *= M_PI/180;
    qreal length = line.length();
    QPointF point2(length*cos(angle), -1*length*sin(angle));
    line.setP2(activeSegment()->p1() + point2);
    m_activeSegment->setP2(line.p2());
    emit activeSegmentChanged();
}

void Level::startActiveSegment(QPointF point)
{
    m_activeSegment->setP1(point);
    m_activeSegment->setP2(point);
    setSegmentStarted(true);
    emit activeSegmentChanged();
}

void Level::finishActiveSegment()
{
    setSegmentStarted(false);
    Segment* segment = new Segment(this);
    segment->setP1(m_activeSegment->p1());
    segment->setP2(m_activeSegment->p2());
    m_segments.push_back(segment);
    emit segmentsChanged();
}


void Level::updatePositions()
{
    qreal dist = 0.2;
    QList<BallInfo*> ballsToRemove;

    for(auto &ball : m_balls)
    {
        QPointF futurePos(ball->pos().x()+dist*ball->stepX, ball->pos().y()+dist*ball->stepY);

        int x = int(ball->pos().x());
        int y = int(ball->pos().y());
        if(m_obstacles[gridSize().first*y + x] || m_obstacles[gridSize().first*(y+1) + x] ||
                m_obstacles[gridSize().first*y + x+1] || m_obstacles[gridSize().first*(y+1) + x+1])
            ball->setAngle(ball->angle() + M_PI);

        futurePos.setX(ball->pos().x()+dist*ball->stepX);
        futurePos.setY(ball->pos().y()+dist*ball->stepY);
        ball->setPos(futurePos);

        for(Segment* segment : m_segments)
        {
            QPointF segmentOffset(-0.5, -0.5);
            QLineF testSegment(segment->p1()+segmentOffset, segment->p2()+segmentOffset);
            int direction = int(round(segment->angle()/45))%4;
            if(direction%2 == 0)
            {
                QLineF testLine(QPointF(futurePos.x(), futurePos.y()-(double)BALL_D/2), QPointF(futurePos.x(), futurePos.y()+(double)BALL_D/2));
                QLineF testLine2(QPointF(futurePos.x()-(double)BALL_D/2, futurePos.y()), QPointF(futurePos.x()+(double)BALL_D/2, futurePos.y()));
                QLineF::IntersectionType type = testLine.intersects(testSegment, nullptr);
                QLineF::IntersectionType type2 = testLine2.intersects(testSegment, nullptr);
                if(type == QLineF::BoundedIntersection || type2 == QLineF::BoundedIntersection)
                {
                    ball->setAngle(ball->angle()+M_PI);
                    m_segments.removeOne(segment);
                    delete segment;
                    emit segmentsChanged();
                    break;
                }
            }
            else
            {
                qreal offset = (double)BALL_D/(2*sqrt(2));
                QLineF testLine(QPointF(futurePos.x()-offset, futurePos.y()-offset), QPointF(futurePos.x()+offset, futurePos.y()+offset));
                QLineF testLine2(QPointF(futurePos.x()-offset, futurePos.y()+offset), QPointF(futurePos.x()+offset, futurePos.y()-offset));
                QLineF::IntersectionType type = testLine.intersects(testSegment, nullptr);
                QLineF::IntersectionType type2 = testLine2.intersects(testSegment, nullptr);
                if(type == QLineF::BoundedIntersection || type2 == QLineF::BoundedIntersection)
                {
                    if((direction == 1 && ball->stepY == 0) || (direction == 3 && ball->stepX == 0))
                        ball->setAngle(ball->angle()-M_PI/2);
                    else
                        ball->setAngle(ball->angle()+M_PI/2);
                    m_segments.removeOne(segment);
                    delete segment;
                    emit segmentsChanged();
                    break;
                }
            }
        }

        for(GoalInfo* goal : m_goals)
        {
            QLineF testLine(goal->pos(), ball->pos());
            if(testLine.length() < (double)BALL_D/2)
            {
                if(goal->color() != ball->color())
                {
                    setGameState(3);
                    ballsToRemove = m_balls;
                    break;
                }
                else if(m_balls.size() == 1)
                {
                    setGameState(2);
                    ballsToRemove = m_balls;
                    break;
                }
                ballsToRemove.push_back(ball);
            }
        }
    }
    for(auto &ball : ballsToRemove)
    {
        m_balls.removeOne(ball);
        delete ball;
    }
    emit ballsChanged();
}


void Level::fromString(std::string levelInfo)
{
    m_obstacles.clear();
    m_goals.clear();
    m_balls.clear();
    m_segments.clear();
    //scores.clear();

    stringstream ss;
    ss.str(levelInfo);
    int n, temp;
    ss>>m_gridSize.first>>m_gridSize.second;
    m_gridSize.first += 2;
    m_gridSize.second += 2;
    char c;
    for(int i = 0; i < m_gridSize.first*m_gridSize.second; ++i)
        m_obstacles.push_back(false);
    for(int i = 0; i < m_gridSize.first; ++i)
    {
        if(i == 0 || i == m_gridSize.first-1)
        {
            for(int j = 0; j < m_gridSize.second; ++j)
                m_obstacles[i*m_gridSize.first+j] = true;
        }
        else
        {
            for(int j = 0; j < m_gridSize.second; ++j)
            {
                if(j == 0 || j == m_gridSize.second-1)
                {
                    m_obstacles[i*m_gridSize.first+j] = true;
                }
                else
                {
                    ss>>c;
                    if(c == '1')
                        m_obstacles[i*m_gridSize.first+j] = true;
                }
            }
        }
    }
    /*for(int i = 0; i <= m_gridSize.first; ++i)
    {
        m_obstacles.push_back(QPair<int, int>(i, 0));
        m_obstacles.push_back(QPair<int, int>(i, m_gridSize.second));
    }
    for(int i = 1; i < m_gridSize.second; ++i)
    {
        m_obstacles.push_back(QPair<int, int>(0, i));
        m_obstacles.push_back(QPair<int, int>(m_gridSize.first, i));
    }*/
    for(int i = 0; i < m_obstacles.size(); ++i)
    {
        if(i%m_gridSize.first == 0)
            cout<<endl;
        if(m_obstacles[i])
            cout<<"1";
       else
            cout<<"0";
    }
    cout<<endl;

    //ss>>source.first>>source.second;
    ss>>temp;
    ss>>temp;
    ss>>n;
    for(int i = 0; i < n; ++i)
    {
        GoalInfo* goal = new GoalInfo(this);
        int x, y;
        ss>>x>>y;
        goal->setPos(QPointF(x+1, y+1));
        int r, g, b;
        ss>>r>>g>>b;
        goal->setColor(QColor(r, g, b));
        m_goals.push_back(goal);
    }
    ss>>n;
    for(int i = 0; i < n; ++i)
    {
        BallInfo* ball = new BallInfo(this);
        int x, y;
        qreal angle;
        ss>>x>>y>>angle>>temp;
        ball->setPos(QPointF(x+1, y+1));
        ball->setAngle(angle);
        int r, g, b;
        ss>>r>>g>>b;
        ball->setColor(QColor(r, g, b));
        m_balls.push_back(ball);
    }
    ss>>n;
    //int score;
    for(int i = 0; i < n; ++i)
    {
        //ss>>score;
        ss>>temp;
        //scores.push_back(score);
    }

    emit obstaclesChanged();
    emit ballsChanged();
    emit goalsChanged();
    emit segmentsChanged();
    setGameState(0);
    //sort(scores.begin(), scores.end());
}

/*void Level::addScore(int score)
{
    scores.push_back(score);
    sort(scores.begin(), scores.end());
}
*/
