#-------------------------------------------------
#
# Project created by QtCreator 2020-03-30T13:16:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = icp
TEMPLATE = app
CONFIG += c++17

SOURCES += main.cpp\
        mainwindow.cpp \
    model/linka.cpp \
    model/spoj.cpp \
    model/dataholder.cpp \
    model/road.cpp \
    connectionitem.cpp \
    controller/mapscene.cpp \
    controller/itineraryscene.cpp

HEADERS  += mainwindow.h \
    model/linka.h \
    model/spoj.h \
    model/dataholder.h \
    model/road.h \
    connectionitem.h \
    controller/mapscene.h \
    controller/itineraryscene.h

FORMS    += mainwindow.ui \
    connectionitem.ui

RESOURCES += \
    resource.qrc
