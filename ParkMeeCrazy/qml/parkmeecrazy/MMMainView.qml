import QtQuick 1.0

Rectangle {
    id: box
    width: 768
    height: 1280
    color: "black";

    Rectangle {
        id: popupWon
        visible: false
        anchors.centerIn: parent
        z: 999
        width: 450
        height: 250
        color: "black"
        radius: 20
        border.color: "gray"
        border.width: 5

        function dismiss() {
            popupWon.visible=false;
            titleLogo.opacity = 1;
            board.opacity = 1;
            board.enabled = true;
            toolBox.opacity = 1;
            toolBox.enabled = true;
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top
            anchors.topMargin: 40
            font.pixelSize: 48
            text: "You've won!\nTotal moves: " + board.boardMoves
            color: "red"
            horizontalAlignment: Text.AlignHCenter

        }
        MMButton {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.bottomMargin: 10
            height: 50
            text: " Restart Level "
            gradientColor: "#550000"
            font.pixelSize: 28
            radius: 10
            onClicked: {
                parent.dismiss();
                if(boardUtils.loadLevel(board.levelIndex)) {
                    board.refreshBoardData();
                }
            }
        }
        MMButton {
            anchors.bottom: parent.bottom
            anchors.rightMargin: 20
            anchors.right: parent.right
            anchors.bottomMargin: 10
            gradientColor: "#005500"
            height: 50
            text: " Next Level "
            font.pixelSize: 28
            radius: 10
            onClicked: {
                parent.dismiss();
                if(boardUtils.loadLevel(board.levelIndex+1)) {
                    board.refreshBoardData();
                }
            }
        }
    }

    Rectangle {
        id: chooseLevel
        visible: false
        //anchors.horizontalCenter: parent.horizontalCenter
        //anchors.centerIn: parent
        //anchors.left:parent.left
        y:0;
        x:-300;
        z: 999
        width: 300
        height: parent.height
        color: "black"
        //radius: 20
        border.color: "gray"
        border.width: 5

        NumberAnimation {
            id: animShow;
            running: chooseLevel.visible;
            target: chooseLevel; property: "x"; from: -300; to: 0; duration: 100
        }
        NumberAnimation {
            id: animHide;
            running: false;
            target: chooseLevel; property: "x"; from: 0; to: -300; duration: 100
            onCompleted: chooseLevel.visible=false;
        }

        function dismiss() {
            animHide.running = true;
            titleLogo.opacity = 1;
            board.opacity = 1;
            board.enabled = true;
            toolBox.opacity = 1;
            toolBox.enabled = true;
        }

        ListView {
            id: lvLevels
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            //anchors.bottom: btnOK.top
            //anchors.fill: parent
            model: boardUtils.getMaxLevels()
            width: 170;
            boundsBehavior: Flickable.StopAtBounds;
            delegate: Rectangle {
                color: (ListView.isCurrentItem ? "orange": "black")
                border.width: 1
                border.color: "gray"
                width: parent.width
                property int crtIndex: index
                property int isCrtItem: ListView.isCurrentItem?1:0
                height: 50;
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    color: parent.isCrtItem ? "red" : "white"
                    text: "Level #" + (index+1)
                    font.pixelSize: 30
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        lvLevels.currentIndex = crtIndex;
                    }
                }
            }
        }

        MMButton {
            id: btnOK
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 10
            gradientColor: "#005500"
            text: " OK "
            font.pixelSize: 24
            radius: 10
            height: 60
            width: 105
            onClicked: {
                parent.dismiss();
                if(boardUtils.loadLevel(lvLevels.currentIndex+1)) {
                    board.refreshBoardData();
                }
            }
        }
        MMButton {
            anchors.top: btnOK.bottom
            anchors.topMargin: 60
            anchors.right: parent.right
            anchors.rightMargin: 10
            gradientColor: "#550000"
            text: " Cancel "
            font.pixelSize: 24
            radius: 10
            height: 60
            width: 105
            onClicked: {
                parent.dismiss();
            }
        }
    }

    function youHaveWon() {
        popupWon.visible=true;
        titleLogo.opacity = 0.4;
        board.opacity = 0.8;
        board.enabled = false;
        toolBox.opacity = 0.4;
        toolBox.enabled = false;
    }

    function doChooseLevel() {
        chooseLevel.visible=true;
        titleLogo.opacity = 0.4;
        board.opacity = 0.8;
        board.enabled = false;
        toolBox.opacity = 0.4;
        toolBox.enabled = false;
    }

    function doAbout() {
        about.visible=true;
        titleLogo.opacity = 0.4;
        board.opacity = 0.8;
        board.enabled = false;
        toolBox.opacity = 0.4;
        toolBox.enabled = false;
    }

    Rectangle {
        id: about
        visible: false
        anchors.centerIn: parent
        z: 999
        width: 550
        height: 850
        color: "black"
        radius: 50
        border.color: "gray"
        border.width: 5

        function dismiss() {
            about.visible=false;
            titleLogo.opacity = 1;
            board.opacity = 1;
            board.enabled = true;
            toolBox.opacity = 1;
            toolBox.enabled = true;
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter;
            anchors.top: parent.top
            anchors.topMargin: 20
            font.pixelSize: 24
            text: "A 'Rush Hour'/'Traffic Jam' game clone\n\n" +
                  "Objective: move the red rectangle\n"+
                  "to the exit on the right by dragging\n" +
                  "other rectangles out of the way.\n\n" +
                  "Note: Horizontal rectangles can move\n"+
                  "only left and right.\n" +
                  "Vertical rectangles can move only\n"+
                  "up and down.\n\n" +
                  "Good Luck!\n\n" +
                  "Ported to BlackBerry 10 by: \n" +
                  "Antti Pohjolam antti.pohjola@gmail.com\n"+
                  "ParkMeeCrazy is licenced under GPLv2 license\n"+
                  "you can donwload source from: \n"+
                  "https://github.com/Summeli/BlackBerry-10-Port-A-Thon\n\n"+
                  "Original Creator:: Mures Andone \n<wmures@gmail.com>"
            color: "#cccccc"
            horizontalAlignment: Text.AlignLeft
        }
        MMButton {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 10
            text: " OK "
            font.pixelSize: 28
            gradientColor: "#005500"
            radius: 10
            height: 60
            width:120
            onClicked: {
                parent.dismiss();
            }
        }
    }

    Column {
        Rectangle {
            id: titleLogo
            width: parent.width
            height: 60
            color: "black"
            Row {
                anchors.verticalCenter: parent.verticalCenter
                x: 10
                Text {
                    text: "Park"
                    font.pixelSize: 56
                    color: "orange"
                }
                Text {
                    text: "Mee"
                    font.pixelSize: 56
                    color: "steelblue"
                    font.bold: true
                }
                Text {
                    text: "Crazy!"
                    font.pixelSize: 56
                    color: "darkred"
                }
            }
        }
        Rectangle {
            id: emptySpace 
            height: 50  
            width: box.width
            color: "black"
        }
        Board {
            id: board;
            width: box.width;
            height: box.width;
            gameStarted: 1
            onWon: box.youHaveWon();
        }

        Rectangle {
            id: someEmptySpace
            height: 150
            width: box.width;
            color: "black"
        }
        Rectangle {
            id: toolBox
            width: parent.width
            height: box.height - titleLogo.height - board.height
            color: "black"
            Text {
                id: levelTxt
                anchors.top: parent.top
                text: " Level: " + board.levelIndex
                color: "#ffaaaaaa"
                font.pixelSize: 48
            }
            Text {
                id: movesTxt
                anchors.top: parent.top
                anchors.right: parent.right
                text: "Moves: " + board.boardMoves + " "
                color: "#ffaaaaaa"
                font.pixelSize: 48
            }
            MMButton {
                id: btnNext
                anchors.top: levelTxt.bottom
                anchors.topMargin: 10
                text: " Next Level"
                color: "#ffaaaaaa"
                font.pixelSize: 24
                gradientColor: "#005500"
                height: 50
                width:200
                onClicked: {
                    if(boardUtils.loadLevel(board.levelIndex + 1)) {
                        board.refreshBoardData();
                    }
                }
            }
            MMButton {
                id: btnRestart
                anchors.top: movesTxt.bottom
                anchors.right: parent.right
                anchors.topMargin: 10
                text: "Restart Level "
                color: "#ffaaaaaa"
                font.pixelSize: 24
                gradientColor: "#005500"
                height: 50
                width:200
                onClicked: {
                    console.log("called restart level, current level: "+board.levelIndex);
                    boardUtils.restartLevel();
                    board.refreshBoardData();
                }
            }
            MMButton {
                id: btnJump
                anchors.top: btnNext.bottom
                anchors.topMargin: 20
                text: " Jump to Level"
                color: "#ffaaaaaa"
                font.pixelSize: 24
                gradientColor: "#005500"
                height: 50
                width:200
                onClicked: {
                    box.doChooseLevel();
                }
            }
            MMButton {
                id: btnQuit
                anchors.top: btnRestart.bottom
                anchors.right: parent.right
                anchors.topMargin: 20
                text: " About "
                color: "#ffaaaaaa"
                font.pixelSize: 24
                gradientColor: "#005500"
                height: 50
                width:200
                onClicked: {
                    //box.youHaveWon();
                    //box.doChooseLevel();
                    box.doAbout();
                }
            }
        }
    }
}

