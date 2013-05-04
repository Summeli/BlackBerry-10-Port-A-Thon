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
import flood 1.0

Rectangle {
    id: window

    //width: 768
    //height: 1280
    width: 720
    height: 720
    
    gradient: Gradient {
        GradientStop { position: 1.0; color: "white" }
        GradientStop { position: 0.0; color: "darkgray" }
    }

    FloodModel {
        id: floodModel

        rows: 12
        columns: 12
        colors: internal.colorModel

        onColorChanged: {
            var rectangle = board.children[row * floodModel.columns + column];
            var oldColor = rectangle.color
            rectangle.color = floodModel.color(row, column);
            rectangle.setAnimationColors(oldColor, floodModel.color(row, column));
        }

        onNewArrangement: board.initBoard()
    }

    Statistics {
        id: statistics

        onSaved: statisticsRepeater.model = statisticsRepeater.updateModel()
        onCleared: statisticsRepeater.model = statisticsRepeater.updateModel()
    }

    QtObject {
        id: internal

        property variant colorModel: ["red", "yellow", "green", "blue", "purple", "cyan"]
        property int controlButtonsMargin: 5
        property int stepsCount: floodModel.rows + floodModel.columns
        property int currentStep: 0
        property bool statisticsSaved: false
        property int windowSize: Math.min(window.width, window.height - gameIndicatorLayout.childrenRect.height)
        property int boardSize: 0.85 * windowSize
        property int cellSize: boardSize / floodModel.columns
        property int colorButtonSize: 2 * cellSize - controlButtonsMargin

        function startNewGame()
        {
            internal.statisticsSaved = false;
            internal.currentStep = 0;
            floodModel.init();
        }
    }

    Column {
        id: mainLayout

        anchors.centerIn: parent

        spacing: 5

        Item {
            anchors.horizontalCenter: parent.horizontalCenter

            width: internal.boardSize
            height: internal.boardSize

            Grid {
                id: board

                anchors.centerIn: parent
                width: parent.width
                height: parent.height

                rows: floodModel.rows
                columns: floodModel.columns

                function initBoard()
                {
                    repeater.model = board.colorModel();
                }

                Repeater {
                    id: repeater

                    model: board.colorModel()

                    Rectangle {
                        id: cell

                        width: internal.cellSize
                        height: internal.cellSize
                        color: modelData
                        border.color: "lightgray"
                        smooth: true
                        radius: 5

                        function setAnimationColors(fromColor, toColor)
                        {
                            cellAnimation.from = fromColor;
                            cellAnimation.to = toColor;
                            cellAnimation.start();
                        }

                        ColorAnimation on color { id: cellAnimation; duration: 500 }

                        gradient: Gradient {
                            GradientStop { position: 0.0; color: Qt.lighter(Qt.lighter(cell.color)) }
                            GradientStop { position: 0.3; color: Qt.lighter(cell.color) }
                            GradientStop { position: 0.4; color: cell.color }
                            GradientStop { position: 1.0; color: Qt.darker(cell.color) }
                        }

                        Behavior on opacity { NumberAnimation { duration: Math.random() * 2000 } }

                        opacity: 0.0

                        Component.onCompleted: cell.opacity = 1.0
                    }
                }

                function colorModel()
                {
                    var res = new Array();
                    for (var row = 0; row < floodModel.rows; ++row) {
                        for (var column = 0; column < floodModel.columns; ++column)
                            res.push(floodModel.color(row, column));
                    }
                    return res;
                }
            }
        }
        
        Row {
            id: colorButtonsLayout
            spacing: 5

            anchors.horizontalCenter: parent.horizontalCenter

            Repeater {
                model: internal.colorModel

                Button {
                    id: button

                    width: internal.colorButtonSize
                    height: internal.colorButtonSize

                    color: modelData

                    normalGradient: Gradient {
                        GradientStop { position: 0.0; color: Qt.lighter(Qt.lighter(modelData)) }
                        GradientStop { position: 0.3; color: Qt.lighter(modelData) }
                        GradientStop { position: 0.4; color: modelData }
                        GradientStop { position: 1.0; color: Qt.darker(modelData) }
                    }
                    pressedGradient: Gradient {
                        GradientStop { position: 1.0; color: Qt.lighter(Qt.lighter(modelData)) }
                        GradientStop { position: 0.4; color: Qt.lighter(modelData) }
                        GradientStop { position: 0.3; color: modelData }
                        GradientStop { position: 0.0; color: Qt.darker(modelData) }
                    }

                    onClicked: {
                        if (internal.currentStep < internal.stepsCount && !floodModel.flooded) {
                            ++internal.currentStep;
                            floodModel.setColor(0, 0, button.color);
                        }

                        if (floodModel.flooded || internal.currentStep == internal.stepsCount) {
                            informationDialog.text = floodModel.flooded ? qsTr("You won!\nTo start a new game press 'New game'.")
                                                                        : qsTr("The game is lost.\nTo start a new game press 'New game'.");
                            informationDialog.show();
                            if (!internal.statisticsSaved) {
                                statistics.saveStatistics(floodModel.rows, internal.currentStep, floodModel.flooded ? 1 : 0);
                                internal.statisticsSaved = true;
                            }
                        }
                    }
                }
            }
        }

        
        Row {
            id: gameIndicatorLayout
            spacing: 50

            anchors.horizontalCenter: parent.horizontalCenter

            Item {
                width: scoreText.width + 10
                height: scoreText.height + 10

                Text {
                    id: scoreText
                    anchors.centerIn: parent
                    text: qsTr("Steps: %1/%2").arg(internal.currentStep).arg(internal.stepsCount)
                    font.bold: true
                    font.pixelSize: 26
                }
            }

            Button {
                text: qsTr("New game")
                font.pixelSize: 26
                onClicked: internal.startNewGame()
            }

            Button {
                text: qsTr("Help")
                font.pixelSize: 26
                onClicked: {
                    informationDialog.text = qsTr("The game starts at top left corner.\nThe goal of the game it's to fill a board with one color.\nA color can be selected by pressing one of 6 colored buttons.\nGood luck!");
                    informationDialog.show();
                }
            }
           Button {
               text: qsTr("About")
               font.pixelSize: 26
               onClicked: {
                     informationDialog.text = qsTr("Flood v0.2\n Flood is Licenced under GNU Lesser General Public License \n\n BlackBerry 10 Port By: Antti Pohjola antti.pohjola@gmail.com\n you can find the project sources at: \n"+
                     "https://github.com/Summeli/BlackBerry-10-Port-A-Thon \n Original Author: 2011 Kirill (spirit) Klochkov.");
                     informationDialog.show();
              }
           }
        }
    }

    Dialog {
        id: informationDialog
        property alias text: text.text
        property int statsOpacity: 0
        dialogWidth: 600//buttons.width + 2 * buttons.spacing
        dialogHeight: 700 // contentItem.height

        content: [
            Item {
                id: contentItem

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right

                height: textLayout.height + buttons.height + 2 * textLayout.spacing + buttons.anchors.bottomMargin

                Column {
                    id: textLayout

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    spacing: 20

                    Text {
                        id: text

                        width: parent.width - 10

                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                        font.pixelSize: 26
                    }

                    Grid {
                        anchors.horizontalCenter: parent.horizontalCenter
                        columns: 2
                        rows: 7
                        Repeater {
                            id: statisticsRepeater

                            model: updateModel()

                            Text {
                                text: modelData
                                font.pixelSize: 22
                            }

                            function updateModel()
                            {
                                var wins = statistics.wins(floodModel.rows);
                                var bestBoard = statistics.bestBoard(floodModel.rows);
                                var gamesPlayed = statistics.gamesPlayed(floodModel.rows);
                                var winsPercentage = gamesPlayed > 0 ? Math.round(100 * wins / gamesPlayed) : 0;
                                var losesPercentage = gamesPlayed > 0 ? 100 - winsPercentage : 0;

                                var res = new Array();
                                res.push(qsTr("Board size: "));
                                res.push(qsTr("%1x%2").arg(floodModel.rows).arg(floodModel.columns));

                                res.push(qsTr("Best board: "));
                                res.push(qsTr("%1 step(s)").arg(bestBoard));

                                res.push(qsTr("Games played: "));
                                res.push(gamesPlayed);

                                res.push(qsTr("Games won: "));
                                res.push(wins);

                                res.push(qsTr("Games lost: "));
                                res.push(gamesPlayed - wins);

                                res.push(qsTr("Won: "));
                                res.push(qsTr("%1 %").arg(winsPercentage));

                                res.push(qsTr("Lost: "));
                                res.push(qsTr("%1 %").arg(losesPercentage));

                                return res;
                            }
                        }
                    }
                }
            },
            Row {
                id: buttons

                spacing: 15

                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    id: okButton
                    text: qsTr("OK")
                    font.pixelSize: 26
                    width: clearStatisticsButton.width

                    onClicked: informationDialog.hide()
                }
                Button {
                    id: newGameButton
                    text: qsTr("New game")
                    font.pixelSize: 26
                    width: clearStatisticsButton.width
                    height: okButton.height

                    onClicked: { informationDialog.hide(); internal.startNewGame(); }
                }
                Button {
                    id: clearStatisticsButton
                    text: qsTr("Clear statistics")
                    font.pixelSize: 26
                    height: okButton.height

                    onClicked: statistics.clearStatistics()
                }
            }
        ]
    }
    
}
