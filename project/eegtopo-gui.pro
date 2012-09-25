TEMPLATE = app
TARGET = eegtopo

CONFIG += console

DEFINES += GUI_ENABLED

SOURCES += ../src/main.cpp \
    ../src/electrodes.cpp \
    ../src/electrodegraphics.cpp \
    ../src/parser.cpp \
    ../src/painter.cpp \
    ../src/colormap.cpp \
    ../src/membermap.cpp \
    ../src/gui/guimain.cpp

HEADERS += \
    ../src/electrodes.h \
    ../src/electrodegraphics.h \
    ../src/parser.h \
    ../src/painter.h \
    ../src/colormap.h \
    ../src/membermap.h \
    ../src/gui/guimain.h

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += cairomm-1.0 eigen3
}

#INCLUDEPATH += ../include/eigen3

LIBS += -lboost_program_options

DEPENDPATH += $INCLUDEPATH
