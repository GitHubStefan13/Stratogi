import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    property size appWindowSize;
    property string turnText;

    title: qsTr( "Checkmate!" )
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    x: appWindowSize.width / 2 - width / 2
    y: appWindowSize.height / 2 - height / 2

    Column {
        anchors.centerIn: parent
        id: column
        width: 238
        height: 88
        Layout.alignment: Qt.AlignTop

        Text {
            id: winText
            font.pixelSize: 30
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            text: ( turnText === qsTr( "Black" ) ?
                       qsTr( "White" ) : qsTr( "Black" ) ) + qsTr( " won!!!" )
        }

        Row {
            id: row
            Layout.alignment: Qt.AlignBottom

            RoundButton {
                text: qsTr("Exit")
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft

                onClicked: {
                    reject()
                }
            }

            RoundButton {
                text: qsTr("New Game")
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                onClicked: {
                    accept()
                }
            }
        }


    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
