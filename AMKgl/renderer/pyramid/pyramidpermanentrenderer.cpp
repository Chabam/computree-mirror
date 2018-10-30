#include "pyramidpermanentrenderer.h"

PyramidGLRenderer PyramidPermanentRenderer::m_basicDrawRenderer = PyramidGLRenderer();

PyramidPermanentRenderer::PyramidPermanentRenderer() : BoxDefaultInstancedPermanentRendererSuperClass(GL_TRIANGLES)
{
    setVertexArray(&m_basicDrawRenderer.getValues());
    setNormalIndicesArray(&m_basicDrawRenderer.getIndices());
}

void PyramidPermanentRenderer::addPyramid(const double &cx, const double &cy, const double &cz,
                                          const float &width, const float &height, const float &length,
                                          const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &heightAxisDirection,
                                          const ElementInfo &info,
                                          IChunk **chunk)
{
    Chunk::ShaderParamsType params;

    // TODO : compute bounding box when the InstancedPermanentRenderer::createOrGetChunkForElement method will use other than just the center of the bbox

    Eigen::Vector3d minD(cx, cy, cz);
    Eigen::Vector3d maxD(cx, cy, cz);

    const Chunk &ch = createOrGetChunkForShape(Eigen::AlignedBox3d(minD, maxD));

    if(chunk != NULL)
        *chunk = (Chunk*)&ch;

    params = Chunk::ShaderParamsType::staticConstructFromValues(cx, cy, cz,
                                                                width, height, length,
                                                                widthAxisDirection, heightAxisDirection,
                                                                ch.getOffset());

    addShape(ch, params, info);
}
