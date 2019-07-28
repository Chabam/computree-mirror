#include "meshfacepermanentrenderer.h"

#include "tools/facecloudattributesprovider.h"
#include "interfaces/iglobalfacecloudmanager.h"

MeshFacePermanentRenderer::MeshFacePermanentRenderer() : SuperClass(GL_TRIANGLES, 3, 0, 0)
{
    m_chunkForOutliers = nullptr;
    m_attributesCloudProvider = new FaceCloudAttributesProvider();

    setChunkForOutliersCreator(this);
    setContextAccessor(this);
}

MeshFacePermanentRenderer::~MeshFacePermanentRenderer()
{
    delete m_chunkForOutliers;
    delete m_attributesCloudProvider;
}

void MeshFacePermanentRenderer::addFace(const size_t& faceIndex,
                                        const size_t &globalV1Index,
                                        const size_t &globalV2Index,
                                        const size_t &globalV3Index,
                                        IChunk **chunk)
{
    QMutexLocker locker(m_mutex);

    m_vertexIndices[0] = globalV1Index;
    m_vertexIndices[1] = globalV2Index;
    m_vertexIndices[2] = globalV3Index;

    addObjectVertexes(faceIndex,
                      m_vertexIndices,
                      chunk);
}

FaceCloudAttributesProvider* MeshFacePermanentRenderer::getFaceCloudAttributesProvider() const
{
    return m_attributesCloudProvider;
}

void MeshFacePermanentRenderer::draw(DrawInfo &info)
{
    /*GLboolean lightingEnabled;
    glGetBooleanv(GL_FLAT, &lightingEnabled);

    if(!lightingEnabled) {*/
        //glEnable(GL_LIGHTING);
        glShadeModel(GL_FLAT);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        SuperClass::draw(info);
        glShadeModel(GL_SMOOTH);

        //glDisable(GL_LIGHTING);
    //}
}

void MeshFacePermanentRenderer::fastDraw(DrawInfo &info)
{
    /*GLboolean lightingEnabled;
    glGetBooleanv(GL_FLAT, &lightingEnabled);

    if(!lightingEnabled) {*/
        //glEnable(GL_LIGHTING);
        glShadeModel(GL_FLAT);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        SuperClass::fastDraw(info);
        glShadeModel(GL_SMOOTH);

        //glDisable(GL_LIGHTING);
    //}
}

MeshFacePermanentRenderer::SuperClass::OtherChunkCollection MeshFacePermanentRenderer::getOthersChunk() const
{
    OtherChunkCollection c = SuperClass::getOthersChunk();

    if(m_chunkForOutliers != nullptr)
        c.push_back(m_chunkForOutliers);

    return c;
}

MeshObjectNM::OutlierChunkType& MeshFacePermanentRenderer::createOrGetChunkForOutliers()
{
    if(m_chunkForOutliers == nullptr) {
        m_chunkForOutliers = new MeshObjectNM::OutlierChunkType(Scene::TriangleGlobalCloud,
                                                                GL_TRIANGLES,
                                                                3,
                                                                *AMKglEA->getGlobalPointCloudManager(),
                                                                *AMKglEA->getGlobalFaceCloudManager());
        m_chunkForOutliers->setContextAccessor(getContextAccessor());
        m_chunkForOutliers->setAttributesAccessor(getAttributesAccessor());
        m_chunkForOutliers->setDrawModeToUse(getDrawModeToUse());
        m_chunkForOutliers->init();
    }

    return (*m_chunkForOutliers);
}
