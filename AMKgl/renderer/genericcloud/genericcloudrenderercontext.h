#ifndef GENERICCLOUDRENDERERCONTEXT_H
#define GENERICCLOUDRENDERERCONTEXT_H

#include "genericcloudrenderershaders.h"
#include "renderer/renderercontext.h"

/**
 * @brief Keep all elements necessary to draw basic elements in a
 *        specific opengl context
 */
class GenericCloudRendererContext : public RendererContext<GenericCloudRendererShaders>
{
public:
    GenericCloudRendererContext(const IGraphicsDocument* doc, const QOpenGLContext* newContext) :
        RendererContext<GenericCloudRendererShaders>(doc, newContext) {}
};

#endif // GENERICCLOUDRENDERERCONTEXT_H
