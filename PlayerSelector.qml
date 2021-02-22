import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Control {
    id: selector
    property int playerCount: playerCountSelect.currentValue
    anchors.margins: 5
    width: mainRow.width
    height: Math.max(column1.height, column2.height)

    Component.onCompleted: {
        console.log(selector.height)
        console.log(column1.height)
        console.log(column2.height)

    }

    RowLayout {
        id: mainRow
        spacing: 6

        ColumnLayout {
            id: column1
            spacing: 6
            Label {
                Layout.minimumWidth: 120
                height: 32
                font.pointSize: 12
                text: "Enter number of players"
            }

            ComboBox {
                id: playerCountSelect
                model: [2, 3, 4]
            }
        }
        ColumnLayout {
            id: column2
            spacing: 6

            Repeater {
                id: playerList
                model: selector.playerCount

                Rectangle {
                    height: 32
                    Layout.minimumWidth: 200

                    // Expose the player name
                    property alias playerName: textField.text

                    TextField {
                        id: textField
                        font.pointSize: 12
                        text: "Player " + (index+1) + " name"
                    }
                }
            }
        }
    }
}
