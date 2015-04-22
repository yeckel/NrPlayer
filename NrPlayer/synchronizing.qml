import QtQuick 2.0

Rectangle {
    id: rectangle1
    width: 800
    height: 600
    color: "#4f78ab"
    anchors.fill: parent
    Text {
        id: pairingNummerText
        x: 275
        y: 229
        text: "Synchronizing"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 130
    }
}
