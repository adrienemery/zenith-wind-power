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
INCLUDEPATH = "/usr/local/include/SFM"
LIBS += -L"/usr/local/lib" -lsfml-window

# Serial includes
include(qextserialport.pri)
#CONFIG += extserialport

# openCV includes
INCLUDEPATH += “/usr/local/include”
LIBS += -L"/usr/local/lib/" \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy \
-lopencv_flann \

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

#This is for 64bit - change x86 to x64 for 64 bit libraries
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
    xinputGamepad.cpp \
    controlalgorithm.cpp \
    autopilot.cpp \
    imageprocessing.cpp \
    quadrent.cpp \
    controlwindow.cpp \
    quadrantitem.cpp


HEADERS  += mainwindow.h \
    ../../External Libs/SDLMain.h \
    powerinfo.h \
    kitecolortracker.h \
    kite.h \
    addkite.h \
    xinputGamepad.h \
    controlalgorithm.h \
    autopilot.h \
    imageprocessing.h \
    quadrent.h \
    controlwindow.h \
    quadrantitem.h

FORMS    += mainwindow.ui \
    powerinfo.ui \
    addkite.ui \
    imageprocessing.ui \
    controlwindow.ui

RESOURCES += \
    MyResources.qrc

