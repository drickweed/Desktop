!include(../staticlibs.pri):message("Error including common library build settings")

HEADERS += folders.h \
	folderselectionmodel.h
SOURCES += folders.cpp \
	folderselectionmodel.cpp

include(../common.pri)
