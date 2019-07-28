#include "cylinderpermanentrenderer.h"

// 50 points for one cylinder in normal draw
CylinderGLRenderer CylinderPermanentRenderer::m_basicDrawRenderer = CylinderGLRenderer(50);

// 10 points for one cylinder in fast draw
CylinderGLRenderer CylinderPermanentRenderer::m_fastDrawRenderer = CylinderGLRenderer(10);

CylinderPermanentRenderer::CylinderPermanentRenderer() : BoxDefaultInstancedPermanentRendererSuperClass(GL_TRIANGLE_STRIP)
{
    setVertexArray(&m_basicDrawRenderer.getValues());
    setNormalIndicesArray(&m_basicDrawRenderer.getIndices());

    setFastVertexArray(&m_fastDrawRenderer.getValues());
    setFastIndicesArray(&m_fastDrawRenderer.getIndices());
}

void CylinderPermanentRenderer::addCylinder(const double &cx, const double &cy, const double &cz,
                                            const float &width, const float &height, const float &length,
                                            const Eigen::Vector3f &widthAxisDirection, const Eigen::Vector3f &heightAxisDirection,
                                            const ElementInfo &info,
                                            IChunk** chunk)
{
    Chunk::ShaderParamsType params;

    // TODO : computee bounding box when the InstancedPermanentRenderer::createOrGetChunkForElement method will use other than just the center of the bbox

    Eigen::Vector3d minD(cx, cy, cz);
    Eigen::Vector3d maxD(cx, cy, cz);

    const Chunk &ch = createOrGetChunkForShape(Eigen::AlignedBox3d(minD, maxD));

    if(chunk != nullptr)
        *chunk = (Chunk*)&ch;

    params = Chunk::ShaderParamsType::staticConstructFromValues(cx, cy, cz,
                                                                width, height, length,
                                                                widthAxisDirection, heightAxisDirection,
                                                                ch.getOffset(),
                                                                1.0);

    addShape(ch, params, info);
}
