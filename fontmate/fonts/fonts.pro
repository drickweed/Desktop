!include(../staticlibs.pri):message("Error including common library build settings")
HEADERS += fontutils.h \
    print.h \
    pdffontextractor.h
SOURCES += fontutils.cpp \
    print.cpp \
    pdffontextractor.cpp
win32:SOURCES += fontutils-win.cpp
unix:SOURCES += fontutils-unix.cpp
osx:SOURCES += fontutils-osx.cpp
include(../common.pri)
