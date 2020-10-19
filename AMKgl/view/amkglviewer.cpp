#include "amkglviewer.h"
#include "amkglapp.h"
#include "tools/opengl/openglinfo.h"
#include "scene/permanentitemscenebymodel.h"
#include "actions/picking/actionpickanyelements.h"
#include "visitor/applycustomfunctiontoobjectvisitor.h"
#include "tools/opengl/cylinderglrenderer.h"

#include <manipulatedCameraFrame.h>

#include <QOpenGLContext>
#include <QInputDialog>

AMKglViewer::AMKglViewer(const IGraphicsDocument* doc, QWidget *parent) :
    QGLViewer(parent)
{
    setAttribute(Qt::WA_NoSystemBackground);

    m_document = nullptr;
    m_debugModeEnabled = false;

    m_pickingAction = new ActionPickAnyElements();
    m_pickingAction->setGlViewer(this);

	qglviewer::Camera* currentCamera = QGLViewer::camera();
    m_camera = new SpecialCam(*currentCamera);
    setCamera(m_camera);
	delete currentCamera;

    //camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);

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

    connect(&m_fastDrawTimer, SIGNAL(timeout()), this, SLOT(fastDrawTimerTimeOut()));
    connect(QGLViewer::camera()->frame(), SIGNAL(spun()), QGLViewer::camera()->frame(), SLOT(stopSpinning()));

    setSceneRadius(20);
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
    qglviewer::Vec cameraPosition = camera()->position();

    return Eigen::Vector3d(cameraPosition.x, cameraPosition.y, cameraPosition.z);
}

Eigen::Matrix4d AMKglViewer::getCameraModelViewMatrix() const
{
    GLdouble modelViewMatrix[16];
    camera()->getModelViewMatrix(modelViewMatrix);

    Eigen::Matrix4d matrix;
    matrix << modelViewMatrix[0], modelViewMatrix[4], modelViewMatrix[8], modelViewMatrix[12],
              modelViewMatrix[1], modelViewMatrix[5], modelViewMatrix[9], modelViewMatrix[13],
              modelViewMatrix[2], modelViewMatrix[6], modelViewMatrix[10], modelViewMatrix[14],
              modelViewMatrix[3], modelViewMatrix[7], modelViewMatrix[11], modelViewMatrix[15];

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
    QString selected = formats.value(snapshotFormat(), formats.value("JPEG"));

    const QString fn = QFileDialog::getSaveFileName(nullptr, tr("Choisissez un nom de fichier"), snapshotFileName(), formats.values().join(";;"), &selected);

    if(fn.isEmpty())
        return;

    bool ok;
    const double factor = QInputDialog::getDouble(nullptr, tr("Resolution [%1 x %2]").arg(size().width()).arg(size().height()), tr("Facteur de multiplication"), m_screenshotMultiplicationFactor, 1, 4, 1, &ok);

    if(!ok)
        return;

    m_screenshotMultiplicationFactor = factor;

    setSnapshotFileName(fn);
    setSnapshotFormat(formats.key(selected));

    m_takeScreenshot = true;
    update();
}

void AMKglViewer::init()
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

    QGLViewer::paintGL();
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

    m_camera->m_useNormalCamera = true;

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

    const QColor bgColor = backgroundColor();
    glClearColor(bgColor.redF(), bgColor.greenF(), bgColor.blueF(), bgColor.alphaF());

    QGLViewer::preDraw();

    m_camera->m_useNormalCamera = false;
    m_camera->loadModelViewMatrix(true);

    if(getPermanentSceneToRender() != nullptr)
        getPermanentSceneToRender()->preDraw(*m_drawInfo);
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

    if(getPermanentSceneToRender() != nullptr) {
        if(getPermanentSceneToRender()->mustReduceNumberOfPointsInNormalDraw()) {
            m_inFastDraw = true;
            getPermanentSceneToRender()->fastDraw(*m_drawInfo);
        } else {
            getPermanentSceneToRender()->draw(*m_drawInfo);
        }
    }
}

void AMKglViewer::fastDraw()
{
    if(context() != QOpenGLContext::currentContext())
        return;

    m_inFastDraw = false;

    if(getPermanentSceneToRender() != nullptr) {

        if(!getPermanentSceneToRender()->mustReduceNumberOfPointsInNormalDraw()
                && !getPermanentSceneToRender()->mustReduceNumberOfPointsInFastDraw()) {
            getPermanentSceneToRender()->draw(*m_drawInfo);
            return;
        }


        if(getPermanentSceneToRender()->mustReduceNumberOfPointsInFastDraw()) {
            m_inFastDraw = true;
            getPermanentSceneToRender()->fastDraw(*m_drawInfo);
        } else {
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

    m_camera->m_useNormalCamera = true;
    m_camera->loadModelViewMatrix(true);

    subPostDraw(*m_drawInfo);

    QGLViewer::postDraw();

    //drawOrigineAxes();

    GLdouble mwMatrix[16];
    camera()->getModelViewMatrix(mwMatrix);

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
        img.save(snapshotFileName());

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

    if(clickAction(Qt::Key(0), e->modifiers(), e->button()) != QGLViewer::SELECT)
        QGLViewer::mousePressEvent(e);
}

void AMKglViewer::mouseMoveEvent(QMouseEvent *e)
{
    if(m_params.nMouseButtonPressed > 0)
        restartFastDrawTimer();

    if(getPickingAction()->mouseMoveEvent(e))
        return;

    QGLViewer::mouseMoveEvent(e);
}

void AMKglViewer::mouseReleaseEvent(QMouseEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->mouseReleaseEvent(e))
        return;

    --m_params.nMouseButtonPressed;
    m_params.nMouseButtonPressed = qMax(m_params.nMouseButtonPressed, 0);

    if(clickAction(Qt::Key(0), e->modifiers(), e->button()) != QGLViewer::SELECT)
        QGLViewer::mouseReleaseEvent(e);
}

void AMKglViewer::mouseDoubleClickEvent(QMouseEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->mouseDoubleClickEvent(e))
        return;

    QGLViewer::mouseDoubleClickEvent(e);
}

void AMKglViewer::wheelEvent(QWheelEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->wheelEvent(e))
        return;

    QGLViewer::wheelEvent(e);
}

void AMKglViewer::resizeEvent(QResizeEvent *e)
{
    restartFastDrawTimer();
    QGLViewer::resizeEvent(e);
}

void AMKglViewer::showEvent(QShowEvent *e)
{
    restartFastDrawTimer();
    QGLViewer::showEvent(e);
}

void AMKglViewer::keyPressEvent(QKeyEvent *e)
{
    if(getPickingAction()->keyPressEvent(e))
        return;

    if(e->key() == Qt::Key_Escape)
        return;

    QGLViewer::keyPressEvent(e);
}

void AMKglViewer::keyReleaseEvent(QKeyEvent *e)
{
    if(getPickingAction()->keyReleaseEvent(e))
        return;

    if(e->key() == Qt::Key_Escape)
        return;

    QGLViewer::keyReleaseEvent(e);
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
    qglviewer::Vec center = camera()->sceneCenter();

    glBegin(GL_LINES);

    glColor3f(1,0,0);
    glVertex3f(center.x, center.y, center.z);
    glVertex3f(center.x+1, center.y, center.z);

    glColor3f(0,1,0);
    glVertex3f(center.x, center.y, center.z);
    glVertex3f(center.x, center.y+1, center.z);

    glColor3f(0,0,1);
    glVertex3f(center.x, center.y, center.z);
    glVertex3f(center.x, center.y, center.z+1);

    glEnd();
}

void AMKglViewer::restartFastDrawTimer()
{
    m_fastDrawTimer.start();
    m_params.fastDraw = true;

    emit fastDrawSet();
}

void AMKglViewer::SpecialCam::loadModelViewMatrix(bool reset) const
{
    if(m_useNormalCamera) {
        Camera::loadModelViewMatrix(reset);
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
