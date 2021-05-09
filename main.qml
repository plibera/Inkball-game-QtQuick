import QtQuick 2.15
import QtQuick.Window 2.15
import ballInfo 1.0
import goalInfo 1.0
import level 1.0
import QtQuick.Controls 2.12

Window {
    id: gameWindow
    width: 640
    height: 670
    visible: true
    title: qsTr("InkBall")

    Level {
        id: currentLevel
    }

    Column {
        GameScene {
            id: gameScene
            level: currentLevel
            width: gameWindow.width < gameWindow.height-buttonRow.height ? gameWindow.width : gameWindow.height-buttonRow.height

            GameInfo {
                color: active ? "red" : "transparent"
                active: currentLevel.gameState == 3
                msg: "Game Over"
            }

            GameInfo {
                color: active ? "#40ff40" : "transparent"
                active: currentLevel.gameState == 2
                msg: "Game Won!"
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

            Button {
                text: "Previous Level"
                height: 30
                width: gameScene.width/4 - 3*parent.spacing/4
                onClicked: {
                    currentLevel.prevLevel()
                }
            }

            Button {
                text: "Next Level"
                height: 30
                width: gameScene.width/4  - 3*parent.spacing/4
                onClicked: {
                    currentLevel.nextLevel()
                }
            }

            Button {
                text: "Reset Level"
                height: 30
                width: gameScene.width/4  - 3*parent.spacing/4
                onClicked: {
                    currentLevel.nextLevel()
                    currentLevel.prevLevel()
                }
            }

            Button {
                text: "Play Level"
                height: 30
                width: gameScene.width/4  - 3*parent.spacing/4
                onClicked: {
                    currentLevel.nextLevel()
                    currentLevel.prevLevel()
                    currentLevel.setGameState(1)
                }
            }
        }
    }
}
