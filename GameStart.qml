import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: gameStart
    anchors.top: parent.top
    height: parent.height
    width: parent.width
    signal playGame

    Component.onCompleted: {
        console.log(selector.right)
        console.log(quitButton.left)
        game.reset()
    }

    RowLayout {
        id: layout
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        RoundButton {
            id: playButton
            text: "Play"
            width: 40
            radius: 2
            font.pointSize: 12
            Layout.alignment: Qt.AlignVCenter
            onClicked: {
                var players = selector.playerArray
                for (var i =0; i < players.count; i++) {
                    console.log(players.itemAt(i).playerName)
                    game.addPlayer(players.itemAt(i).playerName)
                }
                gameStart.playGame()
            }
        }

        PlayerSelector {
            id: selector
            Layout.alignment: Qt.AlignVCenter

            onHeightChanged: {
                console.log("selector height: ", selector.height)
                console.log("selector implicit height: ", selector.implicitHeight)
                console.log("layout height: ", layout.height)
                console.log("selector x: ", selector.x)
                console.log("selector y: ", selector.y)
            }
        }

        RoundButton {
            id: quitButton
            Layout.alignment: Qt.AlignVCenter
            text: "Quit"
            Layout.minimumWidth: 40
            radius: 2
            font.pointSize: 12
            onClicked: {
                game.quit()
            }
        }
    }
}
