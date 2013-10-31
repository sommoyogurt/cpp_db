include(../common.pri)

TARGET = firebird
TEMPLATE = lib
CONFIG += staticlib

SOURCES += firebird_driver.cpp \
    firebird_connection.cpp

HEADERS += firebird_driver.h \
    firebird_connection.h

INCLUDEPATH += ../cpp_db \
               ../tools \

win32 {
INCLUDEPATH += include_win_32
LIBS += $$PWD/lib_win_32/fbclient_ms.lib
}

macx {
INCLUDEPATH += include_macosx_64
LIBS += $$PWD/lib_macosx_64/fbclient_ms.lib
}

