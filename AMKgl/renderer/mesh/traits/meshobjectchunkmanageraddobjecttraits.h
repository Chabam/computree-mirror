#ifndef MESHOBJECTCHUNKMANAGERADDOBJECTTRAITS_H
#define MESHOBJECTCHUNKMANAGERADDOBJECTTRAITS_H

#include "renderer/ichunk.h"

/**
 *@brief Inherit from this class to add the method "addObjectVertexes" to your class. It will search for a
 *       chunk that can add the "objectIndex".
 */
template< class SuperClassT >
class MeshObjectChunkManagerAddObjectTraits : public SuperClassT
{
public:

    MeshObjectChunkManagerAddObjectTraits(GLenum glMode,
                                          const quint8& nVertexPerObject,
                                          const size_t& beginGlobalIndex,
                                          const size_t& cloudSize) : SuperClassT(glMode,
                                                                                 nVertexPerObject,
                                                                                 beginGlobalIndex,
                                                                                 cloudSize)
    {
    }

    virtual ~MeshObjectChunkManagerAddObjectTraits() {}

    /**
     * @brief Add an object and his vertexes to the founded chunk (search a chunk for the object index)
     * @param objectIndex : object global index
     * @param vertexGlobalIndexes : global indices of vertex (the size of the array must be equal to getNVertexPerObject())
     * @param chunk : the chunk where the object was added (can be nullptr if you don't want to know)
     */
    virtual void addObjectVertexes(const size_t& objectIndex,
                                   const size_t* vertexGlobalIndexes,
                                   IChunk** chunk = nullptr)
    {
        size_t chunkI = this->getChunkUniqueIndexForObjectAt(objectIndex);

        typename SuperClassT::ChunkType& ch = this->createOrGetChunkFromUniqueIndex(chunkI);

        ch.addObjectVertexes(objectIndex,
                             vertexGlobalIndexes,
                             chunk);
    }
};

#endif // MESHOBJECTCHUNKMANAGERADDOBJECTTRAITS_H
