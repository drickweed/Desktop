include("../../plugins.pri")
include("../../../jblib.pri")
QT += xml \
    sql

# Input
HEADERS += localdb.h \
    localdbconfigdialog.h \
    report.h \
    settings.h
SOURCES += localdb.cpp \
    localdbconfigdialog.cpp \
    report.cpp \
    settings.cpp
FORMS += localdbconfigdialog.ui
OTHER_FILES += localdb.sql
LIBS += -L/usr/local/lib \
    -ljb-database
RESOURCES += localdb.qrc
include(../../install.pri)
