import QtQuick 6

Image {
    property bool enabled: false
    property string playMode
    source: "qrc:/Undo_disabled.png"

    MouseArea {
        anchors.fill: parent

        onPressed: {
            if( undoBtn.enabled )
                undoBtn.source = "qrc:/Undo_clicked.png"
        }

        onReleased: {
            if( undoBtn.enabled )
                undoBtn.source = "qrc:/Undo.png"
        }

        onClicked: {
            switch(playMode)
            {
                case "Chess":
                    chessGameImpl.undo()
                    if(chessGameImpl.turnsCount() === 0)
                      disable();
                    break;

                case "Shogi":
                    shogiGameImpl.undo()
                    if(shogiGameImpl.turnsCount() === 0)
                      disable();
                    break;

            }
        }
    }

    function disable() {
        enabled = false
        source = "qrc:/Undo_disabled.png"
    }

    function enable() {
        enabled = true
        source = "qrc:/Undo.png"
    }
}
