#ifndef ABSTRACTPERMANENTRENDERER_H
#define ABSTRACTPERMANENTRENDERER_H

#include <QOpenGLContext>

#include "renderer/drawmode.h"
#include "renderer/attributes/iattributesaccessor.h"
#include "object/iobjectstypeaccessor.h"
#include "renderer/generic/generictypedef.h"
#include "renderer/tools/flagspropertymanager.h"
#include "includecolor.h"

class IGraphicsDocument;
class DrawInfo;
class AnyElementAttributesProvider;
class IObjectsVisitor;

class AbstractPermanentRenderer : public IObjectsTypeAccessor
{
public:
    AbstractPermanentRenderer();
    virtual ~AbstractPermanentRenderer() {}

    /**
     * @brief Set the attributes accessor
     */
    virtual void setAttributesAccessor(const IAttributesAccessor* accessor);

    /**
     * @brief Returns the attributes accessor
     */
    IAttributesAccessor* getAttributesAccessor() const;

    /**
     * @brief Call it after you have set the attribute accessor and all necessary elements
     */
    virtual void init() {}

    /**
     * @brief Returns the element attributes provider used
     */
    AnyElementAttributesProvider* getAnyElementsAttributesProvider() const;

    /**
     * @brief Return the flags property manager used
     */
    FlagsPropertyManager* getFlagsPropertyManager() const;

    /**
     * @brief Call this method when you add this renderer to
     *        your graphics view and pass it your opengl context
     */
    virtual void addedTo(const IGraphicsDocument *doc, const QOpenGLContext* newContext) = 0;

    /**
     * @brief Call this method when you remove this renderer from
     *        your graphics view and pass it your opengl context
     */
    virtual void removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext) = 0;

    /**
     * @brief Returns the number of vertex or element that represent one object in clouds (info, color, etc...).
     *        Per example if this method returns :
     *        - 1 ==> For each object there was 1 color/info/etc... so you can use the object index to
     *                get the color/info/etc... (to simplify 1 object is composed by 1 vertex)
     *
     *        - 3 ==> For each object there was 3 color/info/etc... so you must multiply the object
     *                index by 3 to get the first color/info/etc... and iterate over the next 2 color/info/etc...
     *                (to simplify 1 object is composed by 3 vertex, each vertex has a color/info/etc...)
     */
    virtual int getNumberOfJumpForOneObject() const = 0;

    /**
     * @brief Remove all object (same as clearVertex())
     */
    virtual void clearObjects() = 0;

    /**
     * @brief Returns the total number of object that was added to this renderer
     */
    virtual size_t countObjects() const = 0;

    /**
     * @brief Call to visit all objects
     * @param visitor : is an object that will receive all objects
     */
    virtual void visitObjects(IObjectsVisitor& visitor) = 0;

    /**
     * @brief Call this method to update all elements in all context. It will create all
     *        necessary VBO, IBO, etc... to draw it
     */
    virtual void updateInAllContext() = 0;

    /**
     * @brief Call this method to update infos of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateInfoInAllContext() = 0;

    /**
     * @brief Call this method to update colors of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateColorInAllContext() = 0;

    /**
     * @brief Call this method to update normals of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateNormalInAllContext() = 0;

    /**
     * @brief Call this method to update only VAO of elements in all context.
     */
    virtual void updateOnlyVAOInAllContext() = 0;

    /**
     * @brief Define the draw mode to use. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    virtual void setDrawModeToUse(DrawMode mode) = 0;

    /**
     * @brief Call it when you want to destroy all opengl elements (when you close your window)
     */
    virtual void destroyGL(const QOpenGLContext *context) = 0;

    /**
     * @brief Draw all elements normally (all elements are drawn with a good precision)
     */
    virtual void draw(DrawInfo &info) = 0;

    /**
     * @brief Draw all elements fast (all elements are drawn with a minimum number of points)
     */
    virtual void fastDraw(DrawInfo &info) = 0;

    /**
     * @brief Draw text or other elements
     */
    virtual void postDraw(DrawInfo &info) = 0;

    /**
     * @brief Set the current color used when add vertex
     */
    void setCurrentColor(const Basic::LocalColor& color);

    /**
     * @brief Return the current color used when vertex is added
     */
    Basic::LocalColor getCurrentColor() const;

private:
    IAttributesAccessor*    m_attributesAccessor;
    Basic::LocalColor       m_currentColor;
    FlagsPropertyManager    m_flagsPropertyManager;

protected:
    /**
     * @brief Called when the current color changed
     */
    virtual void currentColorChanged() {}
};

#endif // ABSTRACTPERMANENTRENDERER_H
