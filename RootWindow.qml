import QtQuick          2.12
import QtQuick.Controls 2.12
import QtQuick.Window   2.12
import QtQuick.Layouts  1.12

ApplicationWindow {
    id:             mainWindow
    minimumWidth:   Math.min(215 * Screen.pixelDensity, Screen.width)
    minimumHeight:  Math.min(120 * Screen.pixelDensity, Screen.height)
    visible:        true
    color:          "turquoise"

    RowLayout {
        anchors.top: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter


        Label {
            text: "Enter number of players"
        }

        ComboBox {
            id: playerCount
            model: [1, 2, 3, 4, 5]
        }

        Button {
            id: countOk
            text: "Ok"

            onClicked: {

            }
        }

    }

}
