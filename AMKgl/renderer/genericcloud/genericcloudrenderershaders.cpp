#include "genericcloudrenderershaders.h"

#include "amkglapp.h"

#include "tools/opengl/openglinfo.h"
#include "renderer/tools/shaderstools.h"

#include <QOpenGLFunctions_2_0>

GenericCloudRendererShaders::GenericCloudRendererShaders()
{
    m_currentMode = AMKgl::GCM_Object;
    m_initialized = false;
}

GenericCloudRendererShaders::~GenericCloudRendererShaders()
{
    destroyGL();
}

void GenericCloudRendererShaders::setCurrentMode(AMKgl::GenericCloudMode cloudMode)
{
    m_currentMode = cloudMode;
}

AMKgl::GenericCloudMode GenericCloudRendererShaders::getCurrentMode() const
{
    return m_currentMode;
}

QOpenGLShaderProgram* GenericCloudRendererShaders::getShaderProgram() const
{
    if(m_currentMode == AMKgl::GCM_Vertex)
        return m_byVertexInfos.m_program;

    return m_byObjectInfos.m_program;
}

bool GenericCloudRendererShaders::init(const QOpenGLContext* context)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(context)
#endif

    bool ok = true;

    if(!m_initialized) {

        // TODO
        ok = false;
        /*
        // opengl version > 2.0
        if(OpenGlInfo::staticCheckOpenglVersionMinimumVersion(context, 2, 0)) {
            m_byObjectInfos.m_program = new QOpenGLShaderProgram();

            QString finalSourceCode;
            QString error;
            ok = ShadersTools::staticMergeShadersFromSourceFile(QStringList() <<  "://shaders/common.vert" << "://shaders/genericcloud/byobject.vert", finalSourceCode, &error);

            if(!ok) {
                AMKglLOG->addErrorMessage(error);
                delete m_byObjectInfos.m_program;
                m_byObjectInfos.m_program = nullptr;
            }

            ShadersTools_CHECK_PROGRAM_ERROR(m_byObjectInfos.m_program, m_byObjectInfos.m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, finalSourceCode));
            ShadersTools_CHECK_PROGRAM_ERROR(m_byObjectInfos.m_program, m_byObjectInfos.m_program->addShaderFromSourceFile(QOpenGLShader::Geometry, "://shaders/genericcloud/byobject.glsl"));
            ShadersTools_CHECK_PROGRAM_ERROR(m_byObjectInfos.m_program, m_byObjectInfos.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "://shaders/genericcloud/byobject.frag"));
            ShadersTools_CHECK_PROGRAM_ERROR(m_byObjectInfos.m_program, m_byObjectInfos.m_program->link());
            ShadersTools_CHECK_PROGRAM_ERROR(m_byObjectInfos.m_program, m_byObjectInfos.m_program->bind());

            if(ok) {
                m_byObjectInfos.initLocation();
                m_byObjectInfos.m_program->release();
            }
        }

        if(ok) {
            m_byVertexInfos.m_program = new QOpenGLShaderProgram();

            QString finalSourceCode;
            QString error;
            ok = ShadersTools::staticMergeShadersFromSourceFile(QStringList() <<  "://shaders/common.vert" << "://shaders/genericcloud/byobject.vert", finalSourceCode, &error);

            if(!ok) {
                AMKglLOG->addErrorMessage(error);
                delete m_byVertexInfos.m_program;
                m_byVertexInfos.m_program = nullptr;
            }

            ShadersTools_CHECK_PROGRAM_ERROR(m_byVertexInfos.m_program, m_byVertexInfos.m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, finalSourceCode));
            ShadersTools_CHECK_PROGRAM_ERROR(m_byVertexInfos.m_program, m_byVertexInfos.m_program->link());
            ShadersTools_CHECK_PROGRAM_ERROR(m_byVertexInfos.m_program, m_byVertexInfos.m_program->bind());

            if(ok) {
                m_byVertexInfos.initLocation();
                m_byVertexInfos.m_program->release();
            } else {
                delete m_byObjectInfos.m_program;
                m_byObjectInfos.m_program = nullptr;
            }
        }*/

        m_initialized = true;
    }

    return ok;
}

bool GenericCloudRendererShaders::bindAndConfigureByDefaultShader(const IGraphicsDocument *document)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(document)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::bindShader()
{
    // TODO
    return false;
}

void GenericCloudRendererShaders::releaseShader()
{
    // TODO
}

bool GenericCloudRendererShaders::bindAndSetVertexAttributeToShader(QOpenGLBuffer &vertex)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(vertex)
#endif

    // TODO
    return false;
}

bool GenericCloudRendererShaders::setVertexAttributeToShader(const GenericCloudRendererShaders::Vertex *pointer)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(pointer)
#endif

    // TODO
    return false;
}

bool GenericCloudRendererShaders::bindAndSetInfoAttributeToShader(QOpenGLBuffer &info, const QOpenGLContext *context)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
    Q_UNUSED(context)
#endif

    // TODO
    return false;
}

bool GenericCloudRendererShaders::setInfoAttributeToShader(const GenericCloudRendererShaders::Info *pointer, const QOpenGLContext *context)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(pointer)
    Q_UNUSED(context)
#endif

    // TODO
    return false;
}

bool GenericCloudRendererShaders::bindAndSetColorAttributeToShader(QOpenGLBuffer &color)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(color)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::setColorAttributeToShader(const GenericCloudRendererShaders::Color *pointer)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(pointer)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::setUseColorAttribute(bool enabled)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(enabled)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::bindAndSetNormalAttributeToShader(QOpenGLBuffer &normal)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(normal)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::setNormalAttributeToShader(const GenericCloudRendererShaders::Normal *pointer)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(pointer)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::bindAndSetInfoAttributeToShader(QOpenGLTexture &info)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(info)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::bindAndSetColorAttributeToShader(QOpenGLTexture &color)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(color)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::bindAndSetNormalAttributeToShader(QOpenGLTexture &normal)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(normal)
#endif
    // TODO
    return false;
}

bool GenericCloudRendererShaders::setUseNormalAttribute(bool enabled)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(enabled)
#endif
    // TODO
    return false;
}

void GenericCloudRendererShaders::disableAttributeOfShader()
{
    // TODO
}

int GenericCloudRendererShaders::getShaderVertexAttributeLocation() const
{
    // TODO
    return -1;
}

int GenericCloudRendererShaders::getShaderColorAttributeLocation() const
{
    // TODO
    return -1;
}

int GenericCloudRendererShaders::getShaderInfoAttributeLocation() const
{
    // TODO
    return -1;
}

int GenericCloudRendererShaders::getShaderNormalAttributeLocation() const
{
    // TODO
    return -1;
}

void GenericCloudRendererShaders::destroyGL()
{
    delete m_byVertexInfos.m_program;
    m_byVertexInfos.m_program = nullptr;
    m_byVertexInfos.resetLocation();

    delete m_byObjectInfos.m_program;
    m_byObjectInfos.m_program = nullptr;
    m_byObjectInfos.resetLocation();

    m_initialized = false;
}

void GenericCloudRendererShaders::ByVertexShader::resetLocation()
{
    m_vertexLocation = -1;
    m_infoLocation = -1;
    m_colorLocation = -1;
    m_normalLocation = -1;
}

void GenericCloudRendererShaders::ByVertexShader::initLocation()
{
    // TODO
}

void GenericCloudRendererShaders::ByObjectShader::resetLocation()
{
    m_vertexLocation = -1;
    m_infoLocation = -1;
    m_colorLocation = -1;
    m_normalLocation = -1;
    m_nPrimitiveLocation = -1;
    m_nAdjustValueForXXXIndexLocation = -1;
}

void GenericCloudRendererShaders::ByObjectShader::initLocation()
{
    // TODO
}
