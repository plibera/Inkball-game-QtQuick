#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QColor>
#include <QtMath>

#define EPSILON 0.0001

#define SCENE_W 800
#define SCENE_H 800

#define BALL_D 30

#define GRID_W 20
#define GRID_H 20

#define FPS 60
#define SPEED 300

#define MAIN_MENU_DEFAULT_W 600
#define MAIN_MENU_DEFAULT_H 520
#define BEST_SCORES_DISPLAYED 10

const qreal OBS_W = SCENE_W/GRID_W;
const qreal OBS_H = SCENE_H/GRID_H;

const QColor backgroundColor(200, 200, 200);
const QColor obstacleColor(50, 50, 50);
const QColor obstacleOutlineColor(0, 0, 0);
const QColor segmentShadowColor(150, 150, 150);
const QColor segmentColor(70, 70, 70);

#endif // CONSTANTS_H
