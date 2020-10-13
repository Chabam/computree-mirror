TEMPLATE = lib
TARGET = muparser
CONFIG -= qt
CONFIG += c++17 warn_off staticlib # shared dll

DESTDIR = $$PWD
# DLLDESTDIR = $$PWD

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


# TEMPLATE = lib
# TARGET = muparser
# CONFIG *= C++17 shared dll
# 
# DLLDESTDIR = $$PWD\lib
# 
# INCLUDEPATH += $$PWD\include
# 
# DEFINES += MUPARSER_STATIC
# 
# HEADERS = \
#           include\muParser.h \
#           include\muParserBase.h \
#           include\muParserBytecode.h \
#           include\muParserCallback.h \
#           include\muParserDef.h \
#           include\muParserDLL.h \
#           include\muParserError.h \
#           include\muParserFixes.h \
#           include\muParserInt.h \
#           include\muParserTemplateMagic.h \
#           include\muParserTest.h \
#           include\muParserToken.h \
#           include\muParserTokenReader.h
# 
# SOURCES = \
#           src\muParser.cpp \
#           src\muParserBase.cpp \
#           src\muParserBytecode.cpp \
#           src\muParserCallback.cpp \
#           src\muParserDLL.cpp \
#           src\muParserError.cpp \
#           src\muParserInt.cpp \
#           src\muParserTest.cpp \
#           src\muParserTokenReader.cpp
