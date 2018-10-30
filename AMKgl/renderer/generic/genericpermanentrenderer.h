#ifndef GENERICPERMANENTRENDERER_H
#define GENERICPERMANENTRENDERER_H

#include "renderer/generic/chunkedgeneric.h"
#include "renderer/permanentrenderer.h"
#include "genericrenderercontext.h"

#include <vector>
#include <QHash>

class ICoordinateSystemManager;

/**
 * @brief Use this class to render in opengl vertex that don't change its position. It is a renderer
 *        optimized for static vertex that draw it with the "basic" technique
 */
class GenericPermanentRenderer : public PermanentRenderer< GenericRendererContext >
{
    typedef PermanentRenderer< GenericRendererContext > SuperClass;
public:
    typedef ChunkedGeneric Chunk;

    /**
     * @brief Construct a permanent renderer to draw vertex with a version of opengl under 3.3
     */
    GenericPermanentRenderer(GLenum glMode);
    ~GenericPermanentRenderer();

    /**
     * @brief Call this method to update all vertex in all context. It will create all
     *        necessary VBO, IBO, etc... to draw it
     */
    void updateInAllContext();

    /**
     * @brief Call this method to update infos of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateInfoInAllContext();

    /**
     * @brief Call this method to update colors of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateColorInAllContext();

    /**
     * @brief Call this method to update normals of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateNormalInAllContext();

    /**
     * @brief Call this method to update only VAO of elements in all context.
     */
    void updateOnlyVAOInAllContext();

    /**
     * @brief Define the draw mode to use. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    void setDrawModeToUse(DrawMode mode);

    /**
     * @brief Call it when you want to destroy all opengl vertex (when you close your window)
     */
    void destroyGL(const QOpenGLContext *context);

    /**
     * @brief Create or get the chunk to use for an object
     * @param p : the center of the object
     * @param chunkUniqueKey : uint that will contains the unique key of the chunk founded
     * @return the chunk that can be used to add the object (use the method "addVertex")
     */
    const Chunk& createOrGetChunkForObject(const Eigen::Vector3d &p,
                                           uint* chunkUniqueKey = NULL);

    /**
     * @brief Add a vertex for an object. You must add n vertex for one object ! n == getNumberOfJumpForOneObject()
     * @param chunk : the chunk to use to add the vertex. Get it with the method "createOrGetChunkForVertex"
     * @param p : vertex to add
     * @param info : default element info
     */
    void addObjectVertex(const Chunk& chunk,
                         const Eigen::Vector3d &p,
                         const ElementInfo &info = 0,
                         const Color& color = Color(255,255,255,255));

    /**
     * @brief Remove all vertex
     */
    void clearVertex();

    /**
     * @brief Returns the total number of vertex that was added to this renderer
     */
    size_t countVertex() const;

    /**
     * @brief Remove all object (same as clearVertex())
     */
    void clearObjects();

    /**
     * @brief Call to visit all objects
     * @param visitor : is an object that will receive all objects
     */
    void visitObjects(IObjectsVisitor& visitor);

    /**
     * @brief Returns the total number of object that was added to this renderer
     */
    size_t countObjects() const;

    /**
     * @brief Draw all vertex normally (all vertex are drawn with a good precision)
     */
    void draw(DrawInfo &info);

    /**
     * @brief Draw all vertex fast (all vertex are drawn with a minimum number of vertex)
     */
    void fastDraw(DrawInfo &info);

    /**
     * @brief Draw text or other vertex
     */
    void postDraw(DrawInfo &info);

protected:
    typedef QHash<uint, Chunk*>                                             ChunkCollection;
    typedef QHashIterator<uint, Chunk*>                                     ChunkCollectionQtIterator;

    void currentColorChanged();

private:
    GLenum                          m_glMode;
    DrawMode                        m_drawMode;

    IAttributesAccessor*            m_attributesAccessor;
    ICoordinateSystemManager*       m_coordinateSystemManager;

    ChunkCollection                 m_chunks;

    uint                            m_lastCsKeyUsed;
    Chunk*                          m_lastChunkUsed;

    QMutex*                         m_mutex;
};

#endif // GENERICPERMANENTRENDERER_H
