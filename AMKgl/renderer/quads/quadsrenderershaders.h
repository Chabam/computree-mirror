#ifndef QUADSRENDERERSHADERS_H
#define QUADSRENDERERSHADERS_H

#include "renderer/instanced/instancedarrayobjectmanager.h"
#include "renderer/instanced/instancedrenderershaders.h"
#include "quadsshaderparams.h"

/**
 * @brief Manage shaders to draw quads
 */
class QuadsRendererShaders : public InstancedRendererShaders<QuadsShaderParams>
{
public:
    typedef InstancedArrayObjectManager< QuadsRendererShaders > ArrayObjectManagerType;

    /**
     * @brief Construct an empty object not initialized
     */
    QuadsRendererShaders();
};

#endif // QUADSRENDERERSHADERS_H
