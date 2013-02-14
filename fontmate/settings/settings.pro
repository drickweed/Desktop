!include(../staticlibs.pri):message("Error including common library build settings")

HEADERS += settings.h \
	settings_dialog.h
SOURCES += settings.cpp \
	settings_dialog.cpp
FORMS += settings.ui

TRANSLATIONS = fontmate_he.ts fontmate_en.ts

include(../common.pri)
