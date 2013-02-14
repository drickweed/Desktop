include(../common.pri)
include(../jblib.pri)
TARGET = monitor
TARGET = ../bin/wrmonitor
RC_FILE = monitor.rc
TEMPLATE = app
LIBS += -ljb-calendar \
    -ljb-std \
    -ljb-qt4 \
    -lwrsync
INCLUDEPATH += ../
SOURCES += main.cpp \
    mainwindow.cpp \
    trayicon.cpp \
    monitorview.cpp \
    monitor.cpp \
    aboutdialog.cpp
HEADERS += mainwindow.h \
    trayicon.h \
    monitorview.h \
    monitor.h \
    aboutdialog.h
FORMS += mainwindow.ui \
    monitorview.ui \
    aboutdialog.ui
include(../install.pri)
