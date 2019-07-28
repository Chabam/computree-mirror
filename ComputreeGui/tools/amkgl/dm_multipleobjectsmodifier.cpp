#include "dm_multipleobjectsmodifier.h"

#include "renderer/ichunk.h"
#include "scene/permanentitemscenebymodel.h"
#include "visitor/applycustomfunctiontoobjectvisitor.h"
#include "picker/tools/objectsflagstool.h"

#include "ct_color.h"
#include "ct_normal.h"
#include "elementinfo.h"

#define DO_TO_ALL_CHUNKS(FUNC, PARAM) ChunkCollectionIterator it(m_chunks); \
                               while(it.hasNext()) { \
                                    IChunk* chunk = it.next(); \
                                    chunk->FUNC(PARAM, 0, chunk->countObjects()); \
                               }



DM_MultipleObjectsModifier::DM_MultipleObjectsModifier(const PermanentItemSceneByModel* scene,
                                                       const Scene::ObjectType& objectType,
                                                       const CT_AbstractItemDrawable *item,
                                                       bool createAsModifier)
{
    Q_UNUSED(createAsModifier)

    ApplyCustomFunctionToObjectVisitor visitor;

    ChunkCollection& chunks = m_chunks;
    size_t& nObjects = m_nObjects;

    visitor.setVisitFunction([&chunks, &nObjects](const IChunk* chunk) {
        nObjects += chunk->countObjects();
        chunks.append((IChunk*)chunk);
    });

    if(item == nullptr)
        scene->getPermanentItemSceneForModel(nullptr)->visitObjectsOfTypes(QList<Scene::ObjectType>() << objectType, visitor);
    else
        scene->getPermanentItemSceneForModel(nullptr)->visitObjectsOfTypesOfOneItem(item, QList<Scene::ObjectType>() << objectType, visitor);

    m_lastChunk = nullptr;
}

size_t DM_MultipleObjectsModifier::getNumberOfObjects() const
{
    return m_nObjects;
}

size_t DM_MultipleObjectsModifier::getNumberOfVertexPerObject() const
{
    if(m_chunks.isEmpty())
        return 0;

    return m_chunks.first()->getNumberOfElementPerObject();
}

void DM_MultipleObjectsModifier::transform(const Eigen::Matrix4f &trMatrix)
{
    DO_TO_ALL_CHUNKS(transformObjects, trMatrix);
}

void DM_MultipleObjectsModifier::translate(const Eigen::Vector3f &translation)
{
    DO_TO_ALL_CHUNKS(translateObjects, translation);
}

void DM_MultipleObjectsModifier::rotate(const Eigen::AngleAxis<float> &rotation)
{
    DO_TO_ALL_CHUNKS(rotateObjects, rotation);
}

void DM_MultipleObjectsModifier::rotate(const Eigen::Quaternion<float> &rotation)
{
    DO_TO_ALL_CHUNKS(rotateObjects, rotation);
}

void DM_MultipleObjectsModifier::scale(const Eigen::Vector3f &scaling)
{
    DO_TO_ALL_CHUNKS(scaleObjects, scaling);
}

void DM_MultipleObjectsModifier::transform(const size_t& objectIndex,
                                           const Eigen::Matrix4f &trMatrix)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    chunk->transformObjects(trMatrix, localIndex, 1);
}

void DM_MultipleObjectsModifier::translate(const size_t& objectIndex,
                                           const Eigen::Vector3f &translation)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    chunk->translateObjects(translation, localIndex, 1);
}

void DM_MultipleObjectsModifier::rotate(const size_t& objectIndex,
                                        const Eigen::AngleAxis<float> &rotation)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    chunk->rotateObjects(rotation, localIndex, 1);
}

void DM_MultipleObjectsModifier::rotate(const size_t& objectIndex,
                                        const Eigen::Quaternion<float> &rotation)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    chunk->rotateObjects(rotation, localIndex, 1);
}

void DM_MultipleObjectsModifier::scale(const size_t& objectIndex,
                                       const Eigen::Vector3f &scaling)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    chunk->scaleObjects(scaling, localIndex, 1);
}

bool DM_MultipleObjectsModifier::isColorsAvailable() const
{
    return !m_chunks.isEmpty() && (m_chunks.first()->getObjectColorCloud() != nullptr);
}

const CT_Color& DM_MultipleObjectsModifier::getColor(const size_t &objectIndex,
                                                     const size_t &vertexIndex) const
{
    size_t localIndex;

    IChunk* chunk = const_cast<DM_MultipleObjectsModifier*>(this)->findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractColorCloud* cloud = chunk->getObjectColorCloud();
    return (*cloud)[localIndex][vertexIndex];
}

void DM_MultipleObjectsModifier::setColor(const CT_Color &newColor)
{
    if(m_nObjects == 0)
        return;

    ChunkCollectionIterator it(m_chunks);

    while(it.hasNext()) {
        Basic::AbstractColorCloud* colorCloud = it.next()->createOrGetObjectColorCloud();
        size_t count = colorCloud->size();

        if(count > 0) {
            do {
                (*colorCloud)[--count] = newColor;
            }while(count != 0);
        }
    }
}

void DM_MultipleObjectsModifier::setColor(const size_t &objectIndex, const CT_Color &newColor)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractColorCloud* cloud = chunk->createOrGetObjectColorCloud();
    (*cloud)[localIndex] = newColor;
}

void DM_MultipleObjectsModifier::setColor(const size_t &objectIndex, const size_t &vertexIndex, const CT_Color &newColor)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractColorCloud* cloud = chunk->createOrGetObjectColorCloud();
    (*cloud)[localIndex][vertexIndex] = newColor;
}

bool DM_MultipleObjectsModifier::isNormalsAvailable() const
{
    return !m_chunks.isEmpty() && (m_chunks.first()->getObjectNormalCloud() != nullptr);
}

const CT_Normal& DM_MultipleObjectsModifier::getNormal(const size_t &objectIndex, const size_t &vertexIndex) const
{
    size_t localIndex;

    IChunk* chunk = const_cast<DM_MultipleObjectsModifier*>(this)->findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractNormalCloud* cloud = chunk->getObjectNormalCloud();
    return (*cloud)[localIndex][vertexIndex];
}

void DM_MultipleObjectsModifier::setNormal(const CT_Normal &newNormal)
{
    if(m_nObjects == 0)
        return;

    ChunkCollectionIterator it(m_chunks);

    while(it.hasNext()) {
        Basic::AbstractNormalCloud* cloud = it.next()->createOrGetObjectNormalCloud();
        size_t count = cloud->size();

        if(count > 0) {
            do {
                (*cloud)[--count] = newNormal;
            }while(count != 0);
        }
    }
}

void DM_MultipleObjectsModifier::setNormal(const size_t &objectIndex, const CT_Normal &newNormal)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractNormalCloud* cloud = chunk->createOrGetObjectNormalCloud();
    (*cloud)[localIndex] = newNormal;
}

void DM_MultipleObjectsModifier::setNormal(const size_t &objectIndex,
                                           const size_t &vertexIndex,
                                           const CT_Normal &newNormal)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractNormalCloud* cloud = chunk->createOrGetObjectNormalCloud();
    (*cloud)[localIndex][vertexIndex] = newNormal;
}

bool DM_MultipleObjectsModifier::isInfosAvailable() const
{
    return !m_chunks.isEmpty() && (m_chunks.first()->getObjectInfoCloud() != nullptr);
}

bool DM_MultipleObjectsModifier::isVisible(const size_t &objectIndex) const
{
    size_t localIndex;

    IChunk* chunk = const_cast<DM_MultipleObjectsModifier*>(this)->findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractInfoCloud* cloud = chunk->getObjectInfoCloud();
    return ObjectsFlagsTool::staticIsObjectVisible((*cloud)[localIndex]);
}

void DM_MultipleObjectsModifier::setVisible(bool visible)
{
    ChunkCollectionIterator it(m_chunks);
    while(it.hasNext()) {
        IChunk* chunk = it.next();
        chunk->setObjectsVisible(0, chunk->countObjects(), visible);
    }
}

void DM_MultipleObjectsModifier::setVisible(const size_t &objectIndex,
                                            bool visible)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractInfoCloud* cloud = chunk->createOrGetObjectInfoCloud();

    if(visible) {
        Basic::AbstractInfoCloud::return_type rt = (*cloud)[localIndex];
        ObjectsFlagsTool::staticShowObject(rt);
    } else {
        Basic::AbstractInfoCloud::return_type rt = (*cloud)[localIndex];
        ObjectsFlagsTool::staticHideObjectAndUnselectIt(rt);
    }
}

bool DM_MultipleObjectsModifier::isSelected(const size_t &objectIndex) const
{
    size_t localIndex;

    IChunk* chunk = const_cast<DM_MultipleObjectsModifier*>(this)->findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractInfoCloud* cloud = chunk->getObjectInfoCloud();
    return ObjectsFlagsTool::staticIsObjectSelected((*cloud)[localIndex]);
}

void DM_MultipleObjectsModifier::setSelected(bool selected)
{
    if(m_nObjects == 0)
        return;

    ChunkCollectionIterator it(m_chunks);

    if(selected) {
        while(it.hasNext()) {
            Basic::AbstractInfoCloud* cloud = it.next()->createOrGetObjectInfoCloud();
            size_t count = cloud->size();

            if(count > 0) {
                do {
                    Basic::AbstractInfoCloud::return_type rt = (*cloud)[--count];
                    ObjectsFlagsTool::staticSelectObject(rt);
                }while(count != 0);
            }
        }
    } else {
        while(it.hasNext()) {
            Basic::AbstractInfoCloud* cloud = it.next()->getObjectInfoCloud();

            if(cloud != nullptr) {
                size_t count = cloud->size();

                if(count > 0) {
                    do {
                        Basic::AbstractInfoCloud::return_type rt = (*cloud)[--count];
                        ObjectsFlagsTool::staticUnselectObject(rt);
                    }while(count != 0);
                }
            }
        }
    }
}

void DM_MultipleObjectsModifier::setSelected(const size_t &objectIndex,
                                             bool selected)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractInfoCloud* cloud = chunk->createOrGetObjectInfoCloud();

    if(selected) {
        Basic::AbstractInfoCloud::return_type rt = (*cloud)[localIndex];
        ObjectsFlagsTool::staticSelectObject(rt);
    } else {
        Basic::AbstractInfoCloud::return_type rt = (*cloud)[localIndex];
        ObjectsFlagsTool::staticUnselectObject(rt);
    }
}

bool DM_MultipleObjectsModifier::isFlagOn(const size_t &objectIndex,
                                          const GraphicsObjectFlag &flag) const
{
    size_t localIndex;

    IChunk* chunk = const_cast<DM_MultipleObjectsModifier*>(this)->findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractInfoCloud* cloud = chunk->getObjectInfoCloud();
    return ObjectsFlagsTool::staticIsFlagSet((*cloud)[localIndex], ObjectsFlagsTool::Flag(flag));
}

GraphicsObjectFlags DM_MultipleObjectsModifier::getFlagsValue(const size_t &objectIndex) const
{

    size_t localIndex;

    IChunk* chunk = const_cast<DM_MultipleObjectsModifier*>(this)->findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractInfoCloud* cloud = chunk->getObjectInfoCloud();
    return GraphicsObjectFlags(quint8((*cloud)[localIndex]));
}

void DM_MultipleObjectsModifier::setFlags(const GraphicsObjectFlags &flags,
                                          bool on)
{
    if(m_nObjects == 0)
        return;

    ChunkCollectionIterator it(m_chunks);

    if(on) {
        while(it.hasNext()) {
            Basic::AbstractInfoCloud* cloud = it.next()->createOrGetObjectInfoCloud();
            size_t count = cloud->size();

            if(count > 0) {
                do {
                    Basic::AbstractInfoCloud::return_type rt = (*cloud)[--count];
                    ObjectsFlagsTool::staticSetFlags(rt, quint8(flags));
                }while(count != 0);
            }
        }
    } else {
        while(it.hasNext()) {
            Basic::AbstractInfoCloud* cloud = it.next()->getObjectInfoCloud();

            if(cloud != nullptr) {
                size_t count = cloud->size();

                if(count > 0) {
                    do {
                        Basic::AbstractInfoCloud::return_type rt = (*cloud)[--count];
                        ObjectsFlagsTool::staticUnsetFlags(rt, quint8(flags));
                    }while(count != 0);
                }
            }
        }
    }
}

void DM_MultipleObjectsModifier::setFlags(const size_t &objectIndex,
                                  const GraphicsObjectFlags &flags,
                                  bool on)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractInfoCloud* cloud = chunk->createOrGetObjectInfoCloud();

    if(on) {
        Basic::AbstractInfoCloud::return_type rt = (*cloud)[localIndex];
        ObjectsFlagsTool::staticSetFlags(rt, quint8(flags));
    } else {
        Basic::AbstractInfoCloud::return_type rt = (*cloud)[localIndex];
        ObjectsFlagsTool::staticUnsetFlags(rt, quint8(flags));
    }
}

void DM_MultipleObjectsModifier::toggleFlags(const GraphicsObjectFlags &flags)
{
    if(m_nObjects == 0)
        return;

    ChunkCollectionIterator it(m_chunks);

    while(it.hasNext()) {
        Basic::AbstractInfoCloud* cloud = it.next()->createOrGetObjectInfoCloud();
        size_t count = cloud->size();

        if(count > 0) {
            do {
                Basic::AbstractInfoCloud::return_type rt = (*cloud)[--count];
                ObjectsFlagsTool::staticToggleFlags(rt, quint8(flags));
            }while(count != 0);
        }
    }
}

void DM_MultipleObjectsModifier::toggleFlags(const size_t &objectIndex,
                                     const GraphicsObjectFlags &flags)
{
    size_t localIndex;

    IChunk* chunk = findChunkForObjectIndex(objectIndex, localIndex);

    Q_ASSERT(chunk != nullptr);

    Basic::AbstractInfoCloud* cloud = chunk->createOrGetObjectInfoCloud();
    Basic::AbstractInfoCloud::return_type rt = (*cloud)[localIndex];
    ObjectsFlagsTool::staticToggleFlags(rt, quint8(flags));
}


IChunk* DM_MultipleObjectsModifier::findChunkForObjectIndex(const size_t &objectIndex,
                                                            size_t& chunkLocalObjectIndex)
{
    // if we have already do a search
    if(m_lastChunk != nullptr) {
        size_t nObject = m_lastChunk->countObjects();

        // check if the object index is in the last chunk founded
        if((nObject = (m_lastCount+nObject)) > objectIndex) {
            chunkLocalObjectIndex = objectIndex-m_lastCount;
            return m_lastChunk;
        }

        // if not we check the next chunk
        size_t count = nObject;

        if((m_lastChunkIndex+1) < m_chunks.size()) {
            IChunk* chunk = m_chunks.at(m_lastChunkIndex+1);

            nObject = chunk->countObjects();

            if((nObject = (count+nObject)) > objectIndex) {
                chunkLocalObjectIndex = objectIndex-count;

                m_lastChunk = chunk;
                ++m_lastChunkIndex;
                m_lastCount = count;

                return m_lastChunk;
            }
        }

        // if not we check the previous
        if(m_lastChunkIndex > 0) {
            IChunk* chunk = m_chunks.at(m_lastChunkIndex-1);

            nObject = chunk->countObjects();
            count = m_lastCount - nObject;

            if((nObject = (count+nObject)) > objectIndex) {
                chunkLocalObjectIndex = objectIndex-count;

                m_lastChunk = chunk;
                --m_lastChunkIndex;
                m_lastCount = count;

                return m_lastChunk;
            }
        }
    }

    // search the chunk
    m_lastChunk = nullptr;
    m_lastCount = 0;
    m_lastChunkIndex = 0;

    ChunkCollectionIterator it(m_chunks);

    while(it.hasNext()) {
        IChunk* chunk = it.next();
        m_lastChunk = chunk;

        size_t nObject = chunk->countObjects();

        if((nObject = (m_lastCount+nObject)) > objectIndex) {
            chunkLocalObjectIndex = objectIndex-m_lastCount;
            return m_lastChunk;
        }

        m_lastCount = nObject;
        m_lastChunkIndex++;
    }

    return nullptr;
}
