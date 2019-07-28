#ifndef INSTANCEDARRAYOBJECTMANAGER_H
#define INSTANCEDARRAYOBJECTMANAGER_H

#include <QtOpenGL>
#include <QOpenGLVertexArrayObject>

#include "renderer/tools/bufferobjectmanager.h"
#include "renderer/attributes/vertexattribute.h"

#define INSTANCED_SLOW_VERTEX_BO_UI 0
#define INSTANCED_FAST_VERTEX_BO_UI 1
#define INSTANCED_FAST_INDEX_BO_UI  2
#define INSTANCED_SLOW_INDEX_BO_UI  3
#define INSTANCED_PARAMS_BO_UI      4
#define INSTANCED_INFOS_BO_UI       5
#define INSTANCED_COLORS_BO_UI      6

class IChunk;

template<typename ShadersType>
class InstancedArrayObjectManager
{
public:
    typedef typename ShadersType::ShaderParamsType          ShaderParams;
    typedef QHash<QString, VertexAttribute>                 CustomVertexAttributeCollection;
    typedef QHashIterator<QString, VertexAttribute>         CustomVertexAttributeCollectionIterator;

    InstancedArrayObjectManager() {}

    ~InstancedArrayObjectManager()
    {
        destroyGL();
    }

    /**
     * @brief Create the AO that will draw vertex
     * @param buffersobjects : the bo manager
     * @param shaders : shaders
     */
    bool createVertexAO(void* chunk,
                        BufferObjectManager& bufferObjects,
                        ShadersType& shaders,
                        const CustomVertexAttributeCollection* customAttributesCollection,
                        QOpenGLContext* context)
    {
        if(m_vertexAO.isCreated())
            return false;

        bool ok = m_vertexAO.create();

        if(ok)
            ok = updateVertexAO(chunk, bufferObjects, shaders, customAttributesCollection, context);
        else
            m_vertexAO.destroy();

        return ok;
    }

    /**
     * @brief Update the AO that will draw vertex
     * @param buffersobjects : the bo manager
     * @param shaders : shaders
     */
    bool updateVertexAO(void* chunk,
                        BufferObjectManager& bufferObjects,
                        ShadersType& shaders,
                        const CustomVertexAttributeCollection* customAttributesCollection,
                        QOpenGLContext* context)
    {
        if(!m_vertexAO.isCreated())
            return false;

        QOpenGLBuffer& vbo = bufferObjects.getVertexBO(chunk, INSTANCED_SLOW_VERTEX_BO_UI);
        QOpenGLBuffer& mbo = bufferObjects.getBO(chunk, INSTANCED_PARAMS_BO_UI, QOpenGLBuffer::VertexBuffer);

        if(!vbo.isCreated() || !mbo.isCreated()) {
            m_vertexAO.destroy();
            return false;
        }

        bool ok = shaders.bindShader();

        if(ok)
            m_vertexAO.bind();

        if((ok = shaders.bindAndSetVertexToShader(vbo))) {

            ok = shaders.bindAndSetParamsAttributeToShader(mbo, context);

            QOpenGLBuffer& cbo = bufferObjects.getColorsBO(chunk, INSTANCED_COLORS_BO_UI);

            if(ok && cbo.isCreated())
                ok = shaders.bindAndSetColorAttributeToShader(cbo, context);

            if(ok)
                ok = shaders.bindAndSetInfoAttributeToShader(bufferObjects.getInfosBO(chunk, INSTANCED_INFOS_BO_UI), context);

            if(ok) {
                if(customAttributesCollection != nullptr) {
                    CustomVertexAttributeCollectionIterator it(*customAttributesCollection);

                    while(ok && it.hasNext()) {
                        it.next();

                        const VertexAttribute& attribute = it.value();

                        QOpenGLBuffer& attBO = bufferObjects.getBO(chunk, attribute.computeUniqueKey(), QOpenGLBuffer::VertexBuffer);

                        ok = attBO.isCreated();

                        if(ok) {
                            ok = shaders.bindAndSetCustomVertexAttributeToShader(attribute.name,
                                                                                 attribute.tupleSize,
                                                                                 attribute.type,
                                                                                 attribute.normalized,
                                                                                 attribute.stride,
                                                                                 attBO,
                                                                                 context);
                        }
                    }
                }
            }
        }

        m_vertexAO.release();

        if(!ok)
            m_vertexAO.destroy();

        shaders.releaseShader();

        return ok;
    }

    /**
     * @brief Returns true if you can use AO to draw objects
     */
    bool canUseVertexAO() const
    {
        return m_vertexAO.isCreated();
    }

    /**
     * @brief Bind and returns true if the vertex AO was bind successfully
     */
    bool bindVertexAO()
    {
        if(!m_vertexAO.isCreated())
            return false;

        m_vertexAO.bind();

        return true;
    }

    /**
     * @brief Release vertex AO
     */
    void releaseVertexAO()
    {
        m_vertexAO.release();
    }

    /**
     * @brief Create the AO that will draw vertex
     * @param buffersobjects : the bo manager
     * @param shaders : shaders
     */
    bool createFastVertexAO(void* chunk,
                            BufferObjectManager& bufferObjects,
                            ShadersType& shaders,
                            const CustomVertexAttributeCollection* customFastAttributesCollection,
                            QOpenGLContext* context)
    {
        if(m_fastVertexAO.isCreated())
            return false;

        bool ok = m_fastVertexAO.create();

        if(ok)
            ok = updateFastVertexAO(chunk, bufferObjects, shaders, customFastAttributesCollection, context);
        else
            m_fastVertexAO.destroy();

        return ok;
    }

    /**
     * @brief Update the AO that will draw vertex
     * @param buffersobjects : the bo manager
     * @param shaders : shaders
     */
    bool updateFastVertexAO(void* chunk,
                            BufferObjectManager& bufferObjects,
                            ShadersType& shaders,
                            const CustomVertexAttributeCollection* customFastAttributesCollection,
                            QOpenGLContext* context)
    {
        if(!m_fastVertexAO.isCreated())
            return false;

        QOpenGLBuffer& vbo = bufferObjects.getVertexBO(chunk, INSTANCED_FAST_VERTEX_BO_UI);
        QOpenGLBuffer& mbo = bufferObjects.getBO(chunk, INSTANCED_PARAMS_BO_UI, QOpenGLBuffer::VertexBuffer);

        if(!vbo.isCreated() || !mbo.isCreated()) {
            m_fastVertexAO.destroy();
            return false;
        }

        bool ok = shaders.bindShader();

        if(ok)
            m_fastVertexAO.bind();

        if((ok = shaders.bindAndSetVertexToShader(vbo))) {

            ok = shaders.bindAndSetParamsAttributeToShader(mbo, context);

            QOpenGLBuffer& cbo = bufferObjects.getColorsBO(chunk, INSTANCED_COLORS_BO_UI);

            if(ok && cbo.isCreated())
                ok = shaders.bindAndSetColorAttributeToShader(cbo, context);

            if(ok)
                ok = shaders.bindAndSetInfoAttributeToShader(bufferObjects.getInfosBO(chunk, INSTANCED_INFOS_BO_UI), context);

            if(ok) {
                if(customFastAttributesCollection != nullptr) {
                    CustomVertexAttributeCollectionIterator it(*customFastAttributesCollection);

                    while(ok && it.hasNext()) {
                        it.next();

                        const VertexAttribute& attribute = it.value();

                        QOpenGLBuffer& attBO = bufferObjects.getBO(chunk, attribute.computeUniqueKey(), QOpenGLBuffer::VertexBuffer);

                        ok = attBO.isCreated();

                        if(ok) {
                            ok = shaders.bindAndSetCustomVertexAttributeToShader(attribute.name,
                                                                                 attribute.tupleSize,
                                                                                 attribute.type,
                                                                                 attribute.normalized,
                                                                                 attribute.stride,
                                                                                 attBO,
                                                                                 context);
                        }
                    }
                }
            }
        }

        m_fastVertexAO.release();

        if(!ok)
            m_fastVertexAO.destroy();

        shaders.releaseShader();

        return ok;
    }

    /**
     * @brief Returns true if you can use AO to draw objects
     */
    bool canUseFastVertexAO() const
    {
        return m_fastVertexAO.isCreated();
    }

    /**
     * @brief Bind and returns true if the vertex AO was bind successfully
     */
    bool bindFastVertexAO()
    {
        if(!m_fastVertexAO.isCreated())
            return false;

        m_fastVertexAO.bind();

        return true;
    }

    /**
     * @brief Release vertex AO
     */
    void releaseFastVertexAO()
    {
        m_fastVertexAO.release();
    }

    /**
     * @brief Destroy all
     */
    void destroyGL()
    {
        m_vertexAO.destroy();
        m_fastVertexAO.destroy();
    }

private:
    QOpenGLVertexArrayObject    m_vertexAO;
    QOpenGLVertexArrayObject    m_fastVertexAO;
};

#endif // INSTANCEDARRAYOBJECTMANAGER_H
