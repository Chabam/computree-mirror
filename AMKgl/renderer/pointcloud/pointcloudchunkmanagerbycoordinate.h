#ifndef POINTCLOUDCHUNKMANAGERBYCOORDINATE_H
#define POINTCLOUDCHUNKMANAGERBYCOORDINATE_H

#include "renderer/genericcloud/genericcloudchunkmanagerbycoordinate.h"
#include "scene/tools/permanentiteminformation.h"
#include "chunkedpointcloud.h"

typedef GenericCloudChunkManagerByCoordinate<ChunkedPointCloud, GenericChunkManagerDefaultSuperClass<ChunkedPointCloud::RendererContextType> > PointCloudChunkManagerByCoordinateSuperClass;

class PointCloudChunkManagerByCoordinate : public PointCloudChunkManagerByCoordinateSuperClass
{
public:
    typedef ChunkedPointCloud   PointCloudChunk;

    PointCloudChunkManagerByCoordinate(GLenum glMode,
                                       const quint8& nVertexPerObject,
                                       const size_t& beginGlobalIndex,
                                       const size_t& cloudSize);

    /**
     * @brief Call it when you want to add multiples points
     */
    void beginAddMultiplePoints();

    /**
     * @brief Call it when you have finished to add multiples points
     */
    void endAddMultiplePoints();

    /**
     * @brief Add point by index. (too be faster use method "addPoint" with local and global index if you have already computed the local index)
     * @param globalIndex : global index that must be between ==> [beginGlobalIndex; (beginGlobalIndex+size-1)]
     */
    void addPoint(const size_t &globalIndex, PermanentItemInformation* dispatchInfos = nullptr);

    /**
     * @brief Add point by index (faster than previous method if you have already computed the local index)
     * @param localIndex : local index that must be between ==> [0; (size-1)] (just do the operation : (globalIndex - beginGlobalIndex))
     * @param globalIndex : global index that must be between ==> [beginGlobalIndex; (beginGlobalIndex+size-1)]
     */
    void addPoint(const size_t &localIndex, const size_t &globalIndex, PermanentItemInformation* dispatchInfos = nullptr);

protected:
    /**
     * @brief Redefined to create the PointCloudChunk correctly
     */
    PointCloudChunk* createNewChunk(const size_t& globalBeginIndex,
                                    const size_t& size);

    /**
     * @brief Init the chunk (you can redefine this method if you want to init it differently)
     */
    void initChunk(PointCloudChunk* chunk);

    /**
     * @brief Redefined here because this class was the last manager in stage that don't modify the beginning of the cloud
     */
    void mustUpdateInfoBO(RendererContext* currentContext);

    /**
     * @brief Redefined here because this class was the last manager in stage that don't modify the beginning of the cloud
     */
    void mustUpdateColorBO(RendererContext* currentContext);

    /**
     * @brief Redefined here because this class was the last manager in stage that don't modify the beginning of the cloud
     */
    void mustUpdateNormalBO(RendererContext* currentContext);

    /**
     * @brief Redefined to get the point cloud attributes provider
     */
    void attributesAccessorChanged(const AttributesAccessor *aa);

private:

    /**
     * @brief Last dispatch info used
     */
    DispatchInformation*                    m_lastDispatchInfos;

    /**
     * @brief Used to know if we must use "m_lastDispatchInfos" variable or not (m_multiplePointsActivated = false)
     */
    bool                                    m_multiplePointsActivated;

    /**
     * @brief point cloud provider for this chunk
     */
    PointCloudAttributesProvider*           m_pointCloudProvider;
};

#endif // POINTCLOUDCHUNKMANAGERBYCOORDINATE_H
