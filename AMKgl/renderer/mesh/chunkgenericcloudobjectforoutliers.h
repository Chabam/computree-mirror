#ifndef CHUNKGENERICCLOUDOBJECTFOROUTLIERS_H
#define CHUNKGENERICCLOUDOBJECTFOROUTLIERS_H

#include "abstractchunkgenericcloudobject.h"

#include "renderer/generic/generictypedef.h"
#include "renderer/genericcloud/genericcloudrenderercontext.h"
#include "tools/icloudattributesprovider.h"
#include "interfaces/igenericcloudmanager.h"
#include "interfaces/iglobalpointcloudmanager.h"
#include "interfaces/icoordinatesystemmanager.h"

/**
 * @brief To draw face or edge that vertex was not in the same cloud or same memory limit or same coordinate system,
 *        we must draw it only in basic mode. This chunk is for face or edge considered like "outliers".
 */
class ChunkGenericCloudObjectForOutliers : public AbstractChunkGenericCloudObject
{
private:
    typedef AbstractChunkGenericCloudObject SuperClass;

public:
    typedef GenericCloudRendererContext     Context;

    struct VertexesByOffset {
        Eigen::Vector3d         offset;
        std::vector<FloatPoint> vertexes;
    };

    typedef std::vector<VertexesByOffset> VertexesAndOffsetCollection;

    ChunkGenericCloudObjectForOutliers(Scene::ObjectType objectType,
                                       GLenum glMode,
                                       const quint8& nVertexPerObject,
                                       const IGenericCloudManager& vertexCloudManager,
                                       const IGenericCloudManager& objectCloudManager);
    ~ChunkGenericCloudObjectForOutliers();

    /**
     * @brief Returns the global index of the first vertex in the cloud used by this chunk
     */
    size_t getBeginningOfVertexCloud() const;

    /**
     * @brief Returns the global index of the first object in the cloud used by this chunk
     */
    size_t getBeginningOfObjectCloud() const;

    /**
     * @brief Add an object.
     * @param globalObjectIndex : the global index of the object. Per example for a face it will be the global face index
     * @param vertexGlobalIndexes : array of indexes of vertex of the object. This array must have a size of getNumberOfElementPerObject() !
     * @param chunk : will point to this chunk if it was not nullptr
     */
    void addObjectVertexes(const size_t& globalObjectIndex,
                           const size_t* vertexGlobalIndexes,
                           IChunk** chunk = nullptr);

    /**
     * @brief Returns the first object index in the array
     */
    GLuint* getFirstObjectIndex() const;

    /**
     * @brief Returns the first vertex index in the array
     */
    GLuint* getFirstVertexIndex() const;

    /**
     * @brief Return all vertexes with offset
     */
    const VertexesAndOffsetCollection& getVertexesWithOffset() const;

private:
    /**
     * @brief List of indices of vertexes
     */
    GlobalGlIndexCloudRegistered        m_vertexIndexes;
    GlobalGlIndexCloud*                 m_vertexIndexesCloud;

    /**
     * @brief List of indices of objects
     */
    GlobalGlIndexCloudRegistered        m_objectIndexes;
    GlobalGlIndexCloud*                 m_objectIndexesCloud;

    std::vector<AMKgl::DoublePoint>     m_pointsTmp;

    VertexesAndOffsetCollection         m_vertexesAndOffset;

    /**
     * @brief Keep a pointer to the global point cloud manager just to be faster
     */
    IGlobalPointCloudManager*           m_globalPointCloudManager;

    ICoordinateSystemManager*           m_csm;

    /**
     * @brief Compute the offset to use for the object represented by vertex and add it to the collection and add vertex too
     */
    void computeAndAddOffsetAndVertex(const size_t* vertexGlobalIndexes);

    /**
     * @brief Add a point to this chunk. After you have added all points you must call the method "update" to enable
     *        the draw of new points.
     * @param globalIndex : the global index of the point.
     */
    void addVertex(const size_t &globalIndex);

protected:

    /**
     * @brief Draw normal but use basic method of opengl
     */
    void drawBasic(DrawInfo &info);

    /**
     * @brief Draw normals
     */
    void drawBasicNormals(DrawInfo &info);

    /**
     * @brief Update vao, vbo, etc...
     */
    bool internalUpdate();

    /**
     * @brief Move values (vertex, info, color, etc...) from specified index to specified index
     * @param from : local index of the source
     * @param to : local index of the destination
     * @param size: number of values to move
     */
    void moveObjectsFromTo(const size_t& from, const size_t& to, const size_t& size);

    /**
     * @brief Set the number of objects in all array
     */
    void internalSetNumberOfObjects(const size_t& n, const bool& shrinkToFit);

    /**
     * @brief If the member m_objectColorCloud is nullptr it will be created
     */
    void createObjectColorCloudMemberIfNot();

    /**
     * @brief If the member m_objectNormalCloud is nullptr it will be created
     */
    void createObjectNormalCloudMemberIfNot();

    /**
     * @brief If the member m_objectInfoCloud is nullptr it will be created
     */
    void createObjectInfoCloudMemberIfNot();

    /**
     * @brief Delete from memory all m_objectXXXCloud
     */
    void deleteAllObjectXXXCloud();

    VertexesByOffset* createOrGetVertexesForMiddle(const Eigen::Vector3d& middle);

    void addVertexesTo(const std::vector<DoublePoint>& points, VertexesByOffset* vertexes);
};

#endif // CHUNKGENERICCLOUDOBJECTFOROUTLIERS_H
