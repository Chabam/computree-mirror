#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T13:51:21
#
#-------------------------------------------------

DEFINES *= AMKGL_NO_TODO_WARNINGS

QT       += core gui xml opengl concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AMKgl
TEMPLATE = lib
CONFIG *= staticlib

DESTDIR = compiled
UI_DIR = ui
MOC_DIR = moc
OBJECTS_DIR = obj

# c++17
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++17
} else {
    QMAKE_CXXFLAGS += -std=c++17
}

linux|mingw {
    # Silent warning
    QMAKE_CXXFLAGS += -Wno-deprecated -Wno-deprecated-copy -Wno-deprecated-declarations -Wno-unused -Wmaybe-uninitialized -Wno-c++11-compat
}

macx {
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations -Wno-inconsistent-missing-override
    QMAKE_RANLIB += -no_warning_for_no_symbols
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.0
    # Silent OpenGL warning for MacOS > 10.14
    DEFINES += GL_SILENCE_DEPRECATION
}

MUST_USE_EIGEN = 1

include(../config/include_dependencies.pri)

AMKGL_DEFINES_DIR = generic

exists(AMKgl_defines_computree.pri) {
    USE_USER_DEFINITIONS = true
    # message(AMKgl_defines_computree.pri founded and used)
    include(AMKgl_defines_computree.pri)
    INCLUDEPATH += $$AMKGL_DEFINES_DIR
} else {
    error(To use this engine with your software please do a file AMKgl_defines_computree.pri in .. directory)
}

SOURCES +=\
    renderer/pointcloud/chunkedpointcloud.cpp \
    tools/opengl/openglinfo.cpp \
    drawinfo.cpp \
    scene/permanentscene.cpp \
    renderer/pointcloud/pointrenderershaders.cpp \
    picker/pointspicker.cpp \
    picker/polygonpointspicker.cpp \
    tools/opengl/shapetovolume.cpp \
    poly2tri/poly2tri/common/shapes.cc \
    poly2tri/poly2tri/sweep/advancing_front.cc \
    poly2tri/poly2tri/sweep/cdt.cc \
    poly2tri/poly2tri/sweep/sweep.cc \
    poly2tri/poly2tri/sweep/sweep_context.cc \
    picker/geometry/triangleplane.cpp \
    actions/picking/tools/polygonforpicking.cpp \
    actions/picking/tools/rectangleforpicking.cpp \
    tools/math.cpp \
    renderer/ellipse/ellipsepermanentrenderer.cpp \
    tools/opengl/ellipseglrenderer.cpp \
    tools/opengl/offsetpreparator.cpp \
    renderer/ellipse/ellipserenderershaders.cpp \
    renderer/box/boxpermanentrenderer.cpp \
    renderer/box/boxrenderershaders.cpp \
    tools/opengl/boxglrenderer.cpp \
    renderer/quads/quadspermanentrenderer.cpp \
    renderer/quads/quadsrenderershaders.cpp \
    tools/opengl/quadsglrenderer.cpp \
    renderer/abstractpermanentrenderer.cpp \
    renderer/line/linepermanentrenderer.cpp \
    renderer/generic/genericpermanentrenderer.cpp \
    renderer/generic/genericrenderercontext.cpp \
    renderer/generic/genericrenderershaders.cpp \
    renderer/generic/chunkedgeneric.cpp \
    renderer/triangle/trianglepermanentrenderer.cpp \
    tools/opengl/cylinderglrenderer.cpp \
    renderer/cylinder/cylinderpermanentrenderer.cpp \
    scene/permanentitemscene.cpp \
    scene/tools/permanentiteminformation.cpp \
    actions/picking/actionpickanyelements.cpp \
    picker/tools/pointspickingdefaultfunction.cpp \
    tools/anyelementattributesprovider.cpp \
    tools/objectattributesprovider.cpp \
    picker/genericpicker.cpp \
    picker/tools/intersectionchecker.cpp \
    scene/tools/permanentitemsceneremover.cpp \
    renderer/tools/shaderstools.cpp \
    renderer/ichunk.cpp \
    renderer/point/pointpermanentrenderer.cpp \
    renderer/pyramid/pyramidpermanentrenderer.cpp \
    tools/opengl/pyramidglrenderer.cpp \
    scene/permanentitemscenebymodel.cpp \
    picker/genericpickerbymodel.cpp \
    picker/pointspickerbymodel.cpp \
    picker/polygonpointspickerbymodel.cpp \
    picker/itemspickerbymodel.cpp \
    picker/polygonitemspickerbymodel.cpp \
    renderer/sphere/spherepermanentrenderer.cpp \
    renderer/sphere/sphererenderershaders.cpp \
    tools/opengl/sphereglrenderer.cpp \
    renderer/sphere/spherechunk.cpp \
    picker/objectsprocessorforpicker.cpp \
    picker/tools/itemspickingdefaultfunction.cpp \
    picker/objectsofitempicker.cpp \
    picker/itemspicker.cpp \
    picker/polygonitemspicker.cpp \
    amkglapp.cpp \
    picker/polygonobjectsofitempicker.cpp \
    view/amkglviewer.cpp \
    visitor/itemsselectedvisitor.cpp \
    visitor/selectitemsvisitor.cpp \
    visitor/toggleitemsselectionvisitor.cpp \
    visitor/globalpointssetinfovisitor.cpp \
    tools/itemcolorizer.cpp \
    scene/tools/dispatchinformation.cpp \
    visitor/applycustomfunctiontoitemvisitor.cpp \
    view/drawmodeconfigurator.cpp \
    view/widgettodialog.cpp \
    tools/performancestatisticmanager.cpp \
    visitor/applycustomfunctiontoobjectvisitor.cpp \
    renderer/mesh/chunkedgenericcloud.cpp \
    renderer/genericcloud/genericcloudrenderershaders.cpp \
    renderer/tools/texturemanager.cpp \
    interfaces/igenericcloudmanager.cpp \
    renderer/pointcloud/pointcloudrenderer.cpp \
    renderer/pointcloud/pointcloudchunkmanagerbycoordinate.cpp \
    renderer/pointcloud/pointcloudchunkmanagerbymemorylimit.cpp \
    interfaces/ieasyaccess.cpp \
    renderer/mesh/meshfacepermanentrenderer.cpp \
    renderer/mesh/abstractchunkgenericcloudobject.cpp \
    renderer/mesh/meshedgepermanentrenderer.cpp \
    picker/meshobjectspicker.cpp \
    picker/polygonmeshobjectspicker.cpp \
    picker/polygonmeshobjectspickerbymodel.cpp \
    picker/meshobjectspickerbymodel.cpp \
    visitor/meshobjectssetinfovisitor.cpp \
    renderer/tools/displaylistmanager.cpp \
    renderer/mesh/chunkgenericcloudobjectforoutliers.cpp \
    renderer/tools/bufferobjectmanager.cpp \
    renderer/pointcloud/pointcloudarrayobjectmanager.cpp \
    renderer/chunkcustomupdatevalues.cpp \
    renderer/generic/genericobjectarrayobjectmanager.cpp \
    visitor/applycustomfunctiontoglobalpointsvisitor.cpp \
    visitor/meshobjectsvisitor.cpp \
    renderer/tools/flagspropertymanager.cpp \
    scene/tools/objectsflagspropertymanager.cpp \
    scene/permanentsceneelementtype.cpp \
    view/amkglcamera.cpp \
    tools/amkgldomutils.cpp

HEADERS  += \
    renderer/pointcloud/chunkedpointcloud.h \
    tools/opengl/openglinfo.h \
    drawinfo.h \
    scene/permanentscene.h \
    tools/pointcloudattributesprovider.h \
    renderer/drawmode.h \
    renderer/pointcloud/pointrenderercontext.h \
    renderer/pointcloud/pointrenderershaders.h \
    picker/pointspicker.h \
    picker/polygonpointspicker.h \
    picker/geometry/plane.h \
    tools/opengl/shapetovolume.h \
    poly2tri/poly2tri/poly2tri.h \
    poly2tri/poly2tri/common/shapes.h \
    poly2tri/poly2tri/common/utils.h \
    poly2tri/poly2tri/sweep/advancing_front.h \
    poly2tri/poly2tri/sweep/cdt.h \
    poly2tri/poly2tri/sweep/sweep.h \
    poly2tri/poly2tri/sweep/sweep_context.h \
    picker/geometry/triangleplane.h \
    actions/picking/tools/polygonforpicking.h \
    actions/picking/tools/rectangleforpicking.h \
    tools/math.h \
    renderer/ellipse/ellipsepermanentrenderer.h \
    tools/opengl/ellipseglrenderer.h \
    tools/opengl/offsetpreparator.h \
    renderer/renderercontext.h \
    renderer/renderercontext.hpp \
    renderer/ellipse/ellipserenderershaders.h \
    renderer/chunk.h \
    renderer/chunk.hpp \
    renderer/permanentrenderer.h \
    renderer/permanentrenderer.hpp \
    renderer/instanced/chunkedinstanced.h \
    renderer/instanced/chunkedinstanced.hpp \
    renderer/instanced/instancedrenderershaders.h \
    renderer/instanced/instancedrenderershaders.hpp \
    renderer/ellipse/ellipseshaderparams.h \
    renderer/box/boxpermanentrenderer.h \
    renderer/box/boxshaderparams.h \
    renderer/instanced/instancedpermanentrenderer.h \
    renderer/instanced/instancedpermanentrenderer.hpp \
    renderer/box/boxrenderershaders.h \
    tools/opengl/boxglrenderer.h \
    renderer/quads/quadspermanentrenderer.h \
    renderer/quads/quadsrenderershaders.h \
    renderer/quads/quadsshaderparams.h \
    tools/opengl/quadsglrenderer.h \
    renderer/abstractpermanentrenderer.h \
    renderer/line/linepermanentrenderer.h \
    renderer/generic/generictypedef.h \
    tools/opengl/genericglrenderer.h \
    renderer/generic/genericpermanentrenderer.h \
    renderer/generic/genericrenderercontext.h \
    renderer/generic/genericrenderershaders.h \
    renderer/generic/chunkedgeneric.h \
    renderer/triangle/trianglepermanentrenderer.h \
    tools/opengl/cylinderglrenderer.h \
    renderer/cylinder/cylinderpermanentrenderer.h \
    elementinfo.h \
    scene/permanentitemscene.h \
    scene/tools/permanentiteminformation.h \
    actions/picking/actionpickanyelements.h \
    tools/anyelementattributesprovider.h \
    tools/objectattributesprovider.h \
    renderer/object/iobjectsrendererattributesaccessor.h \
    renderer/object/iobjectcloud.h \
    picker/genericpicker.h \
    picker/tools/intersectionchecker.h \
    scene/tools/permanentitemsceneremover.h \
    renderer/tools/shaderstools.h \
    scene/permanentsceneelementtype.h \
    renderer/ichunk.h \
    renderer/object/iobjectstypeaccessor.h \
    renderer/context/icontextaccessor.h \
    renderer/point/pointpermanentrenderer.h \
    renderer/pyramid/pyramidpermanentrenderer.h \
    tools/opengl/pyramidglrenderer.h \
    picker/tools/iintersectioninterceptor.h \
    scene/permanentitemscenebymodel.h \
    renderer/context/icontextaccessor.hpp \
    picker/genericpickerbymodel.h \
    picker/polygonpointspickerbymodel.h \
    picker/polygonitemspickerbymodel.h \
    renderer/attributes/iattributesaccessor.h \
    scene/tools/pointsdispatchinformation.h \
    renderer/drawmodeaccessor.h \
    renderer/sphere/spherepermanentrenderer.h \
    renderer/sphere/sphereshaderparams.h \
    renderer/sphere/sphererenderershaders.h \
    tools/opengl/sphereglrenderer.h \
    renderer/attributes/vertexattribute.h \
    renderer/instanced/defaultinstancedpermanentrenderer.h \
    renderer/sphere/spherechunk.h \
    picker/objectsprocessorforpicker.h \
    picker/tools/itemspickingdefaultfunction.h \
    picker/objectsofitempicker.h \
    picker/tools/itemspickingtools.h \
    picker/itemspicker.h \
    picker/polygonitemspicker.h \
    interfaces/iglobalpointcloudmanager.h \
    interfaces/icoordinatesystemmanager.h \
    amkglapp.h \
    interfaces/iamkgllog.h \
    picker/polygonobjectsofitempicker.h \
    interfaces/igraphicsdocument.h \
    view/amkglviewer.h \
    interfaces/ieasyaccess.h \
    picker/itemspickerbymodel.h \
    picker/pointspickerbymodel.h \
    visitor/iitemsvisitor.h \
    visitor/itemsselectedvisitor.h \
    visitor/selectitemsvisitor.h \
    visitor/toggleitemsselectionvisitor.h \
    visitor/iglobalpointsvisitor.h \
    visitor/globalpointstestinfovisitor.h \
    visitor/globalpointstestinfovisitor.hpp \
    visitor/globalpointssetinfovisitor.h \
    tools/itemcolorizer.h \
    visitor/ivisitorbymodel.h \
    visitor/iitemsvisitorbymodel.h \
    visitor/iglobalpointsvisitorbymodel.h \
    scene/tools/objectsdispatchinformation.h \
    scene/tools/dispatchinformation.h \
    visitor/applycustomfunctiontoitemvisitor.h \
    view/drawmodeconfigurator.h \
    view/widgettodialog.h \
    view/configurablewidget.h \
    visitor/iobjectsvisitor.h \
    visitor/iobjectsvisitorbymodel.h \
    tools/performancestatistic.h \
    tools/performancestatisticmanager.h \
    visitor/applycustomfunctiontoobjectvisitor.h \
    renderer/mesh/chunkedgenericcloud.h \
    renderer/genericcloud/genericcloudmode.h \
    renderer/genericcloud/genericcloudrenderershaders.h \
    renderer/tools/texturemanager.h \
    renderer/genericcloud/genericcloudrenderercontext.h \
    tools/edgecloudattributesprovider.h \
    tools/facecloudattributesprovider.h \
    interfaces/igenericcloudmanager.h \
    interfaces/iglobaledgecloudmanager.h \
    interfaces/iglobalfacecloudmanager.h \
    tools/genericcloudattributesprovider.h \
    renderer/iglobalcloudlistener.h \
    tools/icloudattributesprovider.h \
    renderer/genericcloud/genericcloudchunkmanagerbycoordinate.h \
    renderer/genericcloud/genericcloudchunkmanagerbymemorylimit.h \
    renderer/genericcloud/genericcloudchunkmanagerbycloud.h \
    renderer/genericcloud/genericchunkmanager.h \
    renderer/genericcloud/genericcloudchunkmanager.h \
    renderer/pointcloud/pointcloudrenderer.h \
    renderer/pointcloud/pointcloudchunkmanagerbycoordinate.h \
    renderer/genericcloud/traits/genericcloudchunkmanagerdefaultcreatechunktraits.h \
    renderer/genericcloud/traits/genericchunkmanagerdefaultinitchunktraits.h \
    renderer/genericcloud/genericchunkmanagerdefaultsuperclass.h \
    renderer/genericcloud/traits/genericcloudchunkmanagerdefaultallchunktraits.h \
    renderer/pointcloud/pointcloudchunkmanagerbymemorylimit.h \
    renderer/mesh/meshfacepermanentrenderer.h \
    renderer/genericcloud/genericobjectcloudchunkmanagerbytexturelimit.h \
    renderer/mesh/imeshobjectchunkforoutlierscreator.h \
    renderer/mesh/meshobjectrendererdefines.h \
    renderer/mesh/traits/meshobjectchunkmanagerlaststagetraits.h \
    renderer/mesh/meshobjectchunkmanagerbycoordinate.h \
    renderer/mesh/traits/meshobjectchunkmanagersecondstagetraits.h \
    renderer/mesh/traits/meshobjectchunkmanageraddobjectvertexestraits.h \
    renderer/mesh/traits/meshobjectchunkmanageraddobjecttraits.h \
    renderer/mesh/traits/meshobjectchunkmanagervertexstagetraits.h \
    renderer/mesh/meshobjectchunkmanagerbyobjectcloud.h \
    renderer/mesh/abstractchunkgenericcloudobject.h \
    renderer/mesh/meshedgepermanentrenderer.h \
    picker/meshobjectspicker.h \
    picker/polygonmeshobjectspicker.h \
    picker/polygonmeshobjectspickerbymodel.h \
    picker/meshobjectspickerbymodel.h \
    visitor/meshobjectstestinfovisitor.h \
    visitor/meshobjectstestinfovisitor.hpp \
    visitor/meshobjectssetinfovisitor.h \
    renderer/tools/displaylistmanager.h \
    renderer/mesh/chunkgenericcloudobjectforoutliers.h \
    renderer/tools/bufferobjectmanager.h \
    renderer/pointcloud/pointcloudarrayobjectmanager.h \
    renderer/chunkcustomupdatevalues.h \
    renderer/generic/genericobjectarrayobjectmanager.h \
    renderer/instanced/instancedarrayobjectmanager.h \
    renderer/tools/dumbarrayobjectmanager.h \
    visitor/applycustomfunctiontoglobalpointsvisitor.h \
    visitor/meshobjectsvisitor.h \
    tools/drawmodeconverter.h \
    picker/tools/objectsflagstool.h \
    picker/tools/pointspickingdefaultfunction.h \
    renderer/tools/objectsflagproperty.h \
    renderer/tools/flagspropertymanager.h \
    scene/tools/objectsflagspropertymanager.h \
    view/amkglcamera.h \
    tools/amkgldomutils.h

isEmpty(USE_USER_DEFINITIONS) {
    HEADERS  += dummy/dummypoint.h \
        dummy/dummycolor.h \
        dummy/dummynormal.h \
        dummy/dummyitem.h \
        dummy/dummygraphicsview.h \
        dummy/dummymodel.h \
        dummy/dummycloudindex.h \
        dummy/dummyglobalcloud.h \
        dummy/dummyitempainter.h \
        dummy/dummyitemmodel.h \
        dummy/dummyitempermanentpainter.h \
        dummy/dummyitempickingpainter.h
} else {
    HEADERS  += \
        $$AMKGL_DEFINES_DIR/defineitem.h \
        $$AMKGL_DEFINES_DIR/includeitem.h \
        $$AMKGL_DEFINES_DIR/definegraphicsview.h \
        $$AMKGL_DEFINES_DIR/includegraphicsview.h \
        $$AMKGL_DEFINES_DIR/definemodel.h \
        $$AMKGL_DEFINES_DIR/includemodel.h \
        $$AMKGL_DEFINES_DIR/definepoint.h \
        $$AMKGL_DEFINES_DIR/includepoint.h \
        $$AMKGL_DEFINES_DIR/definecolor.h \
        $$AMKGL_DEFINES_DIR/definenormal.h \
        $$AMKGL_DEFINES_DIR/includecolor.h \
        $$AMKGL_DEFINES_DIR/includenormal.h \
        $$AMKGL_DEFINES_DIR/defineglobalcloud.h \
        $$AMKGL_DEFINES_DIR/definecloudindexes.h \
        $$AMKGL_DEFINES_DIR/includecloudindex.h \
        $$AMKGL_DEFINES_DIR/includeglobalcloud.h \
        $$AMKGL_DEFINES_DIR/defineitempainter.h \
        $$AMKGL_DEFINES_DIR/includeitempainter.h \
        $$AMKGL_DEFINES_DIR/defineitempermanentpainter.h \
        $$AMKGL_DEFINES_DIR/includeitempermanentpainter.h \
        $$AMKGL_DEFINES_DIR/defineitempickingpainter.h \
        $$AMKGL_DEFINES_DIR/includeitempickingpainter.h \
}

FORMS    += \
    view/drawmodeconfigurator.ui

INCLUDEPATH += poly2tri/poly2tri

win32 : LIBS += -lopengl32 -lglu32
linux : LIBS += -lGL -lGLU -lm
macx  : LIBS += -framework OpenGL

RESOURCES += \
    resource.qrc
