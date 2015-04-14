#-------------------------------------------------
#
# Project created by QtCreator 2015-04-14T10:13:02
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = NrPlayer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    playercontroller.cpp \
    netclient.cpp \
    playlist.cpp \
    visualplayer.cpp

HEADERS += \
    playercontroller.h \
    netclient.h \
    playlist.h \
    visualplayer.h
