#ifndef CHUNKEDPOINTCLOUD_H
#define CHUNKEDPOINTCLOUD_H

#include <vector>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QMutex>

#include "definepoint.h"
#include "definecolor.h"
#include "definenormal.h"
#include "defineglobalcloud.h"

#include "renderer/chunk.h"
#include "pointrenderercontext.h"
#include "renderer/generic/generictypedef.h"

#include <Eigen/Geometry>

class DrawInfo;
class IGraphicsDocument;
class IGlobalPointsVisitor;

using namespace AMKgl;

/**
 * @brief A chunk of the global point cloud. You must create a chunk and set it a offset of only
 *        one coordinate system. You cannot have points from two different coordinate system in the
 *        same chunk !
 */
class ChunkedPointCloud : public Chunk<PointRendererContext>
{
public:
    /**
     * @brief Create a chunk that begin at a specific point in the global cloud
     * @param uniqueKey : unique key of this chunk
     * @param beginGlobalIndex : index of the begin of the first point in the global cloud
     * @param vertexArraySize : size of the tab that contains vertex
     * @param maxNumberOfPointsInFastMode : set the maximum number of points to draw in fast draw mode
     * @param coordinateSystemOffset : offset of the coordinate system of points that will be added to this chunk
     */
    ChunkedPointCloud(const uint& uniqueKey,
                      const size_t &beginGlobalIndex,
                      const size_t &vertexArraySize,
                      const Eigen::Vector3d &coordinateSystemOffset);
    ~ChunkedPointCloud();

    void init() {}

    /**
     * @brief Set the attributes accessor
     */
    void setAttributesAccessor(const IAttributesAccessor* accessor);

    /**
     * @brief Returns true if it was up to date
     */
    bool isUpdated() const;

    /**
     * @brief Update only VAO if getNormalDrawModeUsed() return DM_VAO
     */
    void updateOnlyVAO();

    /**
     * @brief Add a point to this chunk. After you have added all points you must call the method "update" to enable
     *        the draw of new points.
     * @param localIndex : the local index of the point. Must be between [0; (vertexArraySize-1)] naturally.
     */
    void addPoint(const size_t &localIndex);

    /**
     * @brief Remove one or multiple points from this chunk. After you have removed all points you must call the method "update" to update
     *        the draw of points.
     * @param from : index of the first point to remove (not the local or global index but the nTh index in the array to remove). Must be between [0; (countPoints()-1)] naturally.
     * @param count : number of points to remove.
     */
    void removePoints(const size_t& from, const size_t& count = 1);

    /**
     * @brief Remove a point from this chunk. After you have removed all points you must call the method "update" to update
     *        the draw of points.
     * @param from : index of the first point to remove (not the local or global index but the nTh index in the array to remove). Must be between [0; (countPoints()-1)] naturally.
     * @param count : number of points to remove.
     * @param visible : true to set visible, false to hide it
     */
    void setPointsVisible(const size_t& from, const size_t& count = 1, bool visible = true);

    /**
     * @brief Transformation of points. Do nothing !
     */
    void transformObjects(const Eigen::Matrix4f& trMatrix, const size_t& from, const size_t& count) { Q_UNUSED(trMatrix) Q_UNUSED(from) Q_UNUSED(count) }
    void translateObjects(const Eigen::Vector3f& translation, const size_t& from, const size_t& count) { Q_UNUSED(translation) Q_UNUSED(from) Q_UNUSED(count) }
    void rotateObjects(const Eigen::AngleAxis<float>& rotation, const size_t& from, const size_t& count) { Q_UNUSED(rotation) Q_UNUSED(from) Q_UNUSED(count)}
    void rotateObjects(const Eigen::Quaternion<float>& rotation, const size_t& from, const size_t& count) { Q_UNUSED(rotation) Q_UNUSED(from) Q_UNUSED(count) }
    void scaleObjects(const Eigen::Vector3f& scaling, const size_t& from, const size_t& count) { Q_UNUSED(scaling) Q_UNUSED(from) Q_UNUSED(count) }

    /**
     * @brief Inherited from IChunk. Same as "setPointsVisible" method
     */
    void setObjectsVisible(const size_t& from, const size_t& count = 1, bool visible = true) { setPointsVisible(from, count, visible); }

    /**
     * @brief Returns true if at least one object is visible
     */
    bool isAtLeastOneObjectVisible(const size_t &from, const size_t &count) const;

    /**
     * @brief Hide selected points (and unselect it)
     */
    void hidePointsSelected();

    /**
     * @brief Remove all points
     */
    void clearPoints();

    /**
     * @brief Inherited from IChunk. Same as "clearPoints" method
     */
    void clearObjects() { return clearPoints(); }

    /**
     * @brief Remove all objects from this chunk but conserve the size of arrays
     */
    void resetObjects();

    /**
     * @brief Returns the total number of points that was added to this object
     */
    size_t countPoints() const;

    /**
     * @brief Inherited from IChunk. Same as "countPoints" method
     */
    size_t countObjects() const { return countPoints(); }

    /**
     * @brief Returns the number of vertex/colors/infos per object
     */
    quint8 getNumberOfElementPerObject() const { return 1; }

    /**
     * @brief Call to visit points
     * @param visitor : is an object that will receive all points
     */
    void visitPoints(IGlobalPointsVisitor &visitor);

    /**
     * @brief Set the maximum number of points to draw when you call the method "fastDraw"
     * @param maxNumber : the maximum number of points
     */
    void setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber);

    /**
     * @brief Change the beginning of the cloud. You must call this method if a cloud before the cloud managed by
     *        this object was deleted from memory. (Show GlobalCloudManager to know of it works).
     * @param beginGlobalIndex : the new index of the first point.
     */
    void changeBeginningOfCloud(const size_t &beginGlobalIndex);

    /**
     * @brief Draw all points normally (all points are drawn)
     */
    void draw(DrawInfo &info);

    /**
     * @brief Draw all points fast (a reduced number of points are drawn)
     */
    void fastDraw(DrawInfo &info);

    /**
     * @brief Draw text or other elements
     */
    void postDraw(DrawInfo &info);

    /**
     * @brief Lock it (mutex)
     */
    void lock();
    /**
     * @brief Unlock it (mutex)
     */
    void unlock();

    /**
     * @brief Returns the color cloud, or nullptr if it was not created.
     */
    Basic::AbstractColorCloud* getObjectColorCloud() const { return m_objectColorCloud; }

    /**
     * @brief Returns the normal cloud, or nullptr if it was not created.
     */
    Basic::AbstractNormalCloud* getObjectNormalCloud() const { return m_objectNormalCloud; }

    /**
     * @brief Returns the normal cloud, or nullptr if it was not created.
     */
    Basic::AbstractInfoCloud* getObjectInfoCloud() const { return m_objectInfoCloud; }

    /**
     * @brief Returns the color cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    Basic::AbstractColorCloud* createOrGetObjectColorCloud();

    /**
     * @brief Returns the normal cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    Basic::AbstractNormalCloud* createOrGetObjectNormalCloud();

    /**
     * @brief Returns the normal cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    Basic::AbstractInfoCloud* createOrGetObjectInfoCloud();

    /**
     * @brief Returns the color cloud to use, or nullptr if we must no use it
     */
    GlobalColorCloud* getGlobalColorCloud() const;

    /**
     * @brief Returns the normal cloud to use, or nullptr if we must no use it
     */
    GlobalNormalCloud* getGlobalNormalCloud() const;

    /**
     * @brief Returns the normal cloud to use, or nullptr if we must no use it
     */
    GlobalInfoCloud* getGlobalInfoCloud() const;

    /**
     * @brief Returns the color cloud to use
     */
    GlobalColorCloud* createOrGetColorCloud();

    /**
     * @brief Returns the normal cloud to use
     */
    GlobalNormalCloud* createOrGetNormalCloud();

    /**
     * @brief Returns the normal cloud to use
     */
    GlobalInfoCloud* createOrGetInfoCloud();

    /**
     * @brief Returns the first vertex to draw
     */
    FloatPoint* getFirstVertex() const;

    /**
     * @brief Returns the first local index to use
     */
    GLuint* getFirstLocalIndex() const;

    /**
     * @brief Returns the first color to draw
     */
    GlobalColor* getFirstColor() const;

    /**
     * @brief Returns the first normal to draw
     */
    GlobalNormal* getFirstNormal() const;

    /**
     * @brief Returns the first byte info (selection, visibility, etc...)
     */
    ElementInfo* getFirstInfo() const;

    /**
     * @brief Returns the first byte info (selection, visibility, etc...) (create it if it doesn't exist)
     */
    ElementInfo* createOrGetFirstInfo() const;

private:

    typedef std::vector<GLuint> IndicesCollection;

    /**
     * @brief Index of the first point/color/normal in the global cloud
     */
    size_t                  m_beginGlobalIndex;

    /**
     * @brief Size of the point/color/normal cloud
     */
    size_t                  m_vertexArraySize;

    /**
     * @brief Max number of points to draw in fast draw
     */
    size_t                  m_maxNumberOfPointsInFastDrawMode;

    /**
     * @brief Increment variable used in fast draw mode to jump from a point/color/normal to the next
     */
    int                     m_fastStride;

    /**
     * @brief List of indices of points/colors/normals to draw
     */
    IndicesCollection       m_indexes;

    /**
     * @brief Really number of indices in the array (how many to draw)
     */
    size_t                  m_nPoints;

    /**
     * @brief Mutex for thread !
     */
    QMutex*                 m_mutex;

    /**
     * @brief point cloud provider for this chunk
     */
    PointCloudAttributesProvider*    m_pointCloudProvider;

    /**
     * @brief Cloud of colors but for object operation
     */
    Basic::AbstractColorCloud*      m_objectColorCloud;

    /**
     * @brief Cloud of normals but for object operation
     */
    Basic::AbstractNormalCloud*     m_objectNormalCloud;

    /**
     * @brief Cloud of informations but for object operation
     */
    Basic::AbstractInfoCloud*       m_objectInfoCloud;

    /**
     * @brief Not nullptr if developper access it.
     */
    GlobalInfoCloud*                m_globalInfoCloud;

    /**
     * @brief Update vao, vbo, etc...
     */
    bool internalUpdate();

    /**
     * @brief Draw fast but use basic method of opengl
     */
    void fastDrawBasic(DrawInfo &info);

    /**
     * @brief Draw normal but use basic method of opengl
     */
    void drawBasic(DrawInfo &info);

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
     * @brief Fast draw normals
     */
    void fastDrawBasicNormals(DrawInfo &info);

    /**
     * @brief Draw normals
     */
    void drawBasicNormals(DrawInfo &info);

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
     * @brief Call glDrawElements with buffer object or with tab
     */
    void callDrawElements(const size_t &size);

    /**
     * @brief Modify the boolean that inform if we can draw or not
     * @param status : the new status
     */
    void setUpdated(bool status);

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
    void setNumberOfObjects(const size_t& n);
    void internalSetNumberOfObjects(const size_t& n, const bool& shrinkToFit);

    /**
     * @brief Modify the update status of the context
     */
    void setContextUpdated(PointRendererContext* context, bool status);

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

#endif // CHUNKEDPOINTCLOUD_H
