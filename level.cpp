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

void Level::nextLevel()
{
    setLevelId(m_levelId+1);
}

void Level::prevLevel()
{
    setLevelId(m_levelId-1);
}

void Level::updatePositions()
{
    qreal dist = 0.2;

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
    }






    /*for(Segment* segment : inkBallScene->segments)
    {
        if(segment->getColor() != segmentColor)
            continue;
        int direction = int(round(segment->getSegment().angle()/45))%4;
        if(direction%2 == 0)
        {
            QLineF testLine(QPointF(futurePos.x(), futurePos.y()-BALL_D/2), QPointF(futurePos.x(), futurePos.y()+BALL_D/2));
            QLineF testLine2(QPointF(futurePos.x()-BALL_D/2, futurePos.y()), QPointF(futurePos.x()+BALL_D/2, futurePos.y()));
            QLineF::IntersectionType type = testLine.intersects(segment->getSegment(), nullptr);
            QLineF::IntersectionType type2 = testLine2.intersects(segment->getSegment(), nullptr);
            if(type == QLineF::BoundedIntersection || type2 == QLineF::BoundedIntersection)
            {
                setAngle(angle+M_PI);
                inkBallScene->removeItem(segment);
                inkBallScene->segments.removeOne(segment);
                delete segment;
                inkBallScene->update();
                break;
            }
        }
        else
        {
            qreal offset = BALL_D/(2*sqrt(2));
            QLineF testLine(QPointF(futurePos.x()-offset, futurePos.y()-offset), QPointF(futurePos.x()+offset, futurePos.y()+offset));
            QLineF testLine2(QPointF(futurePos.x()-offset, futurePos.y()+offset), QPointF(futurePos.x()+offset, futurePos.y()-offset));
            QLineF::IntersectionType type = testLine.intersects(segment->getSegment(), nullptr);
            QLineF::IntersectionType type2 = testLine2.intersects(segment->getSegment(), nullptr);
            if(type == QLineF::BoundedIntersection || type2 == QLineF::BoundedIntersection)
            {
                if((direction == 1 && stepY == 0) || (direction == 3 && stepX == 0))
                    setAngle(angle-M_PI/2);
                else
                    setAngle(angle+M_PI/2);
                inkBallScene->removeItem(segment);
                inkBallScene->segments.removeOne(segment);
                delete segment;
                inkBallScene->update();
                break;
            }
        }
    }*/



    /*for(Goal* goal : inkBallScene->goals)
    {
        QLineF testLine(goal->pos(), pos());
        if(testLine.length() < BALL_D)
        {
            if(goal->getColor() != color)
            {
                emit inkBallScene->gameOver();
                return;
            }
            else if(inkBallScene->balls.size() == 1 && inkBallScene->waitingBalls.size() == 0)
            {
                emit inkBallScene->gameWon(inkBallScene->getGameTime()/1000);
                return;
            }
            inkBallScene->removeItem(this);
            inkBallScene->balls.removeOne(this);
            inkBallScene->update();
            delete this;
        }
    }*/
}


void Level::fromString(std::string levelInfo)
{
    m_obstacles.clear();
    m_goals.clear();
    m_balls.clear();
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
    //sort(scores.begin(), scores.end());
}

/*void Level::addScore(int score)
{
    scores.push_back(score);
    sort(scores.begin(), scores.end());
}
*/
