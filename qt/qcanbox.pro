QT += core gui widgets serialport

TARGET = qcanbox
TEMPLATE = app

HEADERS = main.h wdg_dbg.h wdg_com.h
SOURCES = main.cpp wdg_dbg.cpp wdg_com.cpp
FORMS += ui/main.ui ui/dbg.ui ui/com.ui

DEPENDPATH += $$PWD/../
INCLUDEPATH += $$PWD/../
HEADERS += $$PWD/../canbox.h $$PWD/../car.h $$PWD/../conf.h
SOURCES += $$PWD/../canbox.c $$PWD/../car.c qcar.c

RESOURCES += qcanbox.qrc

DEFINES += QT_STATICPLUGIN
DEFINES += QCAR

win32 {
	RC_ICONS = sg.ico
	QMAKE_LFLAGS += -static -static-libgcc
	QMAKE_POST_LINK=$$QMAKE_STRIP release/$(TARGET)
}

