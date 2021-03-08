#-------------------------------------------------
#
# Project created by QtCreator 2012-05-28T12:27:19
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = enyo-launcher
TEMPLATE = app


SOURCES += main.cpp\
        e_mainwindow.cpp

HEADERS  += e_mainwindow.h

FORMS    += e_mainwindow.ui

RESOURCES += \
    enyo-launcher.qrc

win32:RC_FILE = ../share/winicon.rc

