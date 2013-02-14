TEMPLATE = app
CONFIG += qtestlib
SOURCES += test.cpp
INCLUDEPATH += ../../model
LIBS +=  ../../model/libmodel.a -ljb-qt4 -ljb-std
POST_TARGETDEPS += ../../model/libmodel.a
