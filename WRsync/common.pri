# NOTES:
# -shared-libgcc : required to make cross dll exceptions work

win32:LIBS += -lmingw32 -Lc:/work/build -shared-libgcc
CONFIG -= debug_and_release
INCLUDEPATH += ../
win32:INCLUDEPATH += C:/work/include/boost/boost_1_42_0
LIBS += -lboost_system -lboost_filesystem -L../wrsync
