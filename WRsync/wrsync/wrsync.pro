include(../jblib.pri)
include(../common.pri)
QT += sql
TEMPLATE = lib
SOURCES += settings.cpp \
	epath.cpp \
	result.cpp \
	pluginmanager.cpp \
	filelistgenerator.cpp \
	remotecomputer.cpp \
	backup.cpp \
	job.cpp \
	removabledrive.cpp \
	fatalexceptionreporter.cpp \
	systemlogentry.cpp \
	knownrules.cpp \
	logfile.cpp \
	shadowmanager.cpp \
	sourcemanager.cpp \
	schedule.cpp \
	schedulemodel.cpp
HEADERS += settings.h \
	epath.h \
	result.h \
	pluginmanager.h \
	filelistgenerator.h \
	remotecomputer.h \
	backup.h \
	job.h \
	global.h \
	removabledrive.h \
	exceptions.h \
	fatalexceptionreporter.h \
	systemlogentry.h \
	knownrules.h \
	logfile.h \
	shadowmanager.h \
	sourcemanager.h \
	schedule.h \
	schedulemodel.h
LIBS += -ljb-qt4 \
	-ljb-std \
	-ljb-database -ljb-calendar
# win32:LIBS += -lvshadow
win32:LIBS += -export-all-symbols
DEFINES += DLL_BUILD

# Mimic windows paths on linux
!win32:DEFINES += MIMIC_WINDOWS
CONFIG += shared
win32:CONFIG += dll
include("../install.pri")
RESOURCES += ../resources/wrsync.qrc
