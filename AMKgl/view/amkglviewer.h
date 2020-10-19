#ifndef AMKGLVIEWER_H
#define AMKGLVIEWER_H


#include <qglviewer.h>
#include <camera.h>

#include <Eigen/Core>

#include "drawinfo.h"
#include "interfaces/igraphicsdocument.h"
#include "definegraphicsview.h"

class PermanentItemSceneByModel;
class ActionPickAnyElements;

/**
 * @brief This class is the viewer for this engine. You must inherit it to
 *        overload virtual pure methods to let the engine access to document,
 *        graphicsview, etc.... and be able to redraw the view.
 */
class AMKglViewer : public QGLViewer, protected QOpenGLFunctions {

    Q_OBJECT

public:

    typedef qglviewer::Camera           CameraType;
    typedef qglviewer::Vec              VecType;
    typedef qglviewer::Quaternion       QuatType;
    typedef PermanentItemSceneByModel   PermanentSceneToRender;

    enum RedrawMethod {
        RedrawOnlyOverlay,  // redraw only overlay (rectangle/polygon selection, text, etc...)
        RedrawOnly3D,       // redraw only 3D
        RedrawAll           // redraw all
    };

    AMKglViewer(const IGraphicsDocument* doc, QWidget *parent);
    ~AMKglViewer();

    /**
     * @brief Redefine this method to returns your graphics view used to draw Items
     */
    virtual AMKgl::GraphicsView* getGraphicsView() const = 0;

    /**
     * @brief Set the document of this view
     */
    void setDocument(const IGraphicsDocument *doc);

    /**
     * @brief Returns the document of this view
     */
    IGraphicsDocument* getDocument() const;

    /**
     * @brief Set the scene that draw permanent to render
     */
    void setPermanentSceneToRender(const PermanentSceneToRender *scene);

    /**
     * @brief Returns the item scene used by this view
     */
    PermanentSceneToRender* getPermanentSceneToRender() const;

    /**
     * @brief Returns the picking action used by this viewer
     */
    ActionPickAnyElements* getPickingAction() const;

    /**
     * @brief Returns the camera's position
     */
    Eigen::Vector3d getCameraPosition() const;

    /**
     * @brief Returns the camera's model view matrix
     */
    Eigen::Matrix4d getCameraModelViewMatrix() const;

    /**
     * @brief Call to draw a text in 2D on this view
     */
    void drawText(int x, int y, const QString& text, const QFont& fnt=QFont());

    /**
     * @brief Returns true if fast draw mode is currently used
     */
    bool isFastDrawModeCurrentlyUsed() const;

    /**
     * @brief Returns the draw info object
     */
    DrawInfo* getDrawInfo() const;

    /**
     * @brief Returns the 2D painter
     */
    QPainter* getPainter() const;

    /**
     * @brief Returns the opengl context
     */
    QOpenGLContext* getNewOpenGlContext() const;

    /**
     * @brief Call it to take a screenshot
     */
    void takeScreenshot();

public slots:
    /**
     * @brief Call to redraw all
     */
    void redraw(RedrawMethod m = RedrawAll);

protected:
    void init() override;
    void paintGL() override;
    void paintEvent(QPaintEvent *e) override;
    void preDraw() override;
    void draw() override;
    void fastDraw() override;
    void postDraw() override;
    virtual void subPostDraw(DrawInfo &info) { Q_UNUSED(info) }
    virtual void drawOverlay(QPainter& painter, DrawInfo &info);

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void showEvent(QShowEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;

    /**
     * @brief Enable/Disable the debug mode.
     *        The debug mode show statistic on the screen, etc...
     */
    void setDebugModeEnabled(bool enableIt);

    /**
     * @brief Returns true if debug mode is enabled
     */
    bool isDebugModeEnabled() const;

private:

    class SpecialCam : public qglviewer::Camera
    {
    public:
        SpecialCam() : qglviewer::Camera(), m_useNormalCamera(true) {}
        SpecialCam(const qglviewer::Camera& other) : qglviewer::Camera(other), m_useNormalCamera(true) {}

        void loadModelViewMatrix(bool reset=true) const;

        bool    m_useNormalCamera;
    };

    struct RenderParams {
        RenderParams() {
            fastDraw = false;
            nMouseButtonPressed = 0;
        }

        bool    fastDraw;
        int     nMouseButtonPressed;
    };

    IGraphicsDocument*          m_document;
    PermanentSceneToRender*     m_permanentItemScene;
    QOpenGLContext*             m_newOpenglContext;
    SpecialCam*                 m_camera;
    RenderParams                m_params;
    QTimer                      m_fastDrawTimer;
    bool                        m_inFastDraw;
    DrawInfo*                   m_drawInfo;
    QPainter*                   m_painter;
    ActionPickAnyElements*      m_pickingAction;
    bool                        m_debugModeEnabled;
    QGLFramebufferObject*       m_glBuffer;
    bool                        m_takeScreenshot;
    double                      m_screenshotMultiplicationFactor;

    /**
     * @brief Check if opengl has send errors and add it to log if true
     */
    void checkOpenglError();

    /**
     * @brief Draw the axes at pivot point of camera
     */
    void drawOrigineAxes();

private slots:
    /**
     * @brief Called when the opengl context is about to be destroyed. We clear all opengl objects.
     */
    void teardownGL();

    /**
     * @brief Called when the timer that control the fast draw has timed out
     */
    void fastDrawTimerTimeOut();

    /**
     * @brief Called to restart the timer that control the fast draw. It was called
     *        when the user use the mouse or when the scene has changed, etc... In other worlds
     *        when an event that requires to draw the scene fast has occured.
     */
    void restartFastDrawTimer();

signals:
    /**
     * @brief Emitted when redraw method is called
     */
    void redrawNecessary();

    /**
     * @brief Emitted when fast draw was set
     */
    void fastDrawSet();

    /**
     * @brief Emitted when the debug mode changed
     */
    void debugModeChanged(bool enabled);
};

#endif // AMKGLVIEWER_H
