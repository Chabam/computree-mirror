#ifndef INSTANCEDPERMANENTRENDERER_H
#define INSTANCEDPERMANENTRENDERER_H

#include "amkglapp.h"
#include "renderer/instanced/chunkedinstanced.h"
#include "renderer/permanentrenderer.h"
#include "interfaces/icoordinatesystemmanager.h"
#include "tools/objectattributesprovider.h"
#include "visitor/iobjectsvisitor.h"

#include "Eigen/Dense"

#include <vector>
#include <QHash>

/**
 * @brief Use this class to render in opengl shapes that don't change its position. It is a renderer
 *        optimized for static shapes that draw it with the "instanced" technique (only available with opengl 3.3)
 */
template<typename Chunk>
class InstancedPermanentRenderer : public PermanentRenderer< typename Chunk::RendererContextType >
{
public:
    typedef typename Chunk::RendererContextType                         InstancedShapeRendererContext;
    typedef typename InstancedShapeRendererContext::ShadersType         ShadersType;
    typedef typename ShadersType::ShaderParamsType                      ShaderParams;
    typedef typename Chunk::CustomVertexAttributeCollection             CustomVertexAttributeCollection;
    typedef typename Chunk::CustomVertexAttributeCollectionIterator     CustomVertexAttributeCollectionIterator;

    /**
     * @brief Construct a permanent renderer to draw shapes with the "instanced" technique. The opengl
     *        mode (GLEnum) used to draw the shape must be returned by the ShaderParams of the Chunk in method
     *        "staticGetGLMode()"
     */
    InstancedPermanentRenderer();

    /**
     * @brief Construct a permanent renderer to draw shapes with the "instanced" technique. The opengl
     *        mode (GLEnum) used to draw the shape was specified in this constructor.
     */
    InstancedPermanentRenderer(GLenum glMode);

    ~InstancedPermanentRenderer();

    /**
     * @brief Set the vertex array to use to draw one shape
     * @param vertexArray : list of points to draw one shape
     */
    void setVertexArray(const std::vector<Eigen::Vector3f>* vertexArray);

    /**
     * @brief Set/add a custom vertex attributes array
     * @param name : name in the shader
     * @param tupleSize : number of component (per example 3 for Eigen::Vector3f)
     * @param type : GL_FLOAT ? GL_UNSIGNED_INT ? etc...
     * @param normalized : GL_TRUE or GL_FALSE
     * @param stride : sizeof(XXX) XXX = Eigen::VectorXf or float or Gluint etc...
     * @param sizeOfOneValueInByte : sizeof(XXX)
     * @param arraySize : vector.size()
     * @param pointer : pointer to the first component of the array
     */
    void setCustomVertexAttribute(const QString& name,
                                  GLint tupleSize,
                                  GLenum type,
                                  GLboolean normalized,
                                  GLsizei stride,
                                  size_t sizeOfOneValueInByte,
                                  size_t arraySize,
                                  GLvoid* pointer);

    // TODO : make a method setCustomFastVertexAttribute !

    /**
     * @brief Set indices to use to draw a normal version of one shape (used in draw)
     * @param fastIndicesArray : the list of indices of points to use in draw method
     */
    void setNormalIndicesArray(const std::vector<GLuint>* normalIndicesArray);

    /**
     * @brief Set vertex array to use to draw a simplified version of one shape (used in fast draw)
     * @param vertexArray : list of points to draw one shape
     */
    void setFastVertexArray(const std::vector<Eigen::Vector3f>* vertexArray);

    /**
     * @brief Set indices to use to draw a simplified version of one shape (used in fast draw)
     * @param fastIndicesArray : the list of indices of points to use in fast draw method
     */
    void setFastIndicesArray(const std::vector<GLuint>* fastIndicesArray);

    /**
     * @brief Call this method to update all shapes in all context. It will create all
     *        necessary VBO, IBO, etc... to draw it
     */
    void updateInAllContext();

    /**
     * @brief Call this method to update infos of shapes in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateInfoInAllContext();

    /**
     * @brief Call this method to update colors of shapes in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    void updateColorInAllContext();

    /**
     * @brief Call this method to update normals of shapes in all context. It will update
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
     * @brief Call it when you want to destroy all opengl shapes (when you close your window)
     */
    void destroyGL(const QOpenGLContext *context);

    /**
     * @brief Create or get the chunk to use for an shape that was in a definded bounding box
     * @param bbox : the bouding box of the shape
     * @param chunk : pointer that will contains the chunk founded
     * @return the chunk that can be used to add the shape with the bouding box passed in parameter
     */
    const Chunk& createOrGetChunkForShape(const Eigen::AlignedBox3d &bbox,
                                          uint* chunkUniqueKey = NULL);

    /**
     * @brief Add an shape
     * @param chunk : the chunk to use to add the shape. Get it with the method "createOrGetChunkForShape"
     * @param params : parameters to send to shader to draw correctly the shape
     * @param info : default shape info
     */
    void addShape(const Chunk& chunk, const ShaderParams &params, const ElementInfo &info);

    /**
     * @brief Remove shapes from this renderer
     * @param chunkUniqueKey : unique key of the chunk
     * @param index : index of the first shape to remove
     * @param count : number of shape to remove after the first (first included)
     */
    void removeShapes(const uint& chunkUniqueKey, const size_t& index, const size_t &count = 1);

    /**
     * @brief Optimized function to remove in one pass multiple shape in multiple range. Per example you can have
     *        a vector composed of object that contains the begin and the end of shape to remove :
     *
     *        struct ST {
     *          ST(size_t b, size_t e) : mBegin(b), mEnd(e) {}
     *
     *          inline size_t begin() { return mBegin; }
     *          inline size_t end() { return mEnd; }
     *          inline size_t count() { return (mEnd-mBegin)+1; }
     *
     *          size_t mBegin;
     *          size_t mEnd;
     *        }
     *
     *        std::vector<ST> vector;
     *        vector.push_back(ST(2,4));
     *        vector.push_back(ST(10,18));
     *        vector.push_back(ST(20,40));
     *
     *        removeShapes(chunkUniqueKey, vector.begin(), vector.end());
     *
     * @param chunkUniqueKey : unique key of the chunk
     * @param begin : begin iterator
     * @param end : end iterator
     */
    template<typename iterator>
    void removeShapes(const uint& chunkUniqueKey, const iterator& begin, const iterator& end);

    /**
     * @brief Show/Hide shapes from this renderer
     * @param chunkUniqueKey : unique key of the chunk
     * @param index : index of the first shape to show/hide
     * @param count : number of shape to hide after the first (first included)
     * @param visible : true to set visible, false to hide it
     */
    void setShapesVisible(const uint& chunkUniqueKey, const size_t& index, const size_t &count = 1, bool visible = true);

    /**
     * @brief Remove all shapes
     */
    void clearShapes();

    /**
     * @brief Returns the total number of shape that was added to this renderer
     */
    size_t countShapes() const;

    /**
     * @brief Returns the total number of shape that was added to this renderer in a specific chunk
     */
    size_t countShapesInChunk(const uint& chunkUniqueKey) const;

    /**
     * @brief Inherited from AbstractPermanentRenderer. Same as "removeShapes"
     */
    void removeObjects(const uint& chunkUniqueKey, const size_t& index, const size_t &count = 1) { removeShapes(chunkUniqueKey, index, count); }

    /**
     * @brief Inherited from AbstractPermanentRenderer. Same as "setShapesVisible"
     */
    void setObjectsVisible(const uint& chunkUniqueKey, const size_t& index, const size_t &count = 1, bool visible = true) { setShapesVisible(chunkUniqueKey, index, count, visible); }

    /**
     * @brief Inherited from AbstractPermanentRenderer. Same as "clearShapes"
     */
    void clearObjects() { clearShapes(); }

    /**
     * @brief Inherited from AbstractPermanentRenderer. Same as "countShapes"
     */
    size_t countObjects() const { return countShapes(); }

    /**
     * @brief Inherited from IObjectsRendererAttributesAccessor
     */
    int getNumberOfJumpForOneObject() const { return 1; }

    /**
     * @brief Call to visit all objects
     * @param visitor : is an object that will receive all objects
     */
    void visitObjects(IObjectsVisitor& visitor);

    /**
     * @brief Draw all shapes normally (all shapes are drawn with a good precision)
     */
    void draw(DrawInfo &info);

    /**
     * @brief Draw all shapes fast (all shapes are drawn with a minimum number of points)
     */
    void fastDraw(DrawInfo &info);

    /**
     * @brief Draw text or other shapes
     */
    void postDraw(DrawInfo &info);

protected:
    typedef QHash<uint, Chunk*>                                             ChunkCollection;
    typedef QHashIterator<uint, Chunk*>                                     ChunkCollectionQtIterator;

    void currentColorChanged();

private:
    ChunkCollection                 m_chunks;

    GLenum                          m_glMode;
    DrawMode                        m_drawMode;

    std::vector<Eigen::Vector3f>*   m_vertexArray;
    CustomVertexAttributeCollection m_customVertexAttributeCollection;
    std::vector<Eigen::Vector3f>*   m_fastVertexArray;
    std::vector<GLuint>*            m_normalIndicesArray;
    std::vector<GLuint>*            m_fastIndicesArray;

    uint                            m_lastCsKeyUsed;
    Eigen::Vector3d                 m_lastOffsetUsed;

    QMutex*                         m_mutex;

protected:

    /**
     * @brief Returns the chunk collection
     */
    ChunkCollection& getChunks();
};

#include "instancedpermanentrenderer.hpp"

#endif // INSTANCEDPERMANENTRENDERER_H
