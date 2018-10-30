#include "pointcloudarrayobjectmanager.h"

#include "pointrenderercontext.h"

PointCloudArrayObjectManager::PointCloudArrayObjectManager()
{
}

PointCloudArrayObjectManager::~PointCloudArrayObjectManager()
{
    destroyGL();
}

bool PointCloudArrayObjectManager::createVertexAO(IChunk *chunk, PointRendererContext& currentContext)
{
    if(m_vertexAO.isCreated())
        return false;

    bool ok = m_vertexAO.create();

    if(ok)
        ok = updateVertexAO(chunk, currentContext);
    else
        m_vertexAO.destroy();

    return ok;
}

bool PointCloudArrayObjectManager::updateVertexAO(IChunk *chunk, PointRendererContext &currentContext)
{
    if(!m_vertexAO.isCreated())
        return false;

    BufferObjectManager& bufferObjects = currentContext.getBufferObjectManager();

    QOpenGLBuffer& vbo = bufferObjects.getVertexBO(chunk, POINTCLOUD_VERTEX_BO_UI);
    QOpenGLBuffer& infbo = bufferObjects.getInfosBO(chunk, POINTCLOUD_INFO_BO_UI);

    if(!vbo.isCreated() || !infbo.isCreated()) {
        m_vertexAO.destroy();
        return false;
    }

    PointRendererShaders& shaders = currentContext.getShaders();

    bool ok = shaders.bindPointShader();

    if(ok)
        m_vertexAO.bind();

    if((ok = shaders.bindAndSetVertexAttributeToPointsShader(vbo))) {

        ok = shaders.bindAndSetInfoAttributeToPointsShader(infbo, currentContext.getOpenglContext());

        QOpenGLBuffer& cbo = bufferObjects.getColorsBO(chunk, POINTCLOUD_COLOR_BO_UI);

        if(ok && cbo.isCreated())
            ok = shaders.bindAndSetColorAttributeToPointsShader(cbo);
        else
            shaders.setUseColorAttribute(false);
    }

    m_vertexAO.release();

    if(!ok)
        m_vertexAO.destroy();

    shaders.releasePointsShader();

    return ok;
}

bool PointCloudArrayObjectManager::canUseVertexAO() const
{
    return m_vertexAO.isCreated();
}

bool PointCloudArrayObjectManager::bindVertexAO()
{
    if(!m_vertexAO.isCreated())
        return false;

    m_vertexAO.bind();

    return true;
}

void PointCloudArrayObjectManager::releaseVertexAO()
{
    m_vertexAO.release();
}

bool PointCloudArrayObjectManager::createNormalsAO(IChunk *chunk, PointRendererContext &currentContext)
{
    if(m_normalsAO.isCreated())
        return false;

    bool ok = m_normalsAO.create();

    if(ok)
        ok = updateNormalsAO(chunk, currentContext);
    else
        m_normalsAO.destroy();

    return ok;
}

bool PointCloudArrayObjectManager::updateNormalsAO(IChunk *chunk, PointRendererContext &currentContext)
{
    if(!m_normalsAO.isCreated())
        return false;

    BufferObjectManager& bufferObjects = currentContext.getBufferObjectManager();

    QOpenGLBuffer& vbo = bufferObjects.getVertexBO(chunk, POINTCLOUD_VERTEX_BO_UI);
    QOpenGLBuffer& infbo = bufferObjects.getInfosBO(chunk, POINTCLOUD_INFO_BO_UI);
    QOpenGLBuffer& nbo = bufferObjects.getNormalsBO(chunk, POINTCLOUD_NORMAL_BO_UI);

    if(!vbo.isCreated() || !infbo.isCreated() || !nbo.isCreated()) {
        m_normalsAO.destroy();
        return false;
    }

    PointRendererShaders& shaders = currentContext.getShaders();

    bool ok = shaders.bindNormalsShader();

    if(ok) {
        m_normalsAO.bind();

        ok = shaders.bindAndSetVertexAttributeToNormalsShader(vbo)
              && shaders.bindAndSetNormalAttributeToNormalsShader(nbo)
              && shaders.bindAndSetInfoAttributeToNormalsShader(infbo, currentContext.getOpenglContext());
    }

    m_normalsAO.release();

    if(!ok)
        m_normalsAO.destroy();

    shaders.releaseNormalsShader();

    return ok;
}

bool PointCloudArrayObjectManager::canUseNormalsAO() const
{
    return m_normalsAO.isCreated();
}

bool PointCloudArrayObjectManager::bindNormalsAO()
{
    if(!m_normalsAO.isCreated())
        return false;

    m_normalsAO.bind();

    return true;
}

void PointCloudArrayObjectManager::releaseNormalsAO()
{
    m_normalsAO.release();
}

void PointCloudArrayObjectManager::destroyGL()
{
    m_vertexAO.destroy();
    m_normalsAO.destroy();
}
