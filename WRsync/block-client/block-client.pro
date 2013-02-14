include(../jblib.pri)
TEMPLATE = app
RC_FILE = ../client/client.rc
TARGET = ../bin/wrsync-console
DEPENDPATH += ../wrsync
INCLUDEPATH += ..

# NOTE: must be console in order the the application to block until it finishes.
# This is in case the backup is inside a batch file as part of a larger process.
CONFIG += console

# Input
HEADERS += ../client/report.h \
	../client/processor.h \
	../client/options.h \
	../client/exitcodes.h
SOURCES += ../client/main.cpp \
	../client/report.cpp \
	../client/processor.cpp \
	../client/options.cpp
LIBS += -lwrsync \
	-ljb-std \
	-ljb-qt4
include(../common.pri)
include(../install.pri)
