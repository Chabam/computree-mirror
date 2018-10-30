#include "trianglepermanentrenderer.h"

TrianglePermanentRenderer::TrianglePermanentRenderer() : GenericPermanentRenderer(GL_TRIANGLES)
{
}

void TrianglePermanentRenderer::addTriangle(const Eigen::Vector3d &p1,
                                            const Eigen::Vector3d &p2,
                                            const Eigen::Vector3d &p3,
                                            const ElementInfo &info,
                                            IChunk** chunk,
                                            const Color &c1,
                                            const Color &c2,
                                            const Color &c3)
{
    Eigen::Vector3d center = (p1 + p2 + p3) / 3.0;

    const Chunk &ch = createOrGetChunkForObject(center);

    if(chunk != NULL)
        *chunk = (Chunk*)&ch;

    addObjectVertex(ch, p1, info, c1);
    addObjectVertex(ch, p2, info, c2);
    addObjectVertex(ch, p3, info, c3);
}
