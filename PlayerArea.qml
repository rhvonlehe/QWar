import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: playerArea
    property bool   cardPlayed
    property string currentCards
    property var    playerInstance

    signal cardRequested

    color: "turquoise"
    border.width: 2
    border.color: playerInstance.winner ? "green" : playerArea.color

    Component.onCompleted: {
        var globalCoords = playerArea.mapToItem(gamePlay, x, y)
        console.log("playerArea.enabled: ", enabled)
        console.log("playerArea coords: ", x, y)
        console.log("area at coords: ", globalCoords.x, globalCoords.y)
        console.log("player area color:", color)
    }

    currentCards: playerInstance.playedCards


    ColumnLayout {
        //        Card {
        //        }

        //        Rectangle { // TODO remove when graphical card ready
        //            height: 20
        //            width: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        Label {
            id: cardsPlayed
            color: "dark blue"
            height: 50
            width: 50
            text: currentCards
        }
        //        }

        Label {
            id: nameLabel
            height: 50
            width: 50
            text: playerInstance.name
        }

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

        Button {
            id: playButton
            height: 50
            width: 50
            enabled: playerInstance.active
            text: playerInstance.buttonText

            onClicked: {
                playerInstance.action()
            }
        }
    }
}

