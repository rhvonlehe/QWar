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

    Connections {
        target: dynamicLoader.item  // target property means src of signal
        ignoreUnknownSignals: true
        function onConnectToServer() {
            currentScreen = "ServerConnect.qml"

        }
        function onCreateServer() {
            currentScreen = "ServerCreate.qml"

        }
    }

}
