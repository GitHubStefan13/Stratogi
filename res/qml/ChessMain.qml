import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import StratogiSignals 1.0

/* THIS QML IS CURRENTLY ONLY MEANT FOR PORTRAIT MODE */

Page {
    id: chessMain
    title: qsTr("Chess")

    // The Height that is left after the Rectangle that is our Chess Board
    property int heightLeft: height - width

    // Delegate for the Pieces as Image
    Component {
        id: pieceDelegate

        Rectangle {
            width: 36
            height: 16

            Image {
                source: imageSource
                sourceSize.width: 256
                sourceSize.height: 256

                width: 16
                height: 16
            }

            Text {
                text: "x" + count
                width: 16
                height: 16
                x: 20
            }
        }


    }


    Column {
        id: mainLayout
        anchors.fill: parent

        Text {
            id: turn
            width: parent.width
            height: heightLeft * 0.1
            objectName: "turn"

            text: qsTr("White")
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            id: topPlayer
            width: parent.width
            height: heightLeft * 0.2

            Text {
                width: parent.width
                height: parent.height * 0.2

                id: topPlayerName
                text: qsTr("Player 2")
                font.pixelSize: 14
                font.bold: true
            }
        }

        ChessBoard {
            id: board

            width: parent.width
            height: width
            cellWidth: width / 9
            cellHeight: cellWidth

            objectName: "board" // This is set in AbstractGame Constructor and needs the same NAME!
        }

        Rectangle {
            id: bottomPlayer
            width: parent.width
            height: heightLeft * 0.2

            Text {
                width: parent.width
                height: parent.height * 0.2

                id: bottomPlayerName
                text: qsTr("Player 1")
                font.pixelSize: 14
                font.bold: true
            }
        }

        Row {
            id: buttonGroup
            width: parent.width
            height: heightLeft * 0.5

            UndoBtn {
                id: undoBtn
                width: 42
                height: 42
                playMode: "Chess"
            }

            RoundButton {
                id: newGame
                text: qsTr("New Game")

                onClicked: {
                    var d = confDialog.createObject()
                    d.text = qsTr("Do you really want to start a new Game?")
                    d.accepted.connect(function(){
                        chessGameImpl.newGame()
                    })
                    d.visible = true
                }
            }
        }

    }

    // Reusable confirmation Dialog
    Component {
        id: confDialog

        MessageDialog {
            title: qsTr("Confirmation")
            text: ""
            onVisibleChanged: if(!visible) destroy(1)
            buttons : MessageDialog.Cancel | MessageDialog.Ok
        }
    }

    CheckMateDialog {
        id: checkmate

        turnText: "TO BE IMPLEMENT, BUT EITHER BLACK OR WHITE WON!"

        onClosed: {
            if (checkmate.accepted())
                chessGameImpl.newGame()
            else
                chessGameImpl.newGame()
        }
    }

    DrawGameDialog {
        id: drawgame

        onClosed: {
            if (drawgame.accepted()) {
                chessGameImpl.newGame()
            }
            else
                chessGameImpl.newGame()
        }
    }

    TransformDialog {
        id: transform

        onClosed: {
            chessBoard.transformation(figure, color, fx, fy)
        }
    }

    // Signal's we receive from C++, see signals.hpp
    Connections {
        target: chessGame

        function onCheckmate() {
            checkmate.open()
        }

        function onDrawgame() {
            drawgame.open()
        }

        function onTransformation(fcolor, fx, fy) {
            transform.fcolor = fcolor
            transform.fx = fx
            transform.fy = fy

            transform.open()
        }

        function onNoMoreUndo() {
            undoBtn.disable()
        }

        function onTurnMade() {
            chessGame.setGameState(true) // We made a Move so the GameState is true

            if(turn.text === "White")
            {
                turn.text = "Black"
            } else {
                turn.text = "White"
            }

            if(chessGameImpl.turnsCount() > 0) {
                undoBtn.enable()
            }


        }

        function onFinishedLoading() {
            chessMain.width = (parent.width < 480) ? 480 : parent.width
            chessMain.height = (parent.height < 720) ? 720 : parent.height
        }
    }


}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/
