#ifndef INSTANCEDRENDERERSHADERS_HPP
#define INSTANCEDRENDERERSHADERS_HPP

#include "instancedrenderershaders.h"

#define INSTANCED_CHECK_PROGRAM_ERROR(FUNC_TO_CALL) if((m_program != nullptr) && !(ok = FUNC_TO_CALL)) { \
                                                    delete m_program; \
                                                    m_program = nullptr; \
                                                  }

#define VERTEXATTRIBUTE_NAME_IN_SHADER "vertex"
#define COLORATTRIBUTE_NAME_IN_SHADER "color"
#define PARAMSATTRIBUTE_NAME_IN_SHADER "params"
#define INFOATTRIBUTE_NAME_IN_SHADER "info"
#define FLAGSCOLOR_NAME_IN_SHADER "flagsColors"

template<typename ShaderParams>
InstancedRendererShaders<ShaderParams>::InstancedRendererShaders(const GLint& glValueRowSize,
                                                                 const GLint& glValueColumnSize,
                                                                 const GLenum& glValueType,
                                                                 const QString& vertexShaderPath) : m_glValueRowSize(glValueRowSize),
                                                                                                    m_glValueColumnSize(glValueColumnSize),
                                                                                                    m_glValueType(glValueType)
{
    m_vertexShaderPath = vertexShaderPath;

    m_program = nullptr;
    m_shaderVertexLocation = -1;
    m_shaderParamsLocation = -1;
    m_shaderColorLocation = -1;
    m_shaderInfoLocation = -1;

    m_initialized = false;

    m_sizeOfParams = 0;

    if((m_glValueType == GL_BYTE)
            || (m_glValueType == GL_UNSIGNED_BYTE))
        m_sizeOfParams = 1;
    else if((m_glValueType == GL_SHORT)
            || (m_glValueType == GL_UNSIGNED_SHORT)
            || (m_glValueType == GL_HALF_FLOAT))
        m_sizeOfParams = 2;
    else if((m_glValueType == GL_INT)
            || (m_glValueType == GL_UNSIGNED_INT)
            || (m_glValueType == GL_FLOAT)
            || (m_glValueType == GL_FIXED)
            || (m_glValueType == GL_INT_2_10_10_10_REV)
            || (m_glValueType == GL_UNSIGNED_INT_2_10_10_10_REV)
            || (m_glValueType == GL_UNSIGNED_INT_10F_11F_11F_REV))
        m_sizeOfParams = 4;
    else if(m_glValueType == GL_DOUBLE)
        m_sizeOfParams = 8;

    if(m_sizeOfParams == 0)
    {
        assert(false);
        qDebug() << "InstancedRendererShaders<ShaderParams>::InstancedRendererShaders" << ", " <<  "Bad case"; return;
    }

    m_rowSize = m_glValueRowSize;

    if(m_rowSize == GL_BGRA)
        m_rowSize = 4;

    if(m_rowSize == 0)
    {
        assert(false);
        qDebug() << "InstancedRendererShaders<ShaderParams>::InstancedRendererShaders" << ", " <<  "Bad case"; return;
    }
}

template<typename ShaderParams>
InstancedRendererShaders<ShaderParams>::~InstancedRendererShaders()
{
    destroyGL();
}

template<typename ShaderParams>
QOpenGLShaderProgram* InstancedRendererShaders<ShaderParams>::getShaderProgram() const
{
    return m_program;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::init(const QOpenGLContext* context)
{
    bool ok = true;

    if(!m_initialized) {

        ok = false;

        if(!m_vertexShaderPath.isEmpty()
                && OpenGlInfo::staticCheckOpenglVersionMinimumVersion(context, 3, 3)) {
            m_program = new QOpenGLShaderProgram();

            QString finalSourceCode;
            QString error;
            ok = ShadersTools::staticMergeShadersFromSourceFile(QStringList() <<  "://shaders/common.vert" << m_vertexShaderPath, finalSourceCode, &error);

            if(!ok) {
                AMKglLOG->addErrorMessage(error);
                delete m_program;
                m_program = nullptr;
                return false;
            }

            INSTANCED_CHECK_PROGRAM_ERROR(getShaderProgram()->addShaderFromSourceCode(QOpenGLShader::Vertex, finalSourceCode));

            INSTANCED_CHECK_PROGRAM_ERROR(getShaderProgram()->link());
            INSTANCED_CHECK_PROGRAM_ERROR(getShaderProgram()->bind());

            if(ok) {
                m_shaderVertexLocation = getShaderProgram()->attributeLocation(VERTEXATTRIBUTE_NAME_IN_SHADER);
                m_shaderColorLocation = getShaderProgram()->attributeLocation(COLORATTRIBUTE_NAME_IN_SHADER);
                m_shaderInfoLocation = getShaderProgram()->attributeLocation(INFOATTRIBUTE_NAME_IN_SHADER);
                m_shaderParamsLocation = getShaderProgram()->attributeLocation(PARAMSATTRIBUTE_NAME_IN_SHADER);
                getShaderProgram()->release();
            }
        }

        m_initialized = true;
    }

    return ok;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::bindAndConfigureByDefaultShader(const IGraphicsDocument *document,
                                                                             const FlagsPropertyManager& fpM)
{
    if(!bindShader(getShaderProgram()))
        return false;

    std::vector<QVector4D> flagsColor = fpM.getColorsForAllFlags(document);

    getShaderProgram()->setUniformValueArray(FLAGSCOLOR_NAME_IN_SHADER, &flagsColor[0], static_cast<int>(flagsColor.size()));

    return true;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::bindShader()
{
    if(getShaderProgram() == nullptr)
        return false;

    return getShaderProgram()->bind();
}

template<typename ShaderParams>
void InstancedRendererShaders<ShaderParams>::releaseShader()
{
    releaseShader(getShaderProgram());
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::bindAndSetVertexToShader(QOpenGLBuffer &vertex)
{
    if((getShaderProgram() == nullptr)
            || (getShaderVertexLocation() == -1)
            || !vertex.isCreated()
            || !vertex.bind())
        return false;


    bool ok = setVertexToShader(nullptr);

    vertex.release();

    return ok;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::setVertexToShader(const Eigen::Vector3f* pointer)
{
    if((getShaderProgram() == nullptr)
            || (getShaderVertexLocation() == -1))
        return false;

    getShaderProgram()->enableAttributeArray(getShaderVertexLocation());
    getShaderProgram()->setAttributeArray(getShaderVertexLocation(), GL_FLOAT, pointer, 3, sizeof(Eigen::Vector3f));

    return true;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::bindAndSetCustomVertexAttributeToShader(const std::string& name, GLint tupleSize, GLenum type, GLboolean normalized, GLsizei stride, QOpenGLBuffer &custom, const QOpenGLContext* context)
{
    if((getShaderProgram() == nullptr)
            || name.empty()
            || !custom.isCreated()
            || !custom.bind())
        return false;

    bool ok = setCustomVertexAttributeToShader(name, tupleSize, type, normalized, stride, nullptr, context);

    custom.release();

    return ok;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::setCustomVertexAttributeToShader(const std::string& name, GLint tupleSize, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer, const QOpenGLContext* context)
{
    if((getShaderProgram() == nullptr)
            || name.empty())
        return false;

    // we must use direct function to pass GL_FALSE or GL_TRUE to "normalized" parameter
    QOpenGLFunctions_2_0* func = context->versionFunctions<QOpenGLFunctions_2_0>();

    if(func == nullptr)
        return false;

    int loc = getShaderProgram()->attributeLocation(name.data());

    if(loc == -1)
        return false;

    // enableAttributeArray(loc)
    func->glEnableVertexAttribArray(loc);

    // setAttributeArray(....)
    func->glVertexAttribPointer(loc,
                                tupleSize,
                                type,
                                normalized,
                                stride,
                                pointer);

    return true;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::bindAndSetParamsAttributeToShader(QOpenGLBuffer &params, const QOpenGLContext* context)
{
    if((getShaderProgram() == nullptr)
            || (getShaderParamsAttributeLocation() == -1)
            || !params.isCreated()
            || !params.bind())
        return false;

    bool ok = setParamsAttributeToShader(nullptr, context);

    params.release();

    return ok;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::setParamsAttributeToShader(const ShaderParams* pointer, const QOpenGLContext* context)
{
    QOpenGLFunctions_3_3_Compatibility* func = context->versionFunctions<QOpenGLFunctions_3_3_Compatibility>();

    if(func == nullptr)
        return false;

    for (int i = 0; i < m_glValueColumnSize; ++i)
    {
        int loc = getShaderParamsAttributeLocation() + i;

        func->glEnableVertexAttribArray(loc);

        if(pointer == nullptr) {
            func->glVertexAttribPointer(loc,
                                        m_glValueRowSize,
                                        m_glValueType,
                                        GL_FALSE,
                                        sizeof(ShaderParams),
                            #if defined(__x86_64__) || _M_X64 // Definition for GCC, Clang, and Intel's compiler or MSVC++
                                        (void *)quint64((m_sizeOfParams * m_rowSize) * i));
                            #else
                                        (void *)quint32((m_sizeOfParams * m_rowSize) * i)); // TODO : check if it's ok ?
                            #endif
        } else {
            func->glVertexAttribPointer(loc,
                                        m_glValueRowSize,
                                        m_glValueType,
                                        GL_FALSE,
                                        sizeof(ShaderParams),
                                        (&(*pointer)(0)) + (m_rowSize * i));
        }

        func->glVertexAttribDivisor(loc, 1);
    }

    return true;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::bindAndSetColorAttributeToShader(QOpenGLBuffer &color, const QOpenGLContext* context)
{
    if((getShaderProgram() == nullptr)
            || (getShaderColorAttributeLocation() == -1)
            || !color.isCreated()
            || !color.bind())
        return false;

    bool ok = setColorAttributeToShader(nullptr, context);

    color.release();

    return ok;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::setColorAttributeToShader(const Color* pointer, const QOpenGLContext* context)
{
    if((getShaderProgram() == nullptr)
            || (getShaderColorAttributeLocation() == -1))
        return false;

    QOpenGLFunctions_3_3_Compatibility* func = context->versionFunctions<QOpenGLFunctions_3_3_Compatibility>();

    if(func == nullptr)
        return false;

    int loc = getShaderColorAttributeLocation();

    func->glEnableVertexAttribArray(loc);

    func->glVertexAttribPointer(loc,
                                4,
                                GL_UNSIGNED_BYTE,
                                GL_TRUE,
                                sizeof(Color),
                                pointer);

    func->glVertexAttribDivisor(loc, 1);

    return true;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::bindAndSetInfoAttributeToShader(QOpenGLBuffer &info, const QOpenGLContext* context)
{
    if((getShaderProgram() == nullptr)
            || (getShaderInfoAttributeLocation() == -1)
            || !info.isCreated()
            || !info.bind())
        return false;

    bool ok = setInfoAttributeToShader(nullptr, context);

    info.release();

    return ok;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::setInfoAttributeToShader(const ElementInfo* pointer, const QOpenGLContext* context)
{
    if((getShaderProgram() == nullptr)
            || (getShaderInfoAttributeLocation() == -1))
        return false;

    QOpenGLFunctions_3_3_Compatibility* func = context->versionFunctions<QOpenGLFunctions_3_3_Compatibility>();

    if(func == nullptr)
        return false;

    int loc = getShaderInfoAttributeLocation();

    func->glEnableVertexAttribArray(loc);

    func->glVertexAttribPointer(loc,
                                1,
                                GL_UNSIGNED_BYTE,
                                GL_FALSE,
                                sizeof(ElementInfo),
                                pointer);

    func->glVertexAttribDivisor(loc, 1);

    return true;
}

template<typename ShaderParams>
void InstancedRendererShaders<ShaderParams>::disableAttributeOfShader(const QOpenGLContext* context)
{
    if(getShaderProgram() == nullptr)
        return;

    QOpenGLFunctions_3_3_Compatibility* func = context->versionFunctions<QOpenGLFunctions_3_3_Compatibility>();

    if(func != nullptr) {
        for(int i=0; i<m_glValueColumnSize; ++i)
            func->glVertexAttribDivisor(getShaderParamsAttributeLocation()+i, 0);

        func->glVertexAttribDivisor(getShaderInfoAttributeLocation(), 0);
        func->glVertexAttribDivisor(getShaderColorAttributeLocation(), 0);
        func->glVertexAttribDivisor(getShaderInfoAttributeLocation(), 0);
    }

    getShaderProgram()->disableAttributeArray(m_shaderVertexLocation);

    for(int i=0; i<m_glValueColumnSize; ++i)
        getShaderProgram()->disableAttributeArray(m_shaderParamsLocation+i);

    getShaderProgram()->disableAttributeArray(m_shaderColorLocation);
    getShaderProgram()->disableAttributeArray(m_shaderInfoLocation);

}

template<typename ShaderParams>
int InstancedRendererShaders<ShaderParams>::getShaderVertexLocation() const
{
    return m_shaderVertexLocation;
}

template<typename ShaderParams>
int InstancedRendererShaders<ShaderParams>::getShaderColorAttributeLocation() const
{
    return m_shaderColorLocation;
}

template<typename ShaderParams>
int InstancedRendererShaders<ShaderParams>::getShaderInfoAttributeLocation() const
{
    return m_shaderInfoLocation;
}

template<typename ShaderParams>
int InstancedRendererShaders<ShaderParams>::getShaderParamsAttributeLocation() const
{
    return m_shaderParamsLocation;
}

template<typename ShaderParams>
void InstancedRendererShaders<ShaderParams>::destroyGL()
{
    delete m_program;
    m_program = nullptr;


    m_shaderVertexLocation = -1;
    m_shaderParamsLocation = -1;
    m_shaderColorLocation = -1;
    m_shaderInfoLocation = -1;

    m_initialized = false;
}

template<typename ShaderParams>
bool InstancedRendererShaders<ShaderParams>::bindShader(QOpenGLShaderProgram *program)
{
    if(program == nullptr)
        return false;

    return program->bind();
}

template<typename ShaderParams>
void InstancedRendererShaders<ShaderParams>::releaseShader(QOpenGLShaderProgram *program)
{
    if(program != nullptr)
        program->release();
}


#endif // INSTANCEDRENDERERSHADERS_HPP
