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

Item {
    id: container

    anchors.fill: parent

    property alias dialogWidth: dialog.width
    property alias dialogHeight: dialog.height
    property alias content: dialogContent.children

    visible: false

    Rectangle {
        id: fog

        anchors.fill: parent
        color: "black"
    }

    MouseArea {
        anchors.fill: parent
    }

    function show()
    {
        container.state = "show";
    }

    function hide()
    {
        container.state = "hide";
    }

    states: [
        State {
            name: "show"
            PropertyChanges { target: container; visible: true; }
        },
        State {
            name: "hide"
            PropertyChanges { target: container; visible: true; }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "show"
            ParallelAnimation {
                NumberAnimation { target: fog; property: "opacity"; from: 0.0; to: 0.75; duration: 300 }
                NumberAnimation { target: dialog; property: "scale"; from: 0.1; to: 1.0; duration: 300 }
            }
        },
        Transition {
            from: "*"
            to: "hide"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { target: fog; property: "opacity"; from: 0.75; to: 0.0; duration: 300 }
                    NumberAnimation { target: dialog; property: "scale"; from: 1.0; to: 0.1; duration: 300 }
                }
                PropertyAction { target: container; property: "visible"; value: "false" }
            }
        }
    ]

    Rectangle {
        id: dialog

        anchors.centerIn: parent

        radius: 10
        smooth: true
        border.color: "lightgray"

        gradient: Gradient {
            GradientStop { position: 0.0; color: "white" }
            GradientStop { position: 1.0; color: "darkgray" }
        }

        Item {
            id: dialogContent
            anchors.fill: parent
            anchors.margins: 10
        }
    }
}
