!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

FORMS += cleaning.ui

SOURCES += main.cpp \
	mainwindow.cpp

HEADERS += mainwindow.h

TEMPLATE = app

INCLUDEPATH += ../processor \
	..\
	../settings \

LIBS += ../settings/libsettings.a \
	../processor/libprocessor.a \
	-ljb-qt4-core \
	-ljb-std -ljb-qt4-database

TARGETDEPS += ../processor/libprocessor.a \
	../settings/libsettings.a

QT += sql
