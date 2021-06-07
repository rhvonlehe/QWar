import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12


RowLayout {
    id: mainRow
    spacing: 6
    property int playerCount: playerCountSelect.currentValue
    property var playerArray: playerList

    onHeightChanged: {
        console.log("mainRow height: ", mainRow.height)
        console.log("selectorTop height: ", mainRow.height)
        console.log("mainRow implicitHeight: ", mainRow.implicitHeight)
        console.log("mainRow implicitHeight: ", mainRow.implicitHeight)
        console.log(mainRow.height)
        console.log(column1.height)
        console.log(column2.height)
    }

    ColumnLayout {
        id: column1
        spacing: 6
        Layout.fillHeight: true
        Label {
            Layout.minimumWidth: 120
            Layout.minimumHeight: 32
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
            model: mainRow.playerCount
            Layout.alignment: Qt.AlignVCenter

            Rectangle {
                Layout.minimumHeight: 32
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

        onHeightChanged: {
            console.log("column2 implicit height: ", implicitHeight)
        }
    }
}
