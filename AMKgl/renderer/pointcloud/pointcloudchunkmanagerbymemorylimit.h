#ifndef POINTCLOUDCHUNKMANAGERBYMEMORYLIMIT_H
#define POINTCLOUDCHUNKMANAGERBYMEMORYLIMIT_H

#include "pointcloudchunkmanagerbycoordinate.h"

#include "renderer/genericcloud/genericcloudchunkmanagerbymemorylimit.h"
#include "includepoint.h"

typedef GenericCloudChunkManagerByMemoryLimitWithDefaultTraits< PointCloudChunkManagerByCoordinate, sizeof(FloatPoint) > PointCloudChunkManagerByMemoryLimitSuperClass;

class PointCloudChunkManagerByMemoryLimit : public PointCloudChunkManagerByMemoryLimitSuperClass
{
public:
    typedef ChunkedPointCloud                                                                   PointCloudChunk;

    PointCloudChunkManagerByMemoryLimit(GLenum glMode,
                                        const quint8& nVertexPerObject,
                                        const size_t& beginGlobalIndex,
                                        const size_t& cloudSize);

    /**
     * @brief Call it when you want to add multiples points with method "addPoint"
     */
    void beginAddMultiplePoints();

    /**
     * @brief Call it when you have finished to add multiples points with method "addPoint"
     */
    void endAddMultiplePoints();

    /**
     * @brief Add a point by its global index
     * @param dispatchInfos : a pointer to a list that will contains informations on where is dispatched points (nullptr if you don't want this information)
     */
    void addPoint(const size_t &globalIndex,
                  PermanentItemInformation* dispatchInfos = nullptr);

    /**
     * @brief Add a point by its global and local index and define the chunkIndex too
     * @param globalIndex : the global index of the point in the cloud
     * @param localIndex : globalIndex - getBeginningOfCloud()
     * @param chunkIndex : chunkIndex = localIndex/getChunkMaxSize();
     * @param dispatchInfos : a pointer to a list that will contains informations on where is dispatched points (nullptr if you don't want this information)
     */
    void addPoint(const size_t &globalIndex,
                  const size_t &localIndex,
                  const size_t &chunkIndex,
                  PermanentItemInformation* dispatchInfos = nullptr);

    /**
     * @brief Add all points between the begin and end index ((globalIndexBegin+count)-1) included
     * @param globalIndexBegin : index to start to add
     * @param count : last index to add
     * @param dispatchInfos : a pointer to a list that will contains informations on where is dispatched points (nullptr if you don't want this information)
     */
    void addPointsFrom(size_t globalIndexBegin,
                       size_t count,
                       PermanentItemInformation *dispatchInfos = nullptr);
private:

    /**
     * @brief True if a developper add multiple points (to accelerate the process)
     */
    bool                m_multipleAddActivated;
};

#endif // POINTCLOUDCHUNKMANAGERBYMEMORYLIMIT_H
