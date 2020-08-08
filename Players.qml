import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: root
    property int playerCount: playerCountSelect.currentValue

    RowLayout {
        id: layout
        anchors.top: parent.top
        anchors.topMargin: 5
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

    RowLayout {
        id: mainRow
        anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 6

        RoundButton {
            text: "Play"
            width: 40
            radius: 2
            font.pointSize: 12
            //                onClicked:
        }

        Column {
            spacing: 6
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
        }

        RoundButton {
            text: "Quit"
            width: 40
            radius: 2
            font.pointSize: 12
        }
    }

}

