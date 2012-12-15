/****************************************************************************
**
** Copyright (C) 2011 Kirill (spirit) Klochkov.
** Contact: klochkov.kirill@gmail.com
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

import QtQuick 1.0

Rectangle {
    id: button

    property alias text: text.text
    property alias font: text.font

    property variant normalGradient: Gradient {
        GradientStop { position: 0.0; color: Qt.lighter(Qt.lighter("blue")) }
        GradientStop { position: 0.3; color: Qt.lighter("blue") }
        GradientStop { position: 0.4; color: "blue" }
        GradientStop { position: 1.0; color: Qt.darker("blue") }
    }
    property variant pressedGradient: Gradient {
        GradientStop { position: 1.0; color: Qt.lighter(Qt.lighter("blue")) }
        GradientStop { position: 0.4; color: Qt.lighter("blue") }
        GradientStop { position: 0.3; color: "blue" }
        GradientStop { position: 0.0; color: Qt.darker("blue") }
    }

    signal clicked()

    border.color: "lightgray"
    smooth: true
    radius: 5

    width: text.width + 10
    height: text.height + 10

    gradient: normalGradient

    Text {
        id: text

        anchors.centerIn: parent
        smooth: true

        color: "white"

        font.bold: true
        font.pixelSize: 26
    }

    MouseArea {
        anchors.fill: parent
        onPressed: parent.gradient = parent.pressedGradient
        onReleased: parent.gradient = parent.normalGradient
        onClicked: button.clicked()
    }
}
