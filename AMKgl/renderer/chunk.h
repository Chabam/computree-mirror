#ifndef CHUNK_H
#define CHUNK_H

#include "ichunk.h"
#include "renderer/context/icontextaccessor.h"
#include "chunkcustomupdatevalues.h"

#include <QtOpenGL/QtOpenGL>

class IGraphicsDocument;

template<typename RendererContextT>
class Chunk : public IChunk
{
public:
    typedef RendererContextT RendererContextType;

    Chunk(const uint& uniqueKey, Scene::ObjectType objectType, const Eigen::Vector3d& offset);
    virtual ~Chunk();

    /**
     * @brief Set the context accessor
     */
    void setContextAccessor(const IContextAccessor<RendererContextT>* accessor);

    /**
     * @brief Returns the context accessor
     */
    IContextAccessor<RendererContextT>* getContextAccessor() const;

    /**
     * @brief Set the current context to use.
     */
    void setCurrentContext(const RendererContextT* context);

    /**
     * @brief Returns the current context used.
     */
    RendererContextT* getCurrentContext() const;

    /**
     * @brief Returns the current document (obtained by the current context)
     */
    IGraphicsDocument* getCurrentDocument() const;

    /**
     * @brief Returns true if it was up to date
     */
    virtual bool isUpdated() const;

    /**
     * @brief Destroy all elements to draw in the current context. After call this function nothing will be draw.
     */
    void destroyGLForCurrentContext();

    /**
     * @brief Destroy all elements to draw in the specified context. After call this function nothing will be draw.
     */
    void destroyGL(const QOpenGLContext* context);

    /**
     * @brief Destroy all elements to draw in the specified context. After call this function nothing will be draw.
     */
    void destroyGL(RendererContextT* context);

private:

    /**
     * @brief Current context that contains all opengl elements (shaders, buffer object, vao, etc...)
     */
    RendererContextT*                       m_currentContext;

    /**
     * @brief Get access to all context
     */
    IContextAccessor<RendererContextT>*     m_contextAccessor;

protected:
    /**
     * @brief Get the selection color to use (get from document)
     */
    void getSelectionColor(float sColor[]);

    /**
     * @brief Modify the boolean that inform if we can draw or not
     * @param status : the new status
     */
    virtual void setUpdated(bool status);

    /**
     * @brief Modify the update status of the context
     */
    virtual void setContextUpdated(RendererContextT* context, bool status);

};

#include "chunk.hpp"

#endif // CHUNK_H
