#-------------------------------------------------
#
# Project created by QtCreator 2014-01-08T12:30:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vflayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    workspacewidget.cpp \
    wsdata.cpp \
    tool.cpp \
    program.cpp \
    settingslider.cpp

HEADERS  += mainwindow.h \
    workspacewidget.h \
    wsdata.h \
    tool.h \
    program.h \
    settingslider.h

FORMS    += mainwindow.ui \
    settingslider.ui
