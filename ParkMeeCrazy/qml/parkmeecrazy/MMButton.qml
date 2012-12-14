import QtQuick 1.0

Rectangle {
    id: container

    property variant text
    signal clicked
    property alias font: text.font
    property string gradientColor: "#999999";

    height: text.height + 10; width: text.width + 20
    border.width: 2
    border.color: "gray"
    radius: 10
    smooth: true

    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: !mouseArea.pressed ? gradientColor : "black"
        }
        GradientStop {
            position: 1.0
            color: !mouseArea.pressed ? "black" : gradientColor
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: container.clicked()
    }

    Text {
        id: text
        anchors.centerIn:parent
        font.pointSize: 10
        text: parent.text
        color: "#cccccc"
    }
}
