######################################################################
# Automatically generated by qmake (3.1) Sun Feb 19 16:27:28 2023
######################################################################

TEMPLATE = app
TARGET = vNes

OBJECTS_DIR = build/obj/
MOC_DIR = build/
RCC_DIR = build/
UI_DIR = build/

CONFIG(debug,debug|release) {
    DESTDIR = $$absolute_path(bin/debug)
} else {
    DESTDIR = $$absolute_path(bin/release)
}

QT += core \
    gui \
    widgets

SOURCES += \
    src/DrawClock.cpp \
    src/PopupClock.cpp \
    src/gen.cpp \
    src/main.cpp \
    src/sets.cpp

HEADERS += \
    src/DrawClock.h \
    src/PopupClock.h \
    src/PopupClock.rc \
    src/gen.h \
    src/resource.h \
    src/sets.h

RESOURCES += \
    src/PopupClock.qrc

FORMS += \
    src/PopupClock.ui \
    src/sets.ui
