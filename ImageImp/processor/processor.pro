!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = lib

CONFIG += staticlib

CONFIG -= debug_and_release_target

SOURCES += clean_db.cpp \
	create_album.cpp

HEADERS += clean_db.h \
	create_album.h

QT += sql

INCLUDEPATH += ..
