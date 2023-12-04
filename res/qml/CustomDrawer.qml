import QtQuick
import QtQuick.Controls

Drawer {
    id: drawerTemplate
    width: parent.width / 2 // 50% of the Screen
    height: parent.height
    interactive: stackView.depth === 1

    signal listView1Clicked(int index)
    signal listView2Clicked(int index)

    property alias listView1: listView1
    property alias listView2: listView2
    property alias imageSource: image.source
    property alias imageText: imageText.text
    property bool showMode: true
    property string switchIconUPSource: ""
    property string switchIconDOWNSource: ""
    Column{
        id: vBox
        anchors.fill: parent

        Rectangle{
            id: currentPi

            width: vBox.width
            height: vBox.height * 0.2
            color: "transparent"
            border.color: "transparent"

            Image {
                id: image
                anchors.fill: parent
                source: "qrc:/Placeholder.qml"
            }

            Text{
                id: imageText
                width: currentPi.width * 0.75
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10

                text: ""
            }

            Image{
                id: switchIcon
                x: imageText.x + imageText.width + 2
                width: height
                height: imageText.height
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10


                source: if(drawerTemplate.showMode) switchIconUPSource; else switchIconDOWNSource;
            }

            MouseArea{
                x: imageText.x
                y: imageText.y
                width: imageText.width + imageText.height
                height: imageText.height
                onClicked: {
                    if(showMode)
                      showMode = false
                    else
                      showMode = true
                }
            }
        }

        ListView {
            id: listView1
            clip: true
            width: vBox.width
            height: vBox.height - currentPi.height

            visible: if(showMode) true; else false;
            focus: true
            currentIndex: -1
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }

            delegate: ItemDelegate {
                id: listItem1
                width: listView1.width
                height: listView1.height / 12

                highlighted: ListView.isCurrentItem

                property int _index: index

                Row{
                    id: hBox1
                    anchors.fill: parent
                    anchors.topMargin: 2
                    anchors.bottomMargin: 2
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2

                    Image {
                        id: listImage1
                        width: hBox1.width * 0.2
                        height: width
                        source: model.icon;
                    }

                    Text {
                       id: listText1
                       width: hBox1.width - listImage1.width
                       height: listItem1.height

                       renderType: Text.NativeRendering
                       font.pixelSize: 18
                       font.family: "Arial"
                       font.bold: true
                       font.capitalization: Font.MixedCase
                       fontSizeMode: Text.Fit
                       style: Text.Normal
                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter
                       textFormat: Text.AutoText


                       text: model.title
                    }
                }

                Rectangle {
                    id: rectangle1
                    width: listItem1.width
                    height: 1
                    color: "black"
                }

                onClicked: {
                    listView1Clicked(listItem1._index)
                    listView1.currentIndex = listItem1._index
                }
            }

            model: drawerModel1

            ScrollIndicator.vertical: ScrollIndicator { }

            // Overwrite in the actual Control with listView1.model = YOUR_MODEL
            ListModel {
                id: drawerModel1

                ListElement {
                    title: "Template Item 1"
                    icon: "qrc:template_icon.png"
                    source: "qrc:/Placeholder.qml"
                }
                ListElement {
                    title: "Template Item 2"
                    icon: "qrc:template_icon.png"
                    source: "qrc:/Placeholder.qml"
                }
            }
        }

        ListView {
            id: listView2
            clip: true
            width: vBox.width
            height: vBox.height - currentPi.height
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }

            visible: if(showMode) false; else true;
            focus: true

            delegate: ItemDelegate {
                id: listItem2
                width: listView2.width
                height: listView2.height / 12
                highlighted: ListView.isCurrentItem

                property int _index: index

                Text {
                   id: listText2
                   width: listItem2.width
                   height: listItem2.height
                   color: ListView.isCurrentItem ? "red" : "black"

                   renderType: Text.NativeRendering
                   font.pixelSize: 18
                   font.family: "Arial"
                   font.bold: true
                   font.capitalization: Font.MixedCase
                   fontSizeMode: Text.Fit
                   style: Text.Normal
                   horizontalAlignment: Text.AlignHCenter
                   verticalAlignment: Text.AlignVCenter
                   textFormat: Text.AutoText

                   text: model.title
                }

                onClicked: {
                    listView2Clicked(listItem2._index)
                    listView2.currentIndex = listItem2._index
                }
            }

            model: drawerModel2

            ScrollIndicator.vertical: ScrollIndicator { }

            // Overwrite in the actual Control with listView2.model = YOUR_MODEL
            ListModel {
                id: drawerModel2

                ListElement {
                    title: "Template Item 1"
                }
                ListElement {
                    title: "Template Item 2"
                }
            }
        }


    }
}
