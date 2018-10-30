#include "pointrenderershaders.h"

#include "amkglapp.h"
#include "interfaces/igraphicsdocument.h"
#include "renderer/tools/flagspropertymanager.h"
#include "renderer/tools/shaderstools.h"
#include "tools/opengl/openglinfo.h"

#include "includepoint.h"
#include "includecolor.h"
#include "includenormal.h"

#include <QOpenGLFunctions_2_0>

#define VERTEXATTRIBUTE_NAME_IN_SHADER "vertex"
#define COLORATTRIBUTE_NAME_IN_SHADER "color"
#define INFOATTRIBUTE_NAME_IN_SHADER "info"
#define FLAGSCOLOR_NAME_IN_SHADER "flagsColors"

#define NORMALATTRIBUTE_NAME_IN_SHADER "normal"

#define NORMALLENGTH_NAME_IN_SHADER "normalLength"
#define NORMALCOLOR_NAME_IN_SHADER "normalColor"

PointRendererShaders::PointRendererShaders()
{
    m_pointsProgram = NULL;
    m_normalsProgram = NULL;
    m_shaderPointsVertexLocation = -1;
    m_shaderPointsInfoLocation = -1;
    m_shaderPointsColorLocation = -1;
    m_shaderNormalsNormalLocation = -1;
    m_shaderNormalsVertexLocation = -1;
    m_shaderNormalsInfoLocation = -1;

    m_initialized = false;
}

PointRendererShaders::~PointRendererShaders()
{
    destroyGL();
}

QOpenGLShaderProgram* PointRendererShaders::getPointsShaderProgram() const
{
    return m_pointsProgram;
}

QOpenGLShaderProgram* PointRendererShaders::getNormalsShaderProgram() const
{
    return m_normalsProgram;
}

bool PointRendererShaders::init(const QOpenGLContext* context)
{
    bool ok = true;

    if(!m_initialized) {

        // opengl version > 3.2
        if(OpenGlInfo::staticCheckOpenglVersionMinimumVersion(context, 3, 2)) {
            m_normalsProgram = new QOpenGLShaderProgram();

            QString finalSourceCode;
            QString error;
            ok = ShadersTools::staticMergeShadersFromSourceFile(QStringList() <<  "://shaders/common.vert" << "://shaders/points/normaldraw.vert", finalSourceCode, &error);

            if(!ok) {
                AMKglLOG->addErrorMessage(error);
                delete m_normalsProgram;
                m_normalsProgram = NULL;
                return false;
            }

            ShadersTools_CHECK_PROGRAM_ERROR(m_normalsProgram, m_normalsProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, finalSourceCode));
            ShadersTools_CHECK_PROGRAM_ERROR(m_normalsProgram, m_normalsProgram->addShaderFromSourceFile(QOpenGLShader::Geometry, "://shaders/points/normaldraw.glsl"));
            ShadersTools_CHECK_PROGRAM_ERROR(m_normalsProgram, m_normalsProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "://shaders/points/normaldraw.frag"));

            ShadersTools_CHECK_PROGRAM_ERROR(m_normalsProgram, m_normalsProgram->link());
            ShadersTools_CHECK_PROGRAM_ERROR(m_normalsProgram, m_normalsProgram->bind());

            if(ok) {
                m_shaderNormalsVertexLocation = getNormalsShaderProgram()->attributeLocation(VERTEXATTRIBUTE_NAME_IN_SHADER);
                m_shaderNormalsNormalLocation = getNormalsShaderProgram()->attributeLocation(NORMALATTRIBUTE_NAME_IN_SHADER);
                m_shaderNormalsInfoLocation = getNormalsShaderProgram()->attributeLocation(INFOATTRIBUTE_NAME_IN_SHADER);
                getNormalsShaderProgram()->release();
            }
        }

        ok = true;
        m_pointsProgram = new QOpenGLShaderProgram();

        QString finalSourceCode;
        QString error;
        ok = ShadersTools::staticMergeShadersFromSourceFile(QStringList() <<  "://shaders/common.vert" << "://shaders/points/simple.vert", finalSourceCode, &error);

        if(!ok) {
            AMKglLOG->addErrorMessage(error);
            delete m_pointsProgram;
            m_pointsProgram = NULL;
            return false;
        }

        ShadersTools_CHECK_PROGRAM_ERROR(m_pointsProgram, m_pointsProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, finalSourceCode));

        ShadersTools_CHECK_PROGRAM_ERROR(m_pointsProgram, m_pointsProgram->link());
        ShadersTools_CHECK_PROGRAM_ERROR(m_pointsProgram, m_pointsProgram->bind());

        if(ok) {
            m_shaderPointsVertexLocation = getPointsShaderProgram()->attributeLocation(VERTEXATTRIBUTE_NAME_IN_SHADER);
            m_shaderPointsColorLocation = getPointsShaderProgram()->attributeLocation(COLORATTRIBUTE_NAME_IN_SHADER);
            m_shaderPointsInfoLocation = getPointsShaderProgram()->attributeLocation(INFOATTRIBUTE_NAME_IN_SHADER);
            getPointsShaderProgram()->release();
        }

        m_initialized = true;
    }

    return ok;
}

bool PointRendererShaders::bindAndConfigureByDefaultPointsShader(const IGraphicsDocument *document,
                                                                 const FlagsPropertyManager& fpM)
{
    if(!bindShader(getPointsShaderProgram()))
        return false;

    std::vector<QVector4D> flagsColor = fpM.getColorsForAllFlags(document);

    getPointsShaderProgram()->setUniformValueArray(FLAGSCOLOR_NAME_IN_SHADER, &flagsColor[0], static_cast<int>(flagsColor.size()));

    return true;
}

bool PointRendererShaders::bindPointShader()
{
    if(getPointsShaderProgram() == NULL)
        return false;

    return getPointsShaderProgram()->bind();
}

void PointRendererShaders::releasePointsShader()
{
    releaseShader(getPointsShaderProgram());
}

bool PointRendererShaders::bindAndConfigureByDefaultNormalsShader(const IGraphicsDocument *document,
                                                                  const FlagsPropertyManager& fpM)
{
    if((document == NULL)
            || !document->mustShowNormals()
            || (document->getNormalsLength() <= 0)
            || !bindShader(getNormalsShaderProgram()))
        return false;

    QColor normalColor = document->getNormalsColor();

    std::vector<QVector4D> flagsColor = fpM.getColorsForAllFlags(document);

    // flags color
    getPointsShaderProgram()->setUniformValueArray(FLAGSCOLOR_NAME_IN_SHADER, &flagsColor[0], static_cast<int>(flagsColor.size()));

    // normal length
    getNormalsShaderProgram()->setUniformValue(NORMALLENGTH_NAME_IN_SHADER, document->getNormalsLength());

    // normal color
    getNormalsShaderProgram()->setUniformValue(NORMALCOLOR_NAME_IN_SHADER, normalColor.redF(), normalColor.greenF(), normalColor.blueF(), normalColor.alphaF());

    return true;
}

bool PointRendererShaders::bindNormalsShader()
{
    if(getNormalsShaderProgram() == NULL)
        return false;

    return getNormalsShaderProgram()->bind();
}

void PointRendererShaders::releaseNormalsShader()
{
    releaseShader(getNormalsShaderProgram());
}

bool PointRendererShaders::bindAndSetVertexAttributeToPointsShader(QOpenGLBuffer &vertex)
{
    if((getPointsShaderProgram() == NULL)
            || (getPointsShaderVertexAttributeLocation() == -1)
            || !vertex.isCreated()
            || !vertex.bind())
        return false;

    bool ok = setVertexAttributeToPointsShader(NULL);

    vertex.release();

    return ok;
}

bool PointRendererShaders::setVertexAttributeToPointsShader(const FloatPoint *point)
{
    if((getPointsShaderProgram() == NULL)
            || (getPointsShaderVertexAttributeLocation() == -1))
        return false;

    getPointsShaderProgram()->enableAttributeArray(getPointsShaderVertexAttributeLocation());
    getPointsShaderProgram()->setAttributeArray(getPointsShaderVertexAttributeLocation(), GL_FLOAT, point, 3, sizeof(FloatPoint));

    return true;
}

bool PointRendererShaders::bindAndSetInfoAttributeToPointsShader(QOpenGLBuffer &info, const QOpenGLContext* context)
{
    if((getPointsShaderProgram() == NULL)
            || (getPointsShaderInfoAttributeLocation() == -1)
            || !info.isCreated()
            || !info.bind())
        return false;

    bool ok = setInfoAttributeToPointsShader(NULL, context);

    info.release();

    return ok;
}

bool PointRendererShaders::setInfoAttributeToPointsShader(const ElementInfo *pointer, const QOpenGLContext* context)
{
    if((getPointsShaderProgram() == NULL)
            || (getPointsShaderInfoAttributeLocation() == -1))
        return false;

    // we must use direct function to pass GL_FALSE to "normalized" parameter
    QOpenGLFunctions_2_0* func = context->versionFunctions<QOpenGLFunctions_2_0>();

    if(func == NULL)
        return false;

    func->glEnableVertexAttribArray(getPointsShaderInfoAttributeLocation());
    func->glVertexAttribPointer(getPointsShaderInfoAttributeLocation(),
                                1,
                                GL_UNSIGNED_BYTE,
                                GL_FALSE,
                                sizeof(ElementInfo),
                                pointer);

    return true;
}

bool PointRendererShaders::bindAndSetColorAttributeToPointsShader(QOpenGLBuffer &color)
{
    if((getPointsShaderProgram() == NULL)
            || (getPointsShaderColorAttributeLocation() == -1)
            || !color.isCreated()
            || !color.bind()) {
        setUseColorAttribute(false);
        return false;
    }

    bool ok = setColorAttributeToPointsShader(NULL);
    setUseColorAttribute(ok);

    color.release();

    return ok;
}

bool PointRendererShaders::setColorAttributeToPointsShader(const GlobalColor *pointer)
{
    if((getPointsShaderProgram() == NULL)
            || (getPointsShaderColorAttributeLocation() == -1))
        return false;

    getPointsShaderProgram()->enableAttributeArray(getPointsShaderColorAttributeLocation());
    getPointsShaderProgram()->setAttributeArray(getPointsShaderColorAttributeLocation(),
                                                GL_UNSIGNED_BYTE,
                                                pointer,
                                                4,
                                                sizeof(GlobalColor));

    setUseColorAttribute(true);

    return true;
}

bool PointRendererShaders::setUseColorAttribute(bool enabled)
{
    if(getPointsShaderProgram() == NULL)
        return false;

    getPointsShaderProgram()->setUniformValue("useFrontColor", enabled);

    return true;
}

void PointRendererShaders::disableAttributeOfPointsShader()
{
    if(getPointsShaderProgram() == NULL)
        return;

    getPointsShaderProgram()->disableAttributeArray(getPointsShaderVertexAttributeLocation());
    getPointsShaderProgram()->disableAttributeArray(getPointsShaderColorAttributeLocation());
    getPointsShaderProgram()->disableAttributeArray(getPointsShaderInfoAttributeLocation());
}

bool PointRendererShaders::bindAndSetVertexAttributeToNormalsShader(QOpenGLBuffer &vertex)
{
    if((getNormalsShaderProgram() == NULL)
            || (getNormalsShaderVertexAttributeLocation() == -1)
            || !vertex.isCreated()
            || !vertex.bind())
        return false;

    bool ok = setVertexAttributeToNormalsShader(NULL);

    vertex.release();

    return ok;
}

bool PointRendererShaders::setVertexAttributeToNormalsShader(const FloatPoint *point)
{
    if((getNormalsShaderProgram() == NULL)
            || (getNormalsShaderVertexAttributeLocation() == -1))
        return false;

    getNormalsShaderProgram()->enableAttributeArray(getNormalsShaderVertexAttributeLocation());
    getNormalsShaderProgram()->setAttributeArray(getNormalsShaderVertexAttributeLocation(), GL_FLOAT, point, 3, sizeof(FloatPoint));

    return true;
}

void PointRendererShaders::disableAttributeOfNormalsShader()
{
    if(getNormalsShaderProgram() == NULL)
        return;

    getNormalsShaderProgram()->disableAttributeArray(getNormalsShaderVertexAttributeLocation());
    getNormalsShaderProgram()->disableAttributeArray(getNormalsShaderNormalAttributeLocation());
    getNormalsShaderProgram()->disableAttributeArray(getNormalsShaderInfoAttributeLocation());
}

bool PointRendererShaders::bindAndSetNormalAttributeToNormalsShader(QOpenGLBuffer &normal)
{
    if((getNormalsShaderProgram() == NULL)
            || (getNormalsShaderNormalAttributeLocation() == -1)
            || !normal.isCreated()
            || !normal.bind())
        return false;

    bool ok = setNormalAttributeToNormalsShader(NULL);

    normal.release();

    return ok;
}

bool PointRendererShaders::setNormalAttributeToNormalsShader(const GlobalNormal *pointer)
{
    if((getNormalsShaderProgram() == NULL)
            || (getNormalsShaderNormalAttributeLocation() == -1))
        return false;

    getNormalsShaderProgram()->enableAttributeArray(getNormalsShaderNormalAttributeLocation());
    getNormalsShaderProgram()->setAttributeArray(getNormalsShaderNormalAttributeLocation(), GL_FLOAT, pointer, 4, sizeof(GlobalNormal));

    return true;
}

bool PointRendererShaders::bindAndSetInfoAttributeToNormalsShader(QOpenGLBuffer &info, const QOpenGLContext* context)
{
    if((getNormalsShaderProgram() == NULL)
            || (getNormalsShaderInfoAttributeLocation() == -1)
            || !info.isCreated()
            || !info.bind())
        return false;

    bool ok = setInfoAttributeToNormalsShader(NULL, context);

    info.release();

    return ok;
}

bool PointRendererShaders::setInfoAttributeToNormalsShader(const ElementInfo *pointer, const QOpenGLContext* context)
{
    if((getNormalsShaderProgram() == NULL)
            || (getNormalsShaderInfoAttributeLocation() == -1))
        return false;

    // we must use direct function to pass GL_FALSE to "normalized" parameter
    QOpenGLFunctions_2_0* func = context->versionFunctions<QOpenGLFunctions_2_0>();

    if(func == NULL)
        return false;

    // enableAttributeArray(m_shaderInfoLocation)
    func->glEnableVertexAttribArray(getNormalsShaderInfoAttributeLocation());

    // setAttributeArray(....)
    func->glVertexAttribPointer(getNormalsShaderInfoAttributeLocation(),
                                1,
                                GL_UNSIGNED_BYTE,
                                GL_FALSE,
                                sizeof(ElementInfo),
                                pointer);

    return true;
}

int PointRendererShaders::getPointsShaderVertexAttributeLocation() const
{
    return m_shaderPointsVertexLocation;
}

int PointRendererShaders::getPointsShaderColorAttributeLocation() const
{
    return m_shaderPointsColorLocation;
}

int PointRendererShaders::getPointsShaderInfoAttributeLocation() const
{
    return m_shaderPointsInfoLocation;
}

int PointRendererShaders::getNormalsShaderVertexAttributeLocation() const
{
    return m_shaderNormalsVertexLocation;
}

int PointRendererShaders::getNormalsShaderNormalAttributeLocation() const
{
    return m_shaderNormalsNormalLocation;
}

int PointRendererShaders::getNormalsShaderInfoAttributeLocation() const
{
    return m_shaderNormalsInfoLocation;
}

void PointRendererShaders::destroyGL()
{
    delete m_pointsProgram;
    m_pointsProgram = NULL;

    delete m_normalsProgram;
    m_normalsProgram = NULL;

    m_shaderPointsVertexLocation = -1;
    m_shaderPointsInfoLocation = -1;
    m_shaderPointsColorLocation = -1;
    m_shaderNormalsNormalLocation = -1;
    m_shaderNormalsVertexLocation = -1;
    m_shaderNormalsInfoLocation = -1;

    m_initialized = false;
}

bool PointRendererShaders::bindShader(QOpenGLShaderProgram *program)
{
    if(program == NULL)
        return false;

    return program->bind();
}

void PointRendererShaders::releaseShader(QOpenGLShaderProgram *program)
{
    if(program != NULL)
        program->release();
}
