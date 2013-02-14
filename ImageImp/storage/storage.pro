!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = lib

CONFIG += staticlib

CONFIG -= debug_and_release_target

QT += sql

HEADERS += set.h \
	query.h \
	get.h

SOURCES += set.cpp \
	query.cpp \
	get.cpp

INCLUDEPATH += ..
