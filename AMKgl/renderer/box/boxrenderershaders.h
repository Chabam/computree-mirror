#ifndef BOXRENDERERSHADERS_H
#define BOXRENDERERSHADERS_H

#include "renderer/instanced/instancedarrayobjectmanager.h"
#include "renderer/instanced/instancedrenderershaders.h"
#include "boxshaderparams.h"

/**
 * @brief Manage shaders to draw box
 */
class BoxRendererShaders : public InstancedRendererShaders<BoxShaderParams>
{
public:
    typedef InstancedArrayObjectManager< BoxRendererShaders > ArrayObjectManagerType;

    /**
     * @brief Construct an empty object not initialized
     */
    BoxRendererShaders();
};

#endif // BOXRENDERERSHADERS_H
