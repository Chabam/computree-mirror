QT = core gui

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

include(../library_shared.pri)

TARGET = ctlibio

DEFINES += CTLIBIO_LIBRARY

HEADERS +=  \
#    readers/ct_reader_ascrgb.h \
#    readers/ct_reader_ascrgb_def_models.h \
#    readers/ct_reader_gdal.h \
#    readers/ct_reader_gdal_def_models.h \
#    readers/ct_reader_larchitect_grid.h \
#    readers/ct_reader_larchitect_grid_def_models.h \
#    readers/ct_reader_opf.h \
#    readers/ct_reader_opf_def_models.h \
#    readers/ct_reader_terrascanprj.h \
#    readers/ct_reader_terrascanprj_def_models.h \
    readers/ct_reader_xyb.h \
#    readers/ct_reader_xyb_def_models.h \
    ctlibio_global.h \
#    readers/ct_reader_obj.h \
#    readers/ct_reader_obj_def_models.h \
#    readers/ct_reader_asciigrid3d.h \
#    readers/ct_reader_asciigrid3d_def_models.h \
#    readers/ct_reader_pgm.h \
#    readers/ct_reader_pgm_def_models.h \
#    readers/ct_reader_pbm.h \
#    readers/ct_reader_pbm_def_models.h \
#    readers/ct_reader_points_ascii.h \
#    readers/ct_reader_points_ascii_def_models.h \
#    readers/ct_reader_idxyz.h \
#    readers/ct_reader_idxyz_def_models.h \
    readers/headers/ct_xybheader.h \
#    readers/ct_reader_ptx.h \
#    readers/tools/ply/internal/plyelement.h \
#    readers/tools/ply/internal/plyproperty.h \
#    readers/tools/ply/internal/plytype.h \
#    readers/tools/ply/plyheader.h \
#    readers/tools/ply/plyheaderreader.h \
#    readers/headers/ct_plyheader.h \
#    readers/ct_reader_ply.h \
#    readers/ct_reader_ply_def_models.h \
#    readers/tools/ply/plyfilepartreader.h \
#    readers/tools/ply/plyfilepartreader.hpp \
#    readers/tools/ply/internal/plyelementreader.h \
#    readers/tools/ply/internal/setter/plyabstractvaluessetter.h \
#    readers/tools/ply/internal/setter/plydirectvaluessetter.h \
#    readers/tools/ply/internal/setter/plyfillvaluessetter.h \
#    readers/tools/ply/internal/plyreadertools.h \
#    readers/tools/ply/internal/setter/plyvaluesrecorder.h \
#    readers/tools/ply/internal/plyqtextstream.h \
#    readers/tools/ply/plycomputreewrappers.h \
#    readers/tools/ply/views/gplyreadconfiguration.h \
#    readers/tools/ply/views/sub/gplycolorconfiguration.h \
#    readers/tools/ply/views/sub/gplyscalarconfiguration.h \
#    readers/tools/ply/views/sub/gplynormalconfiguration.h \
#    readers/tools/ply/views/tools/plyviewtools.h \
#    readers/tools/ply/views/sub/iplygenericconfiguration.h \
#    readers/tools/ply/views/sub/gplyvertexconfiguration.h \
#    readers/tools/ply/plyreadconfiguration.h \
#    readers/tools/ply/internal/plyvertexconfiguration.h \
#    readers/tools/ply/internal/plycolorconfiguration.h \
#    readers/tools/ply/internal/plynormalconfiguration.h \
#    readers/tools/ply/internal/plyscalarconfiguration.h \
#    readers/tools/ply/internal/setter/plyfillscalarvaluessetter.h \
#    readers/tools/ply/internal/setter/plydirectscalarvaluessetter.h \
#    readers/tools/ply/iplyreaderlistener.h \
#    readers/ct_reader_trajectory.h \
#    readers/ct_reader_trajectory_def_model.h

SOURCES += \ 
#    readers/ct_reader_ascrgb.cpp \
#    readers/ct_reader_gdal.cpp \
#    readers/ct_reader_larchitect_grid.cpp \
#    readers/ct_reader_opf.cpp \
#    readers/ct_reader_terrascanprj.cpp \
    readers/ct_reader_xyb.cpp \
#    readers/ct_reader_obj.cpp \
#    readers/ct_reader_asciigrid3d.cpp \
#    readers/ct_reader_pgm.cpp \
#    readers/ct_reader_pbm.cpp \
#    readers/ct_reader_points_ascii.cpp \
#    readers/ct_reader_idxyz.cpp \
    readers/headers/ct_xybheader.cpp \
#    readers/ct_reader_ptx.cpp \
#    readers/tools/ply/plyheader.cpp \
#    readers/tools/ply/internal/plyelement.cpp \
#    readers/tools/ply/internal/plyproperty.cpp \
#    readers/tools/ply/plyheaderreader.cpp \
#    readers/headers/ct_plyheader.cpp \
#    readers/ct_reader_ply.cpp \
#    readers/tools/ply/internal/plyelementreader.cpp \
#    readers/tools/ply/internal/plyqtextstream.cpp \
#    readers/tools/ply/views/gplyreadconfiguration.cpp \
#    readers/tools/ply/views/sub/gplycolorconfiguration.cpp \
#    readers/tools/ply/views/sub/gplyscalarconfiguration.cpp \
#    readers/tools/ply/views/sub/gplynormalconfiguration.cpp \
#    readers/tools/ply/views/sub/gplyvertexconfiguration.cpp \
#    readers/ct_reader_trajectory.cpp

#FORMS += \
#    readers/tools/ply/views/gplyreadconfiguration.ui \
#    readers/tools/ply/views/sub/gplycolorconfiguration.ui \
#    readers/tools/ply/views/sub/gplyscalarconfiguration.ui \
#    readers/tools/ply/views/sub/gplynormalconfiguration.ui \
#    readers/tools/ply/views/sub/gplyvertexconfiguration.ui
