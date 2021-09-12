import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: playerArea
    signal cardPlayed(string playerName)
    property bool   cardPlayed
    property string playerName
    property string currentCard
    property string color
    property int    unplayedCardsCnt
    property int    playedCardsCnt

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
            text: "unplayed: " + unplayedCardsCnt
        }

        Label {
            height: 50
            width: 50
            text: "played: " + playedCardsCnt
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
            text: playerName
        }

        Button {
            id: playButton
            height: 50
            width: 50
            text: "Play Card"
        }
    }

}

