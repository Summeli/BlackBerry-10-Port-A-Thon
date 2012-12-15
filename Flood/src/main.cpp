// Default empty project template
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <QLocale>
#include <QTranslator>
#include "flooditem.h"
using namespace bb::cascades;

int main(int argc, char **argv)
{
    // this is where the server is started etc
    QApplication app(argc, argv);

    QDeclarativeView view;

    view.setWindowTitle("Flood!");
    qmlRegisterType<FloodModel>("flood", 1, 0, "FloodModel");


    view.setSource(QUrl("qrc:///qml/main.qml"));

    view.showFullScreen();

    return app.exec();
}
