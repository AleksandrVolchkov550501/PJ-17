import QtQuick 2.6
import QtQuick.Window 2.2
import QtWebEngine 1.5
import QtWebView 1.1
import QtQuick.Controls 2.1

Item {
    id: win
    width: 800
    height: 600
    visible: true
    BusyIndicator {
        id: busyInd
        anchors.centerIn: parent
        visible: true

    }
    WebEngineView {
        id: wev
        anchors.fill: parent
        url: "https://oauth.vk.com/authorize?client_id=6059494&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=70658&response_type=token&v=5.65&state=123456"
//        url: "https://m.vk.com/feed"
        onUrlChanged: {
            var rx = new RegExp(".*access_token.*")
            if(rx.test(url)) {
            wev.visible = false
            usrProf.parseResponseUrl(url)
            textAuthDone.visible = true }
        }
        onLoadingChanged: {
            busyInd.visible = false
        }
    }
    Text {
        id: textAuthDone
        anchors.centerIn: parent
        scale: 3
        visible: false
        text: qsTr("Авторизация успешно завершена!")
    }
}
