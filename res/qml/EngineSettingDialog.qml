import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: engineSettings
    property size appWindowSize;
    property var game; // The Game the Engine is adjusting the Settings for

    title: qsTr( "Engine Settings" )
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    width: appWindowSize.width - 5
    height: appWindowSize.height - 5

    property int qt_USERROLE: 256
    property int qt_NAMEROLE: qt_USERROLE + 1
    property int qt_TYPEROLE: qt_NAMEROLE + 1
    property int qt_VALUEROLE: qt_TYPEROLE + 1
    property int qt_MINROLE: qt_VALUEROLE + 1
    property int qt_MAXROLE: qt_MINROLE + 1
    property int qt_QVARROLE: qt_MAXROLE + 1

    Rectangle {
        id: rectangle
        color: "#00000000"
        anchors.fill: parent

        ListView {
            id: optionSettingList

            clip: true
            anchors.fill: parent
            model: {
                switch(game) {
                  case "Chess":
                      chessOptionSettingModel;
                      break;
                  case "Shogi":
                      shogiOptionSettingModel;
                      break;
                  default:
                      print("WARNING! optionSettings does not contain a model with the game:" + game)
                      break;
                }
            }

            delegate: Component {
                id: optionSettingComponent
                Loader {
                    height: Math.max(engineSettings.height / 20, 20)
                    width: optionSettingList.width

                    sourceComponent: {
                        switch(type){
                          case "check": check; break;
                          case "spin": spin; break;
                          case "combo": combo; break;
                          case "button": button; break;
                          case "string": string; break;
                          case "filename": filename; break;
                        }
                    }

                    // Loader binds properties to the source component passed to the view delegate
                    // To see which properties need binding look at the C++ Model
                    property string _name: name
                    property string _type: type
                    property string _value: value
                    property int _min: min
                    property int _max: max
                    property var _qvar: qvar

                    property var _index: index
                }
            }
        }

        // check: A checkbox that can either be true or false.
        Component {
            id: check

            CheckBox {
                LayoutMirroring.enabled: true
                text: _name
                checked: _value

                onCheckedChanged: {
                  optionSettingList.model.setData(chessOptionSettingModel.index(_index, 0), checked, qt_VALUEROLE)
                }
            }
        }

        // spin: A spin wheel or slider that can be an integer in a certain range.
        Component {
            id: spin

            Rectangle {
                anchors.fill: parent
                color: "#00000000"

                RowLayout {
                    anchors.fill: parent

                    Text {
                        text: _name
                        fontSizeMode: Text.VerticalFit
                        Layout.fillWidth: true
                    }

                    SpinBox {
                        //Layout.fillWidth: true
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        wheelEnabled: true
                        editable: true
                        stepSize: 1
                        from: _min
                        value: _value
                        to: _max

                        onValueChanged: {
                            chessOptionSettingModel.setData(chessOptionSettingModel.index(_index, 0), value, qt_VALUEROLE)
                        }
                    }
                }
            }
        }

        // combo: A combo box that can have different predefined strings as a value.
        Component {
            id: combo

            RowLayout {
                anchors.fill: parent

                Text {
                    text: _name
                    fontSizeMode: Text.VerticalFit
                }

                ComboBox {
                    Layout.alignment: Qt.AlignRight
                    model: _qvar
                    displayText: _value

                    onDisplayTextChanged: {
                        chessOptionSettingModel.setData(chessOptionSettingModel.index(_index, 0), currentText, qt_QVARROLE)
                    }
                }
            }
        }

        // button: A button that can be pressed to send a command to the engine
        Component {
            id: button

            RoundButton {
                Layout.alignment: Qt.AlignRight
                text: _name

                onClicked: {
                    chessOptionSettingModel.setData(chessOptionSettingModel.index(_index, 0), _value, VALUEROLE)
                }
            }
        }

        // string: A text field that has a string as a value, an empty string has the value <empty>.
        Component {
            id: string

            Text {
                anchors.fill: parent
                fontSizeMode: Text.VerticalFit
                text: _name
            }
        }

        // filename: Similar to string, but is presented as a file browser instead of a text field in the GUI.
        Component {
            id: filename
            Text {
                text: _name
                anchors.fill: parent
                fontSizeMode: Text.VerticalFit
            }
        }

    }
}


