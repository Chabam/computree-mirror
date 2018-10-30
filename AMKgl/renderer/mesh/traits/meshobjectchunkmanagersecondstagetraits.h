#ifndef MESHOBJECTCHUNKMANAGERSECONDSTAGETRAITS_H
#define MESHOBJECTCHUNKMANAGERSECONDSTAGETRAITS_H

#include "meshobjectchunkmanagervertexstagetraits.h"
#include "meshobjectchunkmanageraddobjecttraits.h"

#define MeshObjectDefaultTraitsSetAndPassToNextChunks(M_VAR, TYPEOFVAR, VAR, FUNC) \
    QMutexLocker locker(m_mutex); \
 \
    if(M_VAR != VAR) { \
        M_VAR = (TYPEOFVAR)VAR; \
 \
        typedef typename ChunkManagerSuperClassT::ChunkCollection ChunkCollection; \
        typename ChunkCollection::iterator it = m_chunks.begin(); \
        typename ChunkCollection::iterator end = m_chunks.end(); \
 \
        while(it != end) { \
            (*it)->FUNC(M_VAR); \
            ++it; \
        } \
    }

#define MeshObjectChunkManagerSecondStageTraitsSuperClass MeshObjectChunkManagerAddObjectTraits<MeshObjectChunkManagerFirstStageTraits< ChunkManagerSuperClassT > >

/**
 *@brief Inherit from this class to add all default methods to a mesh chunk manager class.
 */
template< class ChunkManagerSuperClassT >
class MeshObjectChunkManagerSecondStageTraits : public MeshObjectChunkManagerSecondStageTraitsSuperClass
{
public:

    MeshObjectChunkManagerSecondStageTraits(GLenum glMode,
                                        const quint8& nVertexPerObject,
                                        const size_t& beginGlobalIndex,
                                        const size_t& cloudSize) : MeshObjectChunkManagerSecondStageTraitsSuperClass(glMode,
                                                                                           nVertexPerObject,
                                                                                           beginGlobalIndex,
                                                                                           cloudSize)
    {
        m_vertexCloudGlobalBeginIndex = 0;
        m_vertexCloudSize = 0;
    }

    /**
     * @brief Set the begin of the VERTEX cloud (to pass it to the chunk)
     */
    void changeBeginningOfVertexCloud(const size_t& beginIndex)
    {        
        MeshObjectDefaultTraitsSetAndPassToNextChunks(m_vertexCloudGlobalBeginIndex,
                                                      size_t,
                                                      beginIndex,
                                                      changeBeginningOfVertexCloud);
    }

    /**
     * @brief Set the size of the VERTEX cloud (to pass it to the chunk)
     */
    void setVertexCloudSize(const size_t& size)
    {        
        MeshObjectDefaultTraitsSetAndPassToNextChunks(m_vertexCloudSize,
                                                      size_t,
                                                      size,
                                                      setVertexCloudSize);
    }

    /**
     * @brief Set the offset to use when draw objects (to pass it to the chunk)
     */
    void setOffset(const Eigen::Vector3d& offset)
    {
        MeshObjectDefaultTraitsSetAndPassToNextChunks(m_offset,
                                                      Eigen::Vector3d,
                                                      offset,
                                                      setOffset);
    }

private:
    size_t                              m_vertexCloudGlobalBeginIndex;
    size_t                              m_vertexCloudSize;
    Eigen::Vector3d                     m_offset;

protected:

    /**
     * @brief Init the chunk (you can redefine this method if you want to init it differently)
     */
    void initChunk(typename MeshObjectChunkManagerAddObjectVertexesTraits< ChunkManagerSuperClassT >::ChunkType* chunk)
    {
        chunk->changeBeginningOfVertexCloud(m_vertexCloudGlobalBeginIndex);
        chunk->setVertexCloudSize(m_vertexCloudSize);
        chunk->setOffset(m_offset);

        MeshObjectChunkManagerSecondStageTraitsSuperClass::initChunk(chunk);
    }
};

#endif // MESHOBJECTCHUNKMANAGERSECONDSTAGETRAITS_H
