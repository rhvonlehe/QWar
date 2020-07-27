import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: root
    property int playerCount: 0

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
            model: [1, 2, 3, 4]

            onAccepted: {
                playerCount = currentValue
            }
        }

        Repeater {
            model: playerCount

            TextEdit {
                text: "Player " + index + " name"
            }
        }


    }

}
