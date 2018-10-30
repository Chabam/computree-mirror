#ifndef POINTCLOUDARRAYOBJECTMANAGER_H
#define POINTCLOUDARRAYOBJECTMANAGER_H

#include <QtOpenGL>
#include <QOpenGLVertexArrayObject>

class PointRendererContext;
class IChunk;

#define POINTCLOUD_VERTEX_BO_UI    0
#define POINTCLOUD_INDEX_BO_UI     1
#define POINTCLOUD_INFO_BO_UI      2
#define POINTCLOUD_COLOR_BO_UI     3
#define POINTCLOUD_NORMAL_BO_UI    4

/**
 * @brief This class must be a singleton per opengl context. It will manage all array object created
 *        to keep the minimum memory consumption in the graphics card.
 */
class PointCloudArrayObjectManager
{
public:
    PointCloudArrayObjectManager();
    ~PointCloudArrayObjectManager();

    /**
     * @brief Create the AO that will draw vertex
     * @param chunk : chunk that has created buffer object
     * @param currentContext : current context
     */
    bool createVertexAO(IChunk* chunk, PointRendererContext& currentContext);

    /**
     * @brief Update the AO that will draw vertex
     * @param chunk : chunk that has created buffer object
     * @param currentContext : current context
     */
    bool updateVertexAO(IChunk* chunk, PointRendererContext& currentContext);

    /**
     * @brief Returns true if you can use AO to draw objects
     */
    bool canUseVertexAO() const;

    /**
     * @brief Bind and returns true if the vertex AO was bind successfully
     */
    bool bindVertexAO();

    /**
     * @brief Release vertex AO
     */
    void releaseVertexAO();

    /**
     * @brief Create the AO that will draw normals
     * @param chunk : chunk that has created buffer object
     * @param currentContext : current context
     */
    bool createNormalsAO(IChunk* chunk, PointRendererContext& currentContext);

    /**
     * @brief Update the AO that will draw normals
     * @param chunk : chunk that has created buffer object
     * @param currentContext : current context
     */
    bool updateNormalsAO(IChunk* chunk, PointRendererContext& currentContext);

    /**
     * @brief Returns true if you can use AO to draw normals
     */
    bool canUseNormalsAO() const;

    /**
     * @brief Bind and returns true if the normals AO was bind successfully
     */
    bool bindNormalsAO();

    /**
     * @brief Release normals AO
     */
    void releaseNormalsAO();

    /**
     * @brief Destroy all
     */
    void destroyGL();

private:
    QOpenGLVertexArrayObject    m_vertexAO;
    QOpenGLVertexArrayObject    m_normalsAO;
};

#endif // POINTCLOUDARRAYOBJECTMANAGER_H
