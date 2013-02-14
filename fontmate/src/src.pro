SOURCES += main.cpp \
	mainwindow.cpp \
	about.cpp \
	menubar.cpp \
	preview_editing.cpp \
	font_selection.cpp \
	font_folders.cpp \
	font_install.cpp \
	font_search.cpp \
	settings.cpp
TEMPLATE = app
TARGET = FontMate
DESTDIR = ../bin

FORMS += fontmate.ui \
	about.ui
HEADERS += mainwindow.h \
	config.h \
	about.h \
	menubar.h
RC_FILE = fontmate.rc
QT += sql network
include(../common.pri)
LIBS += ../settings/libsettings.a \
	../widgets/libwidgets.a \
	../document/libdocument.a \
	../models/libmodels.a \
	../folders/libfolders.a \
	../resources/libresources.a \
	../fonts/libfonts.a \
	-ljb-widgets \
	-ljb-std \
	-ljb-qt4 \
	-ljb-models \
	-ljb-database
TRANSLATIONS = fontmate_he.ts fontmate_en.ts

# Required for font management functions
win32:LIBS += -lgdi32
TARGETDEPS += ../document/libdocument.a \
	../models/libmodels.a \
	../settings/libsettings.a \
	../widgets/libwidgets.a \
	../folders/libfolders.a \
	../resources/libresources.a
