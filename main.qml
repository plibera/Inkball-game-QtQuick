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
        }

        ButtonGroup {
            buttons: buttonRow.children
        }

        Row {
            id: buttonRow

            Button {
                text: "Previous Level"
                height: 30
                width: gameScene.width/3
                onClicked: {
                    currentLevel.prevLevel()
                }
            }

            Button {
                text: "Next Level"
                height: 30
                width: gameScene.width/3
                onClicked: {
                    currentLevel.nextLevel()
                }
            }

            Button {
                text: "Reset Level"
                height: 30
                width: gameScene.width/3
                onClicked: {
                    currentLevel.nextLevel()
                    currentLevel.prevLevel()
                }
            }
        }
    }
}
