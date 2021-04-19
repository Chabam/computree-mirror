#include "g3dcameracontroller.h"

#include "tools/qglviewer/qglviewertools.h"

#include <Eigen/LU>

#if !defined (_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <QtMath>
#include <QQuaternion>
#include <Qt3DRender/QCamera>

G3DCameraController::G3DCameraController() : DM_GraphicsViewCamera()
{
    m_camManipulator = nullptr;
    m_view = nullptr;
}

void G3DCameraController::setRealCameraManipulator(const GGraphicsViewImp::CameraType *manipulator)
{
    m_camManipulator = (GGraphicsViewImp::CameraType*)manipulator;

    connect(m_camManipulator, SIGNAL(manipulated()), this, SIGNAL(coordinatesChanged()), Qt::DirectConnection);
    //connect(m_camManipulator, SIGNAL(viewCenterChanged(const QVector3D)), this, SLOT(setViewCenter(const QVector3D)), Qt::DirectConnection);
}

AMKglViewer::CameraType *G3DCameraController::getRealCameraManipulator() const
{
    return m_camManipulator;
}

void G3DCameraController::setView(const GGraphicsViewImp *view)
{
    m_view = (GGraphicsViewImp*)view;

    connect(m_view->signalEmitter(), SIGNAL(drawingStarted()), this, SLOT(viewDrawBegin()), Qt::DirectConnection);
    connect(m_view->signalEmitter(), SIGNAL(drawingFinished()), this, SLOT(viewDrawFinished()), Qt::DirectConnection);
}

void G3DCameraController::setLastItemSelectedCameraCenter(double x, double y, double z)
{
    m_lastItemSelectedCenter = Eigen::Vector3d(x, y, z);
}

void G3DCameraController::emitCoordinatesChanged()
{
    emit coordinatesChanged();
}

double G3DCameraController::x() const
{
    return m_camManipulator->position().x();
}

double G3DCameraController::y() const
{
    return m_camManipulator->position().y();
}

double G3DCameraController::z() const
{
    return m_camManipulator->position().z();
}

double G3DCameraController::cx() const
{
    return m_camManipulator->viewCenter().x();
}

double G3DCameraController::cy() const
{
    return m_camManipulator->viewCenter().y();
}

double G3DCameraController::cz() const
{
    return m_camManipulator->viewCenter().z();
}

double G3DCameraController::rx() const
{
    return m_camManipulator->viewVector()[0];
}

double G3DCameraController::ry() const
{
    return m_camManipulator->viewVector()[1];
}

double G3DCameraController::rz() const
{
    return m_camManipulator->viewVector()[2];
}

double G3DCameraController::rw() const
{
    return m_camManipulator->viewVector()[3];
}

float G3DCameraController::focusDistance() const
{
    return m_camManipulator->viewCenter().length();
}

CameraInterface::CameraType G3DCameraController::type() const
{
    return (m_camManipulator->projectionType() == Qt3DRender::QCameraLens::OrthographicProjection) ? CameraInterface::ORTHOGRAPHIC : CameraInterface::PERSPECTIVE;
}

const GLdouble* G3DCameraController::orientationMatrix() const
{
    return QGLViewerTools::matrixToGLDouble(m_camManipulator->projectionMatrix());
}

const GLdouble* G3DCameraController::orientationMatrixInversed() const
{
    return QGLViewerTools::matrixToGLDouble(m_camManipulator->projectionMatrix().inverted());
}

void G3DCameraController::modelViewMatrix(GLdouble m[16]) const
{
    *m = *QGLViewerTools::matrixToGLDouble(m_camManipulator->projectionMatrix());
}

void G3DCameraController::getOrientation(double &q0, double &q1, double &q2, double &q3) const
{
    q0 = rx();
    q1 = ry();
    q2 = rz();
    q3 = rw();
}

Eigen::Vector3d G3DCameraController::upVector() const
{
    return QGLViewerTools::vecToEigen(m_camManipulator->upVector());
}

Eigen::Vector3d G3DCameraController::rightVector() const
{
    return Eigen::Vector3d(1.0, 0.0, 0.0);
    //return QGLViewerTools::vecToEigen(m_camManipulator->rightVector());
}

////////////// SLOTS ////////////////

void G3DCameraController::setX(double x)
{
    QVector3D p = m_camManipulator->position();
    p.setX(x);

    m_camManipulator->setPosition(p);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setY(double y)
{
    QVector3D p = m_camManipulator->position();
    p.setY(y);

    m_camManipulator->setPosition(p);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setZ(double z)
{
    QVector3D p = m_camManipulator->position();
    p.setZ(z);

    m_camManipulator->setPosition(p);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setCX(double cx)
{
    QVector3D c = m_camManipulator->viewCenter();
    c.setX(cx);

    m_camManipulator->setViewCenter(c);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setCY(double cy)
{
    QVector3D c = m_camManipulator->viewCenter();
    c.setY(cy);

    m_camManipulator->setViewCenter(c);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setCZ(double cz)
{
    QVector3D c = m_camManipulator->viewCenter();
    c.setZ(cz);

    m_camManipulator->setViewCenter(c);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setRX(double xRot)
{
    QQuaternion quat = m_camManipulator->rotation(0.0, m_camManipulator->viewVector());
    quat.setX(xRot);
    m_camManipulator->rotate(quat);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setRY(double yRot)
{
    QQuaternion quat = m_camManipulator->rotation(0.0, m_camManipulator->viewVector());
    quat.setY(yRot);
    m_camManipulator->rotate(quat);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setRZ(double zRot)
{
    QQuaternion quat = m_camManipulator->rotation(0.0, m_camManipulator->viewVector());
    quat.setZ(zRot);
    m_camManipulator->rotate(quat);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setRW(double wRot)
{
    QQuaternion quat = m_camManipulator->rotation(0.0, m_camManipulator->viewVector());
    quat.setScalar(wRot);
    m_camManipulator->rotate(quat);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setType(CameraInterface::CameraType type)
{
    m_camManipulator->setProjectionType((type == CameraInterface::ORTHOGRAPHIC) ? Qt3DRender::QCameraLens::OrthographicProjection : Qt3DRender::QCameraLens::PerspectiveProjection);
}

void G3DCameraController::setPointOfView(double cx, double cy, double cz,
                                         double px, double py, double pz,
                                         double rx, double ry, double rz, double rw,
                                         bool redrawView)
{
    m_camManipulator->setViewCenter(QVector3D(cx, cy, cz));
    m_camManipulator->rotate(QQuaternion(rx, ry, rz, rw));
    m_camManipulator->setPosition(QVector3D(px, py, pz));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::setPosition(double x, double y, double z, bool redrawView)
{
    m_camManipulator->setPosition(QVector3D(x, y, z));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::setSceneCenter(double cx, double cy, double cz, bool redrawView)
{
    preventBugInQGLViewerCamera();
    m_camManipulator->setViewCenter(QVector3D(cx, cy, cz));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::setViewDirection(double rx, double ry, double rz, bool redrawView)
{
    m_camManipulator->setViewCenter(QVector3D(rx, ry, rz));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::setOrientation(double q0, double q1, double q2, double q3, bool redrawView)
{
    m_camManipulator->rotate(QQuaternion(q0, q1, q2, q3));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::showEntireScene()
{
    m_camManipulator->viewAll();

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::syncWithCamera(const DM_GraphicsViewCamera *cam)
{
    const G3DCameraController *controller = dynamic_cast<const G3DCameraController*>(cam);

    if(controller != nullptr) {
        m_camManipulator = controller->getRealCameraManipulator();
        //m_camManipulator->setScreenWidthAndHeight(m_view->width(), m_view->height());

        redrawTheView();
    }
}

void G3DCameraController::alignCameraToInvXAxis()
{
    //GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    float dist = m_camManipulator->viewCenter().length();

    // vue de dos
    m_camManipulator->setViewCenter(QVector3D(0.0f, -dist, 0.0f));
    m_camManipulator->setUpVector(QVector3D(0.0f, 0.0f, 1.0f));
    //m_camManipulator->frame()->setTranslation(GGraphicsViewImp::VecType(center.x, center.y+dist, center.z));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToInvYAxis()
{
    //GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    float dist = m_camManipulator->viewCenter().length();

    // vue coté droit
    m_camManipulator->setViewCenter(QVector3D(-dist, 0.0f, 0.0f));
    m_camManipulator->setUpVector(QVector3D(0.0f, 0.0f, 1.0f));
    //m_camManipulator->frame()->setTranslation(GGraphicsViewImp::VecType(center.x+dist, center.y, center.z));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToInvZAxis()
{
    //GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    float dist = m_camManipulator->viewCenter().length();

    // vue de dessous
    m_camManipulator->setViewCenter(QVector3D(0.0f, 0.0f, dist));
    m_camManipulator->setUpVector(QVector3D(0.0f, -1.0f, 0.0f));
    //m_camManipulator->frame()->setTranslation(GGraphicsViewImp::VecType(center.x, center.y, center.z-dist));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToXAxis()
{
    //GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    float dist = m_camManipulator->viewCenter().length();

    // vue de face
    m_camManipulator->setViewCenter(QVector3D(0.0f, dist, 0.0f));
    m_camManipulator->setUpVector(QVector3D(0.0f, 0.0f, 1.0f));
    //m_camManipulator->frame()->setTranslation(GGraphicsViewImp::VecType(center.x, center.y-dist, center.z));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToYAxis()
{
    //GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    float dist = m_camManipulator->viewCenter().length();

    // vue coté gauche
    m_camManipulator->setViewCenter(QVector3D(dist, 0.0f, 0.0f));
    m_camManipulator->setUpVector(QVector3D(0.0f, 0.0f, 1.0f));
    //m_camManipulator->frame()->setTranslation(GGraphicsViewImp::VecType(center.x-dist, center.y, center.z));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToZAxis()
{
    //GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    float dist = m_camManipulator->viewCenter().length();

    // vue de dessus
    m_camManipulator->setViewCenter(QVector3D(0.0f, 0.0f, -dist));
    m_camManipulator->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    //m_camManipulator->frame()->setTranslation(GGraphicsViewImp::VecType(center.x, center.y, center.z+dist));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::initCameraCenter()
{
    setSceneCenter(0, 0, 0, true);
}

void G3DCameraController::fixCameraCenterToItemsBarycenter()
{
    Eigen::Vector3d min, max;
    m_view->getBoundingBoxOfAllItemDrawablePresentInView(min, max);
    setCenterToSpecifiedBox(min, max);
}

void G3DCameraController::fixCameraCenterToSelectedItemsBarycenter()
{
    Eigen::Vector3d min, max;
    m_view->getBoundingBoxOfAllItemDrawableSelectedInView(min, max);

    if(max[0] < min[0])
        m_view->getBoundingBoxOfAllPointsSelectedInView(min, max);

    if(max[0] < min[0])
        m_view->getBoundingBoxOfAllEdgesSelectedInView(min, max);

    if(max[0] < min[0])
        m_view->getBoundingBoxOfAllFacesSelectedInView(min, max);

    setCenterToSpecifiedBox(min, max);
}

void G3DCameraController::fitCameraToVisibleItems()
{
    Eigen::Vector3d min, max;
    m_view->getBoundingBoxOfAllItemDrawablePresentInView(min, max);
    fitToSpecifiedBox(min, max);
}

void G3DCameraController::fitToSpecifiedBox(const Eigen::Vector3d &bot, const Eigen::Vector3d &top)
{
    double size = (top-bot).norm();

    if((size > 0) && (top[0] > bot[0])) {
        m_camManipulator->viewSphere(m_camManipulator->viewCenter(),size);

        preventBugInQGLViewerCamera();

        //m_camManipulator->fitBoundingBox(QGLViewerTools::eigenToVec(bot),
        //                                 QGLViewerTools::eigenToVec(top));

        redrawTheView();

        emit coordinatesChanged();
    }
}

void G3DCameraController::setCenterToSpecifiedBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max, bool redrawView)
{
    double size = (max-min).norm();

    if(max[0] >= min[0]) {

        Eigen::Array3d center;

        if(size == 0)
            center = min.array();
        else
            center = ((min.array()+max.array())/2.0);

        setSceneCenter(center[0],
                       center[1],
                       center[2],
                       redrawView);
    }
}

void G3DCameraController::homePosition()
{
    preventBugInQGLViewerCamera();

    m_camManipulator->viewAll();
    m_camManipulator->setUpVector(QVector3D(0.0f, 0.0f, 1.0f));

    //m_camManipulator->setSceneRadius(1);
    //m_camManipulator->setSceneCenter(GGraphicsViewImp::VecType(0,0,0));
    //m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(0.0f, -1.0f, 0.0f));
    //m_camManipulator->setUpVector(GGraphicsViewImp::VecType(0.0f, 0.0f, 1.0f));
    //m_camManipulator->frame()->setTranslation(GGraphicsViewImp::VecType(0, 1, 0));

    redrawTheView();
}

void G3DCameraController::preventBugInQGLViewerCamera()
{
    // Useless without libQGLViewer ...

    // m_camManipulator->setFieldOfView(M_PI / 2.0);
    // // to correct a bug when the camera is in orthographic mode ; the attribute orthoCoef_ is not updated but with this trick it will be
    // if(m_camManipulator->projectionType() ==  Qt3DRender::QCameraLens::OrthographicProjection)
    // {
    //     m_camManipulator->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    //     m_camManipulator->setProjectionType(Qt3DRender::QCameraLens::OrthographicProjection);
    // }
}

void G3DCameraController::viewDrawBegin()
{
    m_cameraInfoBackup.modelViewProjectionMatrix = m_camManipulator->projectionMatrix();
}

void G3DCameraController::viewDrawFinished()
{
    QMatrix4x4 matrix = m_camManipulator->projectionMatrix();

    if(matrix != m_cameraInfoBackup.modelViewProjectionMatrix) {
        m_cameraInfoBackup.modelViewProjectionMatrix = matrix;
        emitCoordinatesChanged();
    }
}

void G3DCameraController::redrawTheView()
{
    if(m_view != nullptr)
        m_view->redraw();
}
