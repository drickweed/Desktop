#!/bin/sh
QMAKESPEC=win32-g++

qmake -spec win32-g++ -recursive
make release -j3
mingw32-ranlib imp/libimp.a
mingw32-ranlib model/libmodel.a
mingw32-ranlib widgets/libwidgets.a
make release -j3

