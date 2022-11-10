#include "dm_meshobjectsmodifier.h"

#include "tools/edgecloudattributesprovider.h"
#include "tools/facecloudattributesprovider.h"
#include "picker/tools/objectsflagstool.h"

#include "ct_cloudindex/abstract/ct_abstractcloudindext.h"
#include "ct_itemdrawable/abstract/ct_abstractitemdrawable.h"
#include "ct_accessibility/ct_iaccessedgecloud.h"
#include "ct_accessibility/ct_iaccessfacecloud.h"

DM_MeshObjectsModifier::DM_MeshObjectsModifier(const PermanentItemSceneByModel* scene,
                                               const Scene::ObjectType& objectType,
                                               const CT_AbstractItemDrawable* mesh,
                                               bool createAsModifier)
{
    // TODO : modify it to use all clouds of all scenes

    Q_ASSERT((objectType == Scene::LineGlobalCloud) || (objectType == Scene::TriangleGlobalCloud));
    if ((objectType != Scene::LineGlobalCloud) && (objectType != Scene::TriangleGlobalCloud)) {qDebug() << "DM_MeshObjectsModifier::DM_MeshObjectsModifier" << ", " <<  "(objectType != Scene::LineGlobalCloud) && (objectType != Scene::TriangleGlobalCloud)"; return;}

    m_cloudIndex = nullptr;

    PermanentItemScene* itemScene = scene->getPermanentItemSceneForModel(nullptr);

    if(objectType == Scene::LineGlobalCloud) {
        EdgeCloudAttributesProvider* provider = itemScene->getEdgeCloudAttributesProvider();

        if(createAsModifier) {
            m_colorcloud = provider->createOrGetColorCloud();
            m_normalcloud = provider->createOrGetNormalCloud();
            m_infocloud = provider->createOrGetInfoCloud();
        } else {
            m_colorcloud = provider->getColorCloud();
            m_normalcloud = provider->getNormalCloud();
            m_infocloud = provider->getInfoCloud();
        }

        const CT_IAccessEdgeCloud* edgeAccess = dynamic_cast<const CT_IAccessEdgeCloud*>(mesh);

        Q_ASSERT((mesh == nullptr) || (edgeAccess != nullptr));
        if ((mesh != nullptr) && (edgeAccess == nullptr)) {qDebug() << "DM_MeshObjectsModifier::DM_MeshObjectsModifier" << ", " <<  "(mesh != nullptr) && (edgeAccess == nullptr)"; return;}

        if(edgeAccess != nullptr) {
            m_cloudIndex = dynamic_cast<CT_AbstractCloudIndex*>((CT_AbstractEdgeCloudIndex*)edgeAccess->edgeCloudIndex());
            Q_ASSERT(m_cloudIndex != nullptr);
            if (m_cloudIndex == nullptr) {qDebug() << "DM_MeshObjectsModifier::DM_MeshObjectsModifier" << ", " <<  "m_cloudIndex == nullptr"; return;}
        }

    } else if(objectType == Scene::TriangleGlobalCloud) {
        FaceCloudAttributesProvider* provider = itemScene->getFaceCloudAttributesProvider();

        if(createAsModifier) {
            m_colorcloud = provider->createOrGetColorCloud();
            m_normalcloud = provider->createOrGetNormalCloud();
            m_infocloud = provider->createOrGetInfoCloud();
        } else {
            m_colorcloud = provider->getColorCloud();
            m_normalcloud = provider->getNormalCloud();
            m_infocloud = provider->getInfoCloud();
        }

        const CT_IAccessFaceCloud* faceAccess = dynamic_cast<const CT_IAccessFaceCloud*>(mesh);

        Q_ASSERT((mesh == nullptr) || (faceAccess != nullptr));
        if ((mesh != nullptr) && (faceAccess == nullptr)) {qDebug() << "DM_MeshObjectsModifier::DM_MeshObjectsModifier" << ", " <<  "(mesh != nullptr) && (faceAccess == nullptr)"; return;}

        if(faceAccess != nullptr) {
            m_cloudIndex = dynamic_cast<CT_AbstractCloudIndex*>((CT_AbstractEdgeCloudIndex*)faceAccess->faceCloudIndex());
            Q_ASSERT(m_cloudIndex != nullptr);
            if (m_cloudIndex == nullptr) {qDebug() << "DM_MeshObjectsModifier::DM_MeshObjectsModifier" << ", " <<  "m_cloudIndex == nullptr"; return;}
        }
    }
}

size_t DM_MeshObjectsModifier::getNumberOfObjects() const
{
    if(m_cloudIndex != nullptr)
        m_cloudIndex->size();

    return m_colorcloud->size();
}

size_t DM_MeshObjectsModifier::getNumberOfVertexPerObject() const
{
    return 1;
}

bool DM_MeshObjectsModifier::isColorsAvailable() const
{
    return m_colorcloud != nullptr;
}

const CT_Color& DM_MeshObjectsModifier::getColor(const size_t &objectIndex,
                                                  const size_t &vertexIndex) const
{
    Q_UNUSED(vertexIndex)

    if(m_cloudIndex == nullptr)
        return (*m_colorcloud)[objectIndex];

    return (*m_colorcloud)[(*m_cloudIndex)[objectIndex]];
}

void DM_MeshObjectsModifier::setColor(const CT_Color &newColor)
{
    if(m_cloudIndex == nullptr) {
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

void DM_MeshObjectsModifier::setColor(const size_t &objectIndex, const CT_Color &newColor)
{
    if(m_cloudIndex == nullptr)
        (*m_colorcloud)[objectIndex] = newColor;
    else
        (*m_colorcloud)[(*m_cloudIndex)[objectIndex]] = newColor;
}

void DM_MeshObjectsModifier::setColor(const size_t &objectIndex, const size_t &vertexIndex, const CT_Color &newColor)
{
    Q_UNUSED(vertexIndex)

    setColor(objectIndex, newColor);
}

bool DM_MeshObjectsModifier::isNormalsAvailable() const
{
    return m_normalcloud != nullptr;
}

const CT_Normal& DM_MeshObjectsModifier::getNormal(const size_t &objectIndex, const size_t &vertexIndex) const
{
    Q_UNUSED(vertexIndex)

    if(m_cloudIndex == nullptr)
        return (*m_normalcloud)[objectIndex];

    return (*m_normalcloud)[(*m_cloudIndex)[objectIndex]];
}

void DM_MeshObjectsModifier::setNormal(const CT_Normal &newNormal)
{
    if(m_cloudIndex == nullptr) {
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

void DM_MeshObjectsModifier::setNormal(const size_t &objectIndex, const CT_Normal &newNormal)
{
    if(m_cloudIndex == nullptr)
        (*m_normalcloud)[objectIndex] = newNormal;
    else
        (*m_normalcloud)[(*m_cloudIndex)[objectIndex]] = newNormal;
}

void DM_MeshObjectsModifier::setNormal(const size_t &objectIndex, const size_t &vertexIndex, const CT_Normal &newNormal)
{
    Q_UNUSED(vertexIndex)

    setNormal(objectIndex, newNormal);
}

bool DM_MeshObjectsModifier::isInfosAvailable() const
{
    return m_infocloud != nullptr;
}

bool DM_MeshObjectsModifier::isVisible(const size_t &objectIndex) const
{
    if(m_cloudIndex == nullptr)
        return ObjectsFlagsTool::staticIsObjectVisible((*m_infocloud)[objectIndex]);

    return ObjectsFlagsTool::staticIsObjectVisible((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
}

void DM_MeshObjectsModifier::setVisible(bool visible)
{
    if(m_cloudIndex == nullptr) {
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

void DM_MeshObjectsModifier::setVisible(const size_t &objectIndex, bool visible)
{
    if(m_cloudIndex == nullptr) {
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

bool DM_MeshObjectsModifier::isSelected(const size_t &objectIndex) const
{
    if(m_cloudIndex == nullptr)
        return ObjectsFlagsTool::staticIsObjectSelected((*m_infocloud)[objectIndex]);

    return ObjectsFlagsTool::staticIsObjectSelected((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
}

void DM_MeshObjectsModifier::setSelected(bool selected)
{
    if(m_cloudIndex == nullptr) {
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

void DM_MeshObjectsModifier::setSelected(const size_t &objectIndex, bool selected)
{
    if(m_cloudIndex == nullptr) {
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

bool DM_MeshObjectsModifier::isFlagOn(const size_t &objectIndex,
                                       const GraphicsObjectFlag &flag) const
{
    if(m_cloudIndex == nullptr)
        return ObjectsFlagsTool::staticIsFlagSet((*m_infocloud)[objectIndex], ObjectsFlagsTool::Flag(flag));

    return ObjectsFlagsTool::staticIsFlagSet((*m_infocloud)[(*m_cloudIndex)[objectIndex]], ObjectsFlagsTool::Flag(flag));
}

GraphicsObjectFlags DM_MeshObjectsModifier::getFlagsValue(const size_t &objectIndex) const
{
    if(m_cloudIndex == nullptr)
        return GraphicsObjectFlags((*m_infocloud)[objectIndex]);

    return GraphicsObjectFlags((*m_infocloud)[(*m_cloudIndex)[objectIndex]]);
}

void DM_MeshObjectsModifier::setFlags(const GraphicsObjectFlags &flags,
                                       bool on)
{
    if(m_cloudIndex == nullptr) {
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

void DM_MeshObjectsModifier::setFlags(const size_t &objectIndex,
                                       const GraphicsObjectFlags &flags,
                                       bool on)
{
    if(m_cloudIndex == nullptr) {
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

void DM_MeshObjectsModifier::toggleFlags(const GraphicsObjectFlags &flags)
{
    if(m_cloudIndex == nullptr) {
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

void DM_MeshObjectsModifier::toggleFlags(const size_t &objectIndex,
                                          const GraphicsObjectFlags &flags)
{
    if(m_cloudIndex == nullptr) {
        ObjectsFlagsTool::staticToggleFlags((*m_infocloud)[objectIndex], quint8(flags));
    } else {
        ObjectsFlagsTool::staticToggleFlags((*m_infocloud)[(*m_cloudIndex)[objectIndex]], quint8(flags));
    }
}

