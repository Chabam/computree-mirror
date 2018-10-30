#include "boxpermanentrenderer.h"

BoxGLRenderer BoxPermanentRenderer::m_basicDrawRenderer = BoxGLRenderer();

BoxPermanentRenderer::BoxPermanentRenderer() : BoxDefaultInstancedPermanentRendererSuperClass()
{
    setVertexArray(&m_basicDrawRenderer.getValues());
    setNormalIndicesArray(&m_basicDrawRenderer.getIndices());
}

void BoxPermanentRenderer::addBox(const double& cx, const double& cy, const double &cz,
                                  const float& width, const float& height, const float& length,
                                  const Eigen::Vector3f& widthAxisDirection,
                                  const Eigen::Vector3f& heightAxisDirection,
                                  const ElementInfo &info,
                                  IChunk** chunk)
{
    Chunk::ShaderParamsType params;

    // TODO : use all in comment when the InstancedPermanentRenderer::createOrGetChunkForElement method will use other than just the center of the bbox
    /*params = Chunk::ShaderParamsType::staticConstructFromValues(cx, cy, cz,
                                                                 width, height, length,
                                                                 widthAxisDirection, heightAxisDirection,
                                                                 Eigen::Vector3d(0,0,0));

    Eigen::Matrix4f m = Eigen::Matrix4f::Identity();
    params.getMatrix4f(m);

    Eigen::Vector3f min, max;
    min[0] = std::numeric_limits<float>::max();
    min[1] = min[0];
    min[2] = min[0];

    max[0] = -min[0];
    max[1] = -min[0];
    max[2] = -min[0];

    const std::vector<Eigen::Vector3f> &values = m_basicDrawRenderer.getValues();

    const std::size_t& size = values.size();

    for(size_t i=0; i<size; ++i) {
        const Eigen::Vector3f &v = values[i];
        Eigen::Vector4f tmp = Eigen::Vector4f(v[0], v[1], v[2], 0);

        tmp = m * tmp;

        min[0] = qMin(min[0], tmp[0]);
        min[1] = qMin(min[1], tmp[1]);
        min[2] = qMin(min[2], tmp[2]);

        max[0] = qMax(max[0], tmp[0]);
        max[1] = qMax(max[1], tmp[1]);
        max[2] = qMax(max[2], tmp[2]);
    }

    Eigen::Vector3d minD(min[0]+cx, min[1]+cy, min[2]+cz);
    Eigen::Vector3d maxD(max[0]+cx, max[1]+cy, max[2]+cz);*/

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
