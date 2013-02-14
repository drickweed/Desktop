!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

HEADERS += document.h

SOURCES += document.cpp

TEMPLATE = lib

CONFIG += staticlib

CONFIG -= debug_and_release_target

QT += sql


LIBS += ../imp/libimp.a

TARGETDEPS += ../imp/libimp.a

INCLUDEPATH += ../imp ..

