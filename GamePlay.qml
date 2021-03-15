import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    id: gameRoot


    GridLayout {
        id: grid
        columns: 3
        rows: 3

        PlayerArea {
            Layout.column: 0
            Layout.rowSpan: 3
        }

        PlayerArea {
            Layout.row: 1
            Layout.rowSpan: 1


        }

        PlayerArea {

        }
    }







//    Rectangle {
//        TextField {
//            text: "I am here!"
//        }
//    }

}
