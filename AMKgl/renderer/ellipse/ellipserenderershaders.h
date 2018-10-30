#ifndef ELLIPSERENDERERSHADERS_H
#define ELLIPSERENDERERSHADERS_H

#include "renderer/instanced/instancedarrayobjectmanager.h"
#include "renderer/instanced/instancedrenderershaders.h"
#include "ellipseshaderparams.h"

/**
 * @brief Manage shaders to draw ellipses
 */
class EllipseRendererShaders : public InstancedRendererShaders<EllipseShaderParams>
{
public:
    typedef InstancedArrayObjectManager< EllipseRendererShaders > ArrayObjectManagerType;

    /**
     * @brief Construct an empty object not initialized
     */
    EllipseRendererShaders();
};

#endif // ELLIPSERENDERERSHADERS_H
