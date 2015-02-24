#-------------------------------------------------
#
# Project created by QtCreator 2012-05-28T12:27:19
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = enyo-doom
TEMPLATE = app


SOURCES += main.cpp\
        e_mainwindow.cpp

HEADERS  += e_mainwindow.h

FORMS    += e_mainwindow.ui

RESOURCES += \
    enyo-doom.qrc

win32:RC_FILE = winicon.rc

