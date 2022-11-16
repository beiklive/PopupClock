QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DrawClock.cpp \
    PopupClock.cpp \
    gen.cpp \
    main.cpp \
    sets.cpp

HEADERS += \
    DrawClock.h \
    PopupClock.h \
    PopupClock.rc \
    gen.h \
    resource.h \
    sets.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    PopupClock.qrc

FORMS += \
    PopupClock.ui \
    gen.ui \
    sets.ui
