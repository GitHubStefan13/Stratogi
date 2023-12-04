import QtQuick 6

Rectangle {
    property int offset: 10
    property int cellWidth
    property int cellHeight

    id: table_bottom

    signal clicked(int x, int y)
    signal undo()

    Grid {
        id: grid
        width: 400
        height: 200
        anchors.fill: parent
        columns: 10
        rows: 2
        objectName: "grid"

        // Shogi Table
        Repeater {
            anchors.fill: parent
            model: shogiTableBot

            delegate: Cell {
                cellColor: model.CurrentPieceColor
                border.color: model.BorderColor
                cellX: (index % 10) + 200 // Bottom Table adds 200, so we know when we clicked on it
                cellY: (index / 10) + 200 // Bottom Table adds 200, so we know when we clicked on it
                onClicked: function(x,y) { shogiGameImpl.clicked(x, y) }
                objectName: "c"+ cellX + cellY
                width: cellWidth
                height: cellHeight
                source: model.CellImageSource
            }
        }
    }
}





































/*##^##
Designer {
    D{i:0;autoSize:true;height:80;width:400}D{i:1;anchors_x:16;anchors_y:0}
}
##^##*/
