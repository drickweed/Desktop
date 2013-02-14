TEMPLATE = lib
MOC_DIR = ./moc
UI_DIR = ./ui
CONFIG += exceptions \
 staticlib \
 plugin
SOURCES += checksum.cpp \
crc32checksum.cpp \
filechecksumrecord.cpp \
filechecksummodel.cpp \
 filechecksumdb.cpp \
 filechecksummodelproxy.cpp
HEADERS += checksum.h \
crc32checksum.h \
filechecksumrecord.h \
filechecksummodel.h \
 filechecksumdb.h \
 filechecksummodelproxy.h \



