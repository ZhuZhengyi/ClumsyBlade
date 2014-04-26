#-------------------------------------------------
#
# Project created by QtCreator 2012-06-02T19:31:23
#
#-------------------------------------------------

QT       += core sql
QT       -= gui

TARGET   = DataAccessLayer
TEMPLATE = lib

DESTDIR = ../bin

DEFINES += TEST
DEFINES += DATAACCESSLAYER_LIBRARY

SOURCES += \
    dbmanager.cpp \
    ../ShareLib/share_headers/utils/lessthan.cpp \
    ../ShareLib/share_headers/utils/filesystemhelper.cpp \
    dataaccesslayerplugin2.cpp \
    dataaccesslayerplugin.cpp

HEADERS += dataaccesslayer.h\
    dbmanager.h \
    ../ShareLib/share_headers/utils/lessthan.h \
    ../ShareLib/share_headers/utils/filesystemhelper.h \
    dataaccesslayerplugin2.h \
    dataaccesslayerplugin.h
