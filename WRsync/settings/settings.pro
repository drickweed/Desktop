include(../common.pri)
include(../jblib.pri)
RC_FILE = settings.rc
TEMPLATE = app
TRANSLATIONS = settings_he.ts \
    settings_en.ts
HEADERS += mainwindow.h \
    reporterpluginsmodel.h \
    pluginslistview.h \
    destinationpluginsmodel.h \
    processorpluginsmodel.h \
    scheduleview.h \
    dailyschedulemodel.h \
    dailyscheduleview.h \
    filemapdialog.h \
    filetypereportmodel.h \
    filetypereportview.h \
    backupjobsmodel.h
SOURCES += mainwindow.cpp \
    main.cpp \
    reporterpluginsmodel.cpp \
    pluginslistview.cpp \
    destinationpluginsmodel.cpp \
    processorpluginsmodel.cpp \
    scheduleview.cpp \
    dailyschedulemodel.cpp \
    dailyscheduleview.cpp \
    filemapdialog.cpp \
    filetypereportmodel.cpp \
    filetypereportview.cpp \
    backupjobsmodel.cpp
FORMS += mainwindow.ui \
    filemapdialog.ui
INCLUDEPATH += .. \
    ../plugins
LIBS += -lwrsync \
    -ljb-widgets \
    -ljb-std \
    -ljb-qt4 \
    -ljb-calendar
TARGET = ../bin/wrsettings
DEPENDPATH += ../wrsync
include(../install.pri)
OTHER_FILES += settings.rc
