#ifndef GENERICRENDERERCONTEXT_H
#define GENERICRENDERERCONTEXT_H

#include "genericrenderershaders.h"
#include "genericobjectarrayobjectmanager.h"
#include "renderer/renderercontext.h"

/**
 * @brief Keep all elements necessary to draw basic elements in a
 *        specific opengl context
 */
class GenericRendererContext : public RendererContext<GenericRendererShaders>
{
public:
    GenericRendererContext(const IGraphicsDocument* doc, const QOpenGLContext* newContext) :
        RendererContext<GenericRendererShaders>(doc, newContext) {}
};

#endif // GENERICRENDERERCONTEXT_H
