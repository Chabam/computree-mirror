#include "genericobjectarrayobjectmanager.h"

#include "genericrenderercontext.h"
#include "genericrenderershaders.h"

GenericObjectArrayObjectManager::GenericObjectArrayObjectManager()
{
}

GenericObjectArrayObjectManager::~GenericObjectArrayObjectManager()
{
    destroyGL();
}

bool GenericObjectArrayObjectManager::createVertexAO(IChunk *chunk, GenericRendererContext& currentContext)
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

bool GenericObjectArrayObjectManager::updateVertexAO(IChunk *chunk, GenericRendererContext& currentContext)
{
    if(!m_vertexAO.isCreated())
        return false;

    BufferObjectManager& bufferObjects = currentContext.getBufferObjectManager();

    QOpenGLBuffer& vbo = bufferObjects.getVertexBO(chunk, GENERIC_OBJECT_VERTEX_BO_UI);
    QOpenGLBuffer& infbo = bufferObjects.getInfosBO(chunk, GENERIC_OBJECT_INFOS_BO_UI);

    if(!vbo.isCreated() || !infbo.isCreated()) {
        m_vertexAO.destroy();
        return false;
    }

    GenericRendererShaders& shaders = currentContext.getShaders();

    bool ok = shaders.bindShader();

    if(ok)
        m_vertexAO.bind();

    if((ok = shaders.bindAndSetVertexAttributeToShader(vbo))) {

        ok = shaders.bindAndSetInfoAttributeToShader(infbo, currentContext.getOpenglContext());

        QOpenGLBuffer& cbo = bufferObjects.getColorsBO(chunk, GENERIC_OBJECT_COLORS_BO_UI);

        if(ok && cbo.isCreated())
            ok = shaders.bindAndSetColorAttributeToShader(cbo);
        else
            shaders.setUseColorAttribute(false);
    }

    m_vertexAO.release();

    if(!ok)
        m_vertexAO.destroy();

    shaders.releaseShader();

    return ok;
}

bool GenericObjectArrayObjectManager::canUseVertexAO() const
{
    return m_vertexAO.isCreated();
}

bool GenericObjectArrayObjectManager::bindVertexAO()
{
    if(!m_vertexAO.isCreated())
        return false;

    m_vertexAO.bind();

    return true;
}

void GenericObjectArrayObjectManager::releaseVertexAO()
{
    m_vertexAO.release();
}

void GenericObjectArrayObjectManager::destroyGL()
{
    m_vertexAO.destroy();
}
