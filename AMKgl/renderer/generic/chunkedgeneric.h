#ifndef CHUNKEDGENERIC_H
#define CHUNKEDGENERIC_H

#include "renderer/chunk.h"
#include "genericrenderercontext.h"
#include "generictypedef.h"
#include "includecolor.h"
#include "includenormal.h"

/**
 * @brief A chunk of the vertex array. You must create a chunk and set it a offset of only
 *        one coordinate system. You cannot have points from two different coordinate system in the
 *        same chunk !
 */
class ChunkedGeneric : public Chunk<GenericRendererContext>
{
public:

    typedef Basic::LocalVertex           Vertex;
    typedef Basic::LocalColor            Color;
    typedef Basic::LocalInfo             Info;
    typedef Basic::LocalNormal           Normal;
    typedef Basic::VertexArray      VertexArray;
    typedef Basic::ColorArray       ColorArray;
    typedef Basic::InfoArray        InfoArray;
    typedef Basic::NormalArray      NormalArray;
    typedef Basic::ColorCloud       ColorCloud;
    typedef Basic::InfoCloud        InfoCloud;
    typedef Basic::NormalCloud      NormalCloud;

    /**
     * @brief Create a chunk that begin at a specific point in the vertex array and draw points in a specific opengl mode
     * @param uniqueKey : unique key of this chunk
     * @param objectType : type of object draw by this chunk
     * @param glMode : opengl mode to use to draw object
     * @param nElementPerObject : number of vertex/colors/infos per object
     * @param offset : offset of this chunk
     */
    ChunkedGeneric(const uint& uniqueKey,
                   Scene::ObjectType objectType,
                   GLenum glMode,
                   const quint8& nElementPerObject,
                   const Eigen::Vector3d &offset);

    ~ChunkedGeneric();

    /**
     * @brief Returns true if it was up to date
     */
    bool isUpdated() const;

    /**
     * @brief Update only VAO
     */
    void updateOnlyVAO() override;

    /**
     * @brief Update info buffer object if getNormalDrawModeUsed() return DM_VAO or DM_VBO
     */
    void updateInfoBO();

    /**
     * @brief Update color buffer object if getNormalDrawModeUsed() return DM_VAO or DM_VBO
     */
    void updateColorBO();

    /**
     * @brief Update normal buffer object if getNormalDrawModeUsed() return DM_VAO or DM_VBO
     */
    void updateNormalBO();

    /**
     * @brief Returns the number of vertex/colors/infos per object
     */
    quint8 getNumberOfElementPerObject() const;

    /**
     * @brief Add a point to this chunk.
     */
    void addPoint(const Eigen::Vector3f &point,
                  const ElementInfo &info = 0,
                  const Color& color = Color(255,255,255,255),
                  const Normal &normal = Normal());

    /**
     * @brief Show/Hide a point. After you have modified all points you must call the method "update" to update
     *        the draw of points.
     * @param from : index of the first point to show/hide (not the global index or the local index but the index of
     *               the nth point in the list of indexes). Must be between [0; (countPoints()-1)] naturally.
     * @param count : number of points to modify.
     * @param visible : true to set visible, false to hide it
     */
    void setPointsVisible(const size_t& from, const size_t& count = 1, bool visible = true);

    /**
     * @brief Transformation of objects
     */
    void transformObjects(const Eigen::Matrix4f& trMatrix, const size_t& from, const size_t& count);
    void translateObjects(const Eigen::Vector3f& translation, const size_t& from, const size_t& count);
    void rotateObjects(const Eigen::AngleAxis<float>& rotation, const size_t& from, const size_t& count);
    void rotateObjects(const Eigen::Quaternion<float>& rotation, const size_t& from, const size_t& count);
    void scaleObjects(const Eigen::Vector3f& scaling, const size_t& from, const size_t& count);

    /**
     * @brief Inherited from IChunk. One object is composed of n points (n is defined in constructor ==> nElementByObject)
     * @warning NOT same as "countPoints" method
     * @from : index of the first OBJECT to show/hide (not the global index or the local index but the index of
     *               the nth OBJECT in the list of indexes). Must be between [0; (countObjects()-1)] naturally.
     */
    void setObjectsVisible(const size_t& from, const size_t& count = 1, bool visible = true);

    /**
     * @brief Returns true if at least one object is visible
     */
    bool isAtLeastOneObjectVisible(const size_t &from, const size_t &count) const;

    /**
     * @brief Remove all points
     */
    void clearPoints();

    /**
     * @brief Inherited from IChunk. Same as "clearPoints" method
     */
    void clearObjects() { clearPoints(); }

    /**
     * @brief Remove all objects from this chunk but conserve the size of arrays
     */
    void resetObjects();

    /**
     * @brief Returns the total number of points that was added to this object
     */
    size_t countPoints() const;

    /**
     * @brief Inherited from IChunk.
     * @warning NOT same as "countPoints" method
     * @return returns the number of objects. One object is composed of n points (n is defined in constructor ==> nElementByObject)
     */
    size_t countObjects() const { return countPoints()/getNumberOfElementPerObject(); }

    /**
     * @brief Set the maximum number of points to draw when you call the method "fastDraw"
     * @param maxNumber : the maximum number of points
     */
    void setMaxNumberOfPointsToDrawInFastDraw(const size_t &maxNumber);

    /**
     * @brief Change the beginning of the cloud. Just for compatibility (do nothing)
     */
    //void changeBeginningOfCloud(const size_t &beginGlobalIndex);

    /**
     * @brief Returns the color cloud
     */
    ColorCloud* getColorCloud() const;

    /**
     * @brief Returns the normal cloud
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
     * @brief Draw text or other objects
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

    /**
     * @brief Opengl mode to use to draw objects
     */
    GLenum                  m_glMode;

    /**
     * @brief Max number of points to draw in fast draw
     */
    size_t                  m_maxNumberOfPointsInFastDrawMode;

    /**
     * @brief Increment variable used in fast draw mode to jump from a point/color/normal to the next
     */
    int                     m_fastStride;

    /**
     * @brief Collection of vertex
     */
    VertexArray             m_vertexes;

    /**
     * @brief Collection of infos
     */
    InfoArray               m_infos;
    InfoCloud*              m_infosAccess;

    /**
     * @brief Collection of colors
     */
    ColorArray*             m_colors;
    ColorCloud*             m_colorsAccess;

    /**
     * @brief Collection of normals
     */
    NormalArray*            m_normals;
    NormalCloud*            m_normalsAccess;

    /**
     * @brief Really number of indices in the array (how many to draw)
     */
    size_t                  m_nPoints;

    /**
     * @brief Mutex for thread !
     */
    QMutex*                 m_mutex;

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
     * @brief Call glDrawObjects with buffer object or with tab
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
     * @brief Returns the vertex array to use, or NULL if we must no use it
     */
    VertexArray* getVertexArray() const;

    /**
     * @brief Returns the info array to use, or NULL if we must no use it
     */
    InfoArray* getInfoArray() const;

    /**
     * @brief Returns the color array to use, or NULL if we must no use it
     */
    ColorArray* getColorArray() const;

    /**
     * @brief Returns the normal array to use, or NULL if we must no use it
     */
    NormalArray* getNormalArray() const;

    /**
     * @brief Returns the first vertex to draw
     */
    Vertex* getFirstVertex() const;

    /**
     * @brief Returns the first color to draw
     */
    Color* getFirstColor() const;

    /**
     * @brief Returns the first normal to draw
     */
    Normal* getFirstNormal() const;

    /**
     * @brief Returns the first byte info (selection, visibility, etc...)
     */
    Info* getFirstInfo() const;

    /**
     * @brief Modify the update status of the context
     */
    void setContextUpdated(GenericRendererContext* context, bool status);
};

#endif // CHUNKEDGENERIC_H
