import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: root
    property int playerCount: playerCountSelect.currentValue

    RowLayout {
        id: layout
        anchors.top: parent.top
        spacing: 6
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            text: "Enter number of players"
        }

        ComboBox {
            id: playerCountSelect
            model: [2, 3, 4]
        }
    }

    Column {
        anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 3
        Repeater {
            model: root.playerCount

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 120
                height: 32
                TextEdit {
                    font.pointSize: 12
                    text: "Player " + (index+1) + " name"
                }
            }
        }

        RowLayout {
            RoundButton {
                text: "Play"
                radius: 2
                font.pointSize: 12
//                onClicked:
            }
            RoundButton {
                text: "Quit"
                radius: 2
                font.pointSize: 12
            }
        }
    }

}
