!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = lib

CONFIG += staticlib

QT += sql

INCLUDEPATH += ..

CONFIG -= debug_and_release_target

HEADERS += picture.h \
photodate.h \
person.h \
group.h \
album.h \
 template.h \
 const.h

SOURCES += picture.cpp \
photodate.cpp \
person.cpp \
group.cpp \
album.cpp \
 template.cpp

