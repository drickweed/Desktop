!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp

HEADERS += mainwindow.h

RESOURCES = config.qrc

RC_FILE = config.rc

INCLUDEPATH +=  ../settings ..

FORMS += config.ui

LIBS += \
  ../settings/libsettings.a \
  -ljb-qt4-core \
  -ljb-std -ljb-qt4-models

TARGETDEPS += ../settings/libsettings.a \

