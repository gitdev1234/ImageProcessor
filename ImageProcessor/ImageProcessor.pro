#-------------------------------------------------
#
# Project created by QtCreator 2016-07-08T21:58:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageProcessor
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    imageprocessor.cpp

HEADERS  += mainwindow.h \
    imageprocessor.h

FORMS    += mainwindow.ui

RESOURCES += \
    ressources.qrc
