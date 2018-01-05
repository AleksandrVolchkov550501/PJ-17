import QtQuick 2.0
import QtQuick.Window 2.0

Rectangle {
    id: dialWindow
    width: 800
    height: 600
    color: "#424242"

    signal sendMessage(string msg)

    // Модель данных для теста интерфейса
//    ListModel {
//        id: lm
//        ListElement {
//            time: "10:10:10"
//            body: "Test message Test message Test message Test message Test message Test message Test message Test message"
//            from_id: 1
//            user_id: 1
//        }
//        ListElement {
//            time: "10:10:10"
//            body: "Test message Test message Test message Test message Test message Test message Test message Test message"
//            from_id: 1
//            user_id: 1
//        }
//        ListElement {
//            time: "11:11:11"
//            body: "Test message Test message Test message Test message Test message Test message Test message Test message"
//            from_id: 2
//            user_id: 1
//        }
//    }

    ListView {
        id: view
        anchors {
            left: parent.left
            right: parent.right
            bottom: textRect.top
            top: infoPanel.bottom
            margins: 10
        }
        model: dialModel
        verticalLayoutDirection: ListView.BottomToTop

//        section.property: "from_id"       // TODO: подумать над реализацией
//        section.delegate: Rectangle {
//            width: parent.width
//            height: 10
//            color: dialWindow.color
//            radius: 10
//        }

        delegate: Column {
                id: delegate
                width: parent.width
                Rectangle {
                    id: textBgr
                    height: body.height + 10
                    color: (model.user_id === model.from_id) ? "#FFC0CB" : "#E5E5E5"
                    width: parent.width
                    anchors.left: delegate.left
                    Text {
                        id: body
                        text: "[" + model.time + "]" + " " + model.body
                        width: parent.width - 20
                        anchors.leftMargin: 20
                        wrapMode: Text.WordWrap
                        font.pointSize: 12
                    }
                }
            }
    }
    Rectangle {
        id: textRect
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }
        radius: 10
        height: 100
        TextInput {
            id: textInput
            anchors.fill: parent
            anchors.margins: 10
            focus: true
            selectByMouse: true
            font.pointSize: 12
            wrapMode: TextInput.WordWrap
            onAccepted: {
                dialWindow.sendMessage(textInput.text)
                textInput.clear()
            }

        }
    }
    Rectangle {
        color: parent.color
        id: infoPanel
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: 90
        Rectangle {
            radius: 10
            anchors.fill: parent
            anchors.margins: 10
            Image {
                id: photo
                source: companion.getPhoto_50()
                anchors {
                    left: parent.left
                    margins: 10
                    top: parent.top
                }
            }
            Text {
                id: name
                text: companion.getFirstName() + " " + companion.getLastName()
                anchors {
                    left: photo.right
                    right: parent.right
                    baseline: parent.verticalCenter
                    margins: 10
                }

                font.pointSize: 10
                font.bold: true
            }
        }
    }
}
