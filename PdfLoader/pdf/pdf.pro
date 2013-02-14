TEMPLATE = lib
SOURCES += pdfcommandsmodel.cpp \
    pdfgenerator.cpp \
    pdfdocument.cpp \
    pdfscene.cpp \
    util.cpp \
    pdfobjectsmodel.cpp \
    streamextractor.cpp \
    inflate.cpp \
    objectpropertymodel.cpp \
    page.cpp \
    settings.cpp
HEADERS += pdfcommandsmodel.h \
    pdfgenerator.h \
    pdfdocument.h \
    pdfscene.h \
    util.h \
    pdfobjectsmodel.h \
    streamextractor.h \
    inflate.h \
    objectpropertymodel.h \
    page.h \
    settings.h
LIBS += -lpoppler-qt4
CONFIG += staticlib
INCLUDEPATH += ../
