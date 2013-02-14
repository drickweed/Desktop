include("../../plugins.pri")
HEADERS += diskspace.h \
	configdialog.h \
	settings.h
SOURCES += diskspace.cpp \
	configdialog.cpp \
	settings.cpp
LIBS += -ljb-qt4
FORMS += configdialog.ui
include("../../install.pri")
