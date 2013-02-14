!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = lib

CONFIG += staticlib

CONFIG -= debug_and_release_target

QT += sql

INCLUDEPATH += ../imp ..  ../model

LIBS += ../model/libmodel.a \
	../imp/libimp.a

TARGETDEPS += ../imp/libimp.a \
	../model/libmodel.a

HEADERS += unphotographed_people.h \
	photographed_people.h

SOURCES += unphotographed_people.cpp \
	photographed_people.cpp
