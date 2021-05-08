import QtQuick 2.15
import QtQuick.Window 2.15
import ballInfo 1.0
import goalInfo 1.0
import level 1.0

Rectangle {
    id: gameScene
    height: width
    color: "#a0a0a0"
    property Level level

    //Obstacles
    Grid {
        id: obstacles
        anchors.fill: parent
        rows: 20
        columns: 20
        spacing: 0

        Repeater {
            model: 400
            Rectangle {
                width: gameScene.width/20
                height: gameScene.height/20
                color: gameScene.level.obstacles[index] ? "#303030" : "transparent"
                border.color: "Black"
                border.width: gameScene.level.obstacles[index] ? 5 : 0
            }
        }
    }

    //Goals
    Repeater {
        id: goals
        model: gameScene.level.goals.length
        Rectangle {
            GoalInfo {
                id: goal
                pos.x: gameScene.level.goals[index].pos.x
                pos.y: gameScene.level.goals[index].pos.y
                color: gameScene.level.goals[index].color
            }
            width: gameScene.width/20
            height: width
            color: goal.color
            x: goal.pos.x * gameScene.width/20
            y: goal.pos.y * gameScene.height/20
            border.color: "black"

            Rectangle {
                width: radius*2
                height: radius*2
                color: "black"
                radius: parent.width*0.4
                x: parent.width*0.1
                y: parent.height*0.1
            }
        }
    }

    //Balls
    Repeater {
        id: balls
        model: gameScene.level.balls.length
        Rectangle {
            BallInfo {
                id: ball
                pos.x: gameScene.level.balls[index].pos.x
                pos.y: gameScene.level.balls[index].pos.y
                color: gameScene.level.balls[index].color
            }
            width: 2*radius
            height: 2*radius
            color: ball.color
            x: ball.pos.x * gameScene.width/20
            y: ball.pos.y * gameScene.height/20
            radius: gameScene.width/40
            border.color: "black"

            Behavior on x
            {
                NumberAnimation {
                    duration: 30
                }
            }
            Behavior on y
            {
                NumberAnimation {
                    duration: 30
                }
            }
        }
    }


    Timer
    {
        interval: 30;
        running: true;
        repeat: true
        onTriggered: gameScene.level.updatePositions()
    }
}
