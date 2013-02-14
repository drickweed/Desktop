!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = lib

CONFIG += staticlib

CONFIG -= debug_and_release_target

SOURCES += export.cpp \
startup.cpp \
system.cpp \
config.cpp

HEADERS += export.h \
startup.h \
system.h \
config.h

INCLUDEPATH += ..
