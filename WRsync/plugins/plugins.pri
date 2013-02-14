TEMPLATE = lib
CONFIG += plugin
CONFIG -= debug_and_release

INCLUDEPATH += ../.. ../../..
win32:INCLUDEPATH += c:/work/build/include

LIBS += -L../../../wrsync -lwrsync -ljb-std
win32:LIBS += -Lc:/work/build/ -shared-libgcc

# Boost
win32:INCLUDEPATH += C:/work/include/boost/boost_1_42_0
LIBS += -lboost_system -lboost_filesystem
