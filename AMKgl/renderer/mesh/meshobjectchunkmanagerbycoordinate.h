#ifndef MESHOBJECTCHUNKMANAGERBYCOORDINATE_H
#define MESHOBJECTCHUNKMANAGERBYCOORDINATE_H

#include "renderer/genericcloud/genericcloudchunkmanagerbycoordinate.h"

#define MeshObjectChunkManagerByCoordinateSuperClass GenericCloudChunkManagerByCoordinateWithDefaultTraits< ChunkT >

template<typename ChunkT >
class MeshObjectChunkManagerByCoordinate : public MeshObjectChunkManagerByCoordinateSuperClass
{
public:
    MeshObjectChunkManagerByCoordinate(GLenum glMode,
                                       const quint8& nVertexPerObject,
                                       const size_t& beginGlobalIndex,
                                       const size_t& cloudSize) : MeshObjectChunkManagerByCoordinateSuperClass(glMode,
                                                                                                               nVertexPerObject,
                                                                                                               beginGlobalIndex,
                                                                                                               cloudSize)
    {
    }

protected:

    /**
     * @brief Init the chunk (you can redefine this method if you want to init it differently)
     */
    void initChunk(ChunkT* chunk)
    {
        Eigen::Vector3d offset;
        uint uniqueKey;

        this->getCurrentCoordinateSystemInformations(offset, uniqueKey);

        chunk->setOffset(offset);
        //chunk->setUniqueKey(uniqueKey);

        MeshObjectChunkManagerByCoordinateSuperClass::initChunk(chunk);
    }
};

#endif // MESHOBJECTCHUNKMANAGERBYCOORDINATE_H
