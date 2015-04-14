#-------------------------------------------------
#
# Project created by QtCreator 2015-04-14T10:13:02
#
#-------------------------------------------------

QT       += core qml quick network

TARGET = NrPlayer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    playercontroller.cpp \
    netclient.cpp \
    playlist.cpp \
    visualplayer.cpp \
    auth.cpp

HEADERS += \
    playercontroller.h \
    netclient.h \
    playlist.h \
    visualplayer.h \
    auth.h

RESOURCES += \
    nrplayer.qrc

DISTFILES +=

CONFIG += c++11
