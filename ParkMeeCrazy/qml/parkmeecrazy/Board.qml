import QtQuick 1.0


BorderImage {
    border { left: 30; top: 30; right: 30; bottom: 30 }
    horizontalTileMode: BorderImage.Repeat
    verticalTileMode: BorderImage.Round
    source: "border.png"
//Rectangle {
    id: boardContainer
    width: 380; height: 380
    //color:"darkorange"
    property int boardMoves: 0
    property int gameStarted: 0
    property int borderWidth: 32
    property int levelIndex: 0

    Component.onCompleted: {
        refreshBoardData();
    }

    signal won();

    function refreshBoardData() {
        levelIndex = boardUtils.getCurrentLevel();
        boardMoves = boardUtils.getCurrentMoves();
    }

    Rectangle {
        id: boardExit
        width: borderWidth/2
        height: board.cellSize + 10
        //exit is always at cell 5,2
        x: 6 * board.cellSize + borderWidth/2 - 4
        y: 2 * board.cellSize + borderWidth/2 - 2
        color: "black"
        z: 99
        Text {
            anchors.fill: parent
            text: "E\nX\nI\nT"
            style: Text.Outline
            color:"red"
            //styleColor: "gray"
            font.pixelSize: 18
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Rectangle {
        id: board;
        color: "black"
        anchors.centerIn: parent;
        width: parent.width - borderWidth
        height: parent.height - borderWidth
        enabled: boardContainer.gameStarted
        opacity: enabled ? 1 : 0.3

        radius: 10
        property int boardSize: 6
        property int cellSize: width/boardSize;
        Repeater {
            id: rep
            model: carModel
             x: 0;
             y: 0;
             anchors.fill: parent;
             delegate: Item {
                 //color: "black"
                 width: board.cellSize * (Orientation==1? Length : 1);
                 height: board.cellSize * (Orientation==0? Length : 1);
                 x: X * board.cellSize
                 y: Y * board.cellSize
                 property int oldpx: x
                 property int oldpy: y
                 Rectangle {
                     color: Color
                     anchors.centerIn: parent
                     border.width: 3
                     border.color: "#ff333333";
                     radius: 10
                     width: parent.width-2*border.width;
                     height: parent.height-2*border.width;
//                     Text {
//                         text: Name
//                     }
                 }
                 MouseArea {
                     anchors.fill: parent
                     drag.target: parent
                     drag.axis: (Orientation === 0 ? Drag.YAxis : Drag.XAxis);
                     drag.minimumX: DragMinX * board.cellSize
                     drag.maximumX: DragMaxX * board.cellSize
                     drag.minimumY: DragMinY * board.cellSize
                     drag.maximumY: DragMaxY * board.cellSize
                     onPressed: {
                         boardUtils.computeDragMinMaxXY(CarIdx);
                         drag.minimumX = boardUtils.getDragMinX(CarIdx) * board.cellSize;
                         drag.minimumY = boardUtils.getDragMinY(CarIdx) * board.cellSize;
                         drag.maximumX = boardUtils.getDragMaxX(CarIdx) * board.cellSize;
                         drag.maximumY = boardUtils.getDragMaxY(CarIdx) * board.cellSize;
                         parent.oldpx = parent.x
                         parent.oldpy = parent.y
                     }
                     onReleased: {
                         var fx = Math.round((parent.x) / board.cellSize);
                         var fy = Math.round((parent.y)/ board.cellSize);
                         boardUtils.updateCarPosition(CarIdx, fx, fy);
                         //snap to grid
                         parent.x = fx * board.cellSize;
                         parent.y = fy * board.cellSize;
                         console.log("oldpx="+parent.oldpx+" parent.x="+parent.x+" oldpy="+parent.oldpy+" parent.y="+parent.y);
                         var weHaveWon = IsRed && fy===2 && fx===4; //could remove fy check because fy always is 2.
                         if((parent.oldpx!=parent.x || parent.oldpy!=parent.y)) {
                             boardContainer.boardMoves++;
                             boardUtils.setCurrentMoves(boardContainer.boardMoves);
                         }
                         if(weHaveWon) won();
                     }
                 }
             }
         }
    }
}
