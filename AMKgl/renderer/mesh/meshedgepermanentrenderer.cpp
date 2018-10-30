#include "meshedgepermanentrenderer.h"

#include "tools/edgecloudattributesprovider.h"
#include "interfaces/iglobaledgecloudmanager.h"

MeshEdgePermanentRenderer::MeshEdgePermanentRenderer() : SuperClass(GL_LINES, 2, 0, 0)
{
    m_chunkForOutliers = NULL;
    m_attributesCloudProvider = new EdgeCloudAttributesProvider();

    setChunkForOutliersCreator(this);
    setContextAccessor(this);
}

MeshEdgePermanentRenderer::~MeshEdgePermanentRenderer()
{
    delete m_chunkForOutliers;
    delete m_attributesCloudProvider;
}

void MeshEdgePermanentRenderer::addEdge(const size_t &edgeIndex,
                                        const size_t &globalV1Index,
                                        const size_t &globalV2Index,
                                        IChunk **chunk)
{
    QMutexLocker locker(m_mutex);

    m_vertexIndices[0] = globalV1Index;
    m_vertexIndices[1] = globalV2Index;

    addObjectVertexes(edgeIndex,
                      m_vertexIndices,
                      chunk);
}

EdgeCloudAttributesProvider* MeshEdgePermanentRenderer::getEdgeCloudAttributesProvider() const
{
    return m_attributesCloudProvider;
}

MeshEdgePermanentRenderer::SuperClass::OtherChunkCollection MeshEdgePermanentRenderer::getOthersChunk() const
{
    OtherChunkCollection c = SuperClass::getOthersChunk();

    if(m_chunkForOutliers != NULL)
        c.push_back(m_chunkForOutliers);

    return c;
}

MeshObjectNM::OutlierChunkType& MeshEdgePermanentRenderer::createOrGetChunkForOutliers()
{
    if(m_chunkForOutliers == NULL) {
        m_chunkForOutliers = new MeshObjectNM::OutlierChunkType(Scene::LineGlobalCloud,
                                                                GL_LINES,
                                                                2,
                                                                *AMKglEA->getGlobalPointCloudManager(),
                                                                *AMKglEA->getGlobalEdgeCloudManager());
        m_chunkForOutliers->setContextAccessor(getContextAccessor());
        m_chunkForOutliers->setAttributesAccessor(getAttributesAccessor());
        m_chunkForOutliers->setDrawModeToUse(getDrawModeToUse());
        m_chunkForOutliers->init();
    }

    return (*m_chunkForOutliers);
}

