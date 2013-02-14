TEMPLATE = lib
HEADERS += parser.h \
    conversion.h \
    textnode.h \
    segment_parsers.h \
    validators.h
SOURCES += parser.cpp \
    conversion.cpp \
    textnode.cpp \
    segment_parsers.cpp \
    validators.cpp
INCLUDEPATH += ../
CONFIG += staticlib
