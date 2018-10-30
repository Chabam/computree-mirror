#ifndef POINTRENDERERSHADERS_H
#define POINTRENDERERSHADERS_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>

#include "definepoint.h"
#include "definecolor.h"
#include "definenormal.h"

#include "elementinfo.h"

#include "pointcloudarrayobjectmanager.h"

class IGraphicsDocument;
class FlagsPropertyManager;

using namespace AMKgl;

/**
 * @brief Manage shaders to draw points and normals.
 */
class PointRendererShaders
{
public:
    typedef PointCloudArrayObjectManager ArrayObjectManagerType;

    /**
     * @brief Construct an empty object not initialized
     */
    PointRendererShaders();
    ~PointRendererShaders();

    /**
     * @brief Call to init shaders. If it was already initialized it will do nothing, so you can call it
     *        many times if you want.
     */
    bool init(const QOpenGLContext *context);

    /**
     * @brief Returns the shader to use for points
     */
    QOpenGLShaderProgram* getPointsShaderProgram() const;

    /**
     * @brief Returns the shader to use for normals
     */
    QOpenGLShaderProgram* getNormalsShaderProgram() const;

    /**
     * @brief Bind and configure the shader of points
     */
    bool bindAndConfigureByDefaultPointsShader(const IGraphicsDocument *document,
                                               const FlagsPropertyManager &fpM);

    /**
     * @brief Just bind the points shader
     */
    bool bindPointShader();

    /**
     * @brief Release the shader uses for draw points
     */
    void releasePointsShader();

    /**
     * @brief Bind and configure the shader of normals
     */
    bool bindAndConfigureByDefaultNormalsShader(const IGraphicsDocument *document,
                                                const FlagsPropertyManager& fpM);

    /**
     * @brief Just bind the normals shader
     */
    bool bindNormalsShader();

    /**
     * @brief Release the shader uses for draw normals
     */
    void releaseNormalsShader();

    /**
     * @brief Bind the vertex buffer object and set it to attribute of the shader of points
     */
    bool bindAndSetVertexAttributeToPointsShader(QOpenGLBuffer &vertex);

    /**
     * @brief Set the vertex pointer to attribute of the shader
     */
    bool setVertexAttributeToPointsShader(const FloatPoint* point);

    /**
     * @brief Bind the info buffer object and set it to attribute of the shader of points
     */
    bool bindAndSetInfoAttributeToPointsShader(QOpenGLBuffer &info, const QOpenGLContext* context);

    /**
     * @brief Set the info pointer to attribute of the shader
     */
    bool setInfoAttributeToPointsShader(const ElementInfo* pointer, const QOpenGLContext *context);

    /**
     * @brief Bind the color buffer object and set it to attribute of the shader of points
     */
    bool bindAndSetColorAttributeToPointsShader(QOpenGLBuffer &color);

    /**
     * @brief Set the color pointer to attribute of the shader
     */
    bool setColorAttributeToPointsShader(const GlobalColor* pointer);

    /**
     * @brief Set if you want to use the color attribute or not
     * @param enabled : true to use it
     */
    bool setUseColorAttribute(bool enabled);

    /**
     * @brief Disable all attribute array of points shader
     */
    void disableAttributeOfPointsShader();

    /**
     * @brief Bind the vertex buffer object and set it to attribute of the shader of normals
     */
    bool bindAndSetVertexAttributeToNormalsShader(QOpenGLBuffer &vertex);

    /**
     * @brief Set the vertex pointer to attribute of the shader
     */
    bool setVertexAttributeToNormalsShader(const FloatPoint* point);

    /**
     * @brief Bind the normal buffer object and set it to attribute of the shader of normals
     */
    bool bindAndSetNormalAttributeToNormalsShader(QOpenGLBuffer &normal);

    /**
     * @brief Set the normal pointer to attribute of the shader
     */
    bool setNormalAttributeToNormalsShader(const GlobalNormal* pointer);

    /**
     * @brief Bind the normal buffer object and set it to attribute of the shader of normals
     */
    bool bindAndSetInfoAttributeToNormalsShader(QOpenGLBuffer &info, const QOpenGLContext* context);

    /**
     * @brief Set the info pointer to attribute of the shader
     */
    bool setInfoAttributeToNormalsShader(const ElementInfo* pointer, const QOpenGLContext* context);

    /**
     * @brief Disable all attribute array of normals shader
     */
    void disableAttributeOfNormalsShader();

    /**
     * @brief Returns the location of the attribute "vertex" in points shader
     */
    int getPointsShaderVertexAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "color" in points shader
     */
    int getPointsShaderColorAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "info" in points shader
     */
    int getPointsShaderInfoAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "vertex" in points shader
     */
    int getNormalsShaderVertexAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "normal" in points shader
     */
    int getNormalsShaderNormalAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "info" in points shader
     */
    int getNormalsShaderInfoAttributeLocation() const;

    /**
     * @brief Call to destroy shaders (delete from memory shaders set to this object !) and unitialized
     *        this object. You must init this object if you want to use it after that.
     */
    void destroyGL();

private:
    bool                    m_initialized;
    QOpenGLShaderProgram*   m_pointsProgram;
    QOpenGLShaderProgram*   m_normalsProgram;

    int                     m_shaderPointsVertexLocation;
    int                     m_shaderPointsInfoLocation;
    int                     m_shaderPointsColorLocation;

    int                     m_shaderNormalsVertexLocation;
    int                     m_shaderNormalsNormalLocation;
    int                     m_shaderNormalsInfoLocation;

    /**
     * @brief Bind shader
     */
    bool bindShader(QOpenGLShaderProgram* program);

    /**
     * @brief Release the shader
     */
    void releaseShader(QOpenGLShaderProgram* program);
};

#endif // POINTRENDERERSHADERS_H
