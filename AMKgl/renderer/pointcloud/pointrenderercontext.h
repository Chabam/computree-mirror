#ifndef POINTRENDERERCONTEXT_H
#define POINTRENDERERCONTEXT_H

#include "pointrenderershaders.h"
#include "pointcloudarrayobjectmanager.h"
#include "renderer/renderercontext.h"

/**
 * @brief Keep all elements necessary to draw points and normals in a
 *        specific opengl context
 */
class PointRendererContext : public RendererContext<PointRendererShaders>
{
public:
    PointRendererContext(const IGraphicsDocument* doc, const QOpenGLContext* newContext) :
        RendererContext<PointRendererShaders>(doc, newContext) {}
};

#endif // POINTRENDERERCONTEXT_H
