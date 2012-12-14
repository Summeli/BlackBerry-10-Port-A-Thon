import QtQuick 1.0

import com.nokia.meego 1.0 // This is needed for PageStackWindow

PageStackWindow {
    showStatusBar: true
    showToolBar: false
    initialPage: mainPage

//    ToolBar {
//        id: toolbar
//        visible: false;
//    }

    Page {
        id: mainPage
        anchors.fill: parent
        orientationLock: PageOrientation.LockPortrait // <-- This is the magic line
        MMMainView {
            anchors.fill: parent
        }
    }
}
