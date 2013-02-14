TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp 
HEADERS += mainwindow.h
FORMS += mainwindow.ui
INCLUDEPATH += ..
LIBS += -lpoppler-qt4 -lpdf -ljb-qt4 -ljb-std -lwidgets -lgraphics -lobjects -L../objects -L../graphics -L../pdf -L../widgets -lPropertyEditor
