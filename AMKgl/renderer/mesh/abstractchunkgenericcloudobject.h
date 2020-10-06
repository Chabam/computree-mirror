#ifndef ABSTRACTCHUNKGENERICCLOUDOBJECT_H
#define ABSTRACTCHUNKGENERICCLOUDOBJECT_H

#include "renderer/chunk.h"
#include "renderer/generic/generictypedef.h"
#include "renderer/genericcloud/genericcloudrenderercontext.h"

#include "definepoint.h"
#include "includepoint.h"

#include "defineglobalcloud.h"

class ICloudAttributesProvider;

/**
 * @brief Abstract class for chunk that will draw objects from global cloud like edges, faces, etc.... (all except points)
 */
class AbstractChunkGenericCloudObject : public Chunk<GenericCloudRendererContext>
{
private:
    typedef Chunk<GenericCloudRendererContext> SuperClass;

public:
    typedef GenericCloudRendererContext                     Context;
    typedef GenericCloudRendererContext::ShadersType        Shaders;

    typedef Basic::LocalVertex                              Vertex;
    typedef Basic::LocalColor                               Color;
    typedef Basic::LocalInfo                                Info;
    typedef Basic::LocalNormal                              Normal;
    typedef Basic::ColorCloud                               ColorCloud;
    typedef Basic::InfoCloud                                InfoCloud;
    typedef Basic::NormalCloud                              NormalCloud;

    typedef AMKgl::FloatPoint                               FloatPoint;
    typedef AMKgl::GlobalColorCloud                         GlobalColorCloud;
    typedef AMKgl::GlobalInfoCloud                          GlobalInfoCloud;
    typedef AMKgl::GlobalNormalCloud                        GlobalNormalCloud;

    AbstractChunkGenericCloudObject(const uint& uniqueKey,
                                    Scene::ObjectType objectType,
                                    GLenum glMode,
                                    const quint8& nVertexPerObject,
                                    const Eigen::Vector3d& coordinateSystemOffset);
    ~AbstractChunkGenericCloudObject();

    /**
     * @brief Do nothing. Just for compatibility.
     */
    void init() {}

    /**
     * @brief Set the attributes accessor
     */
    void setAttributesAccessor(const IAttributesAccessor* accessor) override;

    /**
     * @brief Set which cloud : vertex color/normal/etc... cloud OR object color/normal/etc... to use when draw object
     * @param cloudOf : Vertex or Object ?
     */
    void setWhichCloudToUseForObjects(AMKgl::GenericCloudMode cloudOf);

    /**
     * @brief Returns which cloud : vertex color/normal/etc... cloud OR object color/normal/etc... to use when draw object
     */
    AMKgl::GenericCloudMode getWhichCloudToUseForObjects() const;

    /**
     * @brief Returns the global index of the first vertex in the cloud used by this chunk
     */
    virtual size_t getBeginningOfVertexCloud() const = 0;

    /**
     * @brief Returns the global index of the first object in the cloud used by this chunk
     */
    virtual size_t getBeginningOfObjectCloud() const = 0;

    /**
     * @brief Transformation of objects. Do nothing !
     */
    void transformObjects(const Eigen::Matrix4f& trMatrix, const size_t& from, const size_t& count) override { Q_UNUSED(trMatrix) Q_UNUSED(from) Q_UNUSED(count) }
    void translateObjects(const Eigen::Vector3f& translation, const size_t& from, const size_t& count) override { Q_UNUSED(translation) Q_UNUSED(from) Q_UNUSED(count) }
    void rotateObjects(const Eigen::AngleAxis<float>& rotation, const size_t& from, const size_t& count) override { Q_UNUSED(rotation) Q_UNUSED(from) Q_UNUSED(count)}
    void rotateObjects(const Eigen::Quaternion<float>& rotation, const size_t& from, const size_t& count) override { Q_UNUSED(rotation) Q_UNUSED(from) Q_UNUSED(count) }
    void scaleObjects(const Eigen::Vector3f& scaling, const size_t& from, const size_t& count) override { Q_UNUSED(scaling) Q_UNUSED(from) Q_UNUSED(count) }

    /**
     * @brief Inherited from IChunk.
     */
    void setObjectsVisible(const size_t& from, const size_t& count = 1, bool visible = true) override;
    bool isAtLeastOneObjectVisible(const size_t &from, const size_t &count) const override;
    void clearObjects() override;
    void resetObjects() override;
    size_t countObjects() const override;
    quint8 getNumberOfElementPerObject() const override;

    /**
     * @brief Draw all points normally (all points are drawn)
     */
    void draw(DrawInfo &info) override;

    /**
     * @brief Draw all points fast (a reduced number of points are drawn)
     */
    void fastDraw(DrawInfo &info) override;

    /**
     * @brief Draw text or other objects
     */
    void postDraw(DrawInfo &info) override;

    /**
     * @brief Lock it (mutex)
     */
    void lock() override {}
    /**
     * @brief Unlock it (mutex)
     */
    void unlock() override {}

    /**
     * @brief Returns the color cloud, or nullptr if it was not created.
     */
    Basic::AbstractColorCloud* getObjectColorCloud() const override;

    /**
     * @brief Returns the normal cloud, or nullptr if it was not created.
     */
    Basic::AbstractNormalCloud* getObjectNormalCloud() const override;

    /**
     * @brief Returns the normal cloud, or nullptr if it was not created.
     */
    Basic::AbstractInfoCloud* getObjectInfoCloud() const override;

    /**
     * @brief Returns the color cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    Basic::AbstractColorCloud* createOrGetObjectColorCloud() override;

    /**
     * @brief Returns the normal cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    Basic::AbstractNormalCloud* createOrGetObjectNormalCloud() override;

    /**
     * @brief Returns the normal cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    Basic::AbstractInfoCloud* createOrGetObjectInfoCloud() override;

    /**
     * @brief Redefined to re-create display list
     */
    void updateInfoInCurrentContext() override;

    /**
     * @brief Redefined to re-create display list
     */
    void updateColorInCurrentContext() override;

    /**
     * @brief Call it to re-create VAO or display list
     */
    void updateOnlyVAO() override;

    ///////////// VERTEX //////////////

    /**
     * @brief Returns the global color cloud of vertex, or nullptr if it was not created yet
     */
    GlobalColorCloud* getVertexGlobalColorCloud() const;

    /**
     * @brief Returns the global normal cloud of vertex, or nullptr if it was not created yet
     */
    GlobalNormalCloud* getVertexGlobalNormalCloud() const;

    /**
     * @brief Returns the global info cloud of vertex, or nullptr if it was not created yet
     */
    GlobalInfoCloud* getVertexGlobalInfoCloud() const;

    /**
     * @brief Returns the global color cloud of vertex and create it if it was not created yet
     */
    GlobalColorCloud* createOrGetVertexGlobalColorCloud();

    /**
     * @brief Returns the global normal cloud of vertex and create it if it was not created yet
     */
    GlobalNormalCloud* createOrGetVertexGlobalNormalCloud();

    /**
     * @brief Returns the global info cloud of vertex and create it if it was not created yet
     */
    GlobalInfoCloud* createOrGetVertexGlobalInfoCloud();

    /**
     * @brief Returns the first vertex
     */
    FloatPoint* getFirstVertex() const;

    /**
     * @brief Returns the first color for vertex, nullptr if the cloud was not created yet !
     */
    Color* getFirstVertexColor() const;

    /**
     * @brief Returns the first normal for vertex, nullptr if the cloud was not created yet !
     */
    Normal* getFirstVertexNormal() const;

    /**
     * @brief Returns the first info for vertex, nullptr if the cloud was not created yet !
     */
    Info* getFirstVertexInfo() const;

    ///////////////// OBJECT ///////////////

    /**
     * @brief Returns the global color cloud of object, or nullptr if it was not created yet
     */
    GlobalColorCloud* getObjectGlobalColorCloud() const;

    /**
     * @brief Returns the global normal cloud of object, or nullptr if it was not created yet
     */
    GlobalNormalCloud* getObjectGlobalNormalCloud() const;

    /**
     * @brief Returns the global info cloud of object, or nullptr if it was not created yet
     */
    GlobalInfoCloud* getObjectGlobalInfoCloud() const;

    /**
     * @brief Returns the global color cloud of object and create it if it was not created yet
     */
    GlobalColorCloud* createOrGetObjectGlobalColorCloud();

    /**
     * @brief Returns the global normal cloud of object and create it if it was not created yet
     */
    GlobalNormalCloud* createOrGetObjectGlobalNormalCloud();

    /**
     * @brief Returns the global info cloud of object and create it if it was not created yet
     */
    GlobalInfoCloud* createOrGetObjectGlobalInfoCloud();

    /**
     * @brief Returns the first color for object, nullptr if the cloud was not created yet !
     */
    Color* getFirstObjectColor() const;

    /**
     * @brief Returns the first normal for object, nullptr if the cloud was not created yet !
     */
    Normal* getFirstObjectNormal() const;

    /**
     * @brief Returns the first byte info (selection, visibility, etc...) for objects, nullptr if the cloud was not created yet !
     */
    Info* getFirstObjectInfo() const;
    Info* createOrGetFirstObjectInfo() const;

    /**
     * @brief Returns the first object index in the array
     */
    virtual GLuint* getFirstObjectIndex() const = 0;

    /**
     * @brief Returns the first vertex index in the array
     */
    virtual GLuint* getFirstVertexIndex() const = 0;

protected:
    /**
     * @brief Number of vertex per object
     */
    quint8                          m_nVertexPerObject;

    /**
     * @brief Opengl mode to use to draw objects
     */
    GLenum                          m_glMode;

    /**
     * @brief Which cloud to use for colors/normals/etc... (vertex or object ?)
     */
    AMKgl::GenericCloudMode m_cloudOfToUse;

    /**
     * @brief Object cloud provider for this chunk
     */
    ICloudAttributesProvider*       m_objectCloudProvider;

    /**
     * @brief Point cloud provider for this chunk
     */
    ICloudAttributesProvider*       m_pointCloudProvider;

    /**
     * @brief Really number of indices in the array of vertex indexes (how many to draw)
     */
    size_t                          m_nPoints;

    /**
     * @brief Really number of indices in the array of objects indexes (how many to draw)
     */
    size_t                          m_nObjects;

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
     * @brief Draw normal but use basic method of opengl
     */
    virtual void drawBasic(DrawInfo &info);

    /**
     * @brief Draw normals
     */
    virtual void drawBasicNormals(DrawInfo &info);

    /**
     * @brief Set the number of objects in all array
     */
    void setNumberOfObjects(const size_t& n) override;
    virtual void internalSetNumberOfObjects(const size_t& n, const bool& shrinkToFit) = 0;

    /**
     * @brief If the member m_objectColorCloud is nullptr it will be created
     */
    virtual void createObjectColorCloudMemberIfNot() = 0;

    /**
     * @brief If the member m_objectNormalCloud is nullptr it will be created
     */
    virtual void createObjectNormalCloudMemberIfNot() = 0;

    /**
     * @brief If the member m_objectInfoCloud is nullptr it will be created
     */
    virtual void createObjectInfoCloudMemberIfNot() = 0;
};

#endif // ABSTRACTCHUNKGENERICCLOUDOBJECT_H
