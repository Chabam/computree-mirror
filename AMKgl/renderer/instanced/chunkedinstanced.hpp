#ifndef CHUNKEDINSTANCED_HPP
#define CHUNKEDINSTANCED_HPP

#include "chunkedinstanced.h"
#include "interfaces/igraphicsdocument.h"
#include "includecolor.h"
#include "includenormal.h"

#define CHUNKED_INSTANCED_RESIZE_VECTOR_ADD     10

template<typename RendererContextT>
quint8 ChunkedInstanced<RendererContextT>::SLOW_DISPLAY_LIST = 0;

template<typename RendererContextT>
quint8 ChunkedInstanced<RendererContextT>::FAST_DISPLAY_LIST = 1;

template<typename RendererContextT>
ChunkedInstanced<RendererContextT>::ChunkedInstanced(const uint& uniqueKey,
                                                     Scene::ObjectType objectType,
                                                     GLenum glMode,
                                                     const std::vector<Eigen::Vector3f> &vertexArray,
                                                     const Eigen::Vector3d& offset,
                                                     const CustomVertexAttributeCollection* customVertexAttributesCollection,
                                                     const std::vector<Eigen::Vector3f> *fastVertexArray,
                                                     const std::vector<GLuint> *normalIndices,
                                                     const std::vector<GLuint> *fastIndices,
                                                     void* funcContext,
                                                     DrawShapeFunc basicDraw, DrawShapeFunc rawDraw, DrawShapeFunc VBODraw, DrawShapeFunc VAODraw,
                                                     DrawShapeFunc basicFastDraw, DrawShapeFunc rawFastDraw, DrawShapeFunc VBOFastDraw, DrawShapeFunc VAOFastDraw) : Chunk<RendererContextT>(uniqueKey, objectType, offset)
{
    m_glMode = glMode;

    m_vertexArray = (std::vector<Eigen::Vector3f>*)&vertexArray;
    m_customVertexAttributesCollection = (CustomVertexAttributeCollection*)customVertexAttributesCollection;

    // TODO : create a m_customFastVertexAttributesCollection

    m_fastVertexArray = (std::vector<Eigen::Vector3f>*)fastVertexArray;
    m_normalIndicesArray = (std::vector<GLuint>*)normalIndices;
    m_fastIndicesArray = (std::vector<GLuint>*)fastIndices;

    m_infosAccess = new InfoCloud(m_infoArray, 1);
    m_colorsAccess = new ColorCloud(m_colorArray, 1);

    m_nShapes = 0;
    m_funcContext = funcContext;

    m_basicDrawFunc = basicDraw;
    m_rawDrawFunc = rawDraw;
    m_VBODrawFunc = VBODraw;
    m_VAODrawFunc = VAODraw;

    m_basicFastDrawFunc = basicFastDraw;
    m_rawFastDrawFunc = rawFastDraw;
    m_VBOFastDrawFunc = VBOFastDraw;
    m_VAOFastDrawFunc = VAOFastDraw;
}
template<typename RendererContextT>
ChunkedInstanced<RendererContextT>::~ChunkedInstanced()
{
    delete m_infosAccess;
    delete m_colorsAccess;
}

template<typename RendererContextT>
GLenum ChunkedInstanced<RendererContextT>::getGlMode() const
{
    return m_glMode;
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::updateOnlyVAO()
{
    if((this->getNormalDrawModeUsed() == DM_VAO) && this->mustUseVAO()) {

        RendererContextT* context = this->getCurrentContext();

        BufferObjectManager& bufferObjects = context->getBufferObjectManager();

        if(getColorCloud() != NULL && !bufferObjects.getColorsBO(this, INSTANCED_COLORS_BO_UI).isCreated())
            ChunkedInstanced<RendererContextT>::setUpdated(false);

        if(ChunkedInstanced<RendererContextT>::isUpdated()) {
            ShadersType& shaders = context->getShaders();
            InstancedArrayObjectManager<ShadersType>& arrayObjects = context->createOrGetArrayObjectManager(this);

            context->makeCurrent();

            arrayObjects.updateVertexAO(this, bufferObjects, shaders, m_customVertexAttributesCollection, context->getOpenglContext());
            arrayObjects.updateFastVertexAO(this, bufferObjects, shaders, m_customVertexAttributesCollection, context->getOpenglContext());

            //buffers.releaseAll();

            shaders.releaseShader();
        } else {
            internalUpdate();
        }
    }

    if((this->getNormalDrawModeUsed() == DM_DISPLAY_LIST)
              || (this->getFastDrawModeUsed() == DM_DISPLAY_LIST)) {

        RendererContextT* context = this->getCurrentContext();

        if(context == NULL)
          return;

        context->destroyAllDisplayList(this);
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::updateColorBO()
{
    if(((this->getNormalDrawModeUsed() == DM_VBO)
            || (this->getNormalDrawModeUsed() == DM_VAO)) && this->mustUseVBO()) {

        BufferObjectManager& buffers = this->getCurrentContext()->getBufferObjectManager();
        buffers.updateColorsBO(this->getFirstColor(), m_colorArray.size());
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::updateInfoBO()
{
    if(((this->getNormalDrawModeUsed() == DM_VBO)
            || (this->getNormalDrawModeUsed() == DM_VAO)) && this->mustUseVBO()) {

        BufferObjectManager& buffers = this->getCurrentContext()->getBufferObjectManager();
        buffers.updateInfosBO(this->getFirstInfo(), m_infoArray.size());
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::addShape(const typename ChunkedInstanced<RendererContextT>::ShaderParamsType &params, const ElementInfo &info)
{
    this->setUpdated(false);

    if(m_shapeArray.size() == m_nShapes) {
        const size_t newSize = m_nShapes + CHUNKED_INSTANCED_RESIZE_VECTOR_ADD;

        m_shapeArray.resize(newSize, params);
        m_infoArray.resize(newSize, info);
        m_colorArray.resize(newSize, this->getCurrentColor());
    } else {
        m_shapeArray[m_nShapes] = params;
        m_infoArray[m_nShapes] = info;
        m_colorArray[m_nShapes] = this->getCurrentColor();
    }

    ++m_nShapes;
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::removeShapes(const size_t& beginLocalIndex, const size_t& count)
{
    this->removeObjects(beginLocalIndex, count);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::setShapesVisible(const size_t& beginLocalIndex, const size_t& count, bool visible)
{
    if(count == 0)
        return;

    size_t end = beginLocalIndex + count;

    if(visible) {
        do {
            ObjectsFlagsTool::staticShowObject(m_infoArray[--end]);
        } while(end != beginLocalIndex);
    } else {
        do {
            ObjectsFlagsTool::staticHideObjectAndUnselectIt(m_infoArray[--end]);
        } while(end != beginLocalIndex);
    }

    this->setUpdated(false);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::transformObjects(const Eigen::Matrix4f& trMatrix, const size_t& from, const size_t& count)
{
    if(count == 0)
        return;

    size_t end = from + count;

    do {
        m_shapeArray[--end].transform(trMatrix);
    } while(end != from);

    this->setUpdated(false);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::translateObjects(const Eigen::Vector3f& translation, const size_t& from, const size_t& count)
{
    if(count == 0)
        return;

    size_t end = from + count;

    do {
        m_shapeArray[--end].translate(translation);
    } while(end != from);

    this->setUpdated(false);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::rotateObjects(const Eigen::AngleAxis<float>& rotation, const size_t& from, const size_t& count)
{
    if(count == 0)
        return;

    size_t end = from + count;

    do {
        m_shapeArray[--end].rotate(rotation);
    } while(end != from);

    this->setUpdated(false);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::rotateObjects(const Eigen::Quaternion<float>& rotation, const size_t& from, const size_t& count)
{
    if(count == 0)
        return;

    size_t end = from + count;

    do {
        m_shapeArray[--end].rotate(rotation);
    } while(end != from);

    this->setUpdated(false);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::scaleObjects(const Eigen::Vector3f& scaling, const size_t& from, const size_t& count)
{
    if(count == 0)
        return;

    size_t end = from + count;

    do {
        m_shapeArray[--end].scale(scaling);
    } while(end != from);

    this->setUpdated(false);
}

template<typename RendererContextT>
bool ChunkedInstanced<RendererContextT>::isAtLeastOneObjectVisible(const size_t &from, const size_t &count) const
{
    if(count == 0)
        return false;

    size_t end = from + count;

    do {
        if(ObjectsFlagsTool::staticIsObjectVisible(m_infoArray[--end]))
            return true;
    } while(end != from);

    return false;
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::clearShape()
{
    this->setUpdated(false);
    m_nShapes = 0;
    m_shapeArray.resize(0);
    m_shapeArray.shrink_to_fit();

    m_infoArray.resize(0);
    m_infoArray.shrink_to_fit();

    m_colorArray.resize(0);
    m_colorArray.shrink_to_fit();
}

template<typename RendererContextT>
size_t ChunkedInstanced<RendererContextT>::countShape() const
{
    return m_nShapes;
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::resetObjects()
{
    this->setUpdated(false);
    m_nShapes = 0;
    m_shapeArray.resize(0);
    m_infoArray.resize(0);
    m_colorArray.resize(0);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::draw(DrawInfo &info)
{
    Q_UNUSED(info)

    if((m_nShapes == 0) || !this->isUpdated())
        return;

    this->lock();

    DrawMode type = this->getNormalDrawModeUsed();

    if(type != DM_NONE) {

        this->pushCoordinateSystemMatrix(info);

        if(type == DM_BASIC) {
            basicDraw(info);
        } else if(type == DM_DISPLAY_LIST) {
            displayListDraw(info);
        } else {
            RendererContextT* context = this->getCurrentContext();

            context->getShaders().bindAndConfigureByDefaultShader(this->getCurrentDocument(), *this->getFlagsPropertyManager());

            if(type == DM_RAW) {
                rawDraw(info);
            } else if(type == DM_VBO) {
                vboDraw(info);
            } else if(type == DM_VAO){
                vaoDraw(info);
            }

            context->getShaders().releaseShader();
        }

        this->popCoordinateSystemMatrix();
    }

    this->unlock();
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::fastDraw(DrawInfo &info)
{
    Q_UNUSED(info)

    if((m_nShapes == 0) || !this->isUpdated())
        return;

    this->lock();

    DrawMode type = this->getFastDrawModeUsed();

    if(type != DM_NONE) {

        this->pushCoordinateSystemMatrix(info);

        if(type == DM_BASIC) {
            basicFastDraw(info);
        } else if(type == DM_DISPLAY_LIST) {
            displayListFastDraw(info);
        } else {
            RendererContextT* context = this->getCurrentContext();

            context->getShaders().bindAndConfigureByDefaultShader(this->getCurrentDocument(), *this->getFlagsPropertyManager());

            if(type == DM_RAW) {
                rawFastDraw(info);
            } else if(type == DM_VBO) {
                vboFastDraw(info);
            } else if(type == DM_VAO) {
                vaoFastDraw(info);
            }

            context->getShaders().releaseShader();
        }

        this->popCoordinateSystemMatrix();
    }

    this->unlock();
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::postDraw(DrawInfo &info)
{
    Q_UNUSED(info)
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::setArrayOfIndicesForFastDraw(const std::vector<GLuint> *fastIndices)
{
    this->setUpdated(false);

    m_fastIndicesArray = (std::vector<GLuint>*)fastIndices;
}

template<typename RendererContextT>
typename ChunkedInstanced<RendererContextT>::Vertex *ChunkedInstanced<RendererContextT>::getFirstVertex() const
{
    if(m_vertexArray->empty())
        return NULL;

    return &((*m_vertexArray)[0]);
}

template<typename RendererContextT>
typename ChunkedInstanced<RendererContextT>::Vertex *ChunkedInstanced<RendererContextT>::getFirstVertexForFast() const
{
    if((m_fastVertexArray == NULL) || m_fastVertexArray->empty())
        return NULL;

    return &((*m_fastVertexArray)[0]);
}

template<typename RendererContextT>
GLuint* ChunkedInstanced<RendererContextT>::getFirstIndex() const
{
    if((m_normalIndicesArray == NULL) || m_normalIndicesArray->empty())
        return NULL;

    return &((*m_normalIndicesArray)[0]);
}

template<typename RendererContextT>
GLuint* ChunkedInstanced<RendererContextT>::getFirstIndexForFast() const
{
    if((m_fastIndicesArray == NULL) || m_fastIndicesArray->empty())
        return NULL;

    return &((*m_fastIndicesArray)[0]);
}

template<typename RendererContextT>
typename ChunkedInstanced<RendererContextT>::ShaderParamsType* ChunkedInstanced<RendererContextT>::getFirstParam() const
{
    if(m_shapeArray.empty())
        return NULL;

    return const_cast<ShaderParamsType*>(&(m_shapeArray[0]));
}

template<typename RendererContextT>
typename ChunkedInstanced<RendererContextT>::Color* ChunkedInstanced<RendererContextT>::getFirstColor() const
{
    if(m_colorArray.empty())
        return NULL;

    return const_cast<Color*>(&(m_colorArray[0]));
}

template<typename RendererContextT>
typename ChunkedInstanced<RendererContextT>::Info* ChunkedInstanced<RendererContextT>::getFirstInfo() const
{
    if(m_infoArray.empty())
        return NULL;

    return const_cast<Info*>(&(m_infoArray[0]));
}

template<typename RendererContextT>
typename ChunkedInstanced<RendererContextT>::ColorCloud* ChunkedInstanced<RendererContextT>::getColorCloud() const
{
    return m_colorsAccess;
}

template<typename RendererContextT>
typename ChunkedInstanced<RendererContextT>::NormalCloud* ChunkedInstanced<RendererContextT>::getNormalCloud() const
{
    return NULL;
}

template<typename RendererContextT>
typename ChunkedInstanced<RendererContextT>::InfoCloud* ChunkedInstanced<RendererContextT>::getInfoCloud() const
{
    return m_infosAccess;
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::basicDraw(DrawInfo& info)
{
    if(m_basicDrawFunc != NULL)
        (*m_basicDrawFunc)(m_funcContext, this, info);
    else
        this->basicFastOrNormalDraw(false);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::displayListDraw(DrawInfo& info)
{
    if(!this->getCurrentContext()->executeOrCreateAndCompileAndExecuteDisplayList(this, SLOW_DISPLAY_LIST)) {
        this->basicDraw(info);
        this->getCurrentContext()->endDisplayList(this, SLOW_DISPLAY_LIST);
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::rawDraw(DrawInfo& info)
{
    if(m_rawDrawFunc != NULL)
        (*m_rawDrawFunc)(m_funcContext, this, info);
    else
        this->rawFastOrNormalDraw(false);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::vboDraw(DrawInfo& info)
{
    if(m_VBODrawFunc != NULL)
        (*m_VBODrawFunc)(m_funcContext, this, info);
    else
        this->vboFastOrNormalDraw(false, info);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::vaoDraw(DrawInfo& info)
{
    if(m_VAODrawFunc != NULL)
        (*m_VAODrawFunc)(m_funcContext, this, info);
    else
        this->vaoFastOrNormalDraw(false, info);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::basicFastDraw(DrawInfo& info)
{
    if(m_basicFastDrawFunc != NULL) {
        (*m_basicFastDrawFunc)(m_funcContext, this, info);
    } else {
        bool fastArrayAvailable = (m_fastVertexArray != NULL) || (m_fastIndicesArray != NULL);

        this->basicFastOrNormalDraw(fastArrayAvailable);
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::displayListFastDraw(DrawInfo& info)
{
    if(!this->getCurrentContext()->executeOrCreateAndCompileAndExecuteDisplayList(this, FAST_DISPLAY_LIST)) {
        this->basicFastDraw(info);
        this->getCurrentContext()->endDisplayList(this, FAST_DISPLAY_LIST);
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::rawFastDraw(DrawInfo& info)
{
    if(m_rawFastDrawFunc != NULL)
        (*m_rawFastDrawFunc)(m_funcContext, this, info);
    else
        this->rawFastOrNormalDraw((m_fastVertexArray != NULL) || (m_fastIndicesArray != NULL));
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::vboFastDraw(DrawInfo& info)
{
    if(m_VBOFastDrawFunc != NULL)
        (*m_VBOFastDrawFunc)(m_funcContext, this, info);
    else
        this->vboFastOrNormalDraw((m_fastVertexArray != NULL) || (m_fastIndicesArray != NULL), info);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::vaoFastDraw(DrawInfo& info)
{
    if(m_VAOFastDrawFunc != NULL)
        (*m_VAOFastDrawFunc)(m_funcContext, this, info);
    else
        this->vaoFastOrNormalDraw((m_fastVertexArray != NULL) || (m_fastIndicesArray != NULL), info);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::basicFastOrNormalDraw(bool fast)
{
    size_t size = 0;

    std::vector<GLuint>* indices = NULL;

    if(fast) {
        indices = m_fastIndicesArray;

        if(m_fastIndicesArray != NULL)
            size = m_fastIndicesArray->size();
        else
            size = m_fastVertexArray->size();
    } else {
        indices = m_normalIndicesArray;

        if(m_normalIndicesArray != NULL)
            size = m_normalIndicesArray->size();
        else
            size = m_vertexArray->size();
    }

    if(size == 0)
        return;

    const size_t nShape = this->countShape();

    Eigen::Matrix4f matrix4f = Eigen::Matrix4f::Identity();

    if(nShape != 0) {

        float selectionColor[4];
        this->getSelectionColor(selectionColor);

        float currentColor[4];
        this->getCurrentColor(currentColor);

        std::vector<Eigen::Vector3f>* vA = m_vertexArray;

        if(fast && (m_fastVertexArray != NULL))
            vA = m_fastVertexArray;

        ShaderParamsType* fM = getFirstParam();
        Info* fI = this->getFirstInfo();
        Color* fC = this->getFirstColor();

        bool modified = false;

        for(size_t i=0; i<nShape; ++i) {

            const Info& info = fI[i];

            if(ObjectsFlagsTool::staticIsObjectVisible(info)){
                if(ObjectsFlagsTool::staticIsObjectSelected(info)) {
                    glColor4fv(selectionColor);
                    modified = true;
                } else if(fC != NULL) {
                    glColor4ubv(&fC[i][0]);
                } else if(modified) {
                    glColor4fv(currentColor);
                    modified = false;
                }

                const ShaderParamsType& param = fM[i];

                param.getMatrix4f(matrix4f);

                glPushMatrix();
                glMultMatrixf(matrix4f.data());

                glBegin(m_glMode);

                this->basicDrawShape(param, *vA, indices, size, fast);

                glEnd();

                glPopMatrix();
            }
        }
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::rawFastOrNormalDraw(bool fast)
{
    size_t size = m_nShapes;

    RendererContextT* context = this->getCurrentContext();

    QOpenGLFunctions_3_3_Compatibility* func = context->getOpenglContext()->template versionFunctions<QOpenGLFunctions_3_3_Compatibility>();

    Eigen::Vector3f* fv = NULL;

    if(fast)
        fv = this->getFirstVertexForFast();

    if(fv == NULL)
        fv = this->getFirstVertex();

    Info* fi = this->getFirstInfo();
    Color* fc = this->getFirstColor();
    ShaderParamsType* fm = this->getFirstParam();

    ShadersType& shaders = context->getShaders();

    // vertex
    shaders.setVertexToShader(fv);

    // info
    shaders.setInfoAttributeToShader(fi, context->getOpenglContext());

    // color
    if(fc != NULL)
        shaders.setColorAttributeToShader(fc, context->getOpenglContext());

    // matrix
    shaders.setParamsAttributeToShader(fm, context->getOpenglContext());

    // custom vertex attributes
    if(m_customVertexAttributesCollection != NULL) {
        CustomVertexAttributeCollectionIterator it(*m_customVertexAttributesCollection);

        while(it.hasNext()) {
            it.next();
            const VertexAttribute& attribute = it.value();

            shaders.setCustomVertexAttributeToShader(attribute.name, attribute.tupleSize, attribute.type, attribute.normalized, attribute.stride, attribute.pointer, context->getOpenglContext());
        }
    }

    if(fast)
        this->callFastDrawElements(size, func);
    else
        this->callDrawElements(size, func);

    shaders.disableAttributeOfShader(context->getOpenglContext());

    if((m_customVertexAttributesCollection != NULL) && (shaders.getShaderProgram() != NULL)) {
        CustomVertexAttributeCollectionIterator it(*m_customVertexAttributesCollection);

        while(it.hasNext()) {
            it.next();
            const VertexAttribute& attribute = it.value();
            shaders.getShaderProgram()->disableAttributeArray(attribute.name.data());
        }
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::vboFastOrNormalDraw(bool fast, DrawInfo& info)
{
    RendererContextT* context = this->getCurrentContext();

    QOpenGLFunctions_3_3_Compatibility* func = context->getOpenglContext()->template versionFunctions<QOpenGLFunctions_3_3_Compatibility>();

    ShadersType& shaders = context->getShaders();
    BufferObjectManager& buffers = context->getBufferObjectManager();

    bool ok = false;

    size_t size = m_nShapes;

    if(fast && buffers.getVertexBO(this, INSTANCED_FAST_VERTEX_BO_UI).isCreated())
        ok = shaders.bindAndSetVertexToShader(buffers.getVertexBO(this, INSTANCED_FAST_VERTEX_BO_UI));
    else if(!fast)
        ok = shaders.bindAndSetVertexToShader(buffers.getVertexBO(this, INSTANCED_SLOW_VERTEX_BO_UI));

    if(ok) {
        ok = shaders.bindAndSetParamsAttributeToShader(buffers.getBO(this, INSTANCED_PARAMS_BO_UI, QOpenGLBuffer::VertexBuffer), context->getOpenglContext());

        QOpenGLBuffer& cbo = buffers.getColorsBO(this, INSTANCED_COLORS_BO_UI);

        if(ok && cbo.isCreated())
            ok = shaders.bindAndSetColorAttributeToShader(cbo, context->getOpenglContext());

        if(ok)
            ok = shaders.bindAndSetInfoAttributeToShader(buffers.getInfosBO(this, INSTANCED_INFOS_BO_UI), context->getOpenglContext());

        // custom vertex attributes
        if(m_customVertexAttributesCollection != NULL) {
            CustomVertexAttributeCollectionIterator it(*m_customVertexAttributesCollection);

            while(ok && it.hasNext()) {
                it.next();

                const VertexAttribute& attribute = it.value();

                QOpenGLBuffer& bo = buffers.getBO(this, attribute.computeUniqueKey(), QOpenGLBuffer::VertexBuffer);

                if(bo.isCreated())
                    ok = context->getShaders().bindAndSetCustomVertexAttributeToShader(attribute.name, attribute.tupleSize, attribute.type, attribute.normalized, attribute.stride, bo, context->getOpenglContext());
                else
                    ok = false;
            }
        }

        if(ok) {
            if(fast)
                this->callFastDrawElements(size, func);
            else
                this->callDrawElements(size, func);
        }

        shaders.disableAttributeOfShader(context->getOpenglContext());
    }

    if(!ok) {
        shaders.releaseShader();

        if(fast)
            this->displayListFastDraw(info);
        else
            this->displayListDraw(info);
    }
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::vaoFastOrNormalDraw(bool fast, DrawInfo& info)
{
    RendererContextT* context = this->getCurrentContext();

    QOpenGLFunctions_3_3_Compatibility* func = context->getOpenglContext()->template versionFunctions<QOpenGLFunctions_3_3_Compatibility>();

    InstancedArrayObjectManager<ShadersType>& arrayObjects = context->createOrGetArrayObjectManager(this);

    bool ok = false;

    size_t size = m_nShapes;

    if(fast) {
        if(arrayObjects.canUseFastVertexAO()) {

            if(arrayObjects.bindFastVertexAO()) {

                ok = true;

                this->callFastDrawElements(size, func);

                arrayObjects.releaseFastVertexAO();
            }
        }
    } else {
        if(arrayObjects.canUseVertexAO()) {

            if(arrayObjects.bindVertexAO()) {

                ok = true;

                this->callDrawElements(size, func);

                arrayObjects.releaseVertexAO();
            }
        }
    }

    if(!ok)
        this->vboFastOrNormalDraw(fast, info);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::callDrawElements(const size_t &nShapeToDraw,
                                                          QOpenGLFunctions_3_3_Compatibility* func)
{
    if(m_normalIndicesArray != NULL) {
        BufferObjectManager& buffers = this->getCurrentContext()->getBufferObjectManager();

        QOpenGLBuffer& ibo = buffers.getIndexBO(this, INSTANCED_SLOW_INDEX_BO_UI);

        if(ibo.isCreated()
                && ibo.bind()) {
            func->glDrawElementsInstanced(m_glMode, (GLsizei)m_normalIndicesArray->size(), GL_UNSIGNED_INT, 0, (GLsizei)nShapeToDraw);
            ibo.release();
            return;
        }

        func->glDrawElementsInstanced(m_glMode, (GLsizei)m_normalIndicesArray->size(), GL_UNSIGNED_INT, &((*m_normalIndicesArray)[0]), (GLsizei)nShapeToDraw);
        return;
    }

    func->glDrawArraysInstanced(m_glMode, 0, (GLsizei)m_vertexArray->size(), (GLsizei)nShapeToDraw);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::callFastDrawElements(const size_t &nShapeToDraw,
                                                              QOpenGLFunctions_3_3_Compatibility* func)
{
    if(m_fastIndicesArray != NULL) {
        BufferObjectManager& buffers = this->getCurrentContext()->getBufferObjectManager();

        QOpenGLBuffer& fibo = buffers.getIndexBO(this, INSTANCED_FAST_INDEX_BO_UI);

        if(fibo.isCreated()
                && fibo.bind()) {
            func->glDrawElementsInstanced(m_glMode, (GLsizei)m_fastIndicesArray->size(), GL_UNSIGNED_INT, 0, (GLsizei)nShapeToDraw);
            fibo.release();
            return;
        }

        func->glDrawElementsInstanced(m_glMode, (GLsizei)m_fastIndicesArray->size(), GL_UNSIGNED_INT, &((*m_fastIndicesArray)[0]), (GLsizei)nShapeToDraw);
        return;
    }

    if(m_fastVertexArray != NULL)
        func->glDrawArraysInstanced(m_glMode, 0, (GLsizei)m_fastVertexArray->size(), (GLsizei)nShapeToDraw);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::moveObjectsFromTo(const size_t &from, const size_t &to, const size_t &size)
{
    std::memcpy(&((*(m_shapeArray.begin()+to))(0)), &((*(m_shapeArray.begin()+from))(0)), sizeof(ShaderParamsType)*size);
    std::memcpy(&(*(m_infoArray.begin()+to)), &(*(m_infoArray.begin()+from)), sizeof(Info)*size);
    std::memcpy(&((*(m_colorArray.begin()+to))(0)), &((*(m_colorArray.begin()+from))(0)), sizeof(Color)*size);
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::basicDrawShape(const typename ChunkedInstanced<RendererContextT>::ShaderParamsType& param,
                                                        const std::vector<Vertex>& vertexArray,
                                                        std::vector<GLuint>* indices,
                                                        const size_t& nVertex,
                                                        bool fast)
{
    Q_UNUSED(param)
    Q_UNUSED(fast)

    if(indices != NULL) {
        for(size_t j=0; j<nVertex; ++j)
            glVertex3fv(&(vertexArray[(*indices)[j]])[0]);
    } else {
        for(size_t j=0; j<nVertex; ++j)
            glVertex3fv(&(vertexArray[j])[0]);
    }
}

template<typename RendererContextT>
bool ChunkedInstanced<RendererContextT>::internalUpdate()
{
    RendererContextT* context = this->getCurrentContext();

    if(context == NULL)
        return false;

    if(this->isUpdated())
        return true;

    context->makeCurrent();

    this->destroyGLForCurrentContext();

    if(m_nShapes == 0) {
        this->setNormalDrawModeUsed(DM_NONE);
        this->setFastDrawModeUsed(DM_NONE);
        this->setUpdated(true);
        return true;
    }

    ShadersType& shaders = context->getShaders();

    if(shaders.getShaderProgram() == NULL) {
        this->setNormalDrawModeUsed(this->getDrawModeToUse() == DM_BASIC ? DM_BASIC : DM_DISPLAY_LIST);
        this->setFastDrawModeUsed(this->getNormalDrawModeUsed());
        this->setUpdated(true);
        return true;
    }

    // NORMAL DRAW : for the moment we will use the draw mode set by user
    this->setNormalDrawModeUsed(this->getDrawModeToUse());
    this->setFastDrawModeUsed(this->getDrawModeToUse());

    // To use DRAW mode RAW or VBO or VAO with must have at least opengl function 3.3
    QOpenGLFunctions_3_3_Compatibility* func = context->getOpenglContext()->template versionFunctions<QOpenGLFunctions_3_3_Compatibility>();

    // if the graphics card don't have this feature we must use the draw mode BASIC
    if(func == NULL) {
        this->setNormalDrawModeUsed(this->getDrawModeToUse() == DM_BASIC ? DM_BASIC : DM_DISPLAY_LIST);
        this->setFastDrawModeUsed(this->getNormalDrawModeUsed());
    } else
        func->initializeOpenGLFunctions();

    bool ok = false;

    if(this->mustUseVBO() && (func != NULL)) {

        BufferObjectManager& bufferObjects = context->getBufferObjectManager();

        ok = shaders.bindShader();

        if(ok) {
            ok = bufferObjects.createVertexBO<Vertex>(this, INSTANCED_SLOW_VERTEX_BO_UI, [](void* object) -> void* { return ((ChunkedInstanced<RendererContextT>*)object)->getFirstVertex(); }, m_vertexArray->size());

            if(ok) {

                if(m_fastVertexArray != NULL)
                    ok = bufferObjects.createVertexBO<Vertex>(this, INSTANCED_FAST_VERTEX_BO_UI, [](void* object) -> void* { return ((ChunkedInstanced<RendererContextT>*)object)->getFirstVertexForFast(); }, m_fastVertexArray->size());

                if(ok && (m_fastIndicesArray != NULL))
                    ok = bufferObjects.createIndexBO<Vertex>(this, INSTANCED_FAST_INDEX_BO_UI, [](void* object) -> void* { return ((ChunkedInstanced<RendererContextT>*)object)->getFirstIndexForFast(); }, m_fastIndicesArray->size());

                if(ok && (m_normalIndicesArray != NULL))
                    ok = bufferObjects.createIndexBO<GLuint>(this, INSTANCED_SLOW_INDEX_BO_UI, [](void* object) -> void* { return ((ChunkedInstanced<RendererContextT>*)object)->getFirstIndex(); }, m_normalIndicesArray->size());

                if(ok) {
                    ok = bufferObjects.createBO<ShaderParamsType>(this, INSTANCED_PARAMS_BO_UI, QOpenGLBuffer::VertexBuffer, [](void* object) -> void* { return ((ChunkedInstanced<RendererContextT>*)object)->getFirstParam(); }, m_shapeArray.size());

                    if(ok)
                        ok = bufferObjects.createInfosBO<Info>(this, INSTANCED_INFOS_BO_UI, [](void* object) -> void* { return ((ChunkedInstanced<RendererContextT>*)object)->getFirstInfo(); }, m_shapeArray.size());

                    if(ok)
                        bufferObjects.createColorsBO<Color>(this, INSTANCED_COLORS_BO_UI, [](void* object) -> void* { return ((ChunkedInstanced<RendererContextT>*)object)->getFirstColor(); }, m_shapeArray.size());

                    if(ok && (m_customVertexAttributesCollection != NULL)) {
                        CustomVertexAttributeCollectionIterator it(*m_customVertexAttributesCollection);

                        while(ok && it.hasNext()) {
                            it.next();

                            QString attKey = it.key();
                            VertexAttribute att = it.value();
                            ok = bufferObjects.createBO(this,
                                                        att.computeUniqueKey(),
                                                        QOpenGLBuffer::VertexBuffer,
                                                        [attKey](void* object) -> void* {
                                                            ChunkedInstanced<RendererContextT>* thisObj = (ChunkedInstanced<RendererContextT>*)object;
                                                            return thisObj->m_customVertexAttributesCollection->value(attKey).pointer;
                                                        },
                                                        att.sizeOfOneValueInByte,
                                                        att.arraySize);
                        }
                    }
                }
            }
        }

        if(!ok) {
            this->setNormalDrawModeUsed(DM_DISPLAY_LIST);
            this->setFastDrawModeUsed(DM_DISPLAY_LIST);
        }

        // Create Vertex Array Object
        if(ok && this->mustUseVAO()) {
            InstancedArrayObjectManager<ShadersType>& arrayObjects = context->createOrGetArrayObjectManager(this);

            arrayObjects.createVertexAO(this, bufferObjects, shaders, m_customVertexAttributesCollection, context->getOpenglContext());
            arrayObjects.createFastVertexAO(this, bufferObjects, shaders, m_customVertexAttributesCollection, context->getOpenglContext());
        }

        //bufferObjects.releaseAll();

        shaders.releaseShader();
    }

    this->setUpdated(true);

    return true;
}

template<typename RendererContextT>
void ChunkedInstanced<RendererContextT>::setNumberOfObjects(const size_t& n)
{
    m_nShapes = n;

    m_shapeArray.resize(m_nShapes+1);
    m_shapeArray.shrink_to_fit();

    m_infoArray.resize(m_nShapes+1);
    m_infoArray.shrink_to_fit();

    m_colorArray.resize(m_nShapes+1);
    m_colorArray.shrink_to_fit();
}

#endif // CHUNKEDINSTANCED_HPP
