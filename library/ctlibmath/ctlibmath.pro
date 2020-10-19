QT = core gui

MUST_USE_EIGEN = 1
include(../../config/library_shared.pri)

TARGET = ctlibmath

DEFINES += CTLIBMATH_LIBRARY

HEADERS += \
    ct_math/ct_math2dlines.h \
    ct_math/ct_mathboundingshape.h \
    ct_math/ct_mathfittedline2d.h \
    ct_math/ct_mathpoint.h \
    ct_math/ct_mathstatistics.h \
    ctlibmath_global.h \
    ct_triangulation/ct_delaunayt.h \
    ct_triangulation/ct_edget.h \
    ct_triangulation/ct_nodet.h \
    ct_triangulation/ct_trianglet.h \
    ct_triangulation/ct_voronoinodet.h

SOURCES += \ 
    ct_math/ct_math2dlines.cpp \
    ct_math/ct_mathboundingshape.cpp \
    ct_math/ct_mathfittedline2d.cpp \
    ct_math/ct_mathpoint.cpp \
    ct_math/ct_mathstatistics.cpp \
    ct_triangulation/ct_delaunayt.cpp \
    ct_triangulation/ct_edget.cpp \
    ct_triangulation/ct_nodet.cpp \
    ct_triangulation/ct_trianglet.cpp \
    ct_triangulation/ct_voronoinodet.cpp
