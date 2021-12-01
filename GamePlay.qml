import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12
import QtQml 2.12


Item {
    id: gamePlay
    signal endGame
    signal deal
//    property var playerArray : game.getPlayers()

    property var playerRow: [ 1, 1, 0, 2 ]
    property var playerCol: [ 0, 2, 1, 1 ]

    ColumnLayout {
        id: mainColumn
        spacing: 2
        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            height: 100
            width: 250
            color: "turquoise"
            RowLayout {
                id: gameButtonsLayout
                anchors.fill: parent
                Button {
                    id: deal
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 80
                    text: "Deal"
                    onClicked: {
                        game.deal()
                    }
                }

                Button {
                    id: end
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 80
                    text: "End Game"
                    onClicked: {
                        gamePlay.endGame()
                        gameModel.reset()
                    }
                }

                Button {
                    id: save
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 80
                    text: "Save Game"
                }
            }
            Component.onCompleted: {
//                console.log("rectangle containing buttons size: ", height, width)
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            anchors.horizontalCenter: Layout.horizontalCenter
            color: "turquoise"

            GridLayout {
                id: gridnew
                columns: 3
                rows: 3
                anchors.fill: parent
                property var playerCnt: gameModel.rowCount()

                Repeater {
//                    model: playerLayoutModel
                    model: gameModel
                    delegate: playerArea
                }

                Component {
                    id: playerArea
                    PlayerArea {
                        Layout.row: playerRow[index]
                        Layout.column: playerCol[index]
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        color: "red"
                        playerInstance: player
                        enabled: gameModel.rowCount() > index ? true : false
                        opacity: gameModel.rowCount() > index ? true : false

                        Component.onCompleted: {
                            console.log("rowCount is ", gameModel.rowCount())
                            console.log("player name ", player.name)
                            console.log("uplayedCardCnt ", player.unplayedCardCnt)
                        }

                        onCardRequested: {
                            console.log("card played")
//                            active = false
                            gameModel.playCard(player)
                        }
                    }
                }

                // Add TableArea here
            }

            Component.onCompleted: {
                console.log("rectangle containing grid size: ", height, width)
                console.log("rectangle center: ", horizontalCenter, verticalCenter)
            }
        }
    }
}

