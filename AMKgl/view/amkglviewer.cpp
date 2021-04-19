#include "amkglviewer.h"
#include "amkglapp.h"
#include "tools/opengl/openglinfo.h"
#include "scene/permanentitemscenebymodel.h"
#include "actions/picking/actionpickanyelements.h"
#include "visitor/applycustomfunctiontoobjectvisitor.h"
#include "tools/opengl/cylinderglrenderer.h"
#include "tools/qglviewer/qglviewertools.h"

#include <GL/GLU.h>
#include <QGL>
#include <QOpenGLContext>
#include <QInputDialog>

AMKglViewer::AMKglViewer(const IGraphicsDocument* doc, QWidget *parent) :
    QOpenGLWidget(parent)
{
    setAttribute(Qt::WA_NoSystemBackground);

    m_document = nullptr;
    m_debugModeEnabled = false;

    m_pickingAction = new ActionPickAnyElements();
    m_pickingAction->setGlViewer(this);

    m_camera = new CameraType();

    m_takeScreenshot = false;
    m_screenshotMultiplicationFactor = 1.0;
    m_glBuffer = nullptr;

    m_inFastDraw = false;
    m_permanentItemScene = nullptr;
    m_newOpenglContext = nullptr;

    m_painter = new QPainter();
    m_drawInfo = new DrawInfo(*m_painter, this);

    m_fastDrawTimer.setInterval(2000);
    m_fastDrawTimer.setSingleShot(true);

    m_axisIsDrawn    = false;
    m_gridIsDrawn    = false;
    m_FPSIsDisplayed = false;
    m_cameraIsEdited = false;
    m_textIsEnabled  = true;
    m_stereoDisplay  = false;

    connect(&m_fastDrawTimer, SIGNAL(timeout()), this, SLOT(fastDrawTimerTimeOut()));
    //connect(m_camera->frame(), SIGNAL(spun()), m_camera->frame(), SLOT(stopSpinning()));

    //m_camera->setSceneRadius(20);
    setMouseTracking(true);

    setDocument(doc);
}

AMKglViewer::~AMKglViewer()
{
    setPermanentSceneToRender(nullptr);

    delete m_glBuffer;
	delete m_pickingAction;
    delete m_drawInfo;
    delete m_painter;
}

void AMKglViewer::setDocument(const IGraphicsDocument *doc)
{
    m_document = const_cast<IGraphicsDocument*>(doc);
}

IGraphicsDocument *AMKglViewer::getDocument() const
{
    return m_document;
}

void AMKglViewer::setPermanentSceneToRender(const PermanentSceneToRender *scene)
{
    if(scene == m_permanentItemScene)
        return;

    if(m_permanentItemScene != nullptr) {
        disconnect(m_permanentItemScene, nullptr, this, nullptr);
        m_permanentItemScene->removedFrom(getDocument(), getNewOpenGlContext());
    }

    m_permanentItemScene = const_cast<PermanentSceneToRender*>(scene);

    if(m_permanentItemScene != nullptr) {

        m_permanentItemScene->addedTo(getDocument(), getNewOpenGlContext());

        connect(m_permanentItemScene, SIGNAL(changesHaveOccured()), this, SLOT(restartFastDrawTimer()), Qt::QueuedConnection);
        connect(m_permanentItemScene, SIGNAL(changesHaveOccured()), this, SLOT(redraw()), Qt::QueuedConnection);
    }
}

AMKglViewer::PermanentSceneToRender* AMKglViewer::getPermanentSceneToRender() const
{
    return m_permanentItemScene;
}

ActionPickAnyElements* AMKglViewer::getPickingAction() const
{
    return m_pickingAction;
}

Eigen::Vector3d AMKglViewer::getCameraPosition() const
{
    QVector3D cameraPosition = m_camera->position();

    return Eigen::Vector3d(cameraPosition.x(), cameraPosition.y(), cameraPosition.z());
}

Eigen::Matrix4d AMKglViewer::getCameraModelViewMatrix() const
{
    QMatrix4x4 modelViewMatrix = m_camera->projectionMatrix();

    Eigen::Matrix4d matrix;
    matrix << modelViewMatrix(0,0), modelViewMatrix(0,1), modelViewMatrix(0,2), modelViewMatrix(0,2),
              modelViewMatrix(1,0), modelViewMatrix(1,1), modelViewMatrix(1,2), modelViewMatrix(1,2),
              modelViewMatrix(2,0), modelViewMatrix(2,1), modelViewMatrix(2,2), modelViewMatrix(2,2),
              modelViewMatrix(3,0), modelViewMatrix(3,1), modelViewMatrix(3,2), modelViewMatrix(3,2);

    return matrix;
}

void AMKglViewer::redraw(RedrawMethod m)
{
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(m)
#endif
    // TODO : find a way to redraw only the overlay (screenshot ?)
    restartFastDrawTimer();
    update();

    emit redrawNecessary();
}

void AMKglViewer::drawText(int x, int y, const QString &text, const QFont &fnt)
{
    Q_UNUSED(fnt)
    Q_UNUSED(x)
    Q_UNUSED(y)

    m_drawInfo->drawText(text);
}

bool AMKglViewer::isFastDrawModeCurrentlyUsed() const
{
    return m_inFastDraw;
}

DrawInfo* AMKglViewer::getDrawInfo() const
{
    return m_drawInfo;
}

QPainter* AMKglViewer::getPainter() const
{
    return m_painter;
}

QOpenGLContext* AMKglViewer::getNewOpenGlContext() const
{
    return m_newOpenglContext;
}

void AMKglViewer::takeScreenshot()
{
    QHash<QString, QString> formats;
    formats.insert("JPEG", "JPEG (*.jpg)");
    formats.insert("PNG", "PNG (*.png)");
    QString selected = formats.value(formats.value("JPEG"));

    const QString fn = QFileDialog::getSaveFileName(nullptr, tr("Choisissez un nom de fichier"), m_snapshotFileName, formats.values().join(";;"), &selected);

    if(fn.isEmpty())
        return;

    bool ok;
    const double factor = QInputDialog::getDouble(nullptr, tr("Resolution [%1 x %2]").arg(size().width(), size().height()), tr("Facteur de multiplication"), m_screenshotMultiplicationFactor, 1, 4, 1, &ok);

    if(!ok)
        return;

    m_screenshotMultiplicationFactor = factor;

    m_snapshotFileName = fn;
    m_snapshotFormat = formats.key(selected);

    m_takeScreenshot = true;
    update();
}

void AMKglViewer::initializeGL()
{
    initializeOpenGLFunctions();

    m_newOpenglContext = context();

    Q_ASSERT(m_newOpenglContext != nullptr);

    connect(m_newOpenglContext, SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);

    OPENGL_INFO->init();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    const GLubyte *glV = glGetString(GL_VERSION);
    const GLubyte *glShV = glGetString(GL_SHADING_LANGUAGE_VERSION);

    AMKglLOG->addDebugMessage(QString("gl version : %1 [%2.%3]").arg(QString((const char*)glV)).arg(getNewOpenGlContext()->format().majorVersion()).arg(getNewOpenGlContext()->format().minorVersion()));
    AMKglLOG->addDebugMessage(QString("gl shader version : %1").arg(QString((const char*)glShV)));

    if((m_document != nullptr) && (m_permanentItemScene == nullptr))
        setPermanentSceneToRender(m_document->getPermanentSceneToRender());

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    const QColor bgColor = QColor(51, 51, 51);
    glClearColor(bgColor.redF(), bgColor.greenF(), bgColor.blueF(), bgColor.alphaF());

    // Clear the buffer where we're going to draw
    if (format().stereo()) {
      glDrawBuffer(GL_BACK_RIGHT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDrawBuffer(GL_BACK_LEFT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } else
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Give time to glInit to finish and then call setFullScreen().
    if (isFullScreen())
      QTimer::singleShot(100, this, SLOT(delayedFullScreen()));
}

void AMKglViewer::paintGL()
{
    redraw();
}

void AMKglViewer::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    if(m_takeScreenshot) {
        delete m_glBuffer;
        m_glBuffer = new QGLFramebufferObject(size()*m_screenshotMultiplicationFactor);
    }

    QGLpreDraw();
    draw();
    QGLpostDraw();

    Q_EMIT drawFinished(true);

    checkOpenglError();
}

void AMKglViewer::preDraw()
{
    if(context() != QOpenGLContext::currentContext())
        return;

    if(m_glBuffer != nullptr)
        m_painter->begin(m_glBuffer);
    else
        m_painter->begin(this);

    m_painter->beginNativePainting();
    m_painter->setRenderHint(QPainter::Antialiasing);
    m_painter->setPen(Qt::white);
    m_drawInfo->reset();

    m_useNormalCamera = true;

    // Save current OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Reset OpenGL parameters
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const QColor bgColor = QColor(51, 51, 51);
    glClearColor(bgColor.redF(), bgColor.greenF(), bgColor.blueF(), bgColor.alphaF());

    QGLpreDraw();

    m_useNormalCamera = false;
    loadModelViewMatrix(true);

    if(getPermanentSceneToRender() != nullptr)
        getPermanentSceneToRender()->preDraw(*m_drawInfo);
}

void AMKglViewer::QGLpreDraw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  loadProjectionMatrix();
  loadModelViewMatrix();

  Q_EMIT drawNeeded();
}

void AMKglViewer::QGLpostDraw() {
  // Reset model view matrix to world coordinates origin
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  loadModelViewMatrix();

  // Save OpenGL state
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  // Set neutral GL state
  glDisable(GL_TEXTURE_1D);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_GEN_Q);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_COLOR_MATERIAL);
  m_foregroundColor = QColor(180.0, 180.0, 180.0);
  glColor4f(m_foregroundColor.redF(),
            m_foregroundColor.greenF(),
            m_foregroundColor.blueF(),
            m_foregroundColor.alphaF());

  //if (m_cameraIsEdited)
  //  m_camera->drawAllPaths();

  // Pivot point, line when camera rolls, zoom region
  //QGLViewer::drawVisualHints();

  //if (m_gridIsDrawn) {
  //  glLineWidth(1.0);
  //  QGLViewer::drawGrid(camera()->sceneRadius());
  //}
  //if (m_axisIsDrawn) {
  //  glLineWidth(2.0);
  //  QGLViewer::drawAxis(camera()->sceneRadius());
  //}

  // FPS computation
  //const unsigned int maxCounter = 20;
  //if (++fpsCounter_ == maxCounter) {
  //  f_p_s_ = 1000.0 * maxCounter / fpsTime_.restart();
  //  fpsString_ = tr("%1Hz", "Frames per seconds, in Hertz")
  //                   .arg(f_p_s_, 0, 'f', ((f_p_s_ < 10.0) ? 1 : 0));
  //  fpsCounter_ = 0;
  //}

  // Restore foregroundColor
  float color[4];
  color[0] = m_foregroundColor.red()   / 255.0f;
  color[1] = m_foregroundColor.green() / 255.0f;
  color[2] = m_foregroundColor.blue()  / 255.0f;
  color[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  //if (m_FPSIsDisplayed)
  //  QGLViewer::displayFPS();
  //if (m_displayMessage)
  //  QGLViewer::drawText(10, height() - 10, m_message);

  // Restore GL state
  glPopAttrib();
  glPopMatrix();
}

void AMKglViewer::draw()
{
    if(context() != QOpenGLContext::currentContext())
        return;

    m_inFastDraw = false;

    if(m_params.fastDraw) {
        fastDraw();
        return;
    }

    if(getPermanentSceneToRender() != nullptr)
    {
        if(getPermanentSceneToRender()->mustReduceNumberOfPointsInNormalDraw())
        {
            m_inFastDraw = true;
            getPermanentSceneToRender()->fastDraw(*m_drawInfo);
        }
        else
        {
            getPermanentSceneToRender()->draw(*m_drawInfo);
        }
    }
}

void AMKglViewer::fastDraw()
{
    if(context() != QOpenGLContext::currentContext())
        return;

    m_inFastDraw = false;

    if(getPermanentSceneToRender() != nullptr)
    {
        if(!getPermanentSceneToRender()->mustReduceNumberOfPointsInNormalDraw()
        && !getPermanentSceneToRender()->mustReduceNumberOfPointsInFastDraw())
        {
            getPermanentSceneToRender()->draw(*m_drawInfo);
            return;
        }

        if(getPermanentSceneToRender()->mustReduceNumberOfPointsInFastDraw())
        {
            m_inFastDraw = true;
            getPermanentSceneToRender()->fastDraw(*m_drawInfo);
        }
        else
        {
            getPermanentSceneToRender()->draw(*m_drawInfo);
        }
    }
}

void AMKglViewer::postDraw()
{
    if(context() != QOpenGLContext::currentContext())
        return;

    /*if(isFastDrawModeCurrentlyUsed())
        m_drawInfo->drawText("FAST MODE");
    else
        m_drawInfo->drawText("NORMAL MODE");*/

    m_useNormalCamera = true;
    loadModelViewMatrix(true);

    subPostDraw(*m_drawInfo);

    QGLpostDraw();

    //drawOrigineAxes();

    //GLdouble mwMatrix[16];
    //m_camera->getModelViewMatrix(mwMatrix);

    // Restore OpenGL state
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    m_painter->endNativePainting();

    drawOverlay(*m_painter, *m_drawInfo);

    m_drawInfo->drawAllTextSaved();

    /*int index = 0;
    for(int i=0; i<4; ++i) {
        QString s;

        for(int j=0; j<4; ++j)
            s += QString().setNum(mwMatrix[index++]) + "    ";

        m_drawInfo->drawText(s);
    }*/

    /*if(m_permanentScene != nullptr)
        m_permanentScene->postDraw(*m_drawInfo);*/

    m_painter->end();

    if(m_takeScreenshot) {
        m_takeScreenshot = false;

        const QImage img = m_glBuffer->toImage();
        img.save(m_snapshotFileName);

        delete m_glBuffer;
        m_glBuffer = nullptr;
    }
}

void AMKglViewer::drawOverlay(QPainter &painter, DrawInfo &info)
{
    getPickingAction()->drawOverlay(painter, info);
}

void AMKglViewer::mousePressEvent(QMouseEvent *e)
{
    if(getPickingAction()->mousePressEvent(e))
        return;

    ++m_params.nMouseButtonPressed;

    //if(clickAction(Qt::Key(0), e->modifiers(), e->button()) != QGLViewer::SELECT)
    QOpenGLWidget::mousePressEvent(e);
}

void AMKglViewer::mouseMoveEvent(QMouseEvent *e)
{
    if(m_params.nMouseButtonPressed > 0)
        restartFastDrawTimer();

    if(getPickingAction()->mouseMoveEvent(e))
        return;

    QOpenGLWidget::mouseMoveEvent(e);
}

void AMKglViewer::mouseReleaseEvent(QMouseEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->mouseReleaseEvent(e))
        return;

    --m_params.nMouseButtonPressed;
    m_params.nMouseButtonPressed = qMax(m_params.nMouseButtonPressed, 0);

    //if(clickAction(Qt::Key(0), e->modifiers(), e->button()) != QGLViewer::SELECT)
    QOpenGLWidget::mouseReleaseEvent(e);
}

void AMKglViewer::mouseDoubleClickEvent(QMouseEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->mouseDoubleClickEvent(e))
        return;

    QOpenGLWidget::mouseDoubleClickEvent(e);
}

void AMKglViewer::wheelEvent(QWheelEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->wheelEvent(e))
        return;

    QOpenGLWidget::wheelEvent(e);
}

void AMKglViewer::resizeEvent(QResizeEvent *e)
{
    restartFastDrawTimer();
    QOpenGLWidget::resizeEvent(e);
}

void AMKglViewer::showEvent(QShowEvent *e)
{
    restartFastDrawTimer();
    QOpenGLWidget::showEvent(e);
}

void AMKglViewer::keyPressEvent(QKeyEvent *e)
{
    if(getPickingAction()->keyPressEvent(e))
        return;

    if(e->key() == Qt::Key_Escape)
        return;

    QOpenGLWidget::keyPressEvent(e);
}

void AMKglViewer::keyReleaseEvent(QKeyEvent *e)
{
    if(getPickingAction()->keyReleaseEvent(e))
        return;

    if(e->key() == Qt::Key_Escape)
        return;

    QOpenGLWidget::keyReleaseEvent(e);
}

void AMKglViewer::setDebugModeEnabled(bool enableIt)
{
    if(m_debugModeEnabled != enableIt) {
        m_debugModeEnabled = enableIt;

        emit debugModeChanged(m_debugModeEnabled);
    }
}

bool AMKglViewer::isDebugModeEnabled() const
{
    return m_debugModeEnabled;
}

void AMKglViewer::teardownGL()
{
    if(getPermanentSceneToRender() != nullptr)
        getPermanentSceneToRender()->destroyGL(getNewOpenGlContext());
}

void AMKglViewer::fastDrawTimerTimeOut()
{
    if(m_params.nMouseButtonPressed == 0) {
        m_params.fastDraw = false;
        update();
    }
}

void AMKglViewer::checkOpenglError()
{
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
       AMKglLOG->addErrorMessage(QString("OpenGL error (%1) : %2").arg((int)err).arg((char*)gluErrorString(err)));
    }
}

void AMKglViewer::drawOrigineAxes()
{
    QVector3D center = m_camera->viewCenter();

    glBegin(GL_LINES);

    glColor3f(1,0,0);
    glVertex3f(center.x(), center.y(), center.z());
    glVertex3f(center.x()+1, center.y(), center.z());

    glColor3f(0,1,0);
    glVertex3f(center.x(), center.y(), center.z());
    glVertex3f(center.x(), center.y()+1, center.z());

    glColor3f(0,0,1);
    glVertex3f(center.x(), center.y(), center.z());
    glVertex3f(center.x(), center.y(), center.z()+1);

    glEnd();
}

void AMKglViewer::restartFastDrawTimer()
{
    m_fastDrawTimer.start();
    m_params.fastDraw = true;

    emit fastDrawSet();
}

void AMKglViewer::loadModelViewMatrix(bool reset) const
{
    if(m_useNormalCamera) {
        //Camera::loadModelViewMatrix(reset);
        return;
    }

    // WARNING: makeCurrent must be called by every calling method
    glMatrixMode(GL_MODELVIEW);

    if (reset) {
        // Identity matrix
        Eigen::Matrix4d m = Eigen::Matrix4d::Identity();

        glLoadMatrixd(m.data());
    }
}

void AMKglViewer::loadProjectionMatrix(bool reset) const {
  // WARNING: makeCurrent must be called by every calling method
  glMatrixMode(GL_PROJECTION);

  if (reset)
    glLoadIdentity();

  computeProjectionMatrix();

  glMultMatrixd(m_projectionMatrix);
}

void AMKglViewer::computeProjectionMatrix() const {
  if (m_projectionMatrixIsUpToDate)
    return;

  const qreal ZNear = m_camera->nearPlane();
  const qreal ZFar = m_camera->farPlane();

  switch (m_camera->projectionType()) {
  case Qt3DRender::QCameraLens::PerspectiveProjection: {
    // #CONNECTION# all non null coefficients were set to 0.0 in constructor.
    const qreal f = 1.0 / tan(m_camera->fieldOfView() / 2.0);
    m_projectionMatrix[0] = f / m_camera->aspectRatio();
    m_projectionMatrix[5] = f;
    m_projectionMatrix[10] = (ZNear + ZFar) / (ZNear - ZFar);
    m_projectionMatrix[11] = -1.0;
    m_projectionMatrix[14] = 2.0 * ZNear * ZFar / (ZNear - ZFar);
    m_projectionMatrix[15] = 0.0;
    break;
  }
  default:
  case Qt3DRender::QCameraLens::OrthographicProjection: {
    GLdouble w, h;
    QGLViewerTools::getOrthoWidthHeight(*m_camera, w, h);
    m_projectionMatrix[0] = 1.0 / w;
    m_projectionMatrix[5] = 1.0 / h;
    m_projectionMatrix[10] = -2.0 / (ZFar - ZNear);
    m_projectionMatrix[11] = 0.0;
    m_projectionMatrix[14] = -(ZFar + ZNear) / (ZFar - ZNear);
    m_projectionMatrix[15] = 1.0;
    break;
  }
  }

  m_projectionMatrixIsUpToDate = true;
}

void AMKglViewer::select(const QMouseEvent *event)
{
  // For those who don't derive but rather rely on the signal-slot mechanism.
  Q_EMIT pointSelected(event);
  select(event->pos());
}

void AMKglViewer::select(const QPoint &point)
{
    Q_UNUSED(point);
  //beginSelection(point);
  //drawWithNames();
  //endSelection(point);
  //postSelection(point);
}
