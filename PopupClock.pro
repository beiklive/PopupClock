QT += core gui widgets

CONFIG += c++17

TEMPLATE = app
TARGET = PopupClock

INCLUDEPATH += inc
OBJECTS_DIR = build/obj/
MOC_DIR = build/
RCC_DIR = build/
UI_DIR = build/

CONFIG(debug,debug|release) {
    DESTDIR = $$absolute_path(bin/debug)
} else {
    DESTDIR = $$absolute_path(bin/release)
}
win32  {
    LIBS += -luser32
}

SOURCES += \
    src/settingmenu.cpp \
    src/main.cpp \
    src/clockbody.cpp \
    src/traymenu.cpp


HEADERS += \
    inc/clockbody.h \
    inc/traymenu.h \
    inc/common.h \
    inc/settingmenu.h

RESOURCES += \
    res/ImgSource.qrc

FORMS += \
    src/settingmenu.ui
