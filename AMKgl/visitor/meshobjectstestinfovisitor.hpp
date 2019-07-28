#ifndef MESHOBJECTSTESTINFOVISITOR_HPP
#define MESHOBJECTSTESTINFOVISITOR_HPP

#include "meshobjectstestinfovisitor.h"

template<typename FoundedCloudIndex>
MeshObjectsTestInfoVisitor<FoundedCloudIndex>::MeshObjectsTestInfoVisitor(Scene::ObjectType type)
{
    Q_ASSERT((type == Scene::TriangleGlobalCloud) || (type == Scene::LineGlobalCloud));

    m_objectType = type;
    m_testFunction = nullptr;
    m_foundedObjects = new FoundedCloudIndex();
}

template<typename FoundedCloudIndex>
MeshObjectsTestInfoVisitor<FoundedCloudIndex>::~MeshObjectsTestInfoVisitor()
{
    delete m_foundedObjects;
}

template<typename FoundedCloudIndex>
void MeshObjectsTestInfoVisitor<FoundedCloudIndex>::compute(PermanentItemSceneByModel* scene)
{
    scene->visitObjectsOfTypes(QList<Scene::ObjectType>() << m_objectType, *this);
}

template<typename FoundedCloudIndex>
void MeshObjectsTestInfoVisitor<FoundedCloudIndex>::visitChunk(const IChunk* chunk)
{
    Q_ASSERT(m_testFunction != nullptr);

    const AbstractChunkGenericCloudObject* objectChunk = dynamic_cast<const AbstractChunkGenericCloudObject*>(chunk);

    ElementInfo* foin = objectChunk->getFirstObjectInfo();
    const size_t nObjects = objectChunk->countObjects();

    const size_t firstGlobalIndex = objectChunk->getBeginningOfObjectCloud();
    GLuint* foi = objectChunk->getFirstObjectIndex();

    if(foin != nullptr) {
        for(size_t i=0; i<nObjects; ++i)
        {
            const GLuint& localIndex = foi[i];

            if(m_testFunction(foin[foi[i]]))
                m_foundedObjects->addIndex(firstGlobalIndex + localIndex);
        }
    } else {
        const ElementInfo defaultInfo = ElementInfo();

        if(m_testFunction(defaultInfo)) {
            for(size_t i=0; i<nObjects; ++i)
                m_foundedObjects->addIndex(firstGlobalIndex + foi[i]);
        }
    }
}

template<typename FoundedCloudIndex>
void MeshObjectsTestInfoVisitor<FoundedCloudIndex>::setFunction(testFunction f)
{
    m_testFunction = f;
}

template<typename FoundedCloudIndex>
FoundedCloudIndex* MeshObjectsTestInfoVisitor<FoundedCloudIndex>::getFoundedObjects() const
{
    return m_foundedObjects;
}

template<typename FoundedCloudIndex>
FoundedCloudIndex* MeshObjectsTestInfoVisitor<FoundedCloudIndex>::takeFoundedObjects()
{
    FoundedCloudIndex* s = m_foundedObjects;
    m_foundedObjects = new FoundedCloudIndex();

    return s;
}

#endif // MESHOBJECTSTESTINFOVISITOR_HPP
