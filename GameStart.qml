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
//        playButton.clicked.connect(dynamicLoader.)
        console.log(selector.right)
        console.log(quitButton.left)
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
            onClicked: {
                var players = selector.playerArray
                for (var i =0; i < players.count; i++) {
                    console.log(players.itemAt(i).playerName)
                    game.addPlayer(players.itemAt(i).playerName)
                }
                gameStart.playGame()

//                game.start() // todo remove
            }
        }

        PlayerSelector {
            id: selector
            anchors.verticalCenter: layout.verticalCenter
        }

        RoundButton {
            id: quitButton
            text: "Quit"
            width: 40
            radius: 2
            font.pointSize: 12
            onClicked: {
                game.quit()
            }
        }
    }
}
