QT = core gui opengl

contains ( QT_VERSION, "^5.*" ) {
    QT *= widgets
}

include(../../config/library_shared.pri)

TARGET = ctlibmodelsextraviews

DEFINES += CTLIBMODELSEXTRAVIEWS_LIBRARY

HEADERS += \
    ctlibmodelsextraviews_global.h \
    ctg_modelslinkconfigurationflowview.h \
    nodeeditor/include/nodes/internal/Compiler.hpp \
    nodeeditor/include/nodes/internal/Connection.hpp \
    nodeeditor/include/nodes/internal/ConnectionGeometry.hpp \
    nodeeditor/include/nodes/internal/ConnectionGraphicsObject.hpp \
    nodeeditor/include/nodes/internal/ConnectionState.hpp \
    nodeeditor/include/nodes/internal/ConnectionStyle.hpp \
    nodeeditor/include/nodes/internal/DataModelRegistry.hpp \
    nodeeditor/include/nodes/internal/Export.hpp \
    nodeeditor/include/nodes/internal/FlowScene.hpp \
    nodeeditor/include/nodes/internal/FlowView.hpp \
    nodeeditor/include/nodes/internal/FlowViewStyle.hpp \
    nodeeditor/include/nodes/internal/memory.hpp \
    nodeeditor/include/nodes/internal/Node.hpp \
    nodeeditor/include/nodes/internal/NodeData.hpp \
    nodeeditor/include/nodes/internal/NodeDataModel.hpp \
    nodeeditor/include/nodes/internal/NodeGeometry.hpp \
    nodeeditor/include/nodes/internal/NodeGraphicsObject.hpp \
    nodeeditor/include/nodes/internal/NodePainterDelegate.hpp \
    nodeeditor/include/nodes/internal/NodeState.hpp \
    nodeeditor/include/nodes/internal/NodeStyle.hpp \
    nodeeditor/include/nodes/internal/OperatingSystem.hpp \
    nodeeditor/include/nodes/internal/PortType.hpp \
    nodeeditor/include/nodes/internal/QStringStdHash.hpp \
    nodeeditor/include/nodes/internal/QUuidStdHash.hpp \
    nodeeditor/include/nodes/internal/Serializable.hpp \
    nodeeditor/include/nodes/internal/Style.hpp \
    nodeeditor/include/nodes/internal/TypeConverter.hpp \
    nodeeditor/include/nodes/Connection \
    nodeeditor/include/nodes/ConnectionStyle \
    nodeeditor/include/nodes/DataModelRegistry \
    nodeeditor/include/nodes/FlowScene \
    nodeeditor/include/nodes/FlowView \
    nodeeditor/include/nodes/FlowViewStyle \
    nodeeditor/include/nodes/Node \
    nodeeditor/include/nodes/NodeData \
    nodeeditor/include/nodes/NodeDataModel \
    nodeeditor/include/nodes/NodeGeometry \
    nodeeditor/include/nodes/NodePainterDelegate \
    nodeeditor/include/nodes/NodeState \
    nodeeditor/include/nodes/NodeStyle \
    nodeeditor/include/nodes/TypeConverter \
    nodeeditor/src/ConnectionBlurEffect.hpp \
    nodeeditor/src/ConnectionPainter.hpp \
    nodeeditor/src/NodeConnectionInteraction.hpp \
    nodeeditor/src/NodePainter.hpp \
    nodeeditor/src/Properties.hpp \
    nodeeditor/src/StyleCollection.hpp \
    tools/ct_modelflowdata.h \
    tools/ct_modelflowdatamodel.h \
    tools/ct_indatatypetooutdatatypeconverter.h \
    tools/ct_helpgraphicsitem.h

FORMS += \
    ctg_modelslinkconfigurationflowview.ui

SOURCES += \
    ctg_modelslinkconfigurationflowview.cpp \
    nodeeditor/src/Connection.cpp \
    nodeeditor/src/ConnectionBlurEffect.cpp \
    nodeeditor/src/ConnectionGeometry.cpp \
    nodeeditor/src/ConnectionGraphicsObject.cpp \
    nodeeditor/src/ConnectionPainter.cpp \
    nodeeditor/src/ConnectionState.cpp \
    nodeeditor/src/ConnectionStyle.cpp \
    nodeeditor/src/DataModelRegistry.cpp \
    nodeeditor/src/FlowScene.cpp \
    nodeeditor/src/FlowView.cpp \
    nodeeditor/src/FlowViewStyle.cpp \
    nodeeditor/src/Node.cpp \
    nodeeditor/src/NodeConnectionInteraction.cpp \
    nodeeditor/src/NodeDataModel.cpp \
    nodeeditor/src/NodeGeometry.cpp \
    nodeeditor/src/NodeGraphicsObject.cpp \
    nodeeditor/src/NodePainter.cpp \
    nodeeditor/src/NodeState.cpp \
    nodeeditor/src/NodeStyle.cpp \
    nodeeditor/src/Properties.cpp \
    nodeeditor/src/StyleCollection.cpp \
    tools/ct_modelflowdata.cpp \
    tools/ct_modelflowdatamodel.cpp \
    tools/ct_indatatypetooutdatatypeconverter.cpp \
    tools/ct_helpgraphicsitem.cpp

INCLUDEPATH += nodeeditor/include
INCLUDEPATH += nodeeditor/include/nodes/internal
INCLUDEPATH += nodeeditor/src

RESOURCES += \
    nodeeditor/resources/resources.qrc
