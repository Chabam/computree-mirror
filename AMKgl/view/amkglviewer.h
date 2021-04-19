#ifndef AMKGLVIEWER_H
#define AMKGLVIEWER_H

#include <Eigen/Core>
#include <Qt3DRender/QCamera>

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
class AMKglViewer : public QOpenGLWidget, protected QOpenGLFunctions {

    Q_OBJECT

public:

    typedef QGLCamera         CameraType;
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
    void initializeGL() override;
    void paintGL() override;
    void paintEvent(QPaintEvent *e) override;
    void preDraw();
    void draw();
    void fastDraw();
    void postDraw();
    void QGLpreDraw();
    void QGLpostDraw();
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

    void loadModelViewMatrix(bool reset=true) const;

    void loadProjectionMatrix(bool reset=true) const;

Q_SIGNALS:
  void viewerInitialized();
  void drawNeeded();
  void drawFinished(bool automatic);
  void animateNeeded();
  void helpRequired();
  void axisIsDrawnChanged(bool drawn);
  void gridIsDrawnChanged(bool drawn);
  void FPSIsDisplayedChanged(bool displayed);
  void textIsEnabledChanged(bool enabled);
  void cameraIsEditedChanged(bool edited);
  void stereoChanged(bool on);
  void pointSelected(const QMouseEvent *e);

private:

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
    CameraType*                 m_camera;
    RenderParams                m_params;
    QTimer                      m_fastDrawTimer;
    bool                        m_inFastDraw;
    DrawInfo*                   m_drawInfo;
    QPainter*                   m_painter;
    ActionPickAnyElements*      m_pickingAction;
    bool                        m_debugModeEnabled;
    QGLFramebufferObject*       m_glBuffer;
    mutable GLdouble            m_projectionMatrix[16];
    mutable bool                m_projectionMatrixIsUpToDate;
    QColor                      m_foregroundColor;
    QColor                      m_backgroundColor;
    bool                        m_takeScreenshot;
    double                      m_screenshotMultiplicationFactor;
    bool                        m_useNormalCamera;
    QString                     m_snapshotFileName;
    QString                     m_snapshotFormat;
    bool                        m_axisIsDrawn;
    bool                        m_gridIsDrawn;
    bool                        m_FPSIsDisplayed;
    bool                        m_cameraIsEdited;
    bool                        m_textIsEnabled;
    bool                        m_stereoDisplay;
    bool                        m_displayMessage;
    QString                     m_message;

public Q_SLOTS:
    void setAxisIsDrawn(bool draw = true)
    {
        m_axisIsDrawn = draw;
        //Q_EMIT axisIsDrawnChanged(draw);
        update();
    }
    void setGridIsDrawn(bool draw = true)
    {
        m_gridIsDrawn = draw;
        //Q_EMIT gridIsDrawnChanged(draw);
        update();
    }
    void setFPSIsDisplayed(bool display = true)
    {
        m_FPSIsDisplayed = display;
        //Q_EMIT FPSIsDisplayedChanged(display);
        update();
    }

  virtual void select(const QMouseEvent *event);
  virtual void select(const QPoint &point);

public:
    bool axisIsDrawn() const { return m_axisIsDrawn; }
    bool gridIsDrawn() const { return m_gridIsDrawn; }
    bool FPSIsDisplayed() const { return m_FPSIsDisplayed; }

private:
    /**
     * @brief Check if opengl has send errors and add it to log if true
     */
    void checkOpenglError();

    /**
     * @brief Draw the axes at pivot point of camera
     */
    void drawOrigineAxes();

    void computeProjectionMatrix() const;

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
