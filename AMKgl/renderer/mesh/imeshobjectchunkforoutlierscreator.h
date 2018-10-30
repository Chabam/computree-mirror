#ifndef IMESHOBJECTCHUNKFOROUTLIERSCREATOR_H
#define IMESHOBJECTCHUNKFOROUTLIERSCREATOR_H

#include "renderer/mesh/meshobjectrendererdefines.h"

class IMeshObjectChunkForOutliersCreator
{
public:
    virtual ~IMeshObjectChunkForOutliersCreator() {}

    /**
     * @brief Returns the chunk to use to add objects that was considered like outliers
     */
    virtual MeshObjectNM::OutlierChunkType& createOrGetChunkForOutliers() = 0;
};

#endif // IMESHOBJECTCHUNKFOROUTLIERSCREATOR_H
