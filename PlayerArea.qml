import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: playerArea
    property string playerName
    property string cardText

    RowLayout {
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

    }

}

