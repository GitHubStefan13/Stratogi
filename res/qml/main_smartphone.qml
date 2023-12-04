import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.1
import Qt.labs.settings 1.1

import "MaterialDesign.js" as MD

ApplicationWindow {
    id: applicationWindow
    visible: true
    title: qsTr("Stratogi")
    height: Screen.height * 0.75
    width: Screen.width / 4

    Settings {
        id: settings
        property string style: "Default"
    }

    /**
     * Load font from local
     */
    FontLoader {
        id: iconFont
        source: "MaterialIcons-Regular.ttf"
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

    header: ToolBar {
        id: toolbar
        Material.foreground: "grey"

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                font.pixelSize: toolbar.height / 2
                text: stackView.depth > 1 ? MD.icons.arrow_back : MD.icons.menu
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop()
                        drawer.listView1.currentIndex = -1
                        drawer.listView2.currentIndex = -1
                    } else {
                        drawer.open()
                    }
                }
            }

            Label {
                id: titleLabel
                text: "Stratogi"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                font.pixelSize: toolbar.height / 2
                text: MD.icons.more_vert
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsDialog.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    CustomDrawer {
        id: drawer
        width: Math.min(applicationWindow.width, applicationWindow.height) / 2 // 50% of the Screen
        height: applicationWindow.height

        imageSource: "qrc:Chess_Background.png"
        listView1.model: registerModel // When the User is signed in
        listView2.model: accountModel // When the User needs to register/SignIn

        ListModel {
            id: registerModel

            property string type

            ListElement {
                type: "Form"
                title: "Register"
                icon: "qrc:Register.png"
                source: "qrc:/RegisterForm.qml"
            }

            ListElement {
                type: "Action"
                title: "Exit"
                icon: "qrc:Exit.png"
            }

        }

        ListModel {
            id: accountModel

            ListElement {
                title: "Profile"
                icon: "qrc:Placeholder.png"
                source: "qrc:/MainMenu.qml"
            }

            ListElement {
                title: "Info"
                icon: "qrc:Info.png"
                source: "qrc:/MainMenu.qml"
            }

            ListElement {
                title: "Logout"
                icon: "qrc:Exit.png"
                source: "qrc:/MainMenu.qml"
            }
        }

        onListView1Clicked: {
            var dynamicComp
            var dynamicDialog

            function dynamicObjectCleanup(){
                dynamicDialog.destroy();
                dynamicComp.destroy();
            }

            switch(registerModel.get(index).type){

                case "Form":
                    stackView.push(registerModel.get(index).source);
                    break;

                case "Dialog":
                    dynamicComp = Qt.createComponent(registerModel.get(index).source);
                    dynamicDialog = dynamicComp.createObject(parent, {"x": 100, "y": 100, "visible": true});

                    dynamicDialog.closed.connect(dynamicObjectCleanup)
                    break;

                case "Action":
                    break;

            }


            drawer.close()
        }

        onListView2Clicked: {
            stackView.push(accountModel.get(index).source)
            drawer.close()
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: mainMenu

        MainMenu {
          id: mainMenu
        }
    }

    Dialog {
        id: settingsDialog
        x: Math.round((applicationWindow.width - width) / 2)
        y: Math.round(applicationWindow.height / 6)
        width: Math.round(Math.min(applicationWindow.width, applicationWindow.height) / 3 * 2)
        modal: true
        focus: true
        title: "Settings"

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.style = styleBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: availableStyles
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            Label {
                text: "Restart required"
                color: "#e41e25"
                visible: styleBox.currentIndex !== styleBox.styleIndex ? true : false
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: "About"
        x: (applicationWindow.width - width) / 2
        y: applicationWindow.height / 6
        width: Math.min(applicationWindow.width, applicationWindow.height) / 3 * 2
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: "About Text."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "Longer About Text"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
