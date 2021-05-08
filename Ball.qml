import QtQuick 2.15
import QtQuick.Window 2.15
import ballInfo 1.0
import level 1.0

Rectangle {
    Level {
        id: level
    }

    BallInfo {
        id: ball
        pos.x: level.balls[ballId].pos.x
        pos.y: level.balls[ballId].pos.y
        color: level.balls[ballId].color
    }
    property int ballId: 0
    width: 2*radius
    height: 2*radius
    color: ball.color
}
