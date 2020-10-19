TEMPLATE = lib
TARGET = muparser
CONFIG -= qt
CONFIG += c++17 warn_off staticlib

DESTDIR = $$PWD

DEFINES += QT_DEPRECATED_WARNINGS MUPARSER_STATIC

linux {
    QMAKE_CXXFLAGS += -Wno-deprecated-copy -Wno-cast-function-type
}

SOURCES += \
    src/muParser.cpp \
    src/muParserBase.cpp \
    src/muParserBytecode.cpp \
    src/muParserCallback.cpp \
    src/muParserError.cpp \
    src/muParserInt.cpp \
    src/muParserTest.cpp \
    src/muParserTokenReader.cpp

INCLUDEPATH += include
