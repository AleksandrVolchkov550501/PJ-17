import QtQuick 2.2

Rectangle {
    width: 250
    height: 700
    ListView {
        id: view
        anchors.fill: parent
        model: clmodel
        spacing: 5
//        highlight: Rectangle {
//                    color: "skyblue"
//                    opacity: 0.5
//                }
        highlightFollowsCurrentItem: true

        delegate: Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 50
                color: (model.online === 1) ? "#7FFFD4" : "#E0FFFF"
                Image {
                    id: photo
                    width: 50
                    height: parent.height
                    source: model.photo_50
                }
                Text {
                    id: name
                    anchors.left: photo.right
                    anchors.margins: 2
                    text: model.lastName + " " + model.firstName
                    font.bold: true
                    elide: Text.ElideRight
                }
                Text {
                    id: status
                    color: "#005dff"
                    anchors.top: name.bottom
                    anchors.left: photo.right
                    anchors.right: online.left
                    anchors.margins: 2
                    text: model.status
                    font.italic: true
                    font.pointSize: 7
                    wrapMode: Text.WordWrap
                    elide: Text.ElideRight
                    maximumLineCount: 2
                }

//                Image {
//                    id: online
//                    source: "images/online.png"
//                    width: parent.height / 2
//                    height: parent.height / 2
//                    anchors.right: parent.right
//                    visible: (model.online === 1) ? true : false
//                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: view.currentIndex = model.index
                    onDoubleClicked: {
                        var ind = view.currentIndex
                        parentWidget.slotShowDialog(clmodel.getUserIdByCurIndex(ind))
                    }
                }
            }
    }
}
