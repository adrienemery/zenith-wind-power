#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T15:06:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KiteControl_Qt_08
TEMPLATE = app

macx {
INCLUDEPATH = "/usr/local/include/SFML"

LIBS += -L"/usr/local/lib" -lsfml-window

# Serial includes
CONFIG += extserialport

}

win32{
# Add SFML include directory, where SFML headers are located
INCLUDEPATH += "C:/sfml/sfml2/include"

# Add SFML library directory to search path
LIBS += -L"C:/sfml/sfml2/lib"

# Add SFML window library, where Joystick is located
LIBS += -lsfml-window
}

SOURCES += main.cpp\
        mainwindow.cpp \
    joystick.cpp \
    powerinfo.cpp \
    kitecolortracker.cpp \
    kite.cpp \
    addkite.cpp \
    xinputGamepad.cpp

HEADERS  += mainwindow.h \
    joystick.h \
    ../../External Libs/SDLMain.h \
    powerinfo.h \
    kitecolortracker.h \
    kite.h \
    addkite.h \
    OverLoads.h \
    xinputGamepad.h \
    TestSVN.h

FORMS    += mainwindow.ui \
    powerinfo.ui \
    addkite.ui

RESOURCES += \
    MyResources.qrc

OTHER_FILES += \
    addons/qextserialport/src/qextserialport.pri
