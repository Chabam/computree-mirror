#ifndef GENERICCHUNKMANAGER_H
#define GENERICCHUNKMANAGER_H

#include <QMutex>

#include "genericchunkmanagerdefaultsuperclass.h"
#include "traits/genericchunkmanagerdefaultinitchunktraits.h"

#include "renderer/drawmode.h"
#include "renderer/context/icontextaccessor.h"
#include "visitor/iobjectsvisitor.h"
#include "visitor/applycustomfunctiontoobjectvisitor.h"
#include "renderer/chunk.h"

#include "drawinfo.h"

/**
 * @brief Class tools that made generic process of typical chunk manager
 */
template<typename ChunkT,
         typename ChunkCollectionT,
         class InheritFromT = GenericChunkManagerDefaultSuperClass<typename ChunkT::RendererContextType> >
class GenericChunkManager : public InheritFromT
{
private:
    typedef GenericChunkManager<ChunkT, ChunkCollectionT, InheritFromT > ThisClass;

public:
    typedef ChunkCollectionT                        ChunkCollection;
    typedef typename ChunkT::RendererContextType    RendererContext;
    typedef RendererContext                         RendererContextType;
    typedef IContextAccessor<RendererContext>       ContextAccessor;
    typedef IAttributesAccessor                     AttributesAccessor;
    typedef ChunkT                                  ChunkType;
    typedef QList<Chunk<RendererContextType>*>      OtherChunkCollection;

    GenericChunkManager(GLenum glMode,
                        const quint8& nVertexPerObject) : InheritFromT()
    {
        m_mutex = new QMutex(QMutex::Recursive);
        m_drawModeToUse = DM_VAO;
        m_glMode = glMode;
        m_nVertexPerObject = nVertexPerObject;
        m_contextAccessor = NULL;
    }

    virtual ~GenericChunkManager()
    {
        this->deleteChunksFromMemory();

        delete m_mutex;
    }

    /**
     * @brief Call it after you have set the attribute accessor and all necessary elements
     */
    virtual void init() {}

    /**
     * @brief Set the context accessor
     */
    virtual void setContextAccessor(const ContextAccessor* ca)
    {
        QMutexLocker locker(m_mutex);

        if(m_contextAccessor != ca) {
            m_contextAccessor = (ContextAccessor*)ca;

            typename ChunkCollection::iterator it = m_chunks.begin();
            typename ChunkCollection::iterator end = m_chunks.end();

            while(it != end) {
                (*it)->setContextAccessor(ca);
                ++it;
            }

            OtherChunkCollection others = this->getOthersChunk();

            typename OtherChunkCollection::iterator itO = others.begin();
            typename OtherChunkCollection::iterator endO = others.end();

            while(itO != endO) {
                (*itO)->setContextAccessor(ca);
                ++itO;
            }
        }
    }

    /**
     * @brief Returns the context accessor
     */
    ContextAccessor* getContextAccessor() const
    {
        return m_contextAccessor;
    }

    /**
     * @brief Lock to be thread safe
     */
    void lock()
    {
        m_mutex->lock();
    }

    /**
     * @brief Unlock the previous mutex
     */
    void unlock()
    {
        m_mutex->unlock();
    }

    /**
     * @brief Returns the number of vertex per object
     */
    quint8 getNVertexPerObject() const
    {
        return m_nVertexPerObject;
    }

    /**
     * @brief Returns the opengl mode to use
     */
    GLenum getGlMode() const
    {
        return m_glMode;
    }

    /**
     * @brief Call this method to update all chunk in all context. It will create all
     *        necessary VBO, IBO, etc... to draw it
     */
    virtual void updateInAllContext()
    {
        QMutexLocker locker(m_mutex);

        typename ChunkCollection::iterator begin = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator beginO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        ContextAccessor::ContextCollectionIterator it(m_contextAccessor->getContexts());

        while(it.hasNext()) {
            it.next();

            RendererContext* context = it.value();

            context->makeCurrent();
            context->getShaders().init(context->getOpenglContext());

            this->mustUpdateInfoBO(context);
            this->mustUpdateColorBO(context);
            this->mustUpdateNormalBO(context);

            typename ChunkCollection::iterator it = begin;

            while(it != end) {
                (*it)->setCurrentContext(context);
                (*it)->updateInCurrentContext();
                (*it)->setCurrentContext(NULL);
                ++it;
            }

            typename OtherChunkCollection::iterator itO = beginO;

            while(itO != endO) {
                (*itO)->setCurrentContext(context);
                (*itO)->updateInCurrentContext();
                (*itO)->setCurrentContext(NULL);
                ++itO;
            }
        }
    }

    /**
     * @brief Call this method to update all chunk in current context. It will create all
     *        necessary VBO, IBO, etc... to draw it
     */
    virtual void updateInCurrentContext()
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = this->getCurrentContext();

        this->mustUpdateInfoBO(c);
        this->mustUpdateColorBO(c);
        this->mustUpdateNormalBO(c);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            (*it)->updateInCurrentContext();
            (*it)->setCurrentContext(NULL);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            (*itO)->updateInCurrentContext();
            (*itO)->setCurrentContext(NULL);
            ++itO;
        }
    }

    /**
     * @brief Call this method to update infos of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateInfoInAllContext()
    {
        QMutexLocker locker(m_mutex);

        typename ChunkCollection::iterator begin = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator beginO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        ContextAccessor::ContextCollectionIterator it(m_contextAccessor->getContexts());

        while(it.hasNext()) {
            it.next();

            RendererContext* context= it.value();

            context->makeCurrent();
            context->getShaders().init(context->getOpenglContext());

            this->mustUpdateInfoBO(context);

            typename ChunkCollection::iterator it = begin;

            while(it != end) {
                (*it)->setCurrentContext(context);
                (*it)->updateInfoInCurrentContext();
                (*it)->setCurrentContext(NULL);
                ++it;
            }

            typename OtherChunkCollection::iterator itO = beginO;

            while(itO != endO) {
                (*itO)->setCurrentContext(context);
                (*itO)->updateInfoInCurrentContext();
                (*itO)->setCurrentContext(NULL);
                ++itO;
            }
        }
    }

    /**
     * @brief Call this method to update infos of elements in current context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateInfoInCurrentContext()
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = this->getCurrentContext();

        this->mustUpdateInfoBO(c);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            (*it)->updateInfoInCurrentContext();
            (*it)->setCurrentContext(NULL);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            (*itO)->updateInfoInCurrentContext();
            (*itO)->setCurrentContext(NULL);
            ++itO;
        }
    }

    /**
     * @brief Call this method to update colors of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateColorInAllContext()
    {
        QMutexLocker locker(m_mutex);

        typename ChunkCollection::iterator begin = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator beginO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        ContextAccessor::ContextCollectionIterator it(m_contextAccessor->getContexts());

        while(it.hasNext()) {
            it.next();

            RendererContext* context= it.value();

            context->makeCurrent();
            context->getShaders().init(context->getOpenglContext());

            this->mustUpdateColorBO(context);

            typename ChunkCollection::iterator it = begin;

            while(it != end) {
                (*it)->setCurrentContext(context);
                (*it)->updateColorInCurrentContext();
                (*it)->setCurrentContext(NULL);
                ++it;
            }

            typename OtherChunkCollection::iterator itO = beginO;

            while(itO != endO) {
                (*itO)->setCurrentContext(context);
                (*itO)->updateColorInCurrentContext();
                (*itO)->setCurrentContext(NULL);
                ++itO;
            }
        }
    }

    /**
     * @brief Call this method to update colors of elements in current context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateColorInCurrentContext()
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = this->getCurrentContext();

        this->mustUpdateColorBO(c);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            (*it)->updateColorInCurrentContext();
            (*it)->setCurrentContext(NULL);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            (*itO)->updateColorInCurrentContext();
            (*itO)->setCurrentContext(NULL);
            ++itO;
        }
    }

    /**
     * @brief Call this method to update normals of elements in all context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateNormalInAllContext()
    {
        QMutexLocker locker(m_mutex);

        typename ChunkCollection::iterator begin = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator beginO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        ContextAccessor::ContextCollectionIterator it(m_contextAccessor->getContexts());

        while(it.hasNext()) {
            it.next();

            RendererContext* context = it.value();

            context->makeCurrent();
            context->getShaders().init(context->getOpenglContext());

            this->mustUpdateNormalBO(context);

            typename ChunkCollection::iterator it = begin;

            while(it != end) {
                (*it)->setCurrentContext(context);
                (*it)->updateNormalInCurrentContext();
                (*it)->setCurrentContext(NULL);
                ++it;
            }

            typename OtherChunkCollection::iterator itO = beginO;

            while(itO != endO) {
                (*itO)->setCurrentContext(context);
                (*itO)->updateNormalInCurrentContext();
                (*itO)->setCurrentContext(NULL);
                ++itO;
            }
        }
    }

    /**
     * @brief Call this method to update normals of elements in current context. It will update
     *        necessary VBO, etc... to draw it
     */
    virtual void updateNormalInCurrentContext()
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = this->getCurrentContext();

        this->mustUpdateNormalBO(c);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            (*it)->updateNormalInCurrentContext();
            (*it)->setCurrentContext(NULL);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            (*itO)->updateNormalInCurrentContext();
            (*itO)->setCurrentContext(NULL);
            ++itO;
        }
    }

    /**
     * @brief Call this method to update only VAO of elements in all context.
     */
    virtual void updateOnlyVAOInAllContext()
    {
        QMutexLocker locker(m_mutex);

        typename ChunkCollection::iterator begin = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator beginO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        ContextAccessor::ContextCollectionIterator it(m_contextAccessor->getContexts());

        while(it.hasNext()) {
            it.next();

            RendererContext* context= it.value();

            context->makeCurrent();
            context->getShaders().init(context->getOpenglContext());

            this->mustUpdateInfoBO(context);

            typename ChunkCollection::iterator it = begin;

            while(it != end) {
                (*it)->setCurrentContext(context);
                (*it)->updateOnlyVAOInCurrentContext();
                (*it)->setCurrentContext(NULL);
                ++it;
            }

            typename OtherChunkCollection::iterator itO = beginO;

            while(itO != endO) {
                (*itO)->setCurrentContext(context);
                (*itO)->updateOnlyVAOInCurrentContext();
                (*itO)->setCurrentContext(NULL);
                ++itO;
            }
        }
    }

    /**
     * @brief Call this method to update only VAO of elements in current context.
     */
    virtual void updateOnlyVAOInCurrentContext()
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = this->getCurrentContext();

        this->mustUpdateNormalBO(c);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            (*it)->updateOnlyVAOInCurrentContext();
            (*it)->setCurrentContext(NULL);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            (*itO)->updateOnlyVAOInCurrentContext();
            (*itO)->setCurrentContext(NULL);
            ++itO;
        }
    }

    /**
     * @brief Define the draw mode to use. If you set it to VBO or VAO and the graphics card
     *        cannot do it, it will be changed to RAW after update.
     */
    virtual void setDrawModeToUse(DrawMode mode)
    {
        QMutexLocker locker(m_mutex);

        if(m_drawModeToUse != mode) {

            m_drawModeToUse = mode;

            typename ChunkCollection::iterator it = m_chunks.begin();
            typename ChunkCollection::iterator end = m_chunks.end();

            while(it != end) {
                (*it)->setDrawModeToUse(mode);
                ++it;
            }

            OtherChunkCollection others = this->getOthersChunk();

            typename OtherChunkCollection::iterator itO = others.begin();
            typename OtherChunkCollection::iterator endO = others.end();

            while(itO != endO) {
                (*itO)->setDrawModeToUse(mode);
                ++itO;
            }
        }
    }

    /**
     * @brief Returns the draw mode to use
     */
    DrawMode getDrawModeToUse() const
    {
        return m_drawModeToUse;
    }

    /**
     * @brief Call it when you want to destroy all opengl vertex (when you close your window)
     */
    virtual void destroyGL(const QOpenGLContext* context)
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = m_contextAccessor->getContexts().value(const_cast<QOpenGLContext*>(context), NULL);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->destroyGL(context);
            ++it;
        }

        deleteChunksThatMustBeInGuiThread();

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->destroyGL(context);
            ++itO;
        }

        if(c != NULL)
            c->destroyGL();
    }

    /**
     * @brief Remove all object
     */
    virtual void clearObjects()
    {
        QMutexLocker locker(m_mutex);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->clearObjects();
            ++it;
        }

        deleteChunksThatMustBeInGuiThread();

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->clearObjects();
            ++itO;
        }
    }

    /**
     * @brief Call to visit all objects
     * @param visitor : is an object that will receive all objects
     */
    void visitObjects(IObjectsVisitor& visitor)
    {
        QMutexLocker locker(m_mutex);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {

            if(!visitor.mustContinueVisit())
                return;

            (*it)->visitObjects(visitor);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {

            if(!visitor.mustContinueVisit())
                return;

            (*itO)->visitObjects(visitor);
            ++itO;
        }
    }

    /**
     * @brief Returns the total number of object that was added to this renderer
     */
    size_t countObjects() const
    {
        QMutexLocker locker(m_mutex);

        size_t count = 0;

        ApplyCustomFunctionToObjectVisitor visitor;
        visitor.setVisitFunction([&count](const IChunk* chunk) {
            count += chunk->countObjects();
        });

        const_cast<ThisClass*>(this)->visitObjects(visitor);

        return count;
    }

    /**
     * @brief Delete all chunks from memory
     */
    virtual void deleteChunksFromMemory()
    {
        QMutexLocker locker(m_mutex);

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            chunkAboutToBeDeleted(*it);
            delete (*it);
            ++it;
        }

        m_chunks.clear();

        deleteChunksThatMustBeInGuiThread();
    }

    /**
     * @brief Draw all vertex normally (all vertex are drawn with a good precision)
     */
    virtual void draw(DrawInfo& info)
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = this->getCurrentContext();

        deleteChunksThatMustBeInGuiThread();

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            (*it)->draw(info);
            (*it)->setCurrentContext(NULL);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            (*itO)->draw(info);
            (*itO)->setCurrentContext(NULL);
            ++itO;
        }
    }

    /**
     * @brief Draw all vertex fast (all vertex are drawn with a minimum number of vertex)
     */
    virtual void fastDraw(DrawInfo& info)
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = this->getCurrentContext();

        deleteChunksThatMustBeInGuiThread();

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            (*it)->fastDraw(info);
            (*it)->setCurrentContext(NULL);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            (*itO)->fastDraw(info);
            (*itO)->setCurrentContext(NULL);
            ++itO;
        }
    }

    /**
     * @brief Draw text or other vertex
     */
    virtual void postDraw(DrawInfo& info)
    {
        QMutexLocker locker(m_mutex);

        RendererContext* c = this->getCurrentContext();

        deleteChunksThatMustBeInGuiThread();

        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            (*it)->postDraw(info);
            (*it)->setCurrentContext(NULL);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            (*itO)->postDraw(info);
            (*itO)->setCurrentContext(NULL);
            ++itO;
        }
    }


protected:

    /**
     * @brief Mutex to be thread safe
     */
    QMutex*                                 m_mutex;

    /**
     * @brief Collection of chunks
     */
    ChunkCollection                         m_chunks;

    /**
     * @brief Collection of chunks
     */
    ChunkCollection                         m_chunksToDeleteInGuiThread;

    /**
     * @brief Current draw mode to use
     */
    DrawMode                                m_drawModeToUse;

    /**
     * @brief GLmode used to draw objects
     */
    GLenum                                  m_glMode;

    /**
     * @brief Number of vertex per object
     */
    quint8                                  m_nVertexPerObject;

    /**
     * @brief Context accessor
     */
    ContextAccessor*                        m_contextAccessor;

    /**
     * @brief Called in update method and delete method to delete chunks in collection "m_chunksToDeleteInGuiThread"
     */
    void deleteChunksThatMustBeInGuiThread() {
        typename ChunkCollection::iterator it = m_chunksToDeleteInGuiThread.begin();
        typename ChunkCollection::iterator end = m_chunksToDeleteInGuiThread.end();

        while(it != end) {
            delete (*it);
            ++it;
        }

        m_chunksToDeleteInGuiThread.clear();
    }

    /**
     * @brief Called when the attributes accessor changed
     */
    virtual void attributesAccessorChanged(const AttributesAccessor* aa)
    {
        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setAttributesAccessor(aa);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setAttributesAccessor(aa);
            ++itO;
        }
    }

    /**
     * @brief Called when the current context changed
     */
    virtual void currentContextChanged(const RendererContext* c)
    {
        typename ChunkCollection::iterator it = m_chunks.begin();
        typename ChunkCollection::iterator end = m_chunks.end();

        while(it != end) {
            (*it)->setCurrentContext(c);
            ++it;
        }

        OtherChunkCollection others = this->getOthersChunk();

        typename OtherChunkCollection::iterator itO = others.begin();
        typename OtherChunkCollection::iterator endO = others.end();

        while(itO != endO) {
            (*itO)->setCurrentContext(c);
            ++itO;
        }
    }

    /**
     * @brief Add the chunk and init it
     */
    virtual void initAndAddChunk(ChunkT* chunk)
    {
        initChunk(chunk);

        addChunkToCollection(chunk);

        chunkAdded(chunk);
    }

    /**
     * @brief Returns others chunk that we must used to draw it or visit it
     */
    virtual OtherChunkCollection getOthersChunk() const { return OtherChunkCollection(); }

    /**
     * @brief Init the chunk (you can redefine this method if you want to init it differently)
     */
    virtual void initChunk(ChunkT* chunk) = 0;

    /**
     * @brief Called when a chunk must be added to the collection
     */
    virtual void addChunkToCollection(ChunkT* chunk) = 0;

    /**
     * @brief Called when a chunk is created and initialized
     */
    virtual void chunkAdded(ChunkT* chunk)
    {
        Q_UNUSED(chunk)
    }

    /**
     * @brief Called when a chunk is about to be deleted
     */
    virtual void chunkAboutToBeDeleted(ChunkT* chunk)
    {
        Q_UNUSED(chunk)
    }

    /**
     * @brief Called when the current context is accessed and you must update
     *        buffer object. Only the developper know in which stage it must be called.
     */
    virtual void mustUpdateInfoBO(RendererContext* currentContext)
    {
        Q_UNUSED(currentContext)
        // do nothing because you must redefine it in your class that inherit from this when you want to update the buffer object
    }

    /**
     * @brief Called when the current context is accessed and you must update
     *        buffer object. Only the developper know in which stage it must be called.
     */
    virtual void mustUpdateColorBO(RendererContext* currentContext)
    {
        Q_UNUSED(currentContext)
        // do nothing because you must redefine it in your class that inherit from this when you want to update the buffer object
    }

    /**
     * @brief Called when the current context is accessed and you must update
     *        buffer object. Only the developper know in which stage it must be called.
     */
    virtual void mustUpdateNormalBO(RendererContext* currentContext)
    {
        Q_UNUSED(currentContext)
        // do nothing because you must redefine it in your class that inherit from this when you want to update the buffer object
    }
};

#endif // GENERICCLOUDCHUNKMANAGER_H
