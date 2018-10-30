#include "quadsrenderershaders.h"

QuadsRendererShaders::QuadsRendererShaders() : InstancedRendererShaders<QuadsShaderParams>(3, 3, GL_FLOAT, "://shaders/quads/simple.vert")
{

}
