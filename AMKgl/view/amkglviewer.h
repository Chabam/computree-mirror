#ifndef AMKGLVIEWER_H
#define AMKGLVIEWER_H

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "amkglcamera.h"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#include <QDomDocument>
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
class AMKglViewer : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
    using SuperClass = QOpenGLWidget;

public:

    typedef AMKglCamera                 CameraType;
    typedef Eigen::Vector3d             VecType;
    typedef Eigen::Quaterniond          QuatType;
    typedef PermanentItemSceneByModel   PermanentSceneToRender;

    enum RedrawMethod {
        RedrawOnlyOverlay,  // redraw only overlay (rectangle/polygon selection, text, etc...)
        RedrawOnly3D,       // redraw only 3D
        RedrawAll           // redraw all
    };

    AMKglViewer(QWidget *parent);
    AMKglViewer(const IGraphicsDocument* doc, QWidget *parent);
    ~AMKglViewer() override;

    /**
     * @brief Redefine this method to returns your graphics view used to draw Items
     */
    virtual AMKgl::GraphicsView* getGraphicsView() const { return nullptr; }

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
     * @brief Returns the camera
     */
    AMKglCamera* camera() const;

    /**
     * @brief Move the camera so the entire scene is visible (using scene radius)
     */
    void showEntireScene();

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

    /**
     * @brief Set the filename to use for the snapshot
     */
    void setSnapshotFileName(const QString& filename);

    /**
     * @brief Returns the current snapshot filename used
     */
    QString snapshotFileName() const;

    /**
     * @brief Enable/Disable the axis display
     */
    void setAxisIsDrawn(bool drawn);

    /**
     * @brief Returns true if axis are drawn
     */
    bool axisIsDrawn() const;

    /**
     * @brief Enable/Disable the grid display
     */
    void setGridIsDrawn(bool drawn);

    /**
     * @brief Returns true if grid is drawn
     */
    bool gridIsDrawn() const;

    /**
     * @brief Enable/Disable the fps display
     */
    void setFPSIsDisplayed(bool displayed);

    /**
     * @brief Returns true if fps is displayed
     */
    bool FPSIsDisplayed() const;

    /**
     * @brief Change the background color
     */
    void setBackgroundColor(const QColor& color);

    /**
     * @brief Returns the current background color
     */
    QColor backgroundColor() const;

    /**
     * @brief Change the foreground color
     */
    void setForegroundColor(const QColor& color);

    /**
     * @brief Returns the current foreground color
     */
    QColor foregroundColor() const;

    /**
     * @brief Create the root dom element that you can save in a xml file to save the current configuration
     * @param name : name of root element
     * @param document : document to use to create QDomElement
     * @return The root dom element
     */
    QDomElement domElement(const QString& name, QDomDocument& document) const;

    /**
     * @brief Use the dom element to restore the previous state (saved by using domElement method)
     * @param element : the dom element created with method domElement()
     */
    void initFromDOMElement(const QDomElement &element);

    /**
     * @brief Draw a 3D arrow along the positive Z axis
     */
    void drawArrow(double length = 1.0, double radius = -1.0, int nbSubdivisions = 12);

    /**
     * @brief Draw the XYZ axis
     * @param length : length of lines
     */
    void drawAxis(double length = 1.0);

    /**
     * @brief Draws a grid in the XY plane, centered on (0,0,0)
     */
    void drawGrid(double size = 1.0, int nbSubdivisions = 10);

public slots:
    /**
     * @brief Call to redraw all
     */
    void redraw(RedrawMethod m = RedrawAll);

protected:
    void initializeGL() override;
    virtual void init() {}
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void paintEvent(QPaintEvent *e) override;
    virtual void preDraw();
    virtual void draw();
    virtual void fastDraw();
    virtual void postDraw();
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
    AMKglCamera*                m_camera;
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

    QString                     m_snapshotFilename;
    QColor                      m_backgroundColor;
    QColor                      m_foregroundColor;

    bool                        m_axisIsDrawn;
    bool                        m_gridIsDrawn;
    bool                        m_FPSIsDisplayed;

    QElapsedTimer               m_postDrawElapsedTimer;
    unsigned int                m_fpsCounter;
    QString                     m_lastFPSValue;

    /**
     * @brief Check if opengl has send errors and add it to log if true
     */
    void checkOpenglError();

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
