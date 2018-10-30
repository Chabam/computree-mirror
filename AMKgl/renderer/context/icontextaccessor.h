#ifndef ICONTEXTACCESSOR_H
#define ICONTEXTACCESSOR_H

#include <QHash>
#include <QOpenGLContext>

#include "amkglapp.h"

class IGraphicsDocument;

class DummyContext {
public:
    DummyContext(const IGraphicsDocument* doc, const QOpenGLContext* newContext) : m_doc(const_cast<IGraphicsDocument*>(doc)), m_newContext(const_cast<QOpenGLContext*>(newContext)) { }

    IGraphicsDocument* getDocument() const { return m_doc; }
    QOpenGLContext* getOpenglContext() const { return m_newContext; }

private:
    IGraphicsDocument*   m_doc;
    QOpenGLContext*     m_newContext;
};

template<typename RendererContextT>
class IContextAccessor {
public:

    typedef QHash<QOpenGLContext*,RendererContextT*> ContextCollection;
    typedef QHashIterator<QOpenGLContext*,RendererContextT*> ContextCollectionIterator;

    virtual ~IContextAccessor();

    /**
     * @brief Call this method when you add this renderer to
     *        your graphics view and pass it your opengl context
     */
    virtual void addedTo(const IGraphicsDocument *doc, const QOpenGLContext* newContext);

    /**
     * @brief Call this method when you remove this renderer from
     *        your graphics view and pass it your opengl context
     */
    virtual void removedFrom(const IGraphicsDocument* doc, const QOpenGLContext* newContext);

    /**
     * @brief Returns all contexts
     */
    typename const IContextAccessor<RendererContextT>::ContextCollection& getContexts() const;

    /**
     * @brief Returns the current context
     */
    RendererContextT* getCurrentContext() const;

    /**
     * @brief Returns the current document
     */
    IGraphicsDocument* getCurrentDocument() const;

private:
    QHash<QOpenGLContext*,RendererContextT*>    m_contexts;
};

#include "icontextaccessor.hpp"

#endif // ICONTEXTACCESSOR_H
