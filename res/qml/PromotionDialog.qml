import QtQuick
import QtQuick.Controls
import StratogiSignals 1.0

Dialog {
    id: promote

    property size appWindowSize;

    title: qsTr( "Promote ?" )
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    x: appWindowSize.width / 2 - width / 2
    y: appWindowSize.height / 2 - height / 2

    property int fcolor: 0
    property int fx: -1
    property int fy: -1
    property int figure: 0 // Default is 0, which is false, we only want a Boolean Value Back not a Figure

    Column {
        anchors.centerIn: parent
        id: column

        ButtonGroup {
            buttons: column.children
        }

        property alias figure: promote.figure
        spacing: 15

        RoundButton {
            text: "Promote"
            anchors.rightMargin: 0
            anchors.right: column.right

            onClicked: {
               figure = 1  // 1 = true, so we now we want to promote
               close()
            }
        }

        RoundButton {
            text: "Cancel"
            anchors.rightMargin: 0
            anchors.right: column.right

            onClicked: {
                close()
            }
        }

    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
