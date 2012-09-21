TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += ../src/main.cpp \
    ../src/electrodes.cpp \
    ../src/electrodegraphics.cpp \
    ../src/parser.cpp \
    ../src/painter.cpp \
    ../src/colormap.cpp \
    ../src/membermap.cpp

HEADERS += \
    ../src/electrodes.h \
    ../src/electrodegraphics.h \
    ../src/parser.h \
    ../src/painter.h \
    ../src/colormap.h \
    ../src/membermap.h

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += cairomm-1.0
}

INCLUDEPATH += ../include/eigen3

LIBS += -lboost_program_options

DEPENDPATH += $INCLUDEPATH
