import QtQuick 2.15
import QtQuick.Window 2.15
import ballInfo 1.0
import goalInfo 1.0
import level 1.0
import QtQuick.Controls 2.12


Window {
    id: gameWindow
    width: 640
    height: 680
    visible: true
    title: qsTr("InkBall")

    Level {
        id: currentLevel
    }

    Column {
        anchors.fill: parent
        spacing: 5
        GameScene {
            id: gameScene
            level: currentLevel
            width: gameWindow.width < gameWindow.height-(buttonRow.height+2*buttonRow.spacing) ? gameWindow.width : gameWindow.height-(buttonRow.height+2*buttonRow.spacing)
            anchors.horizontalCenter: parent.horizontalCenter
            xOffset: (gameWindow.width-gameScene.width)/2
            yOffset: 0

            GameInfo {
                color: active ? "red" : "transparent"
                active: currentLevel.gameState == 3
                msg: "Game over!"
            }

            GameInfo {
                color: active ? "#40ff40" : "transparent"
                active: currentLevel.gameState == 2
                msg: "Game won in "+currentLevel.gameTime+" seconds!"
            }

            GameInfo {
                color: active ? "#707070" : "transparent"
                opacity: 0.8
                active: currentLevel.gameState == 0
                msg: "Level "+(currentLevel.levelId+1)
            }
        }

        ButtonGroup {
            buttons: buttonRow.children
        }

        Row {
            id: buttonRow
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                text: "Previous Level"
                height: 30
                width: gameScene.width/(parent.children.length) - (parent.children.length-1)*parent.spacing/parent.children.length
                onClicked: {
                    currentLevel.prevLevel()
                }
            }

            Button {
                text: "Next Level"
                height: 30
                width: gameScene.width/(parent.children.length) - (parent.children.length-1)*parent.spacing/parent.children.length
                onClicked: {
                    currentLevel.nextLevel()
                }
            }

            Button {
                text: "Reset Level"
                height: 30
                width: gameScene.width/(parent.children.length) - (parent.children.length-1)*parent.spacing/parent.children.length
                onClicked: {
                    currentLevel.nextLevel()
                    currentLevel.prevLevel()
                }
            }

            Button {
                text: "Play Level"
                height: 30
                width: gameScene.width/(parent.children.length) - (parent.children.length-1)*parent.spacing/parent.children.length
                onClicked: {
                    currentLevel.nextLevel()
                    currentLevel.prevLevel()
                    currentLevel.setGameState(1)
                }
            }
        }
    }
}
