#ifndef MESHOBJECTCHUNKMANAGERLASTSTAGETRAITS_H
#define MESHOBJECTCHUNKMANAGERLASTSTAGETRAITS_H

#include "renderer/genericcloud/traits/genericchunkmanagerdefaultinitchunktraits.h"
#include "renderer/mesh/meshobjectrendererdefines.h"
#include "renderer/mesh/imeshobjectchunkforoutlierscreator.h"
#include "renderer/mesh/traits/meshobjectchunkmanageraddobjecttraits.h"
#include "renderer/renderercontext.h"

#define MeshObjectChunkManagerLastStageTraitsSuperClass MeshObjectChunkManagerAddObjectTraits< GenericChunkManagerDefaultInitChunkTraits< ChunkManagerSuperClassT > >

/**
 *@brief Inherit from this class to add all default methods to the last mesh chunk manager class.
 */
template<class ChunkManagerSuperClassT, Scene::ObjectType ObjectTypeT >
class MeshObjectChunkManagerLastStageTraits : public MeshObjectChunkManagerLastStageTraitsSuperClass
{
public:
    MeshObjectChunkManagerLastStageTraits(GLenum glMode,
                                          const quint8& nVertexPerObject,
                                          const size_t& beginGlobalIndex,
                                          const size_t& cloudSize) : MeshObjectChunkManagerLastStageTraitsSuperClass(glMode,
                                                                                                                     nVertexPerObject,
                                                                                                                     beginGlobalIndex,
                                                                                                                     cloudSize)
    {
        m_vertexCloudGlobalBeginIndex = 0;
        m_vertexCloudSize = 0;
    }

    ~MeshObjectChunkManagerLastStageTraits()
    {
        AllChunksCollectionIterator it(m_chunksFullAndAvailable);

        while(it.hasNext()) {
            const QList<MeshObjectNM::FinalChunkType*>& list = it.next().value();

            QListIterator<MeshObjectNM::FinalChunkType*> itL(list);

            itL.next();

            while(itL.hasNext())
                delete itL.next();
        }
    }

    /**
     * @brief Do nothing : Only for compatibility with previous stage
     */
    virtual void setChunkForOutliersCreator(const IMeshObjectChunkForOutliersCreator* creator)
    {
        Q_UNUSED(creator);
    }

    /**
     * @brief Change the begin of the VERTEX cloud (to pass it to the chunk)
     */
    void changeBeginningOfVertexCloud(const size_t& beginIndex)
    {
        QMutexLocker locker(this->m_mutex);

        if(m_vertexCloudGlobalBeginIndex != beginIndex) {
            m_vertexCloudGlobalBeginIndex = beginIndex;

            typename MeshObjectChunkManagerLastStageTraitsSuperClass::ChunkCollection::iterator it = this->m_chunks.begin();
            typename MeshObjectChunkManagerLastStageTraitsSuperClass::ChunkCollection::iterator end = this->m_chunks.end();

            while(it != end) {
                (*it)->changeBeginningOfVertexCloud(beginIndex);
                ++it;
            }
        }
    }

    /**
     * @brief Set the size of the VERTEX cloud
     * @warning Must only be called after this object was created (in "initChunk" method). It must not changed after that !
     */
    void setVertexCloudSize(const size_t& size)
    {
        m_vertexCloudSize = size;
    }

    /**
     * @brief Set the offset to use when draw objects
     * @warning Must only be called after this object was created (in "initChunk" method). It must not changed after that !
     */
    void setOffset(const Eigen::Vector3d& offset)
    {
        m_offset = offset;
    }

    /**
     * @brief Redefined to be able to create new chunk if the last used is full
     */
    MeshObjectNM::FinalChunkType& createOrGetChunkFromUniqueIndex(const size_t& uniqueIndex)
    {
        // get the last chunk created if exist
        QList<MeshObjectNM::FinalChunkType*> allChunks = m_chunksFullAndAvailable.value(uniqueIndex);

        if(allChunks.isEmpty())
            allChunks.append(&MeshObjectChunkManagerLastStageTraitsSuperClass::createOrGetChunkFromUniqueIndex(uniqueIndex));

        MeshObjectNM::FinalChunkType* lastChunk = allChunks.last();

        // if the chunk is full
        if(lastChunk->countObjects() == lastChunk->getObjectCloudSize()) {
            // we must create another chunk and add it to the collection
            lastChunk = createNewChunk(lastChunk->getBeginningOfObjectCloud(), lastChunk->getObjectCloudSize());
            this->initChunk(lastChunk);

            allChunks.append(lastChunk);

            m_chunksFullAndAvailable.insert(uniqueIndex, allChunks);
        }

        return *lastChunk;
    }

protected:
    /**
     * @brief Redefined to change begin of all chunks in the collection
     */
    void changeBeginningOfCloudOfChunk(MeshObjectNM::FinalChunkType* chunk, const size_t& newBegin)
    {
        MeshObjectChunkManagerLastStageTraitsSuperClass::changeBeginningOfCloudOfChunk(chunk, newBegin);

        AllChunksCollectionIterator it(m_chunksFullAndAvailable);

        while(it.hasNext()) {
            it.next();
            const QList<MeshObjectNM::FinalChunkType*>& list = it.value();

            if((list.size() > 1)
                    && (list.first() == chunk)) {
                QListIterator<MeshObjectNM::FinalChunkType*> itL(list);

                itL.next();

                while(itL.hasNext())
                    itL.next()->changeBeginningOfCloud(newBegin);

                return;
            }
        }
    }

    /**
     * @brief Redefined to remove all others chunks from memory
     */
    void chunkAboutToBeDeleted(MeshObjectNM::FinalChunkType* chunk)
    {
        MeshObjectChunkManagerLastStageTraitsSuperClass::chunkAboutToBeDeleted(chunk);

        AllChunksCollectionMutableIterator it(m_chunksFullAndAvailable);

        while(it.hasNext()) {
            it.next();
            const QList<MeshObjectNM::FinalChunkType*>& list = it.value();

            if((list.size() > 1)
                    && (list.first() == chunk)) {
                QListIterator<MeshObjectNM::FinalChunkType*> itL(list);

                itL.next();

                while(itL.hasNext())
                    delete itL.next();

                it.remove();
                return;
            }
        }
    }

    /**
     * @brief Redefined to create the MeshFaceNM::FinalChunkType correctly
     */
    MeshObjectNM::FinalChunkType* createNewChunk(const size_t& globalBeginIndex,
                                                 const size_t& size)
    {
        return new MeshObjectNM::FinalChunkType(0,
                                                ObjectTypeT,
                                                this->getGlMode(),
                                                this->getNVertexPerObject(),
                                                m_vertexCloudGlobalBeginIndex,
                                                m_vertexCloudSize,
                                                globalBeginIndex,
                                                size,
                                                m_offset);
    }

private:
    typedef QHash<size_t, QList<MeshObjectNM::FinalChunkType*> >                    AllChunksCollection;
    typedef QHashIterator<size_t, QList<MeshObjectNM::FinalChunkType*> >            AllChunksCollectionIterator;
    typedef QMutableHashIterator<size_t, QList<MeshObjectNM::FinalChunkType*> >     AllChunksCollectionMutableIterator;

    AllChunksCollection     m_chunksFullAndAvailable;
    size_t                  m_vertexCloudGlobalBeginIndex;
    size_t                  m_vertexCloudSize;
    Eigen::Vector3d         m_offset;
};

#endif // MESHOBJECTCHUNKMANAGERLASTSTAGETRAITS_H
