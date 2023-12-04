import QtQuick 2.12
import QtQuick.Controls 2.5
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "."

Page {
    id: mainMenu
    title: "MainMenu"

    property int page: view.currentIndex

    Settings{
        id: settings

        property int page
    }

    SwipeView {
        id: view

        currentIndex: settings.page
        anchors.fill: parent

        Item {
            id: page1
            property var tabIcon: "qrc:/Placeholder.png"

            Background {
                id: page1Background
                anchors.fill: parent
                imageSource: "qrc:/Chess_Background.png"

                Button {
                    id: page1SinglePlayer
                    x: 140
                    width: 200
                    height: 75
                    anchors.top: parent.top
                    anchors.topMargin: 75
                    anchors.horizontalCenter: parent.horizontalCenter
                    elementText: "Single Player"
                    onClicked: {
                        checkGameState("Chess")
                    }
                }

                Button {
                    id: page1TwoPlayers
                    x: 130
                    y: -8
                    width: 200
                    height: 75
                    elementText: "Two Player"
                    anchors.topMargin: 75
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: page1SinglePlayer.bottom
                    onClicked: {
                        checkGameState("Chess")
                    }
                }
            }
        }

        Item {
            id: page2
            property var tabIcon: "qrc:/Placeholder.png"

            Background {
                id: page2Background
                anchors.fill: parent
                imageSource: "qrc:/Shogi_Background.png"

                Button {
                    id: page2SinglePlayer
                    x: 140
                    width: 200
                    height: 75
                    anchors.top: parent.top
                    anchors.topMargin: 75
                    anchors.horizontalCenter: parent.horizontalCenter
                    elementText: "Single Player"
                    onClicked: {
                        checkGameState("Shogi")
                    }
                }

                Button {
                    id: page2TwoPlayers
                    x: 130
                    y: -8
                    width: 200
                    height: 75
                    elementText: "Two Player"
                    anchors.topMargin: 75
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: page2SinglePlayer.bottom
                    onClicked: {
                        checkGameState("Shogi")
                    }
                }
            }
        }

        Item {
            id: page3
            property var tabIcon: "qrc:/Placeholder.png"
        }

        Item {
            id: page4
            property var tabIcon: "qrc:/Placeholder.png"
        }

        Item {
            id: page5
            property var tabIcon: "qrc:/Placeholder.png"
        }

        onCurrentItemChanged: {
            mainMenu.title = "test#" + view.currentIndex
        }
    }

    PageIndicator {
        id: indicator

        count: view.count
        currentIndex: view.currentIndex

        anchors.bottom: view.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        interactive: true

        delegate: Image {
            source: view.contentModel.get(view.currentIndex).tabIcon
            width: Math.min(mainMenu.width / 5, 64)
            height: width
            opacity: index === indicator.currentIndex ? 0.95 : pressed ? 0.7 : 0.45
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(index !== view.currentIndex) {
                        view.setCurrentIndex(index);
                        source = view.contentModel.get(view.currentIndex).tabIcon
                    }
                }
            }
        }
    }

    function checkGameState(playMode){
        switch (view.currentIndex){

            case 0:
                if(chessGame.getGameState())
                    continueDialog.visible = true
                else
                    startGame(false)
                break;

            case 1:
                if(shogiGame.getGameState())
                    continueDialog.visible = true
                else
                    startGame(false)
                break;
        }


    }

    function startGame(resume){
        switch (view.currentIndex){

            case 0:
               if(!resume)
                   chessBoard.newGame()

               chessGameImpl.gameStarted()
               stackView.push("ChessMain.qml");
               break;

            case 1:
                if(!resume)
                    shogiBoard.newGame()

                shogiGameImpl.gameStarted()
                stackView.push("ShogiMain.qml");
                break;
        }
    }

    MessageDialog {
        id: continueDialog
        icon: StandardIcon.Question
        title: qsTr("Ongoing Game")
        text: qsTr("Do you want to start a new Game?")
        standardButtons: StandardButton.No | StandardButton.Yes
        onYes: {
            startGame(false)
        }
        onNo: {
            startGame(true)
        }
    }

    Component.onCompleted: {
        view.currentIndex = (settings.page) ? settings.page : 0
    }

    Component.onDestruction: {
        settings.page = view.currentIndex
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_x:232;anchors_y:209}D{i:3;anchors_y:66}
D{i:2;anchors_y:66}
}
##^##*/
