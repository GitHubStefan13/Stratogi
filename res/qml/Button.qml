import QtQuick

Item {
    property alias elementText: element.text
    signal clicked()

    id: btn

    implicitHeight: element.implicitHeight * 2
    implicitWidth: element.implicitWidth + 40

    Rectangle {
        id: rectangle
        color: "#2c2c2f"
        radius: 10
        border.width: 2
        anchors.fill: parent

        Text {
            id: element
            x: 138
            y: 29
            color: "#dad3d3"
            text: qsTr("Text")
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 24
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: {
                btn.clicked();
            }
        }
    }

}





/*##^##
Designer {
    D{i:0;autoSize:true;height:75;width:300}D{i:3;anchors_height:100;anchors_width:100}
D{i:1;anchors_height:200;anchors_width:200;anchors_x:69;anchors_y:8}
}
##^##*/
