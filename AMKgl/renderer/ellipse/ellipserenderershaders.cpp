#include "ellipserenderershaders.h"

EllipseRendererShaders::EllipseRendererShaders() : InstancedRendererShaders<EllipseShaderParams>(3, 3, GL_FLOAT, "://shaders/ellipses/simple.vert")
{
}
