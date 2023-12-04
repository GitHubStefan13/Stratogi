import QtQuick

Item {
    property alias rectangleColor: rectangle.color
    property alias imageSource: image.source

    Rectangle {
        id: rectangle
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#b6b6b6"
            }

            GradientStop {
                position: 1
                color: "#000000"
            }
        }
        anchors.fill: parent
    }


    Image {
        id: image
        fillMode: Image.Stretch
        anchors.fill: parent
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:720;width:480}D{i:1;anchors_height:100;anchors_width:100;anchors_x:170;anchors_y:276}
}
##^##*/
