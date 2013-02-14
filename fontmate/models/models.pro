!include(../staticlibs.pri):message("Error including common library build settings")
HEADERS += fonts.h \
	fontfilter.h \
	fontmap.h \
	windowsfontregistry.h \
	font.h \
	fontsdatabase.h \
	glyphsmodel.h \
	scratchboardscene.h \
	scratchboardtextitem.h \
	glyphscomparemodel.h \
	glyph.h
SOURCES += fonts.cpp \
	fontfilter.cpp \
	fontmap.cpp \
	windowsfontregistry.cpp \
	font.cpp \
	fontsdatabase.cpp \
	glyphsmodel.cpp \
	scratchboardscene.cpp \
	scratchboardtextitem.cpp \
	glyphscomparemodel.cpp \
	glyph.cpp
QT += sql
TRANSLATIONS = fontmate_he.ts fontmate_en.ts

include(../common.pri)
