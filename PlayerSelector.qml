import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Control {
    id: playerSelector
    RowLayout {
        id: mainRow
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

    Column {
        spacing: 6
        Repeater {
            id: playerList
            model: playersRoot.playerCount

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 120
                height: 32

                // Expose the player name
                property alias playerName: textField.text

                TextEdit {
                    id: textField
                    font.pointSize: 12
                    text: "Player " + (index+1) + " name"
                }
            }
        }
    }
}
