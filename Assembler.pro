#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T16:09:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Assembler
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    highlighter.cpp \
    assemble.cpp

HEADERS  += \
    mainwindow.h \
    highlighter.h \
    assemble.h

FORMS    += mainwindow.ui
