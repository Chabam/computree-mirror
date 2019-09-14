#include "icontextaccessor.h"

template<typename RendererContextT>
IContextAccessor<RendererContextT>::~IContextAccessor()
{
    qDeleteAll(m_contexts.begin(), m_contexts.end());
}

template<typename RendererContextT>
void IContextAccessor<RendererContextT>::addedTo(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
{
    if(m_contexts.contains(const_cast<QOpenGLContext*>(newContext)))
        return;

    RendererContextT *c = new RendererContextT(doc, newContext);
    m_contexts.insert(const_cast<QOpenGLContext*>(newContext), c);
}

template<typename RendererContextT>
void IContextAccessor<RendererContextT>::removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext)
{
    Q_UNUSED(doc)
    Q_UNUSED(newContext)

    if(!m_contexts.contains(const_cast<QOpenGLContext*>(newContext)))
        return;

    AMKglEA->contextAboutToBeDeleted(newContext);

    RendererContextT *c = m_contexts.take(const_cast<QOpenGLContext*>(newContext));
    delete c;
}

template<typename RendererContextT>
const typename IContextAccessor<RendererContextT>::ContextCollection& IContextAccessor<RendererContextT>::getContexts() const
{
    return m_contexts;
}

template<typename RendererContextT>
RendererContextT* IContextAccessor<RendererContextT>::getCurrentContext() const
{
    return getContexts().value(QOpenGLContext::currentContext(), nullptr);
}

template<typename RendererContextT>
IGraphicsDocument* IContextAccessor<RendererContextT>::getCurrentDocument() const
{
    if(getCurrentContext() == nullptr)
        return nullptr;

    return getCurrentContext()->getDocument();
}
