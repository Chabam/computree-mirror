#include "ichunk.h"

#include "tools/opengl/offsetpreparator.h"
#include "scene/permanentscene.h"

#include <QtOpenGL/QtOpenGL>

IChunk::IChunk(const uint& uniqueKey, Scene::ObjectType objectType, const Eigen::Vector3d &offset)
{
    m_attributesAccessor = nullptr;

    m_uniqueKey = uniqueKey;

    m_objectType = objectType;

    m_offset = offset;

    m_updated = false;

    // By default use vao draw
    m_drawModeToUse = DM_VAO;
    m_drawModeUsedInNormal = DM_NONE;
    m_drawModeUsedInFast = DM_NONE;

    m_currentColor = Basic::LocalColor(QColor(Qt::white));
}

void IChunk::setAttributesAccessor(const IAttributesAccessor* accessor)
{
    m_attributesAccessor = (IAttributesAccessor*)accessor;
}

IAttributesAccessor* IChunk::getAttributesAccessor() const
{
    return m_attributesAccessor;
}

AnyElementAttributesProvider* IChunk::getAnyElementsAttributesProvider() const
{
    return ((getAttributesAccessor() == nullptr) ? nullptr : getAttributesAccessor()->getAnyElementsAttributesProvider());
}

ObjectAttributesProvider* IChunk::getObjectsAttributeProvider() const
{
    return ((getAttributesAccessor() == nullptr) ? nullptr : getAttributesAccessor()->getObjectsAttributeProvider(getTypeOfObjectDrawn()));
}

FlagsPropertyManager* IChunk::getFlagsPropertyManager() const
{
    return ((getAttributesAccessor() == nullptr) ? nullptr : getAttributesAccessor()->getPermanentScene()->getRendererForType(getTypeOfObjectDrawn())->getFlagsPropertyManager());
}

Scene::ObjectType IChunk::getTypeOfObjectDrawn() const
{
    return m_objectType;
}

void IChunk::setDrawModeToUse(DrawMode mode)
{
    if(m_drawModeToUse == mode)
        return;

    setUpdated(false);
    m_drawModeToUse = mode;
}

DrawMode IChunk::getDrawModeToUse() const
{
    return m_drawModeToUse;
}

DrawMode IChunk::getNormalDrawModeUsed() const
{
    // MK : Why i wrote this! it does not make sense ???
    /*if(mustDisableGraphicsCardTransfer()) {
        if((m_drawModeUsedInNormal == DM_VAO) || (m_drawModeUsedInNormal == DM_VBO))
            return DM_RAW;
    }*/

    return m_drawModeUsedInNormal;
}

DrawMode IChunk::getFastDrawModeUsed() const
{
    if(mustDisableGraphicsCardTransfer()) {
        if((m_drawModeUsedInFast == DM_VAO) || (m_drawModeUsedInFast == DM_VBO))
            return DM_RAW;
    }

    return m_drawModeUsedInFast;
}

bool IChunk::mustDisableGraphicsCardTransfer() const
{
    const PermanentScene* scene = (getAttributesAccessor() == nullptr) ? nullptr : getAttributesAccessor()->getPermanentScene();

    // disable VAO or VBO when the scene must reduce number of points in fast draw
    return (scene == nullptr) ? false : scene->mustReduceNumberOfPointsInFastDraw();
}

bool IChunk::update()
{
    return internalUpdate();
}

bool IChunk::isUpdated() const
{
    return m_updated;
}

const Eigen::Vector3d& IChunk::getOffset() const
{
    return m_offset;
}

void IChunk::visitObjects(IObjectsVisitor &visitor)
{
    visitor.visitChunk(this);
}

void IChunk::pushCoordinateSystemMatrix(DrawInfo &info)
{
    OffsetPreparator::staticPushMatrix(getOffset(), info);
}

void IChunk::popCoordinateSystemMatrix()
{
    OffsetPreparator::staticPopMatrix();
}

bool IChunk::mustUseVAO() const
{
    return (m_drawModeToUse == DM_VAO);
}

bool IChunk::mustUseVBO() const
{
    return (m_drawModeToUse == DM_VBO) || (m_drawModeToUse == DM_VAO);
}

bool IChunk::mustUseRAW() const
{
    return (m_drawModeToUse == DM_RAW);
}

void IChunk::setNormalDrawModeUsed(DrawMode mode)
{
    m_drawModeUsedInNormal = mode;
}

void IChunk::setFastDrawModeUsed(DrawMode mode)
{
    m_drawModeUsedInFast = mode;
}

void IChunk::getCurrentColor(float cColor[])
{
    glGetFloatv(GL_CURRENT_COLOR, cColor);
}

void IChunk::setUpdated(bool status)
{
    if(mustChangeUpdateStatus(status)) {
        lock();

        m_updated = status;

        unlock();
    }
}

void IChunk::setCurrentColor(const Basic::LocalColor &color)
{
    m_currentColor = color;
}

Basic::LocalColor IChunk::getCurrentColor() const
{
    return m_currentColor;
}
