import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: playerArea
    property string playerName
    property string cardText

    Component.onCompleted: {
        console.log("playerArea.enabled: ", enabled)

    }

    ColumnLayout {
//        Card {
//        }

        Rectangle { // TODO remove when graphical card ready
            Label {
                text: cardText
            }
        }

        Label {
            id: nameLabel
            text: playerName
        }

        Button {
            id: playButton
            text: "Play Card"
        }
    }

}

