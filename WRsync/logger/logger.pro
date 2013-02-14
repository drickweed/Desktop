include(../common.pri)
include(../jblib.pri)

TEMPLATE = app
TARGET = ../bin/wrlogger
DEPENDPATH += ../wrsync
INCLUDEPATH += ..
CONFIG   += console
# Input
HEADERS += report.h
SOURCES += main.cpp \
	report.cpp
LIBS += -L../wrsync \
	-lwrsync \
	-ljb-std \
	-ljb-qt4

include(../install.pri)
