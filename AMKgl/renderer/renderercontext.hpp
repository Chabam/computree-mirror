#ifndef RENDERERCONTEXT_HPP
#define RENDERERCONTEXT_HPP

#include "renderercontext.h"

template<typename Shaders>
RendererContext<Shaders>::RendererContext(const IGraphicsDocument *doc,
                                          const QOpenGLContext *newOpenglContext)
{
    Q_ASSERT(newOpenglContext != nullptr);

    m_document = const_cast<IGraphicsDocument*>(doc);
    m_newContext = const_cast<QOpenGLContext*>(newOpenglContext);
}

template<typename Shaders>
RendererContext<Shaders>::~RendererContext()
{
    qDeleteAll(m_customValues.begin(), m_customValues.end());
    qDeleteAll(m_displayListsManager.begin(), m_displayListsManager.end());
    qDeleteAll(m_arrayObjectManagers.begin(), m_arrayObjectManagers.end());
}

template<typename Shaders>
bool RendererContext<Shaders>::makeCurrent()
{
    if(getDocument() == nullptr)
        return false;

    QOpenGLWidget* w = getDocument()->getOpengWidgetWithContext(m_newContext);

    if((w == nullptr) || !w->isValid())
        return false;

    w->makeCurrent();

    return true;
}

template<typename Shaders>
Shaders& RendererContext<Shaders>::getShaders() const
{
    return const_cast<Shaders&>(m_shaders);
}

template<typename Shaders>
void RendererContext<Shaders>::destroyGLShaders()
{
    m_shaders.destroyGL();
}

template<typename Shaders>
ChunkCustomUpdateValues* RendererContext<Shaders>::createOrGetChunkCustomUpdateValues(const void* object)
{
    ChunkCustomUpdateValues* b = m_customValues.value((void*)object, nullptr);

    if(b == nullptr) {
        b = new ChunkCustomUpdateValues();
        m_customValues.insert((void*)object, b);
    }

    return b;
}

template<typename Shaders>
void RendererContext<Shaders>::destroyChunkCustomUpdateValues(void* object)
{
    ChunkCustomUpdateValues* b = m_customValues.value(object, nullptr);

    if(b != nullptr) {
        m_customValues.remove(object);
        delete b;
    }
}

template<typename Shaders>
void RendererContext<Shaders>::destroyGLBuffers(void *object)
{
    ArrayObjectManagerType* aom = m_arrayObjectManagers.value(object, nullptr);

    if(aom != nullptr) {
        m_arrayObjectManagers.remove(object);
        delete aom;
    }

    getBufferObjectManager().deleteAllBOFor(object);
}

template<typename Shaders>
GLuint RendererContext<Shaders>::createOrGetDisplayList(void* object, const size_t& uniqueIndex)
{
    DisplayListManager* dlManager = m_displayListsManager.value(object, nullptr);

    if(dlManager == nullptr) {
        dlManager = new DisplayListManager();
        m_displayListsManager.insert(object, dlManager);
    }

    return dlManager->createOrGetDisplayList(uniqueIndex);
}

template<typename Shaders>
bool RendererContext<Shaders>::executeOrCreateAndCompileAndExecuteDisplayList(void* object, const size_t& uniqueIndex)
{
    DisplayListManager* dlManager = m_displayListsManager.value(object, nullptr);

    if(dlManager == nullptr) {
        dlManager = new DisplayListManager();
        m_displayListsManager.insert(object, dlManager);
    }

    return dlManager->executeOrCreateAndCompileAndExecuteDisplayList(uniqueIndex);
}

template<typename Shaders>
void RendererContext<Shaders>::endDisplayList(void* object, const size_t& uniqueIndex)
{
    DisplayListManager* dlManager = m_displayListsManager.value(object, nullptr);

    if(dlManager == nullptr)
        return;

    return dlManager->endDisplayList(uniqueIndex);
}

template<typename Shaders>
void RendererContext<Shaders>::destroyAllDisplayList(void* object)
{
    DisplayListManager* dlManager = m_displayListsManager.value(object, nullptr);

    if(dlManager == nullptr)
        return;

    delete dlManager;
    m_displayListsManager.remove(object);
}

template<typename Shaders>
void RendererContext<Shaders>::destroyAll(void* object)
{
    destroyGLBuffers(object);
    destroyAllDisplayList(object);
    destroyChunkCustomUpdateValues(object);
}

template<typename Shaders>
void RendererContext<Shaders>::destroyGL()
{
    destroyGLShaders();

    getBufferObjectManager().deleteAllBO();

    qDeleteAll(m_customValues.begin(), m_customValues.end());
    m_customValues.clear();

    qDeleteAll(m_arrayObjectManagers.begin(), m_arrayObjectManagers.end());
    m_arrayObjectManagers.clear();

    qDeleteAll(m_displayListsManager.begin(), m_displayListsManager.end());
    m_displayListsManager.clear();
}

template<typename Shaders>
IGraphicsDocument* RendererContext<Shaders>::getDocument() const
{
    return m_document;
}

template<typename Shaders>
QOpenGLContext* RendererContext<Shaders>::getOpenglContext() const
{
    return m_newContext;
}

template<typename Shaders>
TextureManager& RendererContext<Shaders>::getTexturesManager() const
{
    return const_cast<TextureManager&>(m_textureManager);
}

template<typename Shaders>
BufferObjectManager& RendererContext<Shaders>::getBufferObjectManager() const
{    
    return AMKglEA->createOrGetBufferObjectManager(m_newContext);
}

template<typename Shaders>
typename RendererContext<Shaders>::ArrayObjectManagerType& RendererContext<Shaders>::createOrGetArrayObjectManager(void* object)
{
    ArrayObjectManagerType* b = m_arrayObjectManagers.value((void*)object, nullptr);

    if(b == nullptr) {
        b = new ArrayObjectManagerType();
        m_arrayObjectManagers.insert((void*)object, b);
    }

    return *b;
}

#endif // RENDERERCONTEXT_HPP
