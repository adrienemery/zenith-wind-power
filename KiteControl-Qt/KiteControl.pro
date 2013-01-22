#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T15:06:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KiteControl
TEMPLATE = app

macx{

# SFML includes
INCLUDEPATH = "/usr/local/include/SFML"
LIBS += -L"/usr/local/lib" -lsfml-window

# Serial includes
LIBS += -L"/Users/adrienemery/Qt5.0.0/addons/qextserialport/"
INCLUDEPATH = "Users/adrienemery/Qt5.0.0/addons/qextserialport/src"
CONFIG += extserialport

# OpenCv includes

}

win32{

include(addons/qextserialport/src/qextserialport.pri)
INCLUDEPATH += "C:/opencv243/build/include"

# Add SFML include directory, where SFML headers are located
INCLUDEPATH += "C:/sfml/sfml2/include"

# Add SFML library directory to search path
LIBS += -L"C:/sfml/sfml2/lib"
LIBS += -L"C:/opencv243/build/x86/vc10/lib"

# Add SFML window library, where Joystick is located
LIBS += -lsfml-window

# OpenCV include paths and libraries
INCLUDEPATH += C:\\opencv243\\build\\include

#This is for 64bit - change x86 to x64 for 32 bit machines
LIBS += -L"C:\\opencv243\\build\\x86\\vc10\\lib" \
-lopencv_core243d \
-lopencv_imgproc243d \
-lopencv_highgui243d \
-lopencv_ml243d \
-lopencv_video243d \
-lopencv_features2d243d \
-lopencv_calib3d243d \
-lopencv_objdetect243d \
-lopencv_contrib243d \
-lopencv_legacy243d \
-lopencv_flann243d \


}

SOURCES += main.cpp\
        mainwindow.cpp \
    powerinfo.cpp \
    kitecolortracker.cpp \
    kite.cpp \
    addkite.cpp \
    xinputGamepad.cpp

HEADERS  += mainwindow.h \
    ../../External Libs/SDLMain.h \
    powerinfo.h \
    kitecolortracker.h \
    kite.h \
    addkite.h \
    OverLoads.h \
    xinputGamepad.h

FORMS    += mainwindow.ui \
    powerinfo.ui \
    addkite.ui

RESOURCES += \
    MyResources.qrc

