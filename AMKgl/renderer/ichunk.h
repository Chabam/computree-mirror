#ifndef ICHUNK_H
#define ICHUNK_H

#include <QtOpenGL/QtOpenGL>

#include "Eigen/Core"

#include "drawmode.h"
#include "visitor/iobjectsvisitor.h"
#include "object/iobjectstypeaccessor.h"
#include "renderer/attributes/iattributesaccessor.h"
#include "generic/generictypedef.h"
#include "includecolor.h"

class DrawInfo;
class FlagsPropertyManager;

/**
 * @brief Interface for chunk class
 */
class IChunk : public IObjectsTypeAccessor
{
public:
    IChunk(const uint& uniqueKey, Scene::ObjectType objectType, const Eigen::Vector3d &offset);
    virtual ~IChunk() {}

    /**
     * @brief Set the attributes accessor
     */
    virtual void setAttributesAccessor(const IAttributesAccessor* accessor);

    /**
     * @brief Returns the attributes accessor
     */
    IAttributesAccessor* getAttributesAccessor() const;

    /**
     * @brief Returns the provider that get access to attributes if getAttributesAccessor() don't return nullptr, otherwise return nullptr
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider() const;

    /**
     * @brief Returns the provider of attributes of an element
     */
    ObjectAttributesProvider* getObjectsAttributeProvider() const;

    /**
     * @brief Returns the flags property manager
     */
    FlagsPropertyManager* getFlagsPropertyManager() const;

    /**
     * @brief Return the unique key of this chunk
     */
    inline const uint& getUniqueKey() const { return m_uniqueKey; }

    /**
     * @brief Returns the type of object draw by this chunk
     */
    Scene::ObjectType getTypeOfObjectDrawn() const;

    /**
     * @brief Define the draw mode to use. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    virtual void setDrawModeToUse(DrawMode mode);

    /**
     * @brief Returns the draw mode that must be used
     */
    DrawMode getDrawModeToUse() const;

    /**
     * @brief Returns the draw mode really used in normal mode. Not the same than "getDrawModeToUse" if the draw mode to use cannot be used by your
     *        graphics card.
     */
    DrawMode getNormalDrawModeUsed() const;

    /**
     * @brief Returns the draw mode really used in fast mode. Not the same than "getDrawModeToUse" if the draw mode to use cannot be used by your
     *        graphics card.
     */
    DrawMode getFastDrawModeUsed() const;

    /**
     * @brief Returns true if we must disable graphics card transfer (VBO or VAO). Used in "getNormalDrawModeUsed()" && "getFastDrawModeUsed()" to return an alternative draw mode if necessary.
     */
    bool mustDisableGraphicsCardTransfer() const;

    /**
     * @brief All update methods to be compatible with generic chunk manager
     */
    virtual void updateInAllContext() { update(); }
    virtual void updateInCurrentContext() { update(); }
    virtual void updateInfoInAllContext() { update(); }
    virtual void updateInfoInCurrentContext() { update(); }
    virtual void updateColorInAllContext() { update(); }
    virtual void updateColorInCurrentContext() { update(); }
    virtual void updateNormalInAllContext() { update(); }
    virtual void updateNormalInCurrentContext() { update(); }
    virtual void updateOnlyVAOInAllContext() { updateOnlyVAO(); }
    virtual void updateOnlyVAOInCurrentContext() { updateOnlyVAO(); }
    virtual void updateOnlyVAO() = 0;
    /**
     * @brief Update the structure to draw all points added
     */
    virtual bool update();

    /**
     * @brief Returns true if it was up to date
     */
    virtual bool isUpdated() const;

    /**
     * @brief Call it when you change something and you want to disable the update status
     */
    inline void setNotUpdated() { setUpdated(false); }

    /**
     * @brief Destroy all objects for draw. After call this function nothing will be draw.
     */
    virtual void destroyGLForCurrentContext() = 0;

    /**
     * @brief Destroy all elements to draw in the specified context. After call this function nothing will be draw.
     */
    virtual void destroyGL(const QOpenGLContext* context) = 0;

    /**
     * @brief Returns the offset of this chunk
     */
    const Eigen::Vector3d& getOffset() const;

    /**
     * @brief Draw all normally (all objects in maximum details are drawn)
     */
    virtual void draw(DrawInfo &info) = 0;

    /**
     * @brief Draw all faster (simplified objects are drawn)
     */
    virtual void fastDraw(DrawInfo &info) = 0;

    /**
     * @brief Draw text or other objects
     */
    virtual void postDraw(DrawInfo &info) = 0;

    /**
     * @brief Lock it (mutex)
     */
    virtual void lock() = 0;

    /**
     * @brief Unlock it (mutex)
     */
    virtual void unlock() = 0;

    /**
     * @brief Apply a transformation matrix to all vertex of all objects. Use it if you want
     *        to translate/rotate/scale objects.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate or scale an object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of it can be altered !
     */
    virtual void transformObjects(const Eigen::Matrix4f& trMatrix, const size_t& from, const size_t& count) = 0;

    /**
     * @brief Apply a translation to objects.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you translate an object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    virtual void translateObjects(const Eigen::Vector3f& translation, const size_t& from, const size_t& count) = 0;

    /**
     * @brief Apply a rotation to objects.
     *
     *        Do nothing for global points/edges/faces !
     */
    virtual void rotateObjects(const Eigen::AngleAxis<float>& rotation, const size_t& from, const size_t& count) = 0;
    virtual void rotateObjects(const Eigen::Quaternion<float>& rotation, const size_t& from, const size_t& count) = 0;

    /**
     * @brief Scale objects.
     *
     *        Do nothing for global points/edges/faces !
     *
     * @warning Be carreful when you scale the object because if its
     *          coordinates change from one coordinate system to another the system
     *          won't detect it and the draw of the object can be altered !
     */
    virtual void scaleObjects(const Eigen::Vector3f& scaling, const size_t& from, const size_t& count) = 0;

    /**
     * @brief Show/Hide an object. After you have modified all objects you must call the method "update" to update
     *        the draw of objects.
     * @param from : index of the first object to remove. Must be between [0; (countObjects()-1)] naturally.
     * @param count : number of objects to modify.
     * @param visible : true to set visible, false to hide it
     */
    virtual void setObjectsVisible(const size_t& from, const size_t& count = 1, bool visible = true) = 0;

    /**
     * @brief Returns true if at least one object is visible
     */
    virtual bool isAtLeastOneObjectVisible(const size_t& from, const size_t& count = 1) const = 0;

    /**
     * @brief Remove one or multiple objects from this chunk. After you have removed all points you must call the method "update" to update
     *        the draw of objects.
     * @param from : the local index of the first object to remove. Must be between [0; (maxSize-1)] naturally.
     * @param count : number of objects to remove.
     */
    void removeObjects(const size_t& from, const size_t& count = 1) {
        if(count == 0)
            return;

        setUpdated(false);

        size_t size = countObjects();
        const size_t end = from+count;

        if(end == size) {
            size -= count;
            setNumberOfObjects(size);
            return;
        }

        moveObjectsFromTo(end, from, size - end);
        size -= count;

        setNumberOfObjects(size);
    }

    /**
     * @brief Optimized function to remove in one pass multiple objects in multiple range. Per example you can have
     *        a vector composed of object that contains the begin and the end of objects to remove :
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
     *        removeObjects(vector.begin(), vector.end());
     *
     * @param begin : begin iterator
     * @param end : end iterator
     */
    template<typename iterator>
    void removeObjects(const iterator& begin, const iterator& end) {

        if(begin == end)
            return;

        setUpdated(false);

        iterator it = begin;
        size_t size = countObjects();

        if((*it).end()+1 == size) {
            size -= (*it).count();
            setNumberOfObjects(size);
            return;
        }

        size_t from;
        it = end-1;

        if((*it).end()+1 == size) {
            size -= (*it).count();
            --it;
        }

        while(it != begin)
        {
            from = (*it).end()+1;
            moveObjectsFromTo(from, (*it).begin(), size - from);
            size -= (*it).count();
            --it;
        }

        from = (*it).end()+1;
        moveObjectsFromTo(from, (*it).begin(), size - from);
        size -= (*it).count();

        setNumberOfObjects(size);
    }

    /**
     * @brief Remove all objects from this chunk
     */
    virtual void clearObjects() = 0;

    /**
     * @brief Remove all objects from this chunk but conserve the size of arrays
     */
    virtual void resetObjects() = 0;

    /**
     * @brief Returns the total number of objects that was added to this chunk
     */
    virtual size_t countObjects() const = 0;

    /**
     * @brief Call to visit this chunk
     * @param visitor : is an object that will receive this chunk
     */
    void visitObjects(IObjectsVisitor& visitor);

    /**
     * @brief Returns the number of vertex/colors/infos per object
     */
    virtual quint8 getNumberOfElementPerObject() const = 0;

    /**
     * @brief Returns the color cloud, or nullptr if it was not created.
     */
    virtual Basic::AbstractColorCloud* getObjectColorCloud() const = 0;

    /**
     * @brief Returns the normal cloud, or nullptr if it was not created.
     */
    virtual Basic::AbstractNormalCloud* getObjectNormalCloud() const = 0;

    /**
     * @brief Returns the normal cloud, or nullptr if it was not created.
     */
    virtual Basic::AbstractInfoCloud* getObjectInfoCloud() const = 0;

    /**
     * @brief Returns the color cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    virtual Basic::AbstractColorCloud* createOrGetObjectColorCloud() = 0;

    /**
     * @brief Returns the normal cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    virtual Basic::AbstractNormalCloud* createOrGetObjectNormalCloud() = 0;

    /**
     * @brief Returns the normal cloud to use. If it doesn't exist it will be created.
     * @warning If it can't be created the method returns nullptr !
     */
    virtual Basic::AbstractInfoCloud* createOrGetObjectInfoCloud() = 0;

    /**
     * @brief Set the current color used when add vertex
     */
    void setCurrentColor(const Basic::LocalColor& color);

    /**
     * @brief Return the current color used when vertex is added
     */
    Basic::LocalColor getCurrentColor() const;

private:
    /**
     * @brief Unique key
     */
    uint                    m_uniqueKey;

    /**
     * @brief Type of object draw by this chunk
     */
    Scene::ObjectType       m_objectType;

    /**
     * @brief Draw mode set by user
     */
    DrawMode                m_drawModeToUse;

    /**
     * @brief Draw mode really used in normal draw
     */
    DrawMode                m_drawModeUsedInNormal;

    /**
     * @brief Draw mode really used in fast draw
     */
    DrawMode                m_drawModeUsedInFast;

    /**
     * @brief Offset of the coordinate system used by this chunk
     */
    Eigen::Vector3d         m_offset;

    /**
     * @brief False if not updated
     */
    bool                    m_updated;

protected:
    /**
     * @brief Push the matrix to use the offset
     */
    virtual void pushCoordinateSystemMatrix(DrawInfo &info);

    /**
     * @brief Pop the matrix previously pushed
     */
    virtual void popCoordinateSystemMatrix();

    /**
     * @brief Returns true if must use vao
     */
    bool mustUseVAO() const;

    /**
     * @brief Returns true if must use vbo
     */
    bool mustUseVBO() const;

    /**
     * @brief Returns true if must use raw
     */
    bool mustUseRAW() const;

    /**
     * @brief Change the draw mode used in normal mode
     */
    virtual void setNormalDrawModeUsed(DrawMode mode);

    /**
     * @brief Change the draw mode used in fast mode
     */
    virtual void setFastDrawModeUsed(DrawMode mode);

    /**
     * @brief Get the current color used (opengl call)
     */
    virtual void getCurrentColor(float cColor[]);

    /**
     * @brief Update vao, vbo, etc...
     */
    virtual bool internalUpdate() = 0;

    /**
     * @brief Modify the boolean that inform if we can draw or not
     * @param status : the new status
     */
    virtual void setUpdated(bool status);

    /**
     * @brief Return true if the variable m_update must be modified
     */
    inline bool mustChangeUpdateStatus(const bool& newStatus) const { return (newStatus != m_updated); }

    /**
     * @brief Move values (vertex, info, color, etc...) from specified index to specified index
     * @param from : local index of the source
     * @param to : local index of the destination
     * @param size: number of values to move
     */
    virtual void moveObjectsFromTo(const size_t& from, const size_t& to, const size_t& size) = 0;

    /**
     * @brief Set the number of objects and resize all arrays
     */
    virtual void setNumberOfObjects(const size_t& n) = 0;

private:
    IAttributesAccessor*    m_attributesAccessor;
    Basic::LocalColor       m_currentColor;
};

#endif // ICHUNK_H
