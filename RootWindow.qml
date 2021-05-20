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

    property string currentScreen: "GameStart.qml"

    Loader {
        id:             dynamicLoader
        anchors.fill:   parent
        source:         currentScreen

        onLoaded: {

        }
    }

    Connections {
        target: dynamicLoader.item
        onPlayGame: {
            currentScreen = "GamePlay.qml"
            console.log("currentScreen: ", currentScreen)
        }
        onEndGame: {
            currentScreen = "GameStart.qml"
            console.log("currentScreen: ", currentScreen)
        }
    }
}
