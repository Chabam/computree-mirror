#ifndef PERMANENTRENDERER_H
#define PERMANENTRENDERER_H

#include "abstractpermanentrenderer.h"
#include "context/icontextaccessor.h"
#include "renderer/object/iobjectsrendererattributesaccessor.h"
#include "tools/objectattributesprovider.h"

template<typename RendererContextT>
class PermanentRenderer : public AbstractPermanentRenderer, public IContextAccessor<RendererContextT>, public IObjectsRendererAttributesAccessor
{
public:
    PermanentRenderer();
    virtual ~PermanentRenderer() {}

    /**
     * @brief Return the type of object draw by this renderer
     */
    virtual Scene::ObjectType getTypeOfObjectDrawn() const = 0;

    /**
     * @brief Call this method when you add this renderer to
     *        your graphics view and pass it your opengl context
     */
    virtual void addedTo(const IGraphicsDocument *doc, const QOpenGLContext* newContext)
    {
        IContextAccessor<RendererContextT>::addedTo(doc, newContext);

        ObjectAttributesProvider *provider = getAnyElementsAttributesProvider()->getObjectsAttributeProvider(this->getTypeOfObjectDrawn());
        provider->setObjectsRendererAttributesAccessor(this);
    }

    /**
     * @brief Call this method when you remove this renderer from
     *        your graphics view and pass it your opengl context
     */
    virtual void removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
     { IContextAccessor<RendererContextT>::removedFrom(doc, newContext); }

    /**
     * @brief Returns all contexts
     */
    const typename IContextAccessor<RendererContextT>::ContextCollection& getContexts() const
    { return IContextAccessor<RendererContextT>::getContexts(); }

    /**
     * @brief Returns the current context
     */
    RendererContextT* getCurrentContext() const
    { return IContextAccessor<RendererContextT>::getCurrentContext(); }

    /**
     * @brief Returns the current document
     */
    IGraphicsDocument* getCurrentDocument() const
    { return IContextAccessor<RendererContextT>::getCurrentDocument(); }

};

#include "permanentrenderer.hpp"

#endif // PERMANENTRENDERER_H
