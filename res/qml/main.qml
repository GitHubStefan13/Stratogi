import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Controls.Material
import QtQuick.Controls.Universal
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: applicationWindow
    visible: true

    Settings {
        id: settings

        property var x
        property var y
        property var width
        property var height
    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: {
            stackView.pop()
            listView.currentIndex = -1
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenu.open()
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File(F)")
            Action {
                text: qsTr("&New Game(N)")
                onTriggered: {
                   mainMenu.newGame()
                }
            }
            Action {
                text: qsTr("&Surrender(S)")
                onTriggered: {
                    mainMenu.surrenderGame()
                }
            }
            MenuSeparator { }
            Action {
                text: qsTr("&Quit")
                onTriggered: {
                    Qt.quit()
                }
            }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("&Empty") }
        }

        MenuSeparator { }

        Menu {
          title: qsTr("&Game Settings(G)")
          Menu {
            title: qsTr("&Change Game(C)")
            Action {
                text: qsTr("&Shogi")
                onTriggered: {
                    mainMenu.playMode = "Shogi"
                    mainMenu.backgroundImage = "qrc:/Shogi_Background.png"
                }
            }
            Action {
                text: qsTr("&Chess")
                onTriggered: {
                    mainMenu.playMode = "Chess"
                    mainMenu.backgroundImage = "qrc:/Chess_Background.png"
                }
            }
          }
          Action {
              text: qsTr("&Engine Options")
              onTriggered: {
                  mainMenu.showEngineSettings()
              }
          }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: mainMenu

        MainMenu {
          id: mainMenu
          //anchors.fill: parent
        }
    }

    Component.onCompleted: {
      restoreWindowGeometry()
    }

    Component.onDestruction: {
      saveWindowGeometry()
    }

    function saveWindowGeometry() {
        settings.x = applicationWindow.x
        settings.y = applicationWindow.y
        settings.width  = applicationWindow.width
        settings.height = applicationWindow.height
    }

    function restoreWindowGeometry() {
        applicationWindow.width = (settings.width)  ? settings.width : 640
        applicationWindow.height = (settings.height) ? settings.height : 720

        applicationWindow.x = (settings.x) ? settings.x
                                : Screen.width / 2 - applicationWindow.width / 2
        applicationWindow.y = (settings.y) ? settings.y
                                : Screen.height / 2 - applicationWindow.height / 2
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
