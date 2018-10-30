#ifndef DEFAULTINSTANCEDPERMANENTRENDERER
#define DEFAULTINSTANCEDPERMANENTRENDERER

#include "instancedpermanentrenderer.h"

#include "renderer/renderercontext.h"

template<typename Shaders>
class DefaultInstancedPermanentRenderer : public InstancedPermanentRenderer< ChunkedInstanced< RendererContext<Shaders > > >
{
public:
    typedef ChunkedInstanced< RendererContext<Shaders > >  Chunk;
    typedef InstancedPermanentRenderer< Chunk >            InstancedPermanentRendererSuperClass;

    /**
     * @brief Construct a default permanent renderer to draw shapes with the "instanced" technique. The opengl
     *        mode (GLEnum) used to draw the shape must be returned by the ShaderParams of the Chunk in method
     *        "staticGetGLMode()"
     */
    DefaultInstancedPermanentRenderer() : InstancedPermanentRendererSuperClass() {}

    /**
     * @brief Construct a default permanent renderer to draw shapes with the "instanced" technique. The opengl
     *        mode (GLEnum) used to draw the shape was specified in this constructor.
     */
    DefaultInstancedPermanentRenderer(GLenum glMode) : InstancedPermanentRendererSuperClass(glMode) {}
};

#endif // DEFAULTINSTANCEDPERMANENTRENDERER

