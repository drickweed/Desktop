SOURCES += checksumthread.cpp \
 window.cpp \
 filechecksumtable.cpp \
 controller.cpp \
 taskui.cpp \
 summary.cpp \
 controls.cpp \
 main.cpp \
 abortui.cpp \
 statussummaryitem.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
		  qt

MOC_DIR = ./moc
UI_DIR = ./ui

TARGET = ../bin/minisfv
RESOURCES = ../minisfv.qrc
HEADERS += checksumthread.h \
 window.h \
 filechecksumtable.h \
 controller.h \
 abortui.h \
 taskui.h \
 summary.h \
 controls.h \
 statussummaryitem.h

POST_TARGETDEPS += ../model/libmodel.a

INCLUDEPATH += ../model

LIBS +=  ../model/libmodel.a -ljb-std -ljb-qt4 -ljb-widgets

