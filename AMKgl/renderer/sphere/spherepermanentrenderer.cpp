#include "spherepermanentrenderer.h"

#define THETAPHIATTRIBUTE_NAME_IN_SHADER "thetaPhi"

SphereGlRenderer SpherePermanentRenderer::m_basicDrawRenderer = SphereGlRenderer(50);

SpherePermanentRenderer::SpherePermanentRenderer() : SphereDefaultInstancedPermanentRendererSuperClass()
{
    setVertexArray(&m_basicDrawRenderer.getValues());

    if(!m_basicDrawRenderer.getIndices().empty())
        setNormalIndicesArray(&m_basicDrawRenderer.getIndices());

    setCustomVertexAttribute(THETAPHIATTRIBUTE_NAME_IN_SHADER,
                             2,
                             GL_FLOAT,
                             GL_FALSE,
                             sizeof(Eigen::Vector2f),
                             sizeof(Eigen::Vector2f),
                             m_basicDrawRenderer.getThetaPhiValues().size(),
                             (GLvoid*)&m_basicDrawRenderer.getThetaPhiValues()[0](0));
}

void SpherePermanentRenderer::addSphere(const double &cx, const double &cy, const double &cz,
                                        const double &radius,
                                        const double &initTheta, const double &endTheta,
                                        const double &initPhi, const double &endPhi,
                                        bool phiThetaInRadians,
                                        const ElementInfo &info,
                                        IChunk **chunk)
{
    SphereChunk::ShaderParamsType params;

    Eigen::Vector3d minD(cx-radius, cy-radius, cz-radius);
    Eigen::Vector3d maxD(cx+radius, cy+radius, cz+radius);

    const SphereChunk &ch = createOrGetChunkForShape(Eigen::AlignedBox3d(minD, maxD));

    ((SphereChunk*)&ch)->setPermanentRenderer(this);

    if(chunk != nullptr)
        *chunk = (SphereChunk*)&ch;

    params = SphereChunk::ShaderParamsType::staticConstructFromValues(cx, cy, cz,
                                                                radius,
                                                                initTheta, endTheta,
                                                                initPhi, endPhi,
                                                                phiThetaInRadians,
                                                                ch.getOffset());

    addShape(ch, params, info);
}

Eigen::Vector2f* SpherePermanentRenderer::getFirstThetaPhiAttribute() const
{
    return &const_cast<std::vector<Eigen::Vector2f>&>(m_basicDrawRenderer.getThetaPhiValues())[0];
}

