import QtQuick 2.5

Rectangle {
    property alias mouseArea: mouseArea
    property alias imageToShow: imageToShow
    width: 360
    height: 360

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Image {
        id:imageToShow
        anchors.fill: parent
    }
}
