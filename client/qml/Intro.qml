import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: clientIntro
    anchors.top: parent.top
    height: parent.height
    width: parent.width

    signal connectToServer
    signal startServer

    RowLayout {
        id: layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        RoundButton {
            id: clientChoice
            text: "Connect to server"
            font.pointSize: 13
            onClicked: {
                clientIntro.connectToServer()
            }
        }

    }

}
