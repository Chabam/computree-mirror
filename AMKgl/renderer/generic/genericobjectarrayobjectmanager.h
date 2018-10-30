#ifndef GENERICOBJECTARRAYOBJECTMANAGER_H
#define GENERICOBJECTARRAYOBJECTMANAGER_H

#include <QtOpenGL>
#include <QOpenGLVertexArrayObject>

class GenericRendererContext;
class IChunk;

#define GENERIC_OBJECT_VERTEX_BO_UI    0
#define GENERIC_OBJECT_INFOS_BO_UI     1
#define GENERIC_OBJECT_COLORS_BO_UI    2
#define GENERIC_OBJECT_NORMALS_BO_UI   3

/**
 * @brief This class must be a singleton per opengl context. It will manage all array object created
 *        to keep the minimum memory consumption in the graphics card.
 */
class GenericObjectArrayObjectManager
{
public:
    GenericObjectArrayObjectManager();
    ~GenericObjectArrayObjectManager();

    /**
     * @brief Create the AO that will draw vertex
     * @param chunk : chunk that has created buffer object
     * @param currentContext : current context
     */
    bool createVertexAO(IChunk* chunk, GenericRendererContext& currentContext);

    /**
     * @brief Update the AO that will draw vertex
     * @param chunk : chunk that has created buffer object
     * @param currentContext : current context
     */
    bool updateVertexAO(IChunk* chunk, GenericRendererContext& currentContext);

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
     * @brief Destroy all
     */
    void destroyGL();

private:
    QOpenGLVertexArrayObject    m_vertexAO;
};

#endif // GENERICOBJECTARRAYOBJECTMANAGER_H
