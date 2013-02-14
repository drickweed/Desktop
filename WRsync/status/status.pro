include(../jblib.pri)
TEMPLATE = app
TARGET = ../bin/status
DEPENDPATH += ../wrsync
INCLUDEPATH += ..

SOURCES += main.cpp\
		mainwindow.cpp
HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
LIBS += -lwrsync \
	-ljb-std \
	-ljb-qt4 -ljb-widgets

include(../common.pri)
include(../install.pri)
