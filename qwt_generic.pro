#-------------------------------------------------
#
# Project created by QtCreator 2014-02-10T12:11:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qwt_generic
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    canvaspicker.cpp

HEADERS  += mainwindow.h \
    canvaspicker.h

FORMS    += mainwindow.ui

CONFIG += qwt

INCLUDEPATH += "C:/lib/qwt-6.1.2/src"

LIBS += -L"C:/lib/build-qwt-Desktop_Qt_5_4_0_MinGW_32bit-Release/lib" -lqwt
