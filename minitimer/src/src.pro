SOURCES += main.cpp \
 timer.cpp \
 timerview.cpp \
 timerdialog.cpp

TEMPLATE = app

CONFIG += warn_on \
	  thread \
		  qt

TARGET = minitimer

DESTDIR = ../bin

HEADERS += timer.h \
timerview.h \
 timerdialog.h

FORMS += manual_time_select.ui

LIBS += ../resources/libresources.a

POST_TARGETDEPS += ../resources/libresources.a

staticconfig {
        QMAKE_LIBS_QT =
        QMAKE_LIBS_QT_THREAD =
        LIBS += $(QTDIR)/lib/libqt.a -lz -framework Carbon
}
