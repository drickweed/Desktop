include(../jblib.pri)
TEMPLATE = app
TARGET = ../bin/shadow_test
DEPENDPATH += ../wrsync
INCLUDEPATH += ..

# NOTE: must be console in order the the application to block until it fnishes.
# This is in case the backup is inside a batch file as part of a larger process.
CONFIG += console
CONFIG   -= app_bundle

# Input
SOURCES += main.cpp
LIBS += -lwrsync \
	-ljb-std \
	-ljb-qt4
include(../common.pri)
include(../install.pri)
