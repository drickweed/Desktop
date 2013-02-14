include(../jblib.pri)
TEMPLATE = app
# RC_FILE = cron.rc
TARGET = ../bin/wrcron
DEPENDPATH += ../wrsync
INCLUDEPATH += ..
QT -= gui


# Input
#HEADERS +=
SOURCES += main.cpp
LIBS += -lwrsync \
	-ljb-std \
	-ljb-qt4
include(../common.pri)
include(../install.pri)
