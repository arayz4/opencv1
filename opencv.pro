#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T15:22:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

INCLUDEPATH += E:/rnd/opencv/build/include/
INCLUDEPATH += C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409/inc

LIBS += "E:/rnd/opencv/build/x64/vc14/lib/opencv_world310.lib"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/' -lKinect20
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/' -lKinect20d

INCLUDEPATH += $$PWD/'../../../../Program Files/Microsoft SDKs/Kinect/v2.0_1409/inc'
DEPENDPATH += $$PWD/'../../../../Program Files/Microsoft SDKs/Kinect/v2.0_1409/inc'

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/'../../../../Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/libKinect20.a'
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/'../../../../Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/libKinect20d.a'
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/'../../../../Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/Kinect20.lib'
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/'../../../../Program Files/Microsoft SDKs/Kinect/v2.0_1409/Lib/x64/Kinect20d.lib'
