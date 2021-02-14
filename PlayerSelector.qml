import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Control {
    id: playerSelector
    property int playerCount: playerCountSelect.currentValue

    RowLayout {
        id: mainRow
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 6

        ColumnLayout {
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
                model: playerSelector.playerCount

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
}
