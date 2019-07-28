#ifndef MESHOBJECTCHUNKMANAGERADDOBJECTVERTEXESTRAITS_H
#define MESHOBJECTCHUNKMANAGERADDOBJECTVERTEXESTRAITS_H

#include "renderer/ichunk.h"
#include "renderer/mesh/imeshobjectchunkforoutlierscreator.h"

/**
 *@brief Inherit from this class to add the method "addObjectVertexes" to your class. It will search for a
 *       chunk that can add all "vertexGlobalIndexes" or if chunks was different it will add it to the chunk
 *       for outliers.
 */
template< class SuperClassT >
class MeshObjectChunkManagerAddObjectVertexesTraits : public SuperClassT
{
public:

    MeshObjectChunkManagerAddObjectVertexesTraits(GLenum glMode,
                                                  const quint8& nVertexPerObject,
                                                  const size_t& beginGlobalIndex,
                                                  const size_t& cloudSize) : SuperClassT(glMode,
                                                                                         nVertexPerObject,
                                                                                         beginGlobalIndex,
                                                                                         cloudSize)
    {
        m_chunkForOutliersCreator = nullptr;
    }

    virtual ~MeshObjectChunkManagerAddObjectVertexesTraits() {}

    /**
     * @brief Set the creator of chunk for outliers
     */
    virtual void setChunkForOutliersCreator(const IMeshObjectChunkForOutliersCreator* creator)
    {
        m_chunkForOutliersCreator = (IMeshObjectChunkForOutliersCreator*)creator;
    }

    /**
     * @brief Add an object to the founded chunk or if the chunk for each vertex is not the same we use the chunk for outliers
     * @param objectIndex : object global index
     * @param vertexGlobalIndexes : global indices of vertex (the size of the array must be equal to getNVertexPerObject())
     * @param chunk : the chunk where the object was added (can be nullptr if you don't want to know)
     */
    virtual void addObjectVertexes(const size_t& objectIndex,
                           const size_t* vertexGlobalIndexes,
                           IChunk** chunk = nullptr)
    {
        size_t chunkV1I = this->getChunkUniqueIndexForObjectAt(vertexGlobalIndexes[0]);

        bool isSameChunk = true;

        for(int i=1; isSameChunk && (i<this->getNVertexPerObject()); ++i) {
            size_t chunkVXI = this->getChunkUniqueIndexForObjectAt(vertexGlobalIndexes[i]);

            isSameChunk = (chunkVXI == chunkV1I);
        }

        if(isSameChunk) {
            typename SuperClassT::ChunkType& ch = this->createOrGetChunkFromUniqueIndex(chunkV1I);

            ch.addObjectVertexes(objectIndex,
                                 vertexGlobalIndexes,
                                 chunk);
            return;
        }

        MeshObjectNM::OutlierChunkType& ch = m_chunkForOutliersCreator->createOrGetChunkForOutliers();
        ch.addObjectVertexes(objectIndex, vertexGlobalIndexes);

        if(chunk != nullptr)
            *chunk = &ch;
    }

protected:
    IMeshObjectChunkForOutliersCreator* m_chunkForOutliersCreator;
};

#endif // MESHOBJECTCHUNKMANAGERADDOBJECTVERTEXESTRAITS_H
