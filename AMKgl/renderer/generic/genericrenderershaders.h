#ifndef GENERICRENDERERSHADERS_H
#define GENERICRENDERERSHADERS_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#else
#pragma GCC diagnostic ignored "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "Eigen/Core"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic warning "-Wdeprecated-copy"
#else
#pragma GCC diagnostic warning "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#endif

#include "genericobjectarrayobjectmanager.h"
#include "generictypedef.h"

class IGraphicsDocument;
class FlagsPropertyManager;

/**
 * @brief Manage shaders to draw simple elements composed of points/colors/normals/infos
 */
class GenericRendererShaders
{
public:
    typedef GenericObjectArrayObjectManager ArrayObjectManagerType;
    typedef Basic::LocalVertex              Vertex;
    typedef Basic::LocalColor               Color;
    typedef Basic::LocalInfo                Info;
    typedef Basic::LocalNormal              Normal;

    GenericRendererShaders();
    ~GenericRendererShaders();

    /**
     * @brief Call (with a valid current opengl context) to init shaders. If it was already initialized it will do nothing, so you can call it
     *        many times if you want.
     * @return true if it was sucessfully or already initialized, false otherwise
     */
    bool init(const QOpenGLContext *context);

    /**
     * @brief Returns the shader to use. nullptr if it was not created.
     */
    QOpenGLShaderProgram* getShaderProgram() const;

    /**
     * @brief Bind and configure the shader
     */
    bool bindAndConfigureByDefaultShader(const IGraphicsDocument *document,
                                         const FlagsPropertyManager& fpM);

    /**
     * @brief Just bind the shader
     */
    bool bindShader();

    /**
     * @brief Release the shader
     */
    void releaseShader();

    /**
     * @brief Bind the vertex buffer object and set it to attribute of the shader
     */
    bool bindAndSetVertexAttributeToShader(QOpenGLBuffer &vertex);

    /**
     * @brief Set the vertex pointer to attribute of the shader
     */
    bool setVertexAttributeToShader(const Vertex* pointer);

    /**
     * @brief Bind the param buffer object and set it to attribute of the shader
     */
    bool bindAndSetInfoAttributeToShader(QOpenGLBuffer &info, const QOpenGLContext *context);

    /**
     * @brief Set the param pointer to attribute of the shader
     */
    bool setInfoAttributeToShader(const Info* pointer, const QOpenGLContext *context);

    /**
     * @brief Bind the color buffer object and set it to attribute of the shader
     */
    bool bindAndSetColorAttributeToShader(QOpenGLBuffer &color);

    /**
     * @brief Set the color pointer to attribute of the shader
     */
    bool setColorAttributeToShader(const Color* pointer);

    /**
     * @brief Set if you want to use the color attribute or not
     * @param enabled : true to use it
     */
    bool setUseColorAttribute(bool enabled);

    /**
     * @brief Bind the normal buffer object and set it to attribute of the shader
     */
    bool bindAndSetNormalAttributeToShader(QOpenGLBuffer &normal);

    /**
     * @brief Set the normal pointer to attribute of the shader
     */
    bool setNormalAttributeToShader(const Normal* pointer);

    /**
     * @brief Set if you want to use the normal attribute or not
     * @param enabled : true to use it
     */
    bool setUseNormalAttribute(bool enabled);

    /**
     * @brief Disable all attribute array of shader
     */
    void disableAttributeOfShader();

    /**
     * @brief Returns the location of the attribute "vertex" in shader
     */
    int getShaderVertexAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "color" in shader
     */
    int getShaderColorAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "info" in shader
     */
    int getShaderInfoAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "normal" in shader
     */
    int getShaderNormalAttributeLocation() const;

    /**
     * @brief Call to destroy shaders (delete from memory shaders set to this object !) and unitialized
     *        this object. You must init this object if you want to use it after that.
     */
    void destroyGL();

private:
    bool                    m_initialized;
    QOpenGLShaderProgram*   m_program;

    int                     m_shaderVertexLocation;
    int                     m_shaderInfoLocation;
    int                     m_shaderColorLocation;
    int                     m_shaderNormalLocation;

    /**
     * @brief Bind shader
     */
    bool bindShader(QOpenGLShaderProgram* program);

    /**
     * @brief Release the shader
     */
    void releaseShader(QOpenGLShaderProgram* program);
};

#endif // GENERICRENDERERSHADERS_H
