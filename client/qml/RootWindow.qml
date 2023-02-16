import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: mainWindow
    minimumWidth:   Math.min(215 * Screen.pixelDensity, Screen.width)
    minimumHeight:  Math.min(120 * Screen.pixelDensity, Screen.height)
    visible:        true
    color:          "turquoise"

    property string currentScreen: "Intro.qml"

    Loader {
        id:             dynamicLoader
        anchors.fill:   parent
        source:         currentScreen

        onLoaded: {

        }
    }


}
