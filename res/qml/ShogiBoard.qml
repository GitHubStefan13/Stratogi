import QtQuick 6

Rectangle {
    property int offset: 16
    property int cellWidth: ( width - offset * 2 ) / 9
    property int cellHeight: ( height - offset * 2 ) / 9

    id: shogi

    signal clicked(int x, int y)
    signal hovered(int x, int y)
    signal newGame()
    signal transformation(int figure, int color, int x, int y);

    Row {
        id: top
        height: offset

        // Top letters.
        Item { width: offset; height: offset; }
        Repeater {
            model: [ "9", "8", "7", "6", "5", "4", "3", "2", "1" ]
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
            model: [ "A", "B", "C", "D", "E", "F", "G", "H", "I" ]

            delegate: Text {
                width: offset
                height: cellHeight
                text: modelData
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: 14
           }
        }
    }

    Grid {
        id: grid
        rows: 9
        columns: 9
        x: offset
        y: top.y + top.height
        objectName: "grid"

        // Shogi board.
        Repeater {
            model: shogiBoard

            delegate: Cell {
                cellColor: model.CurrentPieceColor
                border.color: model.BorderColor
                cellX: index % 9
                cellY: index / 9
                onClicked: function(x,y) { shogiGameImpl.clicked(x, y) }
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
            model: [ "A", "B", "C", "D", "E", "F", "G", "H", "I" ]

            delegate: Text {
                width: offset
                height: cellHeight
                text: modelData
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: 14
                transform: Rotation {
                    origin.x: width / 2
                    origin.y: height / 2
                }
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
            model: [ "9", "8", "7", "6", "5", "4", "3", "2", "1" ]
            delegate: Text {
                width: cellWidth
                height: offset
                text: modelData
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pixelSize: 14
                transform: Rotation {
                    origin.x: width / 2
                    origin.y: height / 2
                }
            }
        }
        Item { width: offset; height: offset; }
    }
}
