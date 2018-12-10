#include "dm_globalpointsmodifier.h"

#include "tools/pointcloudattributesprovider.h"
#include "picker/tools/objectsflagstool.h"

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_accessibility/ct_iaccesspointcloud.h"

DM_GlobalPointsModifier::DM_GlobalPointsModifier(const PermanentItemSceneByModel *scene,
                                                 const CT_AbstractItemDrawable* itemWithPC, bool createAsModifier)
{
    // TODO : modify it to use all clouds of all scenes

    PermanentItemScene* itemScene = scene->getPermanentItemSceneForModel(NULL);
    PointCloudAttributesProvider* provider = itemScene->getPointCloudAttributesProvider();

    if(createAsModifier) {
        m_colorcloud = provider->createOrGetColorCloud();
        m_normalcloud = provider->createOrGetNormalCloud();
        m_infocloud = provider->createOrGetInfoCloud();
    } else {
        m_colorcloud = provider->getColorCloud();
        m_normalcloud = provider->getNormalCloud();
        m_infocloud = provider->getInfoCloud();
    }

    m_cloudIndex = NULL;

    const CT_IAccessPointCloud* pointAccess = dynamic_cast<const CT_IAccessPointCloud*>(itemWithPC);

    Q_ASSERT((itemWithPC == NULL) || (pointAccess != NULL));

    if(pointAccess != NULL) {
        m_cloudIndex = dynamic_cast<CT_AbstractCloudIndex*>((CT_AbstractPointCloudIndex*)pointAccess->pointCloudIndex());
        Q_ASSERT(m_cloudIndex != NULL);
    }
}

size_t DM_GlobalPointsModifier::getNumberOfObjects() const
{
    if(m_cloudIndex != NULL)
        m_cloudIndex->size();

    return m_colorcloud->size();
}

size_t DM_GlobalPointsModifier::getNumberOfVertexPerObject() const
{
    return 1;
}

bool DM_GlobalPointsModifier::isColorsAvailable() const
{
    return m_colorcloud != NULL;
}

const CT_Color &DM_GlobalPointsModifier::getColor(const size_t &objectIndex,
                                                  const size_t &vertexIndex) const
{
    Q_UNUSED(vertexIndex)

    if(m_cloudIndex == NULL)
        return (*m_colorcloud)[objectIndex];

    return (*m_colorcloud)[(*m_cloudIndex)[objectIndex]];
}

void DM_GlobalPointsModifier::setColor(const CT_Color &newColor)
{
    if(m_cloudIndex == NULL) {
        size_t size = m_colorcloud->size();

        if(size > 0) {
            do {
                (*m_colorcloud)[--size] = newColor;
            }while(size > 0);
        }
    } else {
        size_t size = m_cloudIndex->size();

        if(size > 0) {
            do {
                (*m_colorcloud)[(*m_cloudIndex)[--size]] = newColor;
            }while(size > 0);
        }
    }
}

void DM_GlobalPointsModifier::setColor(const size_t &objectIndex, const CT_Color &newColor)
{
    if(m_cloudIndex == NULL)
        (*m_colorcloud)[objectIndex] = newColor;
    else
        (*m_colorcloud)[(*m_cloudIndex)[objectIndex]] = newColor;
}

void DM_GlobalPointsModifier::setColor(const size_t &objectIndex, const size_t &vertexIndex, const CT_Color &newColor)
{
    Q_UNUSED(vertexIndex)

    setColor(objectIndex, newColor);
}

bool DM_GlobalPointsModifier::isNormalsAvailable() const
{
    return m_normalcloud != NULL;
}

const CT_Normal &DM_GlobalPointsModifier::getNormal(const size_t &objectIndex, const size_t &vertexIndex) const
{
    Q_UNUSED(vertexIndex)

    if(m_cloudIndex == NULL)
        return (*m_normalcloud)[objectIndex];

    return (*m_normalcloud)[(*m_cloudIndex)[objectIndex]];
}

void DM_GlobalPointsModifier::setNormal(const CT_Normal &newNormal)
{
    if(m_cloudIndex == NULL) {
        size_t size = m_normalcloud->size();

        if(size > 0) {
            do {
                (*m_normalcloud)[--size] = newNormal;
            }while(size > 0);
        }
    } else {
        size_t size = m_cloudIndex->size();

        if(size > 0) {
            do {
                (*m_normalcloud)[(*m_cloudIndex)[--size]] = newNormal;
            }while(size > 0);
        }
    }
}

void DM_GlobalPointsModifier::setNormal(const size_t &objectIndex, const CT_Normal &newNormal)
{
    if(m_cloudIndex == NULL)
        (*m_normalcloud)[objectIndex] = newNormal;
    else
        (*m_normalcloud)[(*m_cloudIndex)[objectIndex]] = newNormal;
}

void DM_GlobalPointsModifier::setNormal(const size_t &objectIndex, const size_t &vertexIndex, const CT_Normal &newNormal)
{
    Q_UNUSED(vertexIndex)

    setNormal(objectIndex, newNormal);
}

bool DM_GlobalPointsModifier::isInfosAvailable() const
{
    return m_infocloud != NULL;
}

bool DM_GlobalPointsModifier::isVisible(const size_t &objectIndex) const
{
    if(m_cloudIndex == NULL)
        return ObjectsFlagsTool::staticIsObjectVisible((*m_infocloud)[objectIndex]);

    return ObjectsFlagsTool::staticIsObjectVisible((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
}

void DM_GlobalPointsModifier::setVisible(bool visible)
{
    if(m_cloudIndex == NULL) {
        size_t size = m_infocloud->size();

        if(size > 0) {
            if(visible) {
                do {
                    ObjectsFlagsTool::staticShowObject((*m_infocloud)[--size]);
                }while(size > 0);
            } else {
                do {
                    ObjectsFlagsTool::staticHideObjectAndUnselectIt((*m_infocloud)[--size]);
                }while(size > 0);
            }
        }
    } else {
        size_t size = m_cloudIndex->size();

        if(size > 0) {
            if(visible) {
                do {
                    ObjectsFlagsTool::staticShowObject((*m_infocloud)[(*m_cloudIndex)[--size]]);
                }while(size > 0);
            } else {
                do {
                    ObjectsFlagsTool::staticHideObjectAndUnselectIt((*m_infocloud)[(*m_cloudIndex)[--size]]);
                }while(size > 0);
            }
        }
    }
}

void DM_GlobalPointsModifier::setVisible(const size_t &objectIndex, bool visible)
{
    if(m_cloudIndex == NULL) {
        if(visible)
            ObjectsFlagsTool::staticShowObject((*m_infocloud)[objectIndex]);
        else
            ObjectsFlagsTool::staticHideObjectAndUnselectIt((*m_infocloud)[objectIndex]);
    } else {
        if(visible)
            ObjectsFlagsTool::staticShowObject((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
        else
            ObjectsFlagsTool::staticHideObjectAndUnselectIt((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
    }
}

bool DM_GlobalPointsModifier::isSelected(const size_t &objectIndex) const
{
    if(m_cloudIndex == NULL)
        return ObjectsFlagsTool::staticIsObjectSelected((*m_infocloud)[objectIndex]);

    return ObjectsFlagsTool::staticIsObjectSelected((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
}

void DM_GlobalPointsModifier::setSelected(bool selected)
{
    if(m_cloudIndex == NULL) {
        size_t size = m_infocloud->size();

        if(size > 0) {
            if(selected) {
                do {
                    ObjectsFlagsTool::staticSelectObject((*m_infocloud)[--size]);
                }while(size > 0);
            } else {
                do {
                    ObjectsFlagsTool::staticUnselectObject((*m_infocloud)[--size]);
                }while(size > 0);
            }
        }
    } else {
        size_t size = m_cloudIndex->size();

        if(size > 0) {
            if(selected) {
                do {
                    ObjectsFlagsTool::staticSelectObject((*m_infocloud)[(*m_cloudIndex)[--size]]);
                }while(size > 0);
            } else {
                do {
                    ObjectsFlagsTool::staticUnselectObject((*m_infocloud)[(*m_cloudIndex)[--size]]);
                }while(size > 0);
            }
        }
    }
}

void DM_GlobalPointsModifier::setSelected(const size_t &objectIndex, bool selected)
{
    if(m_cloudIndex == NULL) {
        if(selected)
            ObjectsFlagsTool::staticSelectObject((*m_infocloud)[objectIndex]);
        else
            ObjectsFlagsTool::staticUnselectObject((*m_infocloud)[objectIndex]);
    } else {
        if(selected)
            ObjectsFlagsTool::staticSelectObject((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
        else
            ObjectsFlagsTool::staticUnselectObject((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
    }
}

bool DM_GlobalPointsModifier::isFlagOn(const size_t &objectIndex,
                                       const GraphicsObjectFlag &flag) const
{
    if(m_cloudIndex == NULL)
        return ObjectsFlagsTool::staticIsFlagSet((*m_infocloud)[objectIndex], ObjectsFlagsTool::Flag(flag));

    return ObjectsFlagsTool::staticIsFlagSet((*m_infocloud)[(*m_cloudIndex)[objectIndex]], ObjectsFlagsTool::Flag(flag));
}

GraphicsObjectFlags DM_GlobalPointsModifier::getFlagsValue(const size_t &objectIndex) const
{
    if(m_cloudIndex == NULL)
        return GraphicsObjectFlags((*m_infocloud)[objectIndex]);

    return GraphicsObjectFlags((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
}

void DM_GlobalPointsModifier::setFlags(const GraphicsObjectFlags &flags,
                                       bool on)
{
    if(m_cloudIndex == NULL) {
        size_t size = m_infocloud->size();

        if(size > 0) {
            if(on) {
                do {
                    ObjectsFlagsTool::staticSetFlags((*m_infocloud)[--size], quint8(flags));
                }while(size > 0);
            } else {
                do {
                    ObjectsFlagsTool::staticUnsetFlags((*m_infocloud)[--size], quint8(flags));
                }while(size > 0);
            }
        }
    } else {
        size_t size = m_cloudIndex->size();

        if(size > 0) {
            if(on) {
                do {
                    ObjectsFlagsTool::staticSetFlags((*m_infocloud)[(*m_cloudIndex)[--size]], quint8(flags));
                }while(size > 0);
            } else {
                do {
                    ObjectsFlagsTool::staticUnsetFlags((*m_infocloud)[(*m_cloudIndex)[--size]], quint8(flags));
                }while(size > 0);
            }
        }
    }
}

void DM_GlobalPointsModifier::setFlags(const size_t &objectIndex,
                                       const GraphicsObjectFlags &flags,
                                       bool on)
{
    if(m_cloudIndex == NULL) {
        if(on)
            ObjectsFlagsTool::staticSetFlags((*m_infocloud)[objectIndex], quint8(flags));
        else
            ObjectsFlagsTool::staticUnsetFlags((*m_infocloud)[objectIndex], quint8(flags));
    } else {
        if(on)
            ObjectsFlagsTool::staticSetFlags((*m_infocloud)[(*m_cloudIndex)[objectIndex]], quint8(flags));
        else
            ObjectsFlagsTool::staticUnsetFlags((*m_infocloud)[(*m_cloudIndex)[objectIndex]], quint8(flags));
    }
}

void DM_GlobalPointsModifier::toggleFlags(const GraphicsObjectFlags &flags)
{
    if(m_cloudIndex == NULL) {
        size_t size = m_infocloud->size();

        if(size > 0) {
            do {
                ObjectsFlagsTool::staticToggleFlags((*m_infocloud)[--size], quint8(flags));
            }while(size > 0);
        }
    } else {
        size_t size = m_cloudIndex->size();

        if(size > 0) {
            do {
                ObjectsFlagsTool::staticToggleFlags((*m_infocloud)[(*m_cloudIndex)[--size]], quint8(flags));
            }while(size > 0);
        }
    }
}

void DM_GlobalPointsModifier::toggleFlags(const size_t &objectIndex,
                                          const GraphicsObjectFlags &flags)
{
    if(m_cloudIndex == NULL) {
        ObjectsFlagsTool::staticToggleFlags((*m_infocloud)[objectIndex], quint8(flags));
    } else {
        ObjectsFlagsTool::staticToggleFlags((*m_infocloud)[(*m_cloudIndex)[objectIndex]], quint8(flags));
    }
}
