include("../../plugins.pri")
HEADERS += robocopy.h \
    rsyncconfigdialog.h \
    settings.h
SOURCES += robocopy.cpp \
    rsyncconfigdialog.cpp \
    settings.cpp
LIBS += -ljb-qt4
FORMS += rsyncconfigdialog.ui
