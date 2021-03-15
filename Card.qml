import QtQuick 2.12
import QtQuick.Controls 2.12

// TODO: make this more graphical
Item {
    id: card
    property string cardText: "default"

    Rectangle {
        Label {
            text: cardText
        }
    }
}
