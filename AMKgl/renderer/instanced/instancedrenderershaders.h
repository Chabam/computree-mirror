#ifndef INSTANCEDRENDERERSHADERS_H
#define INSTANCEDRENDERERSHADERS_H

#include <QOpenGLFunctions_2_0>
#include <QOpenGLFunctions_3_3_Compatibility>
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

#include "tools/opengl/openglinfo.h"
#include "renderer/generic/generictypedef.h"
#include "renderer/tools/shaderstools.h"
#include "renderer/tools/flagspropertymanager.h"
#include "interfaces/igraphicsdocument.h"
#include "includecolor.h"
#include "amkglapp.h"

class DrawInfo;

/**
 * @brief Manage shaders to draw instanced elements (only compatible with opengl 3.3 minimum)
 *
 *        This class must be declared like this :
 *
 *        - if you want to map values with a 4x4 matrix :
 *        InstancedRendererShaders< Eigen::Matrix4f >(4, 4, GL_FLOAT, ...)
 *
 *        - if you want to map values with a 4x4 matrix buf you have only a 4x3 matrix :
 *        InstancedRendererShaders< Eigen::<float,3,4> >(3, 4, GL_FLOAT, ...)
 *
 *        Call to "glVertexAttribPointer" will be :
 *
 *        With VBO :
 *
 *        for (int i = 0; i < glValueColumnSize; ++i)
 *              glVertexAttribPointer(loc,
 *                                    glValueRowSize,
 *                                    glValueType,
 *                                    GL_FALSE,
 *                                    sizeof(ShaderParams),
 *                                    (void *)((value_type_size_computed * gl_value_row_size_converted) * i));
 *
 *        With pointer :
 *
 *        for (int i = 0; i < glValueColumnSize; ++i)
 *              glVertexAttribPointer(loc,
 *                                    glValueRowSize,
 *                                    glValueType,
 *                                    GL_FALSE,
 *                                    sizeof(ShaderParams),
 *                                    (&(*pointer)(0)) + (gl_value_row_size_converted * i));
 */
template<typename ShaderParams>
class InstancedRendererShaders
{
public:
    typedef ShaderParams        ShaderParamsType;
    typedef Basic::LocalColor   Color;

    /**
     * @brief Construct an empty object not initialized. You must call "init" method before use it.
     * @param glValueRowSize : Must be 1, 2, 3, 4. Additionally, the symbolic constant GL_BGRA is accepted.
     * @param glValueColumnSize : Must be 1, 2, 3 or 4.
     * @param glValueType : GL_BYTE, GL_UNSIGNED_BYTE,   GL_FLOAT,   etc....
     * @param vertexShaderPath : vertex shader filepath
     */
    InstancedRendererShaders(const GLint& glValueRowSize,
                             const GLint& glValueColumnSize,
                             const GLenum& glValueType,
                             const QString& vertexShaderPath);
    virtual ~InstancedRendererShaders();

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
    bool bindAndConfigureByDefaultShader(const IGraphicsDocument *document, const FlagsPropertyManager &fpM);

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
    bool bindAndSetVertexToShader(QOpenGLBuffer &vertex);

    /**
     * @brief Set the vertex pointer to attribute of the shader
     */
    bool setVertexToShader(const Eigen::Vector3f* pointer);

    /**
     * @brief Bind the info buffer object and set it to attribute of the shader
     */
    bool bindAndSetCustomVertexAttributeToShader(const std::string& name, GLint tupleSize, GLenum type, GLboolean normalized, GLsizei stride, QOpenGLBuffer &custom, const QOpenGLContext* context);

    /**
     * @brief Set the info pointer to attribute of the shader
     */
    bool setCustomVertexAttributeToShader(const std::string& name, GLint tupleSize, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer, const QOpenGLContext* context);

    /**
     * @brief Bind the param buffer object and set it to attribute of the shader
     */
    bool bindAndSetParamsAttributeToShader(QOpenGLBuffer &matrix, const QOpenGLContext* context);

    /**
     * @brief Set the param pointer to attribute of the shader
     */
    bool setParamsAttributeToShader(const ShaderParams* pointer, const QOpenGLContext* context);

    /**
     * @brief Bind the color buffer object and set it to attribute of the shader
     */
    bool bindAndSetColorAttributeToShader(QOpenGLBuffer &color, const QOpenGLContext* context);

    /**
     * @brief Set the color pointer to attribute of the shader
     */
    bool setColorAttributeToShader(const Color* pointer, const QOpenGLContext* context);

    /**
     * @brief Bind the info buffer object and set it to attribute of the shader
     */
    bool bindAndSetInfoAttributeToShader(QOpenGLBuffer &info, const QOpenGLContext* context);

    /**
     * @brief Set the info pointer to attribute of the shader
     */
    bool setInfoAttributeToShader(const ElementInfo* pointer, const QOpenGLContext* context);

    /**
     * @brief Disable all attribute array of shader
     */
    void disableAttributeOfShader(const QOpenGLContext* context);

    /**
     * @brief Returns the location of the attribute "vertex" in shader
     */
    int getShaderVertexLocation() const;

    /**
     * @brief Returns the location of the attribute "color" in shader
     */
    int getShaderColorAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "info" in shader
     */
    int getShaderInfoAttributeLocation() const;

    /**
     * @brief Returns the location of the attribute "params" in shader
     */
    int getShaderParamsAttributeLocation() const;

    /**
     * @brief Call to destroy shaders (delete from memory shaders set to this object !) and unitialized
     *        this object. You must init this object if you want to use it after that.
     */
    void destroyGL();

private:
    QString                 m_vertexShaderPath;

    bool                    m_initialized;
    QOpenGLShaderProgram*   m_program;

    int                     m_shaderVertexLocation;
    int                     m_shaderParamsLocation;
    int                     m_shaderColorLocation;
    int                     m_shaderInfoLocation;

    const GLint             m_glValueRowSize;
    const GLint             m_glValueColumnSize;
    const GLenum            m_glValueType;

    GLint                   m_sizeOfParams;
    GLint                   m_rowSize;

    /**
     * @brief Bind shader
     */
    bool bindShader(QOpenGLShaderProgram* program);

    /**
     * @brief Release the shader
     */
    void releaseShader(QOpenGLShaderProgram* program);
};

#include "instancedrenderershaders.hpp"

#endif // INSTANCEDRENDERERSHADERS_H
