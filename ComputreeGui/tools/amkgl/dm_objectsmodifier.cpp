#include "dm_objectsmodifier.h"

#include "picker/tools/objectsflagstool.h"
#include "ct_color.h"
#include "ct_normal.h"
#include "elementinfo.h"

DM_ObjectsModifier::DM_ObjectsModifier(const IChunk* chunk, const size_t &from, const size_t &count, bool createAsModifier)
{
    m_chunk = (IChunk*)chunk;

    if(createAsModifier) {
        m_colorcloud = m_chunk->createOrGetObjectColorCloud();
        m_normalcloud = m_chunk->createOrGetObjectNormalCloud();
        m_infocloud = m_chunk->createOrGetObjectInfoCloud();
    } else {
        m_colorcloud = m_chunk->getObjectColorCloud();
        m_normalcloud = m_chunk->getObjectNormalCloud();
        m_infocloud = m_chunk->getObjectInfoCloud();
    }

    m_from = from;
    m_count = count;
}

size_t DM_ObjectsModifier::getNumberOfObjects() const
{
    return m_count;
}

size_t DM_ObjectsModifier::getNumberOfVertexPerObject() const
{
    return m_chunk->getNumberOfElementPerObject();
}

void DM_ObjectsModifier::transform(const Eigen::Matrix4f &trMatrix)
{
    m_chunk->transformObjects(trMatrix, m_from, getNumberOfObjects());
}

void DM_ObjectsModifier::translate(const Eigen::Vector3f &translation)
{
    m_chunk->translateObjects(translation, m_from, getNumberOfObjects());
}

void DM_ObjectsModifier::rotate(const Eigen::AngleAxis<float> &rotation)
{
    m_chunk->rotateObjects(rotation, m_from, getNumberOfObjects());
}

void DM_ObjectsModifier::rotate(const Eigen::Quaternion<float> &rotation)
{
    m_chunk->rotateObjects(rotation, m_from, getNumberOfObjects());
}

void DM_ObjectsModifier::scale(const Eigen::Vector3f &scaling)
{
    m_chunk->scaleObjects(scaling, m_from, getNumberOfObjects());
}

void DM_ObjectsModifier::transform(const size_t& objectIndex,
                                   const Eigen::Matrix4f &trMatrix)
{
    m_chunk->transformObjects(trMatrix, m_from + objectIndex, 1);
}

void DM_ObjectsModifier::translate(const size_t& objectIndex,
                                   const Eigen::Vector3f &translation)
{
    m_chunk->translateObjects(translation, m_from + objectIndex, 1);
}

void DM_ObjectsModifier::rotate(const size_t& objectIndex,
                                const Eigen::AngleAxis<float> &rotation)
{
    m_chunk->rotateObjects( rotation, m_from + objectIndex, 1);
}

void DM_ObjectsModifier::rotate(const size_t& objectIndex,
                                const Eigen::Quaternion<float> &rotation)
{
    m_chunk->rotateObjects(rotation, m_from + objectIndex, 1);
}

void DM_ObjectsModifier::scale(const size_t& objectIndex,
                               const Eigen::Vector3f &scaling)
{
    m_chunk->scaleObjects(scaling, m_from + objectIndex, 1);
}

bool DM_ObjectsModifier::isColorsAvailable() const
{
    return m_colorcloud != nullptr;
}

const CT_Color& DM_ObjectsModifier::getColor(const size_t &objectIndex, const size_t &vertexIndex) const
{
    Q_ASSERT(isColorsAvailable());

    return (*m_colorcloud)[objectIndex + m_from][vertexIndex];
}

void DM_ObjectsModifier::setColor(const CT_Color &newColor)
{
    if(m_count == 0)
        return;

    size_t end = m_from + m_count;

    do {
        (*m_colorcloud)[--end] = newColor;
    } while(end != m_from);
}

void DM_ObjectsModifier::setColor(const size_t &objectIndex, const CT_Color &newColor)
{
    (*m_colorcloud)[m_from + objectIndex] = newColor;
}

void DM_ObjectsModifier::setColor(const size_t &objectIndex, const size_t &vertexIndex, const CT_Color &newColor)
{
    (*m_colorcloud)[m_from + objectIndex][vertexIndex] = newColor;
}

bool DM_ObjectsModifier::isNormalsAvailable() const
{
    return m_normalcloud != nullptr;
}

const CT_Normal& DM_ObjectsModifier::getNormal(const size_t &objectIndex, const size_t &vertexIndex) const
{
    Q_ASSERT(isNormalsAvailable());

    return (*m_normalcloud)[objectIndex + m_from][vertexIndex];
}

void DM_ObjectsModifier::setNormal(const CT_Normal &newNormal)
{
    if(m_count == 0)
        return;

    size_t end = m_from + m_count;

    do {
        (*m_normalcloud)[--end] = newNormal;
    } while(end != m_from);
}

void DM_ObjectsModifier::setNormal(const size_t &objectIndex, const CT_Normal &newNormal)
{
    (*m_normalcloud)[m_from + objectIndex] = newNormal;
}

void DM_ObjectsModifier::setNormal(const size_t &objectIndex, const size_t &vertexIndex, const CT_Normal &newNormal)
{
    (*m_normalcloud)[m_from + objectIndex][vertexIndex] = newNormal;
}

bool DM_ObjectsModifier::isInfosAvailable() const
{
    return m_infocloud != nullptr;
}

bool DM_ObjectsModifier::isVisible(const size_t &objectIndex) const
{
    Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
    return ObjectsFlagsTool::staticIsObjectVisible(rt);
}

void DM_ObjectsModifier::setVisible(bool visible)
{
    m_chunk->setObjectsVisible(m_from, m_count, visible);
}

void DM_ObjectsModifier::setVisible(const size_t &objectIndex, bool visible)
{
    if(visible) {
        Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
        ObjectsFlagsTool::staticShowObject(rt);
    } else {
        Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
        ObjectsFlagsTool::staticHideObjectAndUnselectIt(rt);
    }
}

bool DM_ObjectsModifier::isSelected(const size_t &objectIndex) const
{
    Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
    return ObjectsFlagsTool::staticIsObjectSelected(rt);
}

void DM_ObjectsModifier::setSelected(bool selected)
{
    if(m_count == 0)
        return;

    size_t end = m_from + m_count;

    if(selected) {
        do {
            Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[--end];
            ObjectsFlagsTool::staticSelectObject(rt);
        } while(end != m_from);
    } else {
        do {
            Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[--end];
            ObjectsFlagsTool::staticUnselectObject(rt);
        } while(end != m_from);
    }
}

void DM_ObjectsModifier::setSelected(const size_t &objectIndex, bool selected)
{
    if(selected) {
        Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
        ObjectsFlagsTool::staticSelectObject(rt);
    } else {
        Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
        ObjectsFlagsTool::staticUnselectObject(rt);
    }
}

bool DM_ObjectsModifier::isFlagOn(const size_t &objectIndex,
                                  const GraphicsObjectFlag &flag) const
{
    Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
    return ObjectsFlagsTool::staticIsFlagSet(rt, ObjectsFlagsTool::Flag(flag));
}

GraphicsObjectFlags DM_ObjectsModifier::getFlagsValue(const size_t &objectIndex) const
{

    Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
    return GraphicsObjectFlags(quint8(rt));
}

void DM_ObjectsModifier::setFlags(const GraphicsObjectFlags &flags,
                                  bool on)
{
    if(m_count == 0)
        return;

    size_t end = m_from + m_count;

    if(on) {
        do {
            Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[--end];
            ObjectsFlagsTool::staticSetFlags(rt, quint8(flags));
        } while(end != m_from);
    } else {
        do {
            Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[--end];
            ObjectsFlagsTool::staticUnsetFlags(rt, quint8(flags));
        } while(end != m_from);
    }
}

void DM_ObjectsModifier::setFlags(const size_t &objectIndex,
                                  const GraphicsObjectFlags &flags,
                                  bool on)
{
    if(on) {
        Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
        ObjectsFlagsTool::staticSetFlags(rt, quint8(flags));
    } else {
        Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
        ObjectsFlagsTool::staticUnsetFlags(rt, quint8(flags));
    }
}

void DM_ObjectsModifier::toggleFlags(const GraphicsObjectFlags &flags)
{
    if(m_count == 0)
        return;

    size_t end = m_from + m_count;

    do {
        Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[--end];
        ObjectsFlagsTool::staticToggleFlags(rt, quint8(flags));
    } while(end != m_from);
}

void DM_ObjectsModifier::toggleFlags(const size_t &objectIndex,
                                     const GraphicsObjectFlags &flags)
{
    Basic::AbstractInfoCloud::return_type rt = (*m_infocloud)[m_from + objectIndex];
    ObjectsFlagsTool::staticToggleFlags(rt, quint8(flags));
}
