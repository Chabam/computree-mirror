#ifndef CHUNKEDGENERICCLOUD_H
#define CHUNKEDGENERICCLOUD_H

#include "abstractchunkgenericcloudobject.h"

#include "renderer/generic/generictypedef.h"
#include "renderer/genericcloud/genericcloudrenderercontext.h"
#include "renderer/tools/texturemanager.h"

#include "renderer/genericcloud/genericcloudmode.h"
#include "defineglobalcloud.h"

class ICloudAttributesProvider;

/**
 * @brief To draw face or edge we must use another technique than to draw points from global cloud. Per
 *        example a face is an object with 3 index to vertex to draw. If two faces share one point and you
 *        want to color faces not by vertex color but by face color you can do it like this :
 *
 *        glBegin(GL_TRIANGLES)
 *        glColor3fv(&faceColor);
 *        glVertex3fv(&cloud[v1Index]);
 *        glVertex3fv(&cloud[v2Index]);
 *        glVertex3fv(&cloud[v3Index]);
 *        glEnd();
 *
 *        But this technique is very slow.
 *
 *        This class use the basic technique or a texture based technique to draw color/normal/etc... per object (face or edge).
 *
 *        Element (color, normal, etc...) from object is added to vectors and index of objects too, this two vectors is
 *        declared has texture to opengl and send to a geometric shader that use the PrimitiveId to know which element to use
 *        for the current primitive.
 */
class ChunkedGenericCloud : public AbstractChunkGenericCloudObject
{
private:
    typedef AbstractChunkGenericCloudObject SuperClass;

public:
    /**
     * @brief Create a chunk that begin at a specific point in the global point cloud and at a specific
     *        point in the cloud of colors/normals/etc... for the object type
     *
     * @param uniqueKey : unique key of this chunk
     * @param objectType : type of object draw by this chunk
     * @param glMode : opengl mode to use to draw object
     * @param nVertexPerObject : number of vertex per object
     * @param beginVertexGlobalIndex : index of the begin of the first point in the global cloud
     * @param vertexArraySize : size of the vector that contains vertex
     * @param beginElementGlobalIndex : index of the begin of the first element (color, normal, etc...) in the global cloud
     * @param elementArraySize : size of the vector that contains elements
     * @param coordinateSystemOffset : offset of the coordinate system of points that will be added to this chunk
     */
    ChunkedGenericCloud(const uint& uniqueKey,
                        Scene::ObjectType objectType,
                        GLenum glMode,
                        const quint8& nVertexPerObject,
                        const size_t& beginVertexGlobalIndex,
                        const size_t& vertexArraySize,
                        const size_t& beginElementGlobalIndex,
                        const size_t& elementArraySize,
                        const Eigen::Vector3d& coordinateSystemOffset);
    ~ChunkedGenericCloud();

    /**
     * @brief Change the beginning of the OBJECT cloud. You must call this method if a cloud before the cloud managed by
     *        this object was deleted from memory. (Show GlobalCloudManager to know of it works).
     * @param beginGlobalIndex : the new index of the first point.
     */
    inline void changeBeginningOfCloud(const size_t &beginGlobalIndex) { changeBeginningOfObjectCloud(beginGlobalIndex); }

    /**
     * @brief Returns the global index of the first object in the cloud used by this chunk
     */
    inline size_t getBeginningOfCloud() const { return getBeginningOfObjectCloud(); }

    /**
     * @brief Returns the global index of the last object that can be added to this chunk
     */
    inline size_t getEndOfCloud() const { return getEndOfObjectCloud(); }

    /**
     * @brief Change the beginning of the VERTEX cloud. You must call this method if a cloud before the cloud managed by
     *        this object was deleted from memory. (Show GlobalCloudManager to know of it works).
     * @param beginGlobalIndex : the new index of the first point.
     */
    void changeBeginningOfVertexCloud(const size_t &beginGlobalIndex);

    /**
     * @brief Change the beginning of the OBJECT cloud. You must call this method if a cloud before the cloud managed by
     *        this object was deleted from memory. (Show GlobalCloudManager to know of it works).
     * @param beginGlobalIndex : the new index of the first point.
     */
    void changeBeginningOfObjectCloud(const size_t &beginGlobalIndex);

    /**
     * @brief Returns the global index of the first object in the cloud used by this chunk
     */
    size_t getBeginningOfObjectCloud() const;

    /**
     * @brief Returns the global index of the last object that can be added to this chunk
     */
    size_t getEndOfObjectCloud() const;

    /**
     * @brief Returns the size of the object cloud
     */
    size_t getObjectCloudSize() const;

    /**
     * @brief Returns the global index of the first vertex in the cloud used by this chunk
     */
    size_t getBeginningOfVertexCloud() const;

    /**
     * @brief Returns the global index of the last vertex that can be added to this chunk
     */
    size_t getEndOfVertexCloud() const;

    /**
     * @brief Returns the size of the vertex cloud
     */
    size_t getVertexCloudSize() const;

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

private:

    typedef std::vector<GLuint> IndicesCollection;

    /**
     * @brief Index of the first vertex/color/normal in the global point cloud
     */
    size_t                  m_beginVertexGlobalIndex;

    /**
     * @brief Size of the point/color/normal cloud for vertexes
     */
    size_t                  m_vertexArraySize;

    /**
     * @brief Index of the first color/normal in the global element (face/edge) cloud
     */
    size_t                  m_beginObjectGlobalIndex;

    /**
     * @brief Size of the color/normal cloud for elements
     */
    size_t                  m_objectArraySize;

    /**
     * @brief List of indices of vertexes
     */
    IndicesCollection       m_vertexIndexes;

    /**
     * @brief List of indices of objects
     */
    IndicesCollection       m_objectIndexes;

    /**
     * @brief Add a point to this chunk. After you have added all points you must call the method "update" to enable
     *        the draw of new points.
     * @param localIndex : the local index of the point. Must be between [0; (m_vertexArraySize-1)] naturally.
     */
    void addVertex(const size_t &localIndex);

    /**
     * @brief Draw normal but use raw method of opengl (shader)
     */
    void drawRaw(DrawInfo &info);

    /**
     * @brief Draw normal but use VBO
     */
    void drawVBO(DrawInfo &info);

    /**
     * @brief Draw normal but use VAO
     */
    void drawVAO(DrawInfo &info);

    /**
     * @brief Draw normals
     */
    void drawRawNormals(DrawInfo &info);

    /**
     * @brief Draw normals but use VBO
     */
    void drawVBONormals(DrawInfo &info);

    /**
     * @brief Draw normals but use VAO
     */
    void drawVAONormals(DrawInfo &info);

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
};

#endif // CHUNKEDGENERICCLOUD_H
