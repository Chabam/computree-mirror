#ifndef GENERICCLOUDRENDERERSHADERS_H
#define GENERICCLOUDRENDERERSHADERS_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>

#include "Eigen/Core"
#include "renderer/generic/generictypedef.h"
#include "genericcloudmode.h"
#include "renderer/tools/dumbarrayobjectmanager.h"

class IGraphicsDocument;

/**
 * @brief Manage shaders to draw elements from global cloud (face, edge) composed of points/colors/normals/infos
 */
class GenericCloudRendererShaders
{
public:
    typedef DumbArrayObjectManager  ArrayObjectManagerType; // TODO : do VAO for GenericCloud
    typedef Basic::LocalVertex      Vertex;
    typedef Basic::LocalColor       Color;
    typedef Basic::LocalInfo        Info;
    typedef Basic::LocalNormal      Normal;

    GenericCloudRendererShaders();
    ~GenericCloudRendererShaders();

    /**
     * @brief Call (with a valid current opengl context) to init shaders. If it was already initialized it will do nothing, so you can call it
     *        many times if you want.
     * @return true if it was sucessfully or already initialized, false otherwise
     */
    bool init(const QOpenGLContext *context);

    /**
     * @brief Set the current mode to use.
     * @warning You must change it before use methods of type "bindAndSet..." or "set..." otherwise methods will return false
     *          and will do nothing if the current mode is not correct.
     */
    void setCurrentMode(AMKgl::GenericCloudMode cloudMode);

    /**
     * @brief Return the current mode used
     */
    AMKgl::GenericCloudMode getCurrentMode() const;

    /**
     * @brief Returns the shader to use. nullptr if it was not created.
     */
    QOpenGLShaderProgram* getShaderProgram() const;

    /**
     * @brief Bind and configure the shader
     */
    bool bindAndConfigureByDefaultShader(const IGraphicsDocument *document);

    /**
     * @brief Just bind the shader
     */
    bool bindShader();

    /**
     * @brief Release the shader
     */
    void releaseShader();

    ///////////////////// VERTEX MODE ///////////////////

    /**
     * @brief Bind the info buffer object and set it to attribute of the shader
     */
    bool bindAndSetInfoAttributeToShader(QOpenGLBuffer &info, const QOpenGLContext *context);

    /**
     * @brief Set the info pointer to attribute of the shader
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
     * @brief Bind the normal buffer object and set it to attribute of the shader
     */
    bool bindAndSetNormalAttributeToShader(QOpenGLBuffer &normal);

    /**
     * @brief Set the normal pointer to attribute of the shader
     */
    bool setNormalAttributeToShader(const Normal* pointer);

    ///////////////////// OBJECT MODE ///////////////////

    /**
     * @brief Bind the info buffer object and set it to attribute of the shader
     */
    bool bindAndSetInfoAttributeToShader(QOpenGLTexture &info);

    /**
     * @brief Bind the color buffer object and set it to attribute of the shader
     */
    bool bindAndSetColorAttributeToShader(QOpenGLTexture &color);

    /**
     * @brief Bind the normal buffer object and set it to attribute of the shader
     */
    bool bindAndSetNormalAttributeToShader(QOpenGLTexture &normal);

    ///////////////////// ALL MODE ///////////////////

    /**
     * @brief Bind the vertex buffer object and set it to attribute of the shader
     */
    bool bindAndSetVertexAttributeToShader(QOpenGLBuffer &vertex);

    /**
     * @brief Set the vertex pointer to attribute of the shader
     */
    bool setVertexAttributeToShader(const Vertex* pointer);

    /**
     * @brief Set if you want to use the color attribute or not
     * @param enabled : true to use it
     */
    bool setUseColorAttribute(bool enabled);

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
    struct ByVertexShader {
        ByVertexShader() {
            m_program = nullptr;
            resetLocation();
        }

        ~ByVertexShader() { delete m_program; }

        void resetLocation();
        void initLocation();

        QOpenGLShaderProgram*   m_program;
        int                     m_vertexLocation;
        int                     m_infoLocation;
        int                     m_colorLocation;
        int                     m_normalLocation;
    };

    struct ByObjectShader {
        ByObjectShader() {
            m_program = nullptr;
        }

        ~ByObjectShader() { delete m_program; }

        void resetLocation();
        void initLocation();

        QOpenGLShaderProgram*   m_program;
        int                     m_vertexLocation;
        int                     m_indexLocation;
        int                     m_infoLocation;
        int                     m_colorLocation;
        int                     m_normalLocation;
        int                     m_nPrimitiveLocation;
        int                     m_nAdjustValueForXXXIndexLocation;
    };

    bool                    m_initialized;

    ByObjectShader          m_byObjectInfos;
    ByVertexShader          m_byVertexInfos;

    AMKgl::GenericCloudMode m_currentMode;


    /**
     * @brief Bind shader
     */
    bool bindShader(QOpenGLShaderProgram* program);

    /**
     * @brief Release the shader
     */
    void releaseShader(QOpenGLShaderProgram* program);
};

#endif // GENERICCLOUDRENDERERSHADERS_H
