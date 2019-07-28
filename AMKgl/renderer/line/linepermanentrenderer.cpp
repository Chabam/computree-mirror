#include "linepermanentrenderer.h"

LinePermanentRenderer::LinePermanentRenderer() : GenericPermanentRenderer(GL_LINES)
{
}

void LinePermanentRenderer::addLine(const Eigen::Vector3d &p1,
                                    const Eigen::Vector3d &p2,
                                    const ElementInfo &info,
                                    IChunk** chunk,
                                    const Color &c1,
                                    const Color &c2)
{
    Eigen::Vector3d center = (p1 + p2) / 2.0;

    const Chunk &ch = createOrGetChunkForObject(center);

    if(chunk != nullptr)
        *chunk = (Chunk*)&ch;

    addObjectVertex(ch, p1, info, c1);
    addObjectVertex(ch, p2, info, c2);
}
