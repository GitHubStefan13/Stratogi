import QtQuick

Rectangle {
    id: chessB

    property int offset: 16
    property int cellWidth: ( width - offset * 2 ) / 8
    property int cellHeight: ( height - offset * 2 ) / 8

    signal clicked(int x, int y)
    signal hovered(int x, int y)
    signal newGame()
    signal transformation(int figure, int color, int x, int y);
    signal rotationDone()
    signal rotationStarted()

    Row {
        id: top
        height: offset

        // Top letters.
        Item { width: offset; height: offset; }
        Repeater {
            model: [ "A", "B", "C", "D", "E", "F", "G", "H" ]
            delegate: Text {
                width: cellWidth
                height: offset
                text: modelData
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: 14
            }
        }
        Item { width: offset; height: offset; }
    }

    Column {
        y: top.y + top.height

        // Left letters.
        Repeater {
            model: 8

            delegate: Text {
                width: offset
                height: cellHeight
                text: 8 - index
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: 14
           }
        }
    }

    Grid {
        id: grid
        rows: 8
        columns: 8
        x: offset
        y: top.y + top.height
        objectName: "grid"

        // Chess board.
        Repeater {
            model: chessBoard

            delegate: Cell {
                cellColor: model.CurrentPieceColor
                border.color: model.BorderColor
                cellX: index % 8
                cellY: index / 8
                onClicked: function(x,y) { chessGameImpl.clicked(x, y) }
                objectName: "c"+ cellX + cellY
                width: cellWidth
                height: cellHeight
                source: model.CellImageSource
                blueProp: model.BluePieceColor
                redProp: model.RedPieceColor
                checkProp: model.PieceColor
            }
        }
    }

    Column {
        y: grid.y
        x: grid.x + grid.width

        // Right numbers.
        Repeater {
            model: 8

            delegate: Text {
                width: offset
                height: cellHeight
                text: 8 - index
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: 14
           }
        }
    }

    Row {
        id: bottom
        height: offset
        y: grid.y + grid.height

        // Bottom letters.
        Item { width: offset; height: offset; }
        Repeater {
            model: [ "A", "B", "C", "D", "E", "F", "G", "H" ]
            delegate: Text {
                width: cellWidth
                height: offset
                text: modelData
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: 14
            }
        }
        Item { width: offset; height: offset; }
    }
}
