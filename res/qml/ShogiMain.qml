import QtQuick.Window
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import StratogiSignals 1.0

Page {
    id: shogiMain
    title: qsTr("Shogi")

    property int offset: 16
    property int minWidth: offset * 2 + 50 * 8
    property int minHeight: turn.height + table_top.height + board.height + table_bot.height + extras.height + 50

    onWidthChanged: {
        shogiMain.minHeight = Math.abs(turn.height) + Math.abs(table_top.height) + Math.abs(board.height)
                            + Math.abs(table_bot.height) + Math.abs(extras.height) + 50

    }

    CheckMateDialog {
        id: checkmate

        appWindowSize: Qt.size( parent.width, parent.height )
        turnText: turn.text

        onClosed: {
            shogiBoard.newGame()
        }
    }

    DrawGameDialog {
        id: drawgame

        appWindowSize: Qt.size( parent.width, parent.height )

        onClosed: {
            shogiBoard.newGame()
        }
    }

    PromotionDialog {
        id: promote

        appWindowSize: Qt.size( parent.width, parent.height )

        onClosed: {
            shogiBoard.transformation(figure, fcolor, fx, fy)
        }
    }

    // Signal we receive from C++, see signals.hpp
    Connections {
        target: shogiGame

        function onCheckmate() {
            checkmate.open()
        }

        function onDrawgame() {
            drawgame.open()
        }

        function onTransformation(fcolor, fx, fy) { // Technically its called Promotion, but we defined the Signal in a Abstract Class, so its named Transformation instead
            promote.fcolor = fcolor
            promote.fx = fx
            promote.fy = fy

            promote.open()
        }

        function onNoMoreUndo() {
            //undoBtn.disable() TODO: Add undoBtn
        }

        function onTurnMade() {
            shogiGame.setGameState(true) // We made a Move so the GameState is true

            if(turn.text === "White")
                turn.text = "Black"
            else
                turn.text = "White"
        }

        function onFinishedLoading() {
            shogiMain.width = (minWidth < 480) ? 480 : minWidth
            shogiMain.height = (minHeight < 720) ? 720 : minHeight
        }

        function onEngineError(error) {
            engineErrorDialog.text = "Engine Error occurred, cannot continue game \n" + error;
            engineErrorDialog.open();
            engineErrorDialog.accepted.connect(function(){
                stackView.pop();
            })
        }

    }

    Rectangle {
        id: rect
        Text {
            id: turn
            text: qsTr("White")
            anchors.horizontalCenter: rect.horizontalCenter
            objectName: "turn"
            anchors.topMargin: 5
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 14
            font.bold: true
            anchors.top: parent.top
        }

        ShogiTableTop {
            id: table_top
            x: 5
            width: rect.width - 10
            height: cellHeight * 2
            cellWidth: width / 10
            anchors.topMargin: 5
            cellHeight: cellWidth
            anchors.top: turn.bottom
        }

        ShogiBoard {
            id: board
            x: 5
            width: rect.width - 10
            height: width
            cellWidth: width / 10
            anchors.topMargin: 10
            objectName: "board"
            cellHeight: cellWidth
            anchors.top: table_top.bottom
        }

        ShogiTableBottom {
            id: table_bot
            x: 5
            width: rect.width - 10
            height: cellHeight * 2
            cellWidth: width / 10
            anchors.topMargin: 5
            cellHeight: cellWidth
            anchors.top: board.bottom
        }

        Rectangle {
            id: extras
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 5

            PageIndicator {
                id: swipeIndicator
                height: 20
                count: swipeView.count
                anchors.horizontalCenterOffset: - width / 2
                anchors.leftMargin: - width / 2
                anchors.horizontalCenter: extras.horizontalCenter
                anchors.topMargin: 0
                anchors.top: extras.top
                currentIndex: swipeView.currentIndex
            }

            Rectangle {
                id: moveList
                color: "#00000000"
                border.color: "#00000000"

                anchors.right: extras.right
                anchors.left: extras.left

                SwipeView {
                    id: swipeView
                    anchors.fill: parent
                    clip: false

                    Item {
                        id: element
                        Text {
                            id: moveText
                            text: qsTr("Moves:")
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            anchors.top: parent.top
                            anchors.topMargin: 0
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            objectName: "turn"
                            font.pixelSize: 14
                            font.bold: true
                        }

                        TextArea {
                            id: textArea
                            text: shogiBoard.moves
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: 0
                            anchors.right: parent.right
                            anchors.rightMargin: 0
                            anchors.left: parent.left
                            anchors.leftMargin: 0
                            anchors.top: moveText.bottom
                            anchors.topMargin: 0
                            textFormat: Text.AutoText
                            horizontalAlignment: Text.AlignLeft
                            wrapMode: Text.WordWrap
                            font.pointSize: 10
                            font.bold: true
                            placeholderText: ""
                        }
                    }
                    currentIndex: 0
                }
                anchors.topMargin: 0
                anchors.bottomMargin: 5
                anchors.rightMargin: 5
                anchors.top: swipeIndicator.bottom
                anchors.bottom: parent.bottom
            }
            anchors.topMargin: 5
            anchors.bottomMargin: 5
            anchors.bottom: parent.bottom
            anchors.top: table_bot.bottom
        }
        anchors.fill: parent
    }

    MessageDialog {
        id: engineErrorDialog
        title: qsTr("Engine Error")
        text: ""
        buttons : MessageDialog.Ok
    }
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:16}D{i:10}
}
##^##*/
