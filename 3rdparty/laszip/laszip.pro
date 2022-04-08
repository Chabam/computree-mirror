TEMPLATE = lib
TARGET = laszip
CONFIG -= qt
CONFIG += c++14 staticlib

DEFINES += LASZIPDLL_EXPORTS

DESTDIR = $$PWD/lib

HEADERS += $$PWD/include/laszip
HEADERS += $$files($$PWD/src/*.hpp, true)

SOURCES += $$files($$PWD/src/*.cpp, true)

INCLUDEPATH += $$PWD/include
