include("../../plugins.pri")
HEADERS += local.h \
	configdialog.h \
	settings.h \
	driveidmodel.h
SOURCES += local.cpp \
	configdialog.cpp \
	settings.cpp \
	driveidmodel.cpp
LIBS += -ljb-qt4
FORMS += configdialog.ui
QT += network
include("../../install.pri")
