MOC_DIR = ./moc
UI_DIR = ./ui
SOURCES += application.cpp \
    gui.cpp \
    installer.cpp \
    tweak.cpp \
    utils.cpp \
    version.cpp \
    main.cpp \
    appsmodel.cpp \
    modelitems.cpp \
    model.cpp \
    urls.cpp \
    importhandler.cpp \
    downloader.cpp \
    httpdownloader.cpp \
    controller.cpp \
    configurationui.cpp \
    config.cpp \
    helpui.cpp \
    tweakaction.cpp \
    tweakindexhandler.cpp \
    tweaksmodel.cpp \
    tweakrunner.cpp \
    tipwidget.cpp
FORMS += auto-mate.ui \
    configuration.ui \
    help.ui \
    tip.ui
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt \
    release \
    uitools
TARGET = ../bin/auto-mate
RESOURCES += resources.qrc
HEADERS += application.h \
    gui.h \
    installer.h \
    tweak.h \
    version.h \
    utils.h \
    appsmodel.h \
    modelitems.h \
    model.h \
    urls.h \
    importhandler.h \
    downloader.h \
    httpdownloader.h \
    controller.h \
    const.h \
    configurationui.h \
    config.h \
    helpui.h \
    tweakaction.h \
    tweakindexhandler.h \
    tweaksmodel.h \
    tweakrunner.h \
    tipwidget.h \
    ../plugins/interface/DownloaderInterface.h
QT += xml \
    network
LIBS += -lboost_filesystem-mt \
    -lboost_system-mt \
    -ljb-std \
    -ljb-qt4 \
    -ljb-widgets \
    -lpopt \
    -L/usr/local/lib

# LIBS += -L/home/tlailax/crossdev/bin
INCLUDEPATH += /usr/local/include
