#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "chunk.h"

template<typename RendererContextT>
Chunk<RendererContextT>::Chunk(const uint& uniqueKey, Scene::ObjectType objectType, const Eigen::Vector3d &offset) : IChunk(uniqueKey, objectType, offset)
{
    m_currentContext = nullptr;
    m_contextAccessor = nullptr;
}

template<typename RendererContextT>
Chunk<RendererContextT>::~Chunk()
{
    typename IContextAccessor<RendererContextT>::ContextCollectionIterator itC(getContextAccessor()->getContexts());

    while(itC.hasNext())
        itC.next().value()->destroyAll(this);
}

template<typename RendererContextT>
void Chunk<RendererContextT>::setContextAccessor(const IContextAccessor<RendererContextT>* accessor)
{
    m_contextAccessor = (IContextAccessor<RendererContextT>*)accessor;
}

template<typename RendererContextT>
IContextAccessor<RendererContextT>* Chunk<RendererContextT>::getContextAccessor() const
{
    return m_contextAccessor;
}

template<typename RendererContextT>
void Chunk<RendererContextT>::setCurrentContext(const RendererContextT* context)
{
    if(m_currentContext == context)
        return;

    m_currentContext = (RendererContextT*)context;
}

template<typename RendererContextT>
RendererContextT* Chunk<RendererContextT>::getCurrentContext() const
{
    return m_currentContext;
}

template<typename RendererContextT>
IGraphicsDocument* Chunk<RendererContextT>::getCurrentDocument() const
{
    if(m_currentContext == nullptr)
        return nullptr;

    return m_currentContext->getDocument();
}

template<typename RendererContextT>
void Chunk<RendererContextT>::destroyGLForCurrentContext()
{
    lock();

    destroyGL(m_currentContext);

    unlock();
}

template<typename RendererContextT>
void Chunk<RendererContextT>::destroyGL(const QOpenGLContext* context)
{
    lock();

    RendererContextT* rc = getContextAccessor()->getContexts().value(const_cast<QOpenGLContext*>(context), nullptr);

    destroyGL(rc);

    unlock();
}

template<typename RendererContextT>
void Chunk<RendererContextT>::destroyGL(RendererContextT* context)
{
    lock();

    IChunk::setUpdated(false);

    if(context == nullptr)
        return;

    setContextUpdated(context, false);
    context->destroyAll(this);

    unlock();
}

template<typename RendererContextT>
void Chunk<RendererContextT>::getSelectionColor(float sColor[])
{
    QColor sCol = Qt::red;

    if(getCurrentDocument() != nullptr)
        sCol = getCurrentDocument()->getSelectionColor();

    sColor[0] = sCol.redF();
    sColor[1] = sCol.greenF();
    sColor[2] = sCol.blueF();
    sColor[3] = sCol.alphaF();
}

template<typename RendererContextT>
void Chunk<RendererContextT>::setUpdated(bool status)
{
    if(mustChangeUpdateStatus(status)) {

        IChunk::setUpdated(status);

        this->lock();

        if(m_currentContext != nullptr)
            setContextUpdated(m_currentContext, status);

        if(status == false) {
            typename IContextAccessor<RendererContextT>::ContextCollectionIterator it(getContextAccessor()->getContexts());

            while(it.hasNext())
                setContextUpdated(it.next().value(), status);
        }

        this->unlock();
    }
}

template<typename RendererContextT>
void Chunk<RendererContextT>::setContextUpdated(RendererContextT* context, bool status)
{
    Q_UNUSED(status)

    ChunkCustomUpdateValues* cuv = context->createOrGetChunkCustomUpdateValues(this);
    cuv->setUserValue("DrawModeToUse", (int)getDrawModeToUse());
}

template<typename RendererContextT>
bool Chunk<RendererContextT>::isUpdated() const
{
    if(!IChunk::isUpdated())
        return false;

    RendererContextT* context = getCurrentContext();

    if(context != nullptr)
    {
        ChunkCustomUpdateValues* cuv = context->createOrGetChunkCustomUpdateValues(this);

        if(cuv->getUserValue("DrawModeToUse", (int)getDrawModeToUse()).toInt() != ((int)getDrawModeToUse()))
            return false;
    }

    return true;
}

#endif // CHUNK_HPP
