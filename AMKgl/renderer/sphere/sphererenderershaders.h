#ifndef SPHERERENDERERSHADERS_H
#define SPHERERENDERERSHADERS_H

#include "renderer/instanced/instancedarrayobjectmanager.h"
#include "renderer/instanced/instancedrenderershaders.h"
#include "sphereshaderparams.h"

/**
 * @brief Manage shaders to draw sphere
 */
class SphereRendererShaders : public InstancedRendererShaders<SphereShaderParams>
{
public:
    typedef InstancedArrayObjectManager< SphereRendererShaders > ArrayObjectManagerType;

    /**
     * @brief Construct an empty object not initialized
     */
    SphereRendererShaders();
};

#endif // SPHERERENDERERSHADERS_H
