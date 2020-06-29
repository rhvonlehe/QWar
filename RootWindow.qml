import QtQuick          2.12
import QtQuick.Controls 2.12
import QtQuick.Window   2.12
import QtQuick.Layouts  1.12

ApplicationWindow {
    id:             mainWindow
    minimumWidth:   Math.min(215 * Screen.pixelDensity, Screen.width)
    minimumHeight:  Math.min(120 * Screen.pixelDensity, Screen.height)
    visible:        true

    RowLayout {
        anchors.top: parent

        Label {
            text: "Enter number of players"
        }

        ComboBox {
            model: [1, 2, 3, 4, 5]

            onAccepted: {

            }
        }
    }

}
