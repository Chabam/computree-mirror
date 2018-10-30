#ifndef MESHOBJECTCHUNKMANAGERVERTEXSTAGETRAITS_H
#define MESHOBJECTCHUNKMANAGERVERTEXSTAGETRAITS_H

#include "meshobjectchunkmanageraddobjectvertexestraits.h"

#define MeshObjectChunkManagerVertexStageTraitsSuperClass MeshObjectChunkManagerAddObjectVertexesTraits< ChunkManagerSuperClassT >

/**
 *@brief Inherit from this class to add propagation method of IMeshObjectChunkForOutliersCreator to a mesh chunk manager class.
 */
template< class ChunkManagerSuperClassT >
class MeshObjectChunkManagerVertexStageTraits : public MeshObjectChunkManagerVertexStageTraitsSuperClass
{
public:
    MeshObjectChunkManagerVertexStageTraits(GLenum glMode,
                                            const quint8& nVertexPerObject,
                                            const size_t& beginGlobalIndex,
                                            const size_t& cloudSize) : MeshObjectChunkManagerVertexStageTraitsSuperClass(glMode,
                                                                                                                         nVertexPerObject,
                                                                                                                         beginGlobalIndex,
                                                                                                                         cloudSize)
    {
    }

protected:

    /**
     * @brief Init the chunk (you can redefine this method if you want to init it differently)
     */
    void initChunk(typename MeshObjectChunkManagerVertexStageTraitsSuperClass::ChunkType* chunk)
    {
        chunk->setChunkForOutliersCreator(this->m_chunkForOutliersCreator);

        MeshObjectChunkManagerVertexStageTraitsSuperClass::initChunk(chunk);
    }
};

#endif // MESHOBJECTCHUNKMANAGERVERTEXSTAGETRAITS_H
