#include "pointpermanentrenderer.h"

PointPermanentRenderer::PointPermanentRenderer() : GenericPermanentRenderer(GL_POINTS)
{

}

void PointPermanentRenderer::addPoint(const Eigen::Vector3d &p, const ElementInfo &info, IChunk **chunk, const Color& color)
{
    const Chunk &ch = createOrGetChunkForObject(p);

    if(chunk != nullptr)
        *chunk = (Chunk*)&ch;

    addObjectVertex(ch, p, info, color);
}
