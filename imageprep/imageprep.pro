TARGET = imageprep
TEMPLATE = app
QT += network
LIBS += -L/usr/local/lib
LIBS += -ljb-std \
    -ljb-qt4 \
    -ljb-image
SOURCES += main.cpp \
    mainwindow.cpp \
    imageeditorscene.cpp \
    translation.cpp \
    target.cpp \
    document.cpp \
    targetbutton.cpp \
    imageeditorview.cpp \
    cropareaitem.cpp \
    settings.cpp \
    settingsdialog.cpp \
    collectionnamedialog.cpp \
    collectiontools.cpp \
    uploaddialog.cpp \
    targetremovedialog.cpp
HEADERS += mainwindow.h \
    imageeditorscene.h \
    translation.h \
    target.h \
    document.h \
    targetbutton.h \
    imageeditorview.h \
    cropareaitem.h \
    imageviewwidget.h \
    settings.h \
    settingsdialog.h \
    collectionnamedialog.h \
    collectiontools.h \
    uploaddialog.h \
    targetremovedialog.h
FORMS += mainwindow.ui \
    settingsdialog.ui \
    collectionnamedialog.ui \
    uploaddialog.ui \
    targetremovedialog.ui
RESOURCES = imageprep.qrc
