import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: clientIntro
    anchors.top: parent.top
    height: parent.height
    width: parent.width

    signal connectToServer
    signal createServer

    RowLayout {
        id: layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        RoundButton {
            id: beClientChoice
            text: "Connect to server"
            font.pointSize: 13
            onClicked: {
                clientIntro.connectToServer()
            }
        }

        RoundButton {
            id: beServerChoice
            text: "Create a server"
            font.pointSize: 13
            onClicked: {
                clientIntro.createServer()
            }
        }
    }

    Connections {
        target: clientIntro
        ignoreUnknownSignals: true
        function onConnectToServer() {
            currentScreen = "ServerConnect.qml"
            clientAdapter.connectRemote();  // TODO: fix hard-coding here
        }
        function onCreateServer() {
            currentScreen = "ServerCreate.qml"
            clientAdapter.startAndConnectLocal();
        }
    }
}
