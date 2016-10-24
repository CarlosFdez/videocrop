.
#-------------------------------------------------
#
# Project created by QtCreator 2016-06-03T20:04:52
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets av avwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = video-crop
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    videoseekbar.cpp \
    util.cpp \
    singlevideoprocessor.cpp \
    videoexportprocessor.cpp

HEADERS  += mainwindow.h \
    videoseekbar.h \
    util.h \
    singlevideoprocessor.h \
    videoexportprocessor.h

FORMS    += \
    mainwindow.ui


CONFIG += c++14

# Extra requirements for deploy
# - libvlc dll files in path
# - plugins folder in path
# - Release only, dunno how to get it working in debug

## qtav
INCLUDEPATH += $$PWD/dependencies/qtav/include/
Release:LIBS += -L$$PWD/dependencies/qtav/lib -lQt5AVWidgets -lQt5AV
Debug:LIBS += -L$$PWD/dependencies/qtav/lib -lQt5AVWidgetsd -lQt5AVd

DISTFILES += \
    Readme.md
