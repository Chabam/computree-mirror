#include "ellipsepermanentrenderer.h"

// 50 points for one ellipse in normal draw
EllipseGLRenderer EllipsePermanentRenderer::m_basicDrawRenderer = EllipseGLRenderer(50);

EllipsePermanentRenderer::EllipsePermanentRenderer() : EllipseDefaultInstancedPermanentRendererSuperClass()
{
    // 5 points for a simplified version of one ellipse
    for(int i=0; i<50; i+=10)
        m_fastIndicesArray.push_back(i);

    setVertexArray(&m_basicDrawRenderer.getValues());

    setFastIndicesArray(&m_fastIndicesArray);
}

void EllipsePermanentRenderer::addEllipse(const double& cx, const double& cy, const double &cz,
                                          const float& majorAxisRadius, const float& minorAxisRadius,
                                          const Eigen::Vector3f& majorAxisDirection,
                                          const Eigen::Vector3f& normal,
                                          const ElementInfo &info,
                                          IChunk** chunk)
{
    // TODO : contruct the bbox ! when the InstancedPermanentRenderer::createOrGetChunkForElement method will use other than just the center of the bbox

    Eigen::Vector3d minD(cx, cy, cz);
    Eigen::Vector3d maxD(cx, cy, cz);

    const Chunk &ch = createOrGetChunkForShape(Eigen::AlignedBox3d(minD, maxD));

    if(chunk != nullptr)
        *chunk = (Chunk*)&ch;

    Chunk::ShaderParamsType params = Chunk::ShaderParamsType::staticConstructFromValues(cx, cy, cz,
                                                                                        majorAxisRadius, minorAxisRadius,
                                                                                        majorAxisDirection, normal,
                                                                                        ch.getOffset());

    addShape(ch, params, info);
}
