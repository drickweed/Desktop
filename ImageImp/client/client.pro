
!include(/usr/local/crossdev/qmake/win32.pri) {
	message("Error including windows settings")
}

TEMPLATE = app
RC_FILE = imageimp.rc
QT += sql

TARGET = imageimp

INCLUDEPATH += ../processor \
  ../settings \
  ../document \
  ../storage \
  ../widgets \
  ../model_proxies \
  ../model \
  ../imp \
  ..

LIBS += ../widgets/libwidgets.a \
  ../document/libdocument.a \
  ../model_proxies/libmodel_proxies.a \
  ../model/libmodel.a \
  ../processor/libprocessor.a \
  ../settings/libsettings.a \
  ../imp/libimp.a  \
  ../storage/libstorage.a \
  -ljb-qt4-core -ljb-qt4-widgets -ljb-qt4-image -ljb-qt4-database -ljb-std

TARGETDEPS += ../processor/libprocessor.a \
  ../settings/libsettings.a \
  ../document/libdocument.a \
  ../storage/libstorage.a \
  ../widgets/libwidgets.a \
  ../model_proxies/libmodel_proxies.a \
  ../model/libmodel.a \
  ../imp/libimp.a

SOURCES += main.cpp
