include(../jblib.pri)
TEMPLATE = app
RC_FILE = client.rc
TARGET = ../bin/wrsync
DEPENDPATH += ../wrsync
INCLUDEPATH += ..

# Input
HEADERS += report.h \
	processor.h \
	options.h \
	exitcodes.h
SOURCES += main.cpp \
	report.cpp \
	processor.cpp \
	options.cpp
LIBS += -lwrsync \
	-ljb-std \
	-ljb-qt4
include(../common.pri)
include(../install.pri)
