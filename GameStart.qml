import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: gameStart
    anchors.top: parent.top
    height: parent.height
    width: parent.width

    PlayerSelector {
        anchors.horizontalCenter: parent.horizontalCenter
    }
}














//Item {
//    id: gameStart
//    property int playerCount: playerCountSelect.currentValue
//    signal playGame

//    RowLayout {
//        id: layout
//        anchors.top: parent.top
//        anchors.topMargin: 5
//        spacing: 6
//        anchors.horizontalCenter: parent.horizontalCenter

//        Label {
//            text: "Enter number of players"
//        }

//        ComboBox {
//            id: playerCountSelect
//            model: [2, 3, 4]
//        }
//    }

//    RowLayout {
//        id: mainRow
//        anchors.centerIn: parent
//        anchors.horizontalCenter: parent.horizontalCenter
//        spacing: 6

//        RoundButton {
//            text: "Play"
//            width: 40
//            radius: 2
//            font.pointSize: 12
//            onClicked: {
//                for (var i =0; i < playerList.count; i++) {
//                    console.log(playerList.itemAt(i).playerName)
//                    game.addPlayer(playerList.itemAt(i).playerName)
//                }
//                gameStart.playGame()

//                game.start()
//            }
//        }

//        Column {
//            spacing: 6
//            Repeater {
//                id: playerList
//                model: playersRoot.playerCount

//                Rectangle {
//                    anchors.horizontalCenter: parent.horizontalCenter
//                    width: 120
//                    height: 32

//                    // Expose the player name
//                    property alias playerName: textField.text

//                    TextEdit {
//                        id: textField
//                        font.pointSize: 12
//                        text: "Player " + (index+1) + " name"
//                    }
//                }
//            }
//        }

//        RoundButton {
//            text: "Quit"
//            width: 40
//            radius: 2
//            font.pointSize: 12
//            onClicked: {
//                game.quit()
//            }
//        }
//    }
//}

