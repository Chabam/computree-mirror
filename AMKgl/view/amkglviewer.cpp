#include "amkglviewer.h"
#include "amkglapp.h"
#include "tools/opengl/openglinfo.h"
#include "scene/permanentitemscenebymodel.h"
#include "actions/picking/actionpickanyelements.h"
#include "visitor/applycustomfunctiontoobjectvisitor.h"
#include "tools/opengl/cylinderglrenderer.h"
#include "tools/amkgldomutils.h"

#include <QOpenGLContext>
#include <QInputDialog>

#ifdef Q_OS_MAC
#include <OpenGL/glu.h>
#elif defined(Q_OS_LINUX)
#include <GL/glu.h>
#else
#include <GL/glu.h>
#endif

AMKglViewer::AMKglViewer(QWidget* parent) : AMKglViewer(nullptr, parent)
{
}

AMKglViewer::AMKglViewer(const IGraphicsDocument* doc, QWidget *parent) :
    SuperClass(parent), //QOpenGLFunctions(),
    m_backgroundColor(51, 51, 51),
    m_foregroundColor(180, 180, 180),
    m_axisIsDrawn(false),
    m_gridIsDrawn(false),
    m_FPSIsDisplayed(false),
    m_fpsCounter(0),
    m_lastFPSValue("?Hz")
{
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);

    m_document = nullptr;
    m_debugModeEnabled = false;

    m_pickingAction = new ActionPickAnyElements();
    m_pickingAction->setGlViewer(this);

    m_camera = new AMKglCamera();
    //camera()->setProjectionType(AMKglCamera::PT_ORTHOGRAPHIC);

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

    setSnapshotFileName("snapshot");

    connect(&m_fastDrawTimer, SIGNAL(timeout()), this, SLOT(fastDrawTimerTimeOut()));
    connect(camera(), SIGNAL(manipulated()), this, SLOT(update()));

    //camera()->setSceneRadius(20);
    showEntireScene();
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
    delete m_camera;
}

void AMKglViewer::setDocument(const IGraphicsDocument *doc)
{
    m_document = const_cast<IGraphicsDocument*>(doc);
}

IGraphicsDocument* AMKglViewer::getDocument() const
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

AMKglCamera* AMKglViewer::camera() const
{
    return m_camera;
}

void AMKglViewer::showEntireScene()
{
    m_camera->showEntireScene();
    update();
}

Eigen::Vector3d AMKglViewer::getCameraPosition() const
{
    return camera()->position();
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
    QString selected = formats.value("JPEG");

    const QString fn = QFileDialog::getSaveFileName(nullptr, tr("Choisissez un nom de fichier"), snapshotFileName(), formats.values().join(";;"), &selected);

    if(fn.isEmpty())
        return;

    bool ok;
    const double factor = QInputDialog::getDouble(nullptr, tr("Resolution [%1 x %2]").arg(size().width()).arg(size().height()), tr("Facteur de multiplication"), m_screenshotMultiplicationFactor, 1, 4, 1, &ok);

    if(!ok)
        return;

    m_screenshotMultiplicationFactor = factor;

    setSnapshotFileName(fn);

    m_takeScreenshot = true;
    update();
}

void AMKglViewer::setSnapshotFileName(const QString& filename)
{
    if(QFileInfo(filename).suffix().isEmpty())
        m_snapshotFilename = filename + ".jpg";
    else
        m_snapshotFilename = filename;
}

QString AMKglViewer::snapshotFileName() const
{
    return m_snapshotFilename;
}

void AMKglViewer::setAxisIsDrawn(bool drawn)
{
    m_axisIsDrawn = drawn;
}

bool AMKglViewer::axisIsDrawn() const
{
    return m_axisIsDrawn;
}

void AMKglViewer::setGridIsDrawn(bool drawn)
{
    m_gridIsDrawn = drawn;
}

bool AMKglViewer::gridIsDrawn() const
{
    return m_gridIsDrawn;
}

void AMKglViewer::setFPSIsDisplayed(bool displayed)
{
    m_FPSIsDisplayed = displayed;
}

bool AMKglViewer::FPSIsDisplayed() const
{
    return m_FPSIsDisplayed;
}

void AMKglViewer::setBackgroundColor(const QColor& color)
{
    m_backgroundColor = color;
}

QColor AMKglViewer::backgroundColor() const
{
    return m_backgroundColor;
}

void AMKglViewer::setForegroundColor(const QColor& color)
{
    m_foregroundColor = color;
}

QColor AMKglViewer::foregroundColor() const
{
    return m_foregroundColor;
}

QDomElement AMKglViewer::domElement(const QString& name, QDomDocument& document) const
{
    QDomElement de = document.createElement(name);

    QDomElement stateNode = document.createElement("State");
    stateNode.appendChild(AMKglDomUtils::QColorDomElement(foregroundColor(), "foregroundColor", document));
    stateNode.appendChild(AMKglDomUtils::QColorDomElement(backgroundColor(), "backgroundColor", document));
    de.appendChild(stateNode);

    QDomElement displayNode = document.createElement("Display");
    AMKglDomUtils::setBoolAttribute(displayNode, "axisIsDrawn", axisIsDrawn());
    AMKglDomUtils::setBoolAttribute(displayNode, "gridIsDrawn", gridIsDrawn());
    AMKglDomUtils::setBoolAttribute(displayNode, "FPSIsDisplayed", FPSIsDisplayed());
    de.appendChild(displayNode);

    QDomElement geometryNode = document.createElement("Geometry");

    QWidget* topLevelW = topLevelWidget();
    geometryNode.setAttribute("width", QString::number(topLevelW->width()));
    geometryNode.setAttribute("height", QString::number(topLevelW->height()));
    geometryNode.setAttribute("posX", QString::number(topLevelW->pos().x()));
    geometryNode.setAttribute("posY", QString::number(topLevelW->pos().y()));

    de.appendChild(geometryNode);

    de.appendChild(camera()->domElement("Camera", document));

    return de;
}

void AMKglViewer::initFromDOMElement(const QDomElement& element)
{
    QDomElement child = element.firstChild().toElement();

    while (!child.isNull()) {
      if (child.tagName() == "State") {
        QDomElement ch = child.firstChild().toElement();
        while (!ch.isNull()) {
          if (ch.tagName() == "foregroundColor")
            setForegroundColor(AMKglDomUtils::QColorFromDom(ch));
          if (ch.tagName() == "backgroundColor")
            setBackgroundColor(AMKglDomUtils::QColorFromDom(ch));
          ch = ch.nextSibling().toElement();
        }
      }

      if (child.tagName() == "Display") {
        setAxisIsDrawn(AMKglDomUtils::boolFromDom(child, "axisIsDrawn", false));
        setGridIsDrawn(AMKglDomUtils::boolFromDom(child, "gridIsDrawn", false));
        setFPSIsDisplayed(AMKglDomUtils::boolFromDom(child, "FPSIsDisplayed", false));
      }

      if (child.tagName() == "Geometry") {
          const int width = AMKglDomUtils::intFromDom(child, "width", 600);
          const int height = AMKglDomUtils::intFromDom(child, "height", 400);
          topLevelWidget()->resize(width, height);
          camera()->setScreenWidthAndHeight(this->width(), this->height());

          QPoint pos;
          pos.setX(AMKglDomUtils::intFromDom(child, "posX", 0));
          pos.setY(AMKglDomUtils::intFromDom(child, "posY", 0));
          topLevelWidget()->move(pos);
      }

      if (child.tagName() == "Camera") {
        camera()->initFromDOMElement(child);
      }

      child = child.nextSibling().toElement();
    }
}

void AMKglViewer::drawArrow(double length, double radius, int nbSubdivisions)
{
    static GLUquadric* quadric = gluNewQuadric();

    if (radius < 0.0)
      radius = 0.05 * length;

    const double head = 2.5 * (radius / length) + 0.1;
    const double coneRadiusCoef = 4.0 - 5.0 * head;

    gluCylinder(quadric, radius, radius, length * (1.0 - head / coneRadiusCoef),
                nbSubdivisions, 1);
    glTranslated(0.0, 0.0, length * (1.0 - head));
    gluCylinder(quadric, coneRadiusCoef * radius, 0.0, head * length,
                nbSubdivisions, 1);
    glTranslated(0.0, 0.0, -length * (1.0 - head));
}

void AMKglViewer::drawAxis(double length)
{
    const double lineWidth = length / 40.0;
    const double lineHeight = length / 30.0;
    const double lineShift = 1.04 * length;

    GLboolean lighting, colorMaterial;
    glGetBooleanv(GL_LIGHTING, &lighting);
    glGetBooleanv(GL_COLOR_MATERIAL, &colorMaterial);

    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);

    // X line
    glVertex3d(lineShift, lineWidth, -lineHeight);
    glVertex3d(lineShift, -lineWidth, lineHeight);
    glVertex3d(lineShift, -lineWidth, -lineHeight);
    glVertex3d(lineShift, lineWidth, lineHeight);

    // Y line
    glVertex3d(lineWidth, lineShift, lineHeight);
    glVertex3d(0.0, lineShift, 0.0);
    glVertex3d(-lineWidth, lineShift, lineHeight);
    glVertex3d(0.0, lineShift, 0.0);
    glVertex3d(0.0, lineShift, 0.0);
    glVertex3d(0.0, lineShift, -lineHeight);

    // Z line
    glVertex3d(-lineWidth, lineHeight, lineShift);
    glVertex3d(lineWidth, lineHeight, lineShift);
    glVertex3d(lineWidth, lineHeight, lineShift);
    glVertex3d(-lineWidth, -lineHeight, lineShift);
    glVertex3d(-lineWidth, -lineHeight, lineShift);
    glVertex3d(lineWidth, -lineHeight, lineShift);
    glEnd();

    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    // X arrow
    float color[4];
    color[0] = 0.7f;
    color[1] = 0.7f;
    color[2] = 1.0f;
    color[3] = 1.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
    drawArrow(length, 0.01 * length);

    // Y arrow
    color[0] = 1.0f;
    color[1] = 0.7f;
    color[2] = 0.7f;
    color[3] = 1.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawArrow(length, 0.01 * length);
    glPopMatrix();

    // Z arrow
    color[0] = 0.7f;
    color[1] = 1.0f;
    color[2] = 0.7f;
    color[3] = 1.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawArrow(length, 0.01 * length);
    glPopMatrix();

    if (colorMaterial)
      glEnable(GL_COLOR_MATERIAL);
    if (!lighting)
      glDisable(GL_LIGHTING);
}

void AMKglViewer::drawGrid(double size, int nbSubdivisions)
{
    GLboolean lighting;
    glGetBooleanv(GL_LIGHTING, &lighting);

    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    for (int i = 0; i <= nbSubdivisions; ++i) {
      const double pos = size * (2.0 * i / nbSubdivisions - 1.0);
      glVertex2d(pos, -size);
      glVertex2d(pos, +size);
      glVertex2d(-size, pos);
      glVertex2d(size, pos);
    }
    glEnd();

    if (lighting)
      glEnable(GL_LIGHTING);
}

void AMKglViewer::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    // set background color
    glClearColor(GLclampf(m_backgroundColor.redF()),
                 GLclampf(m_backgroundColor.greenF()),
                 GLclampf(m_backgroundColor.blueF()),
                 GLclampf(m_backgroundColor.alphaF()));

    // clear buffers
    if (format().stereo())
    {
        glDrawBuffer(GL_BACK_RIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawBuffer(GL_BACK_LEFT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    m_newOpenglContext = context();

    Q_ASSERT(m_newOpenglContext != nullptr);
    if (m_newOpenglContext == nullptr) {qDebug() << "AMKglViewer::initializeGL" << ", " <<  "m_newOpenglContext == nullptr"; return;}

    connect(m_newOpenglContext, SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);

    OPENGL_INFO->init();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    const GLubyte *glV = glGetString(GL_VERSION);
    const GLubyte *glShV = glGetString(GL_SHADING_LANGUAGE_VERSION);

    AMKglLOG->addDebugMessage(QString("gl version : %1 [%2.%3]").arg(QString(reinterpret_cast<const char*>(glV))).arg(getNewOpenGlContext()->format().majorVersion()).arg(getNewOpenGlContext()->format().minorVersion()));
    AMKglLOG->addDebugMessage(QString("gl shader version : %1").arg(QString(reinterpret_cast<const char*>(glShV))));

    if((m_document != nullptr) && (m_permanentItemScene == nullptr))
        setPermanentSceneToRender(m_document->getPermanentSceneToRender());

    init();
}

void AMKglViewer::paintGL()
{
    redraw();
}

void AMKglViewer::resizeGL(int width, int height)
{
    SuperClass::resizeGL(width, height);
    glViewport(0, 0, GLint(width), GLint(height));
    camera()->setScreenWidthAndHeight(this->width(), this->height());
}

void AMKglViewer::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    if(m_takeScreenshot) {
        delete m_glBuffer;
        m_glBuffer = new QGLFramebufferObject(size()*m_screenshotMultiplicationFactor);
    }

    makeCurrent();

    preDraw();

    if (camera()->isManipulated())
        fastDraw();
    else
        draw();

    postDraw();

    checkOpenglError();

    doneCurrent();
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

    // Init in normal mode
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera()->setUseNormalMode(true);
    camera()->loadProjectionMatrix();
    camera()->loadModelViewMatrix();

    // Save current OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(GLclampf(m_backgroundColor.redF()),
                 GLclampf(m_backgroundColor.greenF()),
                 GLclampf(m_backgroundColor.blueF()),
                 GLclampf(m_backgroundColor.alphaF()));

    // Go to anormal mode
    camera()->setUseNormalMode(false);
    camera()->loadModelViewMatrix(true);

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

    m_inFastDraw = true;

    if(getPermanentSceneToRender() != nullptr) {
        if(!getPermanentSceneToRender()->mustReduceNumberOfPointsInNormalDraw()
                && !getPermanentSceneToRender()->mustReduceNumberOfPointsInFastDraw()) {
            m_inFastDraw = false;
            getPermanentSceneToRender()->draw(*m_drawInfo);
            return;
        }

        if(getPermanentSceneToRender()->mustReduceNumberOfPointsInFastDraw()) {
            getPermanentSceneToRender()->fastDraw(*m_drawInfo);
        } else {
            m_inFastDraw = false;
            getPermanentSceneToRender()->draw(*m_drawInfo);
        }
    }
}

void AMKglViewer::postDraw()
{
    if(context() != QOpenGLContext::currentContext())
        return;
//    if(isFastDrawModeCurrentlyUsed())
//        m_drawInfo->drawText("FAST MODE");
//    else
//        m_drawInfo->drawText("NORMAL MODE");

    // Return no normal mode
    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    camera()->setUseNormalMode(true);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    const auto cameraPositionBackup = camera()->position();
    camera()->setPosition(cameraPositionBackup - camera()->pivotPoint());
    camera()->loadModelViewMatrix(true);
    camera()->setPosition(cameraPositionBackup);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    subPostDraw(*m_drawInfo);

    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Save OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // Set neutral GL state
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);
#ifdef GL_TEXTURE_3D // OpenGL 1.2 Only...
    glDisable(GL_TEXTURE_3D);
#endif

    glDisable(GL_TEXTURE_GEN_Q);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

#ifdef GL_RESCALE_NORMAL // OpenGL 1.2 Only...
    glEnable(GL_RESCALE_NORMAL);
#endif

    glDisable(GL_COLOR_MATERIAL);
    glColor4f(GLfloat(m_foregroundColor.redF()),
              GLfloat(m_foregroundColor.greenF()),
              GLfloat(m_foregroundColor.blueF()),
              GLfloat(m_foregroundColor.alphaF()));

    if (gridIsDrawn()) {
        glLineWidth(1.0);
        drawGrid(camera()->sceneRadius());
    }
    if (axisIsDrawn()) {
        glLineWidth(2.0);
        drawAxis(camera()->sceneRadius());
    }

    // FPS computation
    const unsigned int maxCounter = 20;
    if (++m_fpsCounter == maxCounter) {
        const qint64 elapsed = m_postDrawElapsedTimer.restart();

        if(elapsed > 0)
        {
            const double fps = 1000.0 * (double(maxCounter) / double(elapsed));
            m_lastFPSValue = tr("%1Hz").arg(fps, 0, 'f', ((fps < 10.0) ? 1 : 0));
        }

        m_fpsCounter = 0;
    }

    // Restore foregroundColor
    float color[4];
    color[0] = m_foregroundColor.red() / 255.0f;
    color[1] = m_foregroundColor.green() / 255.0f;
    color[2] = m_foregroundColor.blue() / 255.0f;
    color[3] = 1.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    if (FPSIsDisplayed())
    {
        drawText(10,
                 int(1.5 * ((QApplication::font().pixelSize() > 0)
                            ? QApplication::font().pixelSize()
                            : QApplication::font().pointSize())),
                 m_lastFPSValue);
    }

    // Restore GL state
    camera()->loadModelViewMatrix(true);
    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    m_painter->endNativePainting();

    drawOverlay(*m_painter, *m_drawInfo);

    m_drawInfo->drawAllTextSaved();

    m_painter->end();

    if(m_takeScreenshot) {
        m_takeScreenshot = false;

        const QImage img = m_glBuffer->toImage();
        img.save(QFileInfo(snapshotFileName()).absoluteFilePath());

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

    m_camera->mousePressEvent(e);
}

void AMKglViewer::mouseMoveEvent(QMouseEvent *e)
{
    if(m_params.nMouseButtonPressed > 0)
        restartFastDrawTimer();

    if(getPickingAction()->mouseMoveEvent(e))
        return;

    m_camera->mouseMoveEvent(e);
}

void AMKglViewer::mouseReleaseEvent(QMouseEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->mouseReleaseEvent(e))
        return;

    --m_params.nMouseButtonPressed;
    m_params.nMouseButtonPressed = qMax(m_params.nMouseButtonPressed, 0);

    m_camera->mouseReleaseEvent(e);
}

void AMKglViewer::mouseDoubleClickEvent(QMouseEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->mouseDoubleClickEvent(e))
        return;

    m_camera->mouseDoubleClickEvent(e);
}

void AMKglViewer::wheelEvent(QWheelEvent *e)
{
    restartFastDrawTimer();

    if(getPickingAction()->wheelEvent(e))
        return;

    m_camera->wheelEvent(e);
}

void AMKglViewer::resizeEvent(QResizeEvent *e)
{
    restartFastDrawTimer();
    SuperClass::resizeEvent(e);
}

void AMKglViewer::showEvent(QShowEvent *e)
{
    restartFastDrawTimer();
    SuperClass::showEvent(e);
}

void AMKglViewer::keyPressEvent(QKeyEvent *e)
{
    if(getPickingAction()->keyPressEvent(e))
        return;

    m_camera->keyPressEvent(e);
}

void AMKglViewer::keyReleaseEvent(QKeyEvent *e)
{
    if(getPickingAction()->keyReleaseEvent(e))
        return;

    m_camera->keyReleaseEvent(e);
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

void AMKglViewer::restartFastDrawTimer()
{
    m_fastDrawTimer.start();
    m_params.fastDraw = true;

    emit fastDrawSet();
}
