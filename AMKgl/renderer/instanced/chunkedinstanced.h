#ifndef CHUNKEDINSTANCED_H
#define CHUNKEDINSTANCED_H

#include <QtOpenGL/QtOpenGL>
#include <QOpenGLFunctions_3_3_Compatibility>
#include <vector>

#include "renderer/attributes/vertexattribute.h"
#include "renderer/chunk.h"
#include "renderer/generic/generictypedef.h"
#include "renderer/tools/bufferobjectmanager.h"

#include "instancedarrayobjectmanager.h"

#include "drawinfo.h"
#include "picker/tools/objectsflagstool.h"

/**
 * @brief A chunk that must be used to draw vertex in "instanced" mode (opengl 3.3 function). It take a vertex
 *        array and draw it multiple times. The number of times the vertices are drawn corresponds to the
 *        number of shapes (ShaderParamsType) added to that chunk.
 *
 *        Typically you will use this class to draw ellipse, rectangle, box, etc.... Many common shapes !
 *
 *        For each shape you want to draw you will add a ShaderParamsType that will transform the basic shape (the vertex array)
 *
 *        The class/struct ShaderParamsType must have predefined methods to be used with this class :
 *
 *        + static inline GLenum staticGetGLMode() { return GL_XXXX; } // returns the glEnum mode used to draw vertex array
 *
 *        + inline void getMatrix4f(Eigen::Matrix4f &matrix4f) { ... } // convert transformation to a 4x4 matrix before draw the shape in basic mode
 *
 *        + inline const type& operator()(const int &index) const {... } // return a constante reference to the first float/int/uint/etc... at row 'index'
 *
 */
template<typename RendererContextT>
class ChunkedInstanced : public Chunk<RendererContextT>
{
public:
    typedef RendererContextT                            RendererContextType;
    typedef typename RendererContextType::ShadersType   ShadersType;
    typedef typename ShadersType::ShaderParamsType      ShaderParamsType;
    typedef Basic::LocalVertex                          Vertex;
    typedef Basic::LocalColor                           Color;
    typedef Basic::LocalInfo                            Info;
    typedef Basic::ColorCloud                           ColorCloud;
    typedef Basic::NormalCloud                          NormalCloud;
    typedef Basic::InfoCloud                            InfoCloud;

    typedef QHash<QString, VertexAttribute>             CustomVertexAttributeCollection;
    typedef QHashIterator<QString, VertexAttribute>     CustomVertexAttributeCollectionIterator;

    typedef void (*DrawShapeFunc)(void* context, ChunkedInstanced<RendererContextT> *obj, DrawInfo &info);

    /**
     * @brief Create a chunk
     * @param uniqueKey : unique key of this chunk
     * @param objectType : type of object draw by this chunk
     * @param glMode : the opengl mode to use to draw vertex
     * @param vertexArray : the array that contains vertex to draw
     * @param offset : offset of shapes that will be added to this chunk
     * @param normalIndices : array that contains indices of points to draw in normal mode, if NULL all points will be draw in normal mode
     * @param fastIndices : array that contains indices of points to draw in fast mode, if NULL all points will be draw in fast mode
     * @param funcContext : a context that will be passed to function for draw shapes
     * @param basicDraw, rawDraw, ... : functions that will be called to draw shapes in normal mode (can be NULL if you want that predefined function was called)
     * @param basicFastDraw, rawFastDraw, ... : functions that will be called to fast draw shapes in fast mode  (can be NULL if you want that predefined function was called)
     */
    ChunkedInstanced(const uint& uniqueKey,
                     Scene::ObjectType objectType,
                     GLenum glMode,
                     const std::vector<Eigen::Vector3f>& vertexArray,
                     const Eigen::Vector3d& offset,
                     const CustomVertexAttributeCollection* customVertexAttributesCollection = NULL,
                     const std::vector<Eigen::Vector3f>* fastVertexArray = NULL,
                     const std::vector<GLuint> *normalIndices = NULL,
                     const std::vector<GLuint> *fastIndices = NULL,
                     void* funcContext = NULL,
                     DrawShapeFunc basicDraw = NULL,
                     DrawShapeFunc rawDraw = NULL,
                     DrawShapeFunc VBODraw = NULL,
                     DrawShapeFunc VAODraw = NULL,
                     DrawShapeFunc basicFastDraw = NULL,
                     DrawShapeFunc rawFastDraw = NULL,
                     DrawShapeFunc VBOFastDraw = NULL,
                     DrawShapeFunc VAOFastDraw = NULL);
    ~ChunkedInstanced();

    /**
     * @brief Return the opengl mode to use to draw one shape
     */
    GLenum getGlMode() const;

    /**
     * @brief Update only VAO if getNormalDrawModeUsed() return DM_VAO
     */
    void updateOnlyVAO();

    /**
     * @brief Update color buffer object if getNormalDrawModeUsed() return DM_VAO or DM_VBO
     */
    void updateColorBO();

    /**
     * @brief Update info buffer object if getNormalDrawModeUsed() return DM_VAO or DM_VBO
     */
    void updateInfoBO();

    /**
     * @brief Add a shape. The shape is defined by a transform matrix (scale/position/rotation)
     * @param params : parameters to send to shader to draw correctly the shape
     * @param info : default shape info
     */
    void addShape(const ShaderParamsType &params, const Info &info);

    /**
     * @brief Remove shape from this chunk
     * @param from : index of the first shape to remove. Must be between [0; (countShape()-1)] naturally.
     * @param count : number of shape to remove after the first (first included)
     */
    void removeShapes(const size_t& from, const size_t& count = 1);

    /**
     * @brief Same as "removeObjects" method from IChunk
     */
    template<typename iterator>
    void removeShapes(const iterator& begin, const iterator& end) { removeObjects(begin, end); }

    /**
     * @brief Show/Hide shape from this chunk
     * @param from : index of the first shape to remove. Must be between [0; (countShape()-1)] naturally.
     * @param count : number of shape to show/hide after the first (first included)
     * @param visible : true to set visible, false to hide it
     */
    void setShapesVisible(const size_t& from, const size_t& count = 1, bool visible = true);

    /**
     * @brief Transformation of objects
     */
    void transformObjects(const Eigen::Matrix4f& trMatrix, const size_t& from, const size_t& count);
    void translateObjects(const Eigen::Vector3f& translation, const size_t& from, const size_t& count);
    void rotateObjects(const Eigen::AngleAxis<float>& rotation, const size_t& from, const size_t& count);
    void rotateObjects(const Eigen::Quaternion<float>& rotation, const size_t& from, const size_t& count);
    void scaleObjects(const Eigen::Vector3f& scaling, const size_t& from, const size_t& count);

    /**
     * @brief Inherited from IChunk. Same as "setShapesVisible" method
     */
    void setObjectsVisible(const size_t& from, const size_t& count = 1, bool visible = true) { setShapesVisible(from, count, visible); }

    /**
     * @brief Returns true if at least one object is visible
     */
    bool isAtLeastOneObjectVisible(const size_t &from, const size_t &count) const;

    /**
     * @brief Remove all shape
     */
    void clearShape();

    /**
     * @brief Inherited from IChunk. Same as "clearShape" method
     */
    void clearObjects() { return clearShape(); }

    /**
     * @brief Remove all objects from this chunk but conserve the size of arrays
     */
    void resetObjects();

    /**
     * @brief Returns the total number of shape that was added to this chunk
     */
    size_t countShape() const;

    /**
     * @brief Inherited from IChunk. Same as "countShape" method
     */
    size_t countObjects() const { return countShape(); }

    /**
     * @brief Returns the number of vertex/colors/infos per object
     */
    quint8 getNumberOfElementPerObject() const { return 1; }

    /**
     * @brief Set the array that contains indices of points to draw in fast mode
     * @param fastIndices : array that contains indices of points to draw in fast mode, if NULL all points will be draw in fast mode
     */
    void setArrayOfIndicesForFastDraw(const std::vector<GLuint> *fastIndices);

    /**
     * @brief Returns the first vertex
     */
    Vertex* getFirstVertex() const;

    /**
     * @brief Returns the first vertex
     */
    Vertex* getFirstVertexForFast() const;

    /**
     * @brief Returns the first index
     */
    GLuint* getFirstIndex() const;

    /**
     * @brief Returns the first index
     */
    GLuint* getFirstIndexForFast() const;

    /**
     * @brief Returns the first matrix
     */
    ShaderParamsType* getFirstParam() const;

    /**
     * @brief Returns the first color
     */
    Color* getFirstColor() const;

    /**
     * @brief Returns the first info
     */
    Info* getFirstInfo() const;

    /**
     * @brief Returns the color cloud
     */
    ColorCloud* getColorCloud() const;

    /**
     * @brief Returns the normals cloud
     */
    NormalCloud* getNormalCloud() const;

    /**
     * @brief Returns the info cloud
     */
    InfoCloud* getInfoCloud() const;

    /**
     * @brief Draw all points normally (all points are drawn)
     */
    void draw(DrawInfo &info);

    /**
     * @brief Draw all points fast (a reduced number of points are drawn)
     */
    void fastDraw(DrawInfo &info);

    /**
     * @brief Draw text or other shapes
     */
    void postDraw(DrawInfo &info);

    /**
     * @brief Lock it (mutex)
     */
    void lock() {}

    /**
     * @brief Unlock it (mutex)
     */
    void unlock() {}

    /**
     * @brief Returns the color cloud, or NULL if it was not created.
     */
    Basic::AbstractColorCloud* getObjectColorCloud() const { return getColorCloud(); }

    /**
     * @brief Returns the normal cloud, or NULL if it was not created.
     */
    Basic::AbstractNormalCloud* getObjectNormalCloud() const { return getNormalCloud(); }

    /**
     * @brief Returns the normal cloud, or NULL if it was not created.
     */
    Basic::AbstractInfoCloud* getObjectInfoCloud() const { return getInfoCloud(); }

    /**
     * @brief Returns the color cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns NULL !
     */
    Basic::AbstractColorCloud* createOrGetObjectColorCloud() { return getColorCloud(); }

    /**
     * @brief Returns the normal cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns NULL !
     */
    Basic::AbstractNormalCloud* createOrGetObjectNormalCloud() { return getNormalCloud(); }

    /**
     * @brief Returns the normal cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns NULL !
     */
    Basic::AbstractInfoCloud* createOrGetObjectInfoCloud() { return getInfoCloud(); }

private:
    void*                               m_funcContext;

    GLenum                              m_glMode;

    std::vector<ShaderParamsType>       m_shapeArray;
    std::vector<Info>                   m_infoArray;
    CustomVertexAttributeCollection*    m_customVertexAttributesCollection;
    InfoCloud*                          m_infosAccess;
    std::vector<Color>                  m_colorArray;
    ColorCloud*                         m_colorsAccess;
    size_t                              m_nShapes;

    std::vector<Vertex>*                m_vertexArray;
    std::vector<Vertex>*                m_fastVertexArray;
    std::vector<GLuint>*                m_normalIndicesArray;
    std::vector<GLuint>*                m_fastIndicesArray;

    // functions
    DrawShapeFunc                       m_basicDrawFunc;
    DrawShapeFunc                       m_rawDrawFunc;
    DrawShapeFunc                       m_VBODrawFunc;
    DrawShapeFunc                       m_VAODrawFunc;

    DrawShapeFunc                       m_basicFastDrawFunc;
    DrawShapeFunc                       m_rawFastDrawFunc;
    DrawShapeFunc                       m_VBOFastDrawFunc;
    DrawShapeFunc                       m_VAOFastDrawFunc;

    static quint8 SLOW_DISPLAY_LIST;
    static quint8 FAST_DISPLAY_LIST;

    /**
     * @brief Default method to draw shapes in basic mode
     */
    void basicDraw(DrawInfo& info);

    /**
     * @brief Method to draw shapes in display list mode
     */
    void displayListDraw(DrawInfo& info);

    /**
     * @brief Default method to draw shapes in raw mode
     */
    void rawDraw(DrawInfo &info);

    /**
     * @brief Default method to draw shapes in vbo mode
     */
    void vboDraw(DrawInfo &info);

    /**
     * @brief Default method to draw shapes in vao mode
     */
    void vaoDraw(DrawInfo& info);

    /**
     * @brief Default method to fast draw shapes in basic mode
     */
    void basicFastDraw(DrawInfo& info);

    /**
     * @brief Method to fast draw shapes in display list mode
     */
    void displayListFastDraw(DrawInfo& info);

    /**
     * @brief Default method to fast draw shapes in raw mode
     */
    void rawFastDraw(DrawInfo& info);

    /**
     * @brief Default method to fast draw shapes in vbo mode
     */
    void vboFastDraw(DrawInfo &info);

    /**
     * @brief Default method to fast draw shapes in vao mode
     */
    void vaoFastDraw(DrawInfo& info);

    /**
     * @brief Default method to draw shapes in basic mode fast or not
     */
    void basicFastOrNormalDraw(bool fast);

    /**
     * @brief Default method to draw shapes in raw mode fast or not
     */
    void rawFastOrNormalDraw(bool fast);

    /**
     * @brief Default method to fast draw shapes in vbo mode fast or not
     */
    void vboFastOrNormalDraw(bool fast, DrawInfo &info);

    /**
     * @brief Default method to fast draw shapes in vao mode fast or not
     */
    void vaoFastOrNormalDraw(bool fast, DrawInfo& info);

    /**
     * @brief Method that call the good opengl function to draw shapes
     */
    void callDrawElements(const size_t &nShapeToDraw, QOpenGLFunctions_3_3_Compatibility *func);

    /**
     * @brief Method that call the good opengl function to draw shapes
     */
    void callFastDrawElements(const size_t &nShapeToDraw, QOpenGLFunctions_3_3_Compatibility *func);

protected:

    /**
     * @brief Called when a shape must be draw in basic mode
     */
    virtual void basicDrawShape(const ShaderParamsType& param,
                                const std::vector<Vertex>& vertexArray,
                                std::vector<GLuint>* indices,
                                const size_t& nVertex,
                                bool fast);

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
    void setNumberOfObjects(const size_t& n);
};

#include "chunkedinstanced.hpp"

#endif // CHUNKEDINSTANCED_H


