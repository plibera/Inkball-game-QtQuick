import QtQuick 2.15
import QtQuick.Window 2.15
import ballInfo 1.0
import goalInfo 1.0
import level 1.0
import QtQuick.Controls 2.12

Rectangle {
    id: gameOverInfo
    width: gameScene.width/2
    height: width/2
    x: gameScene.x+gameScene.width/4
    y: gameScene.y+gameScene.height/2-height/2
    radius: width/16
    border.width: active ? 5 : 0
    border.color: "black"
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    property bool active
    property string msg
    Text {
        text: parent.active ? parent.msg : ""
        font.pointSize: parent.width / 12
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
    }
}
