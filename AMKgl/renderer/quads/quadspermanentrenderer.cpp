#include "quadspermanentrenderer.h"

QuadsGLRenderer QuadsPermanentRenderer::m_basicDrawRenderer = QuadsGLRenderer();

QuadsPermanentRenderer::QuadsPermanentRenderer() : QuadsDefaultInstancedPermanentRendererSuperClass()
{
    setVertexArray(&m_basicDrawRenderer.getValues());
}

void QuadsPermanentRenderer::addQuads(const double &cx, const double &cy, const double &cz,
                                      const float &width, const float &length,
                                      const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &lengthAxisDirection,
                                      const ElementInfo &info,
                                      IChunk** chunk)
{
    // TODO : contruct the bbox ! when the InstancedPermanentRenderer::createOrGetChunkForElement method will use other than just the center of the bbox

    Eigen::Vector3d minD(cx, cy, cz);
    Eigen::Vector3d maxD(cx, cy, cz);

    const Chunk &ch = createOrGetChunkForShape(Eigen::AlignedBox3d(minD, maxD));

    if(chunk != NULL)
        *chunk = (Chunk*)&ch;

    Chunk::ShaderParamsType params = Chunk::ShaderParamsType::staticConstructFromValues(cx, cy, cz,
                                                                                        width, length,
                                                                                        widthAxisDirection, lengthAxisDirection,
                                                                                        ch.getOffset());

    addShape(ch, params, info);
}
