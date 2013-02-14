!include(../staticlibs.pri):message("Error including common library build settings")

LIBS += ../models/libmodels.a
TARGETDEPS += ../models/libmodels.a
HEADERS += document.h
SOURCES += document.cpp
QT += sql

include(../common.pri)
