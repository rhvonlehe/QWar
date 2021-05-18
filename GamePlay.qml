import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12
import QtQml 2.12


Item {
    id: gameRoot
    property var playerArray : game.getPlayers()

    ListModel {
        id: playerLayoutModel
        ListElement {
            property var row: 1
            property var column: 0
        }
        ListElement {
            property var row: 1
            property var column: 2
        }
        ListElement {
            property var row: 0
            property var column: 1
        }
        ListElement {
            property var row: 2
            property var column: 1
        }
    }

    GridLayout {
        id: gridnew
        columns: 3
        rows: 3
        anchors.fill: parent
        property var playerCnt: playerArray.length

        Repeater {
            model: playerLayoutModel
            delegate: playerArea
        }

        Component {
            id: playerArea
            PlayerArea {
                playerName: playerArray[index]
                Layout.row: model.row
                Layout.column: model.column
                Layout.fillHeight: true
                Layout.fillWidth: true
                enabled: gridnew.playerCnt > index ? true : false
                opacity: gridnew.playerCnt > index ? true : false
            }
        }
    }
}

//        Component.onCompleted: {
//            console.log("count: ", playerCnt)
//        }

