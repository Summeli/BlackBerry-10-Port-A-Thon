#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include "boardutils.h"
#include <qplatformdefs.h> // MEEGO_EDITION_HARMATTAN

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ParkMeeCrazy");
    app.setOrganizationDomain("ParkMeeCrazy");
    app.setOrganizationName("ParkMeeCrazy");

    //QString boardString = "1,10 0,2,1,1,2,red 0,3,1,0,2,DarkGreen 2,3,1,0,2,purple3 1,4,1,0,3,orange 1,5,1,0,3,seagreen 0,0,0,0,2,blue3 0,4,0,0,2,SteelBlue1 3,0,0,0,3,yellow 4,1,0,0,2,gold3 5,1,0,0,2,IndianRed1";
    BoardUtils bu;
    //bu.readBoardFromString(boardString);

    QDeclarativeView viewer;
    //viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);

    viewer.setWindowTitle("ParkMeeCrazy!");

    QDeclarativeContext *ctxt = viewer.rootContext();
    CarModel *carModel = bu.getCarModel();
    ctxt->setContextProperty("carModel", carModel);
    ctxt->setContextProperty("boardUtils", &bu);

#if defined(MEEGO_EDITION_HARMATTAN)
    viewer.setMainQmlFile(QLatin1String("qml/parkmeecrazy/meego-main.qml"));
    qDebug() << "AAAAAAAAAAAAAAAAAAAA DEFINED MEEEEEEEEEEEGGGGGGOOOOOOOOOOOOOO" ;
#else
  //  viewer.setMainQmlFile(QLatin1String("qml/parkmeecrazy/MMMainView.qml"));
    viewer.setSource(QUrl("qrc:///qml/parkmeecrazy/MMMAinView.qml"));
   // viewer.setSource(QUrl("asset:///MMMainView.qml"));
#endif

    //viewer.showExpanded();
    viewer.showFullScreen();
    return app.exec();
}
