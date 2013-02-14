CONFIG -= debug_and_release_target
TEMPLATE = lib
CONFIG += staticlib
FORMS += fontpreviewbar.ui \
	tagmanager.ui \
	tagselector.ui \
	previewtypesview.ui \
	fontpreviewsview.ui
SOURCES += fonteditorpreview.cpp \
	fontpreviewcontainer.cpp \
	fontpreviewbar.cpp \
	fontpreviewstack.cpp \
	fontsview.cpp \
	fontitemdelegate.cpp \
	tagmanager.cpp \
	tagselector.cpp \
	fontquickpreview.cpp \
	previewtypesview.cpp \
	glyphsview.cpp \
	glyphitemdelegate.cpp \
	fontpreviewsview.cpp \
	fontpreviewtabs.cpp \
	scratchboard.cpp
HEADERS += fonteditorpreview.h \
	fontpreviewcontainer.h \
	fontpreviewbar.h \
	fontpreviewstack.h \
	fontsview.h \
	fontitemdelegate.h \
	tagmanager.h \
	tagselector.h \
	fontquickpreview.h \
	previewtypesview.h \
	glyphsview.h \
	glyphitemdelegate.h \
	fontpreviewsview.h \
	fontpreviewtabs.h \
	scratchboard.h
TRANSLATIONS = fontmate_he.ts fontmate_en.ts
QT += sql
include(../common.pri)
