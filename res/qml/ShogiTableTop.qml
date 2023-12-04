import QtQuick 6

Rectangle {
    property int offset: 10
    property int cellWidth
    property int cellHeight

    id: table_top

    signal clicked(int x, int y)
    signal undo()

    Grid {
        id: grid
        anchors.fill: parent
        columns: 10
        rows: 2
        objectName: "grid"

        // Shogi Table
        Repeater {
            anchors.fill: parent
            model: shogiTableTop

            delegate: Cell {
                cellColor: model.CurrentPieceColor
                border.color: model.BorderColor
                cellX: (index % 10) + 100 // Top Table adds 100, so we know when we clicked on it
                cellY: (index / 10) + 100 // Top Table adds 100, so we know when we clicked on it
                onClicked: function(x,y) { shogiGameImpl.clicked(x, y) }
                objectName: "c"+ cellX + cellY
                width: cellWidth
                height: cellHeight
                source: model.CellImageSource
            }
        }
    }
}





































/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_x:16;anchors_y:0}
}
 ##^##*/
