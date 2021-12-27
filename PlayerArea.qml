import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: playerArea
    property bool   cardPlayed
    property string currentCard
    property string color
    property var    playerInstance

    signal cardRequested

    Component.onCompleted: {
        var globalCoords = playerArea.mapToItem(gamePlay, x, y)
        console.log("playerArea.enabled: ", enabled)
        console.log("playerArea coords: ", x, y)
        console.log("area at coords: ", globalCoords.x, globalCoords.y)
    }

    ColumnLayout {
        //        Card {
        //        }

        //        Rectangle { // TODO remove when graphical card ready
        //            height: 20
        //            width: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            height: 50
            width: 50
            text: "unplayed: " + playerInstance.unplayedCardCnt
        }

        Label {
            height: 50
            width: 50
            text: "played: " + playerInstance.playedCardCnt
        }

        Label {
            color: playerArea.color
            height: 50
            width: 50
            text: currentCard
        }
        //        }


        Label {
            id: nameLabel
            height: 50
            width: 50
            text: playerInstance.name
        }

        Button {
            id: playButton
            height: 50
            width: 50
            enabled: playerInstance.active
            text: "Play Card"

            onClicked: {
                playerInstance.playCard()
            }
        }
    }

//    Connections {
//        target: playerInstance
//        ignoreUnknownSignals: true
//        function onActiveChanged() {
//            currentScreen = "GamePlay.qml"
//            console.log("currentScreen: ", currentScreen)
//        }
//    }
}

