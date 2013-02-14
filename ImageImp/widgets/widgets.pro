!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = lib

CONFIG += staticlib

CONFIG -= debug_and_release_target

QT += sql

INCLUDEPATH += ../imp \
  ../model \
  ../model_proxies \
  ..

LIBS += ../model_proxies/libmodel_proxies.a \
  ../model/libmodel.a \
  ../imp/libimp.a \

TARGETDEPS += ../imp/libimp.a \
  ../model/libmodel.a \
  ../model_proxies/libmodel_proxies.a

HEADERS += photos_selection_view.h \
	photos_import_view.h \
	photodates_view.h \
	people_view.h \
	mainwindow.h \
	groups_view.h \
	albums_view.h \
	templates_view.h \
	translations.h

SOURCES += photos_selection_view.cpp \
	photos_import_view.cpp \
	photodates_view.cpp \
	people_view.cpp \
	mainwindow.cpp \
	groups_view.cpp \
	albums_view.cpp \
	search_editors.cpp \
	tables.cpp \
	sections.cpp \
	actions.cpp \
	settings.cpp \
	templates_view.cpp

FORMS += imageimp.ui

TRANSLATIONS = imageimp_he.ts \
	imageimp_en.ts
