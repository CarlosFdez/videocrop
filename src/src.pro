#-------------------------------------------------
#
# Project created by QtCreator 2017-01-08T18:19:24
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets av avwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../video-crop
TEMPLATE = app

! include( ../common.pri ) {
    error( "Missing common.pri in the root directory" )
}

SOURCES += main.cpp\
        mainwindow.cpp \
    videoseekbar.cpp \
    util.cpp \
    singlevideoprocessor.cpp \
    videoexportprocessor.cpp \
    rangecontainer.cpp \
    exportdialog.cpp \
    videopositionlabel.cpp \
    rangeinputarea.cpp

HEADERS  += mainwindow.h \
    videoseekbar.h \
    util.h \
    singlevideoprocessor.h \
    videoexportprocessor.h \
    rangecontainer.h \
    exportdialog.h \
    videopositionlabel.h \
    rangeinputarea.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    ../images.qrc
