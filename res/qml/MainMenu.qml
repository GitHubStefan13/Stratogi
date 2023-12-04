import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import "."

Page {
    id: mainMenu

    property alias backgroundImage: background.imageSource
    property string playMode: settings.playMode

    property var shogiMain // ShogiMain is the Component for "ShogiMain.qml" needs to be created first!
    property var chessMain // ChessMain is the Component for "ChessMain.qml" needs to be created first!

    Settings{
        id: settings

        property var backgroundImage
        property var playMode
    }

    Background {
        id: background
        anchors.fill: parent
        imageSource: settings.backgroundImage

        Column{
            anchors.fill: parent
            anchors.topMargin: 35
            anchors.bottomMargin: 35
            anchors.leftMargin: 75
            anchors.rightMargin: 75
            anchors.horizontalCenter: parent.horizontalCenter

            spacing: 35

            Button {
                id: single_player
                anchors.horizontalCenter: parent.horizontalCenter
                elementText: "Single Player"
                onClicked: {
                    checkGameState(true)
                }
            }

            Button {
                id: two_player
                elementText: "Two Player"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    checkGameState(false)
                }
            }
        }


    }

    function checkGameState(useEngine){
        switch (playMode){
        case "Shogi":
            if(shogiGame.getGameState())
                continueDialog.visible = true
            else
                startGame(false)

            shogiGameImpl.useEngine(useEngine)

            break;

        case "Chess":
            if(chessGame.getGameState())
                continueDialog.visible = true
            else
                startGame(false)

            chessGameImpl.useEngine(useEngine)

            break;
        }
    }

    function startGame(resume){
        switch (playMode){
        case "Shogi":
            if(!resume) // New Game unless we want to resume, Dialog will ask User if needed
                shogiBoard.newGame()

            shogiGameImpl.gameStarted()

            if(!shogiMain) { // Create if not already created
                shogiMain = Qt.createComponent("ShogiMain.qml")
            }

            stackView.push(shogiMain)
            break;

        case "Chess":
            if(!resume) // New Game unless we want to resume, Dialog will ask User if needed
                chessBoard.newGame()

            chessGameImpl.gameStarted()

            if(!chessMain) // Create if not already created
                chessMain = Qt.createComponent("ChessMain.qml")

            stackView.push(chessMain);
            break;
        }
    }

    function newGame() {
        switch (playMode){
        case "Shogi":
            shogiGameImpl.newGame()
            shogiGameImpl.gameStarted()
            break;

        case "Chess":
            chessGameImpl.newGame()
            chessGameImpl.gameStarted()

            break;
        }
    }

    function surrenderGame() {
        switch (playMode){
        case "Shogi":
            break;

        case "Chess":
            break;
        }
    }

    function showEngineSettings() {
        engineSettingDialog.title = playMode + "-Engine Settings"
        engineSettingDialog.game = playMode
        engineSettingDialog.appWindowSize = Qt.size( parent.width, parent.height )

        engineSettingDialog.visible = true
    }


    Component.onCompleted: {
        mainMenu.backgroundImage = (settings.backgroundImage) ? settings.backgroundImage
                                                              : ""
        mainMenu.playMode = (settings.playMode) ? settings.playMode
                                                : "Chess"
    }

    Component.onDestruction: {
        settings.backgroundImage = mainMenu.backgroundImage
        settings.playMode = mainMenu.playMode

        if(shogiMain)
          shogiMain.destroy()
        if(chessMain)
          chessMain.destroy()
    }

    MessageDialog {
        id: continueDialog
        text: qsTr("Ongoing Game")
        informativeText: qsTr("Do you want to start a new Game?")
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onButtonClicked: function (button, role) {
            switch (button) {
                case MessageDialog.Ok:
                    startGame(true)
                    break;

                case MessageDialog.No:
                    startGame(false)
                    break;
            }
        }
    }

    EngineSettingDialog {
        id: engineSettingDialog
    }
}


