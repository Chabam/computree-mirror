#include "genericrenderershaders.h"

#include "amkglapp.h"
#include "interfaces/igraphicsdocument.h"
#include "renderer/tools/flagspropertymanager.h"

#include "renderer/tools/shaderstools.h"
#include "tools/opengl/openglinfo.h"

#include "includecolor.h"
#include "includenormal.h"

#include <QOpenGLFunctions_2_0>

#define VERTEXATTRIBUTE_NAME_IN_SHADER "vertex"
#define COLORATTRIBUTE_NAME_IN_SHADER "frontColor"
#define INFOATTRIBUTE_NAME_IN_SHADER "info"
#define FLAGSCOLOR_NAME_IN_SHADER "flagsColors"

#define NORMALATTRIBUTE_NAME_IN_SHADER "normal"

#define NORMALLENGTH_NAME_IN_SHADER "normalLength"
#define NORMALCOLOR_NAME_IN_SHADER "normalColor"

GenericRendererShaders::GenericRendererShaders()
{
    m_program = NULL;
    m_initialized = false;
    m_shaderVertexLocation = -1;
    m_shaderInfoLocation = -1;
    m_shaderColorLocation = -1;
    m_shaderNormalLocation = -1;
}

GenericRendererShaders::~GenericRendererShaders()
{
    destroyGL();
}

QOpenGLShaderProgram* GenericRendererShaders::getShaderProgram() const
{
    return m_program;
}

bool GenericRendererShaders::init(const QOpenGLContext* context)
{
    bool ok = true;

    if(!m_initialized) {

        ok = false;

        // opengl version > 2.0
        if(OpenGlInfo::staticCheckOpenglVersionMinimumVersion(context, 2, 0)) {
            m_program = new QOpenGLShaderProgram();

            QString finalSourceCode;
            QString error;
            ok = ShadersTools::staticMergeShadersFromSourceFile(QStringList() <<  "://shaders/common.vert" << "://shaders/generic/simple.vert", finalSourceCode, &error);

            if(!ok) {
                AMKglLOG->addErrorMessage(error);
                delete m_program;
                m_program = NULL;
                return false;
            }

            ShadersTools_CHECK_PROGRAM_ERROR(m_program, m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, finalSourceCode));
            ShadersTools_CHECK_PROGRAM_ERROR(m_program, m_program->link());
            ShadersTools_CHECK_PROGRAM_ERROR(m_program, m_program->bind());

            if(ok) {
                m_shaderVertexLocation = getShaderProgram()->attributeLocation(VERTEXATTRIBUTE_NAME_IN_SHADER);
                m_shaderColorLocation = getShaderProgram()->attributeLocation(COLORATTRIBUTE_NAME_IN_SHADER);
                m_shaderInfoLocation = getShaderProgram()->attributeLocation(INFOATTRIBUTE_NAME_IN_SHADER);
                m_shaderNormalLocation = getShaderProgram()->attributeLocation(NORMALATTRIBUTE_NAME_IN_SHADER);
                getShaderProgram()->release();
            }
        }

        m_initialized = true;
    }

    return ok;
}

bool GenericRendererShaders::bindAndConfigureByDefaultShader(const IGraphicsDocument *document,
                                                             const FlagsPropertyManager& fpM)
{
    if(!bindShader(getShaderProgram()))
        return false;

    std::vector<QVector4D> flagsColor = fpM.getColorsForAllFlags(document);

    getShaderProgram()->setUniformValueArray(FLAGSCOLOR_NAME_IN_SHADER, &flagsColor[0], static_cast<int>(flagsColor.size()));

    return true;
}

bool GenericRendererShaders::bindShader()
{
    if(getShaderProgram() == NULL)
        return false;

    return getShaderProgram()->bind();
}

void GenericRendererShaders::releaseShader()
{
    releaseShader(getShaderProgram());
}

bool GenericRendererShaders::bindAndSetVertexAttributeToShader(QOpenGLBuffer &vertex)
{
    if((getShaderProgram() == NULL)
            || (getShaderVertexAttributeLocation() == -1)
            || !vertex.isCreated()
            || !vertex.bind())
        return false;


    bool ok = setVertexAttributeToShader(NULL);

    vertex.release();

    return ok;
}

bool GenericRendererShaders::setVertexAttributeToShader(const Vertex* pointer)
{
    if((getShaderProgram() == NULL)
            || (getShaderVertexAttributeLocation() == -1))
        return false;

    getShaderProgram()->enableAttributeArray(getShaderVertexAttributeLocation());
    getShaderProgram()->setAttributeArray(getShaderVertexAttributeLocation(), GL_FLOAT, pointer, 3, sizeof(Vertex));

    return true;
}

bool GenericRendererShaders::bindAndSetInfoAttributeToShader(QOpenGLBuffer &info, const QOpenGLContext *context)
{
    if((getShaderProgram() == NULL)
            || (getShaderInfoAttributeLocation() == -1)
            || !info.isCreated()
            || !info.bind())
        return false;

    bool ok = setInfoAttributeToShader(NULL, context);

    info.release();

    return ok;
}

bool GenericRendererShaders::setInfoAttributeToShader(const Info* pointer, const QOpenGLContext* context)
{
    if((getShaderProgram() == NULL)
            || (getShaderInfoAttributeLocation() == -1))
        return false;

    // we must use direct function to pass GL_FALSE to "normalized" parameter
    QOpenGLFunctions_2_0* func = context->versionFunctions<QOpenGLFunctions_2_0>();

    if(func == NULL)
        return false;

    // enableAttributeArray(m_shaderInfoLocation)
    func->glEnableVertexAttribArray(getShaderInfoAttributeLocation());

    // setAttributeArray(....)
    func->glVertexAttribPointer(getShaderInfoAttributeLocation(),
                                1,
                                GL_UNSIGNED_BYTE,
                                GL_FALSE,
                                sizeof(Info),
                                pointer);

    return true;
}

bool GenericRendererShaders::bindAndSetColorAttributeToShader(QOpenGLBuffer &color)
{
    if((getShaderProgram() == NULL)
            || (getShaderColorAttributeLocation() == -1)
            || !color.isCreated()
            || !color.bind()) {
        setUseColorAttribute(false);
        return false;
    }

    bool ok = setColorAttributeToShader(NULL);
    setUseColorAttribute(ok);

    color.release();

    return ok;
}

bool GenericRendererShaders::setColorAttributeToShader(const Color* pointer)
{
    if((getShaderProgram() == NULL)
            || (getShaderColorAttributeLocation() == -1))
        return false;

    getShaderProgram()->enableAttributeArray(getShaderColorAttributeLocation());
    getShaderProgram()->setAttributeArray(getShaderColorAttributeLocation(),
                                                GL_UNSIGNED_BYTE,
                                                pointer,
                                                4,
                                                sizeof(Color));
    setUseColorAttribute(true);

    return true;
}

bool GenericRendererShaders::setUseColorAttribute(bool enabled)
{
    if(getShaderProgram() == NULL)
        return false;

    getShaderProgram()->setUniformValue("useFrontColor", enabled);

    return true;
}

bool GenericRendererShaders::bindAndSetNormalAttributeToShader(QOpenGLBuffer &normal)
{
    if((getShaderProgram() == NULL)
            || (getShaderNormalAttributeLocation() == -1)
            || !normal.isCreated()
            || !normal.bind()) {
        setUseNormalAttribute(false);
        return false;
    }

    bool ok = setNormalAttributeToShader(NULL);
    setUseNormalAttribute(ok);

    normal.release();

    return ok;
}

bool GenericRendererShaders::setNormalAttributeToShader(const Normal *pointer)
{
    if((getShaderProgram() == NULL)
            || (getShaderNormalAttributeLocation() == -1))
        return false;

    getShaderProgram()->enableAttributeArray(getShaderNormalAttributeLocation());
    getShaderProgram()->setAttributeArray(getShaderNormalAttributeLocation(),
                                          GL_FLOAT,
                                          pointer,
                                          4,
                                          sizeof(Normal));

    setUseNormalAttribute(true);

    return true;
}

bool GenericRendererShaders::setUseNormalAttribute(bool enabled)
{
    if(getShaderProgram() == NULL)
        return false;

    getShaderProgram()->setUniformValue("useNormal", enabled);

    return true;
}

void GenericRendererShaders::disableAttributeOfShader()
{
    if(getShaderProgram() == NULL)
        return;

    getShaderProgram()->disableAttributeArray(getShaderVertexAttributeLocation());
    getShaderProgram()->disableAttributeArray(getShaderColorAttributeLocation());
    getShaderProgram()->disableAttributeArray(getShaderInfoAttributeLocation());
    getShaderProgram()->disableAttributeArray(getShaderNormalAttributeLocation());
}

int GenericRendererShaders::getShaderVertexAttributeLocation() const
{
    return m_shaderVertexLocation;
}

int GenericRendererShaders::getShaderColorAttributeLocation() const
{
    return m_shaderColorLocation;
}

int GenericRendererShaders::getShaderInfoAttributeLocation() const
{
    return m_shaderInfoLocation;
}

int GenericRendererShaders::getShaderNormalAttributeLocation() const
{
    return m_shaderNormalLocation;
}

void GenericRendererShaders::destroyGL()
{
    delete m_program;
    m_program = NULL;


    m_shaderVertexLocation = -1;
    m_shaderInfoLocation = -1;
    m_shaderColorLocation = -1;
    m_shaderNormalLocation = -1;

    m_initialized = false;
}

bool GenericRendererShaders::bindShader(QOpenGLShaderProgram *program)
{
    if(program == NULL)
        return false;

    return program->bind();
}

void GenericRendererShaders::releaseShader(QOpenGLShaderProgram *program)
{
    if(program != NULL)
        program->release();
}
