VERSION = 0.2

TEMPLATE = lib
TARGET = flood
QT += declarative
CONFIG += qt plugin

RESOURCES += resources.qrc
TARGET = $$qtLibraryTarget($$TARGET)
uri = flood

# Input
SOURCES += \
    flood_plugin.cpp \
    flood.cpp

HEADERS += \
    flood_plugin.h \
    flood.h

QML_DIR = ../qml

OTHER_FILES = $$QML_DIR/qmldir \
    $$QML_DIR/*.qml

QML_FILES = $$QML_DIR/*.qml

qmldir.files = $$QML_DIR/qmldir $$QML_DIR/*.qml
symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
} else:unix || win32-g++ {
    installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
