import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: serverConnect

    // test
    signal testSend

    RowLayout {
        Text {
            id: name
            text: qsTr("This is the server connect screen")
        }

        RoundButton {
            id: testSend
            text: "Test sending a message"
            font.pointSize: 13
            onClicked: {
                clientIntro.testSend()
            }
        }

    }

    Connections {
        target: serverConnect  // target property means src of signal
        function onTestSend() {

        }
    }

}
