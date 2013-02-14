include("../../plugins.pri")
# Input
HEADERS += ibackup.h \
	ibackupconfigdialog.h \
	settings.h
SOURCES += ibackup.cpp \
	ibackupconfigdialog.cpp \
	settings.cpp
FORMS += ibackupconfigdialog.ui
QT += xml network
include(../../install.pri)
