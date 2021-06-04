import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQml.Models 2.12
import QtQml 2.12


Item {
    id: gameRoot
    signal endGame
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

    ColumnLayout {
        id: mainColumn
        spacing: 2
        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
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
                }

                Button {
                    id: end
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 80
                    text: "End Game"
                    onClicked: {
                        gameRoot.endGame()
                    }
                }

                Button {
                    id: save
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 80
                    text: "Save Game"
                }
            }
//            Component.onCompleted: {
//                console.log("rectangle containing buttons size: ", height, width)
//            }
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
                property var playerCnt: playerArray.length

                Repeater {
                    model: playerLayoutModel
                    delegate: playerArea
                }

                Component {
                    id: playerArea
                    PlayerArea {
                        color: "red"
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

            Component.onCompleted: {
                console.log("rectangle containing grid size: ", height, width)
                console.log("rectangle center: ", horizontalCenter, verticalCenter)
            }
        }
    }

}


//        Component.onCompleted: {
//            console.log("count: ", playerCnt)
//        }

