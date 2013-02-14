include(../common.pri)
include(../jblib.pri)

TEMPLATE = app
TARGET = ../bin/wrlogtail
DEPENDPATH += ../wrsync
INCLUDEPATH += ..

HEADERS += mainwindow.h
SOURCES += mainwindow.cpp \
	main.cpp
FORMS += mainwindow.ui
LIBS += -L../wrsync \
	-lwrsync \
	-ljb-std \
	-ljb-qt4

include(../install.pri)
