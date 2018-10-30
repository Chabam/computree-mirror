#include "boxrenderershaders.h"

BoxRendererShaders::BoxRendererShaders() : InstancedRendererShaders<BoxShaderParams>(3, 4, GL_FLOAT, "://shaders/box/simple.vert")
{

}
