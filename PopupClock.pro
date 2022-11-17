QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/PopupClock.qrc

FORMS += \
    src/PopupClock.ui \
    src/sets.ui

OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
CONFIG(debug,debug|release) {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/debug)
} else {
    DESTDIR = $$absolute_path($${_PRO_FILE_PWD_}/bin/release)
}