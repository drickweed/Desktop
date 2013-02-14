!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = lib

CONFIG += staticlib

QT += sql

INCLUDEPATH += ../imp ..

LIBS += ../imp/libimp.a

CONFIG -= debug_and_release_target

HEADERS += photos.h \
	photo_selection.h \
	photo_import.h \
	photodates.h \
	people_with_photos.h \
	people_with_photos_query.h \
	people.h \
	groups.h \
	albums.h \
	templates.h

SOURCES += photo_selection.cpp \
	photos.cpp \
	photo_import.cpp \
	photodates.cpp \
	people_with_photos.cpp \
	people_with_photos_query.cpp \
	people.cpp \
	groups.cpp \
	albums.cpp \
	templates.cpp
