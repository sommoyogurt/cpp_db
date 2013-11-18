include(../common.pri)

TARGET = tools
TEMPLATE = lib
CONFIG += staticlib

HEADERS += \
    check_pointer.h \
    coalesce.h \
    compiler_specific.h \
    cpp11_defines.h \
    key_value_pair.h \
    lock_or_throw.h \
    nullable.h \
    nullable_types.h \
    value_is_null.h

SOURCES += \
    value_is_null.cpp
