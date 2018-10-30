#include "sphererenderershaders.h"

SphereRendererShaders::SphereRendererShaders() : InstancedRendererShaders<SphereShaderParams>(3, 3, GL_FLOAT, "://shaders/sphere/simple.vert")
{

}

