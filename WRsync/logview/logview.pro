include(../common.pri)
include(../jblib.pri)
TEMPLATE = app
RC_FILE = logview.rc
TARGET = ../bin/logviewer
LIBS += -ljb-calendar \
	-ljb-std \
	-ljb-qt4 \
	-ljb-widgets \
	-lwrsync
INCLUDEPATH += ../
TRANSLATIONS = logview_he.ts \
	logview_en.ts

# Input
SOURCES += main.cpp \
	mainwindow.cpp \
	logdaydelegate.cpp \
	calendarlogmodel.cpp \
	copyerrorsmodel.cpp \
	jobstatemodel.cpp \
	messages.cpp
HEADERS += mainwindow.h \
	logdaydelegate.h \
	calendarlogmodel.h \
	copyerrorsmodel.h \
	jobstatemodel.h \
	messages.h
FORMS += mainwindow.ui
include(../install.pri)
OTHER_FILES += logview.rc
