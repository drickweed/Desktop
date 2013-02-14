TEMPLATE = app

TARGETDEPS += ../settings/libsettings.a \
  ../processor/libprocessor.a

INCLUDEPATH += ../settings \
  ../processor

LIBS += ../processor/libprocessor.a \
  ../settings/libsettings.a

