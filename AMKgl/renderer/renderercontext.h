#ifndef RENDERERCONTEXT_H
#define RENDERERCONTEXT_H

#include <QOpenGLContext>

#include "amkglapp.h"
#include "renderer/tools/texturemanager.h"
#include "renderer/tools/bufferobjectmanager.h"
#include "renderer/tools/displaylistmanager.h"
#include "renderer/chunkcustomupdatevalues.h"
#include "interfaces/igraphicsdocument.h"

/**
 * @brief Keep all elements necessary to draw elements in a
 *        specific opengl context
 */
template<typename Shaders>
class RendererContext
{
public:
    typedef Shaders                                         ShadersType;
    typedef typename Shaders::ArrayObjectManagerType        ArrayObjectManagerType;

    RendererContext(const IGraphicsDocument* doc,
                    const QOpenGLContext* newOpenglContext);
    ~RendererContext();

    /**
     * @brief Make this context current
     */
    bool makeCurrent();

    /**
     * @brief Return shaders to use for this context
     */
    Shaders& getShaders() const;

    /**
     * @brief Destroy shaders
     */
    void destroyGLShaders();

    /**
     * @brief Create if not exist or return directly an object that you can use to add some custom values to know
     *        if you are updated or not
     */
    ChunkCustomUpdateValues* createOrGetChunkCustomUpdateValues(const void* object);

    /**
     * @brief Destroy the object used to store some custom values in a context
     */
    void destroyChunkCustomUpdateValues(void* object);

    /**
     * @brief Destroy the buffer (if exist) of the object passed in parameter and delete it
     *        from memory
     */
    void destroyGLBuffers(void* object);

    /**
     * @brief Generate a new display list for and return it or just return it if it was already created. You can create
     *        as many dislay list as you want, you must just have a different unique index for each list.
     * @param object : a pointer to your class
     * @param uniqueIndex : a unique index that you must use to retreive the generated list
     */
    GLuint createOrGetDisplayList(void* object, const size_t& uniqueIndex);

    /**
     * @brief This will execute the display list with specified unique index with "glCallList". If this list doesn't
     *        exist it will automatically be created with call to method "id = createOrGetDisplayList()" and
     *        glNewList(id, GL_COMPILE_AND_EXECUTE). After that if you call this method il will use "glCallList". If
     *        you want to destroy the list you must call "destroyGL".
     *
     *        example :
     *
     *        if(!executeOrCreateAndCompileAndExecuteDisplayList(myUniqueIndex)) {
     *              drawMyObject();
     *              endDisplayList(myUniqueIndex);
     *        }
     * @warning Don't forget to call "endDisplayList" if this method returns false !
     * @param object : a pointer to your class
     * @param uniqueIndex : the unique index used in method "createOrGetDisplayList" or if you don't call this method use a unique index.
     * @return false if you must call your draw method, true if not
     */
    bool executeOrCreateAndCompileAndExecuteDisplayList(void* object, const size_t& uniqueIndex);

    /**
     * @brief Call this method after you draw your object if "executeOrCreateAndCompileAndExecuteDisplayList" return false. Otherwise
     *        you don't have to call this method.
     * @param object : a pointer to your class
     * @param uniqueIndex : the unique index used in method "createOrGetDisplayList"
     */
    void endDisplayList(void* object, const size_t& uniqueIndex);

    /**
     * @brief Call this method to destroy all display list of a specified object
     * @param object : a pointer to your class
     */
    void destroyAllDisplayList(void* object);

    /**
     * @brief Destroy all elements that was created for this object
     */
    void destroyAll(void* object);

    /**
     * @brief Destroy all (call it when you want to delete this context)
     */
    void destroyGL();

    /**
     * @brief Returns the document
     */
    IGraphicsDocument* getDocument() const;

    /**
     * @brief Returns the opengl context
     */
    QOpenGLContext* getOpenglContext() const;

    /**
     * @brief Returns the textures manager
     */
    TextureManager& getTexturesManager() const;

    /**
     * @brief Returns the buffer object manager
     */
    BufferObjectManager& getBufferObjectManager() const;

    /**
     * @brief Returns the VAO manager
     */
    ArrayObjectManagerType& createOrGetArrayObjectManager(void* object);

private:
    typedef QHash<void*, DisplayListManager*>           DisplayListCollection;
    typedef QHashIterator<void*, DisplayListManager*>   DisplayListCollectionIterator;

    IGraphicsDocument*                          m_document;
    QOpenGLContext*                             m_newContext;
    Shaders                                     m_shaders;
    QHash<void*, ChunkCustomUpdateValues*>      m_customValues;
    TextureManager                              m_textureManager;
    DisplayListCollection                       m_displayListsManager;
    QHash<void*, ArrayObjectManagerType*>       m_arrayObjectManagers;
};

#include "renderercontext.hpp"

#endif // RENDERERCONTEXT_H
