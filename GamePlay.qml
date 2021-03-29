import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: gameRoot
    property var playerArray : game.getPlayers()


    GridLayout {
        id: grid
        columns: 3
        rows: 3
        anchors.fill: parent
        property var playerCnt: playerArray.length

        // idea for model: ListModel { 0 3 0, 1 0 2, 0 4 0 }

//        ListModel {
//            [0, 3, 0],
//            [1, 0, 2],

//        }

//        Repeater {
//            model: playerArray
//            delegate: PlayerArea
//            {
//                id: index
//                playerName: playerArray[index]
//            }
//        }

        Component.onCompleted: {
            console.log("count: ", playerCnt)
        }


        // Player 1
        PlayerArea {
            id: player1Area
            Layout.row: 1
            Layout.column: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
            playerName: playerArray[0]
        }

        // Player 2
        PlayerArea {
            id: player2Area
            Layout.row: 1
            Layout.column: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            playerName: playerArray[1]
        }

        // Player 3 (optional)
        PlayerArea {
            id: player3Area
            Layout.row: 0
            Layout.column: 1
            Layout.fillHeight: true
            Layout.fillWidth: true
            enabled: grid.playerCnt > 2 ? true: false
            opacity: grid.playerCnt > 2 ? 1 : 0
            playerName: grid.playerCnt > 2 ? playerArray[2] : ""
        }

        // Player 4 (optional)
        PlayerArea
        {
            id: player4Area
            Layout.row: 2
            Layout.column: 1
            Layout.fillHeight: true
            Layout.fillWidth: true
            enabled: grid.playerCnt > 3 ? true : false
            opacity: grid.playerCnt > 3 ? true : false
            playerName: grid.playerCnt > 3 ? playerArray[3] : ""
        }
    }







//    Rectangle {
//        TextField {
//            text: "I am here!"
//        }
//    }

}
