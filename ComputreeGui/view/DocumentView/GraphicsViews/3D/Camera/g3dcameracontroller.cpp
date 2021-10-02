#if !defined (_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <QtMath>

#include <Eigen/LU>

#include "g3dcameracontroller.h"

G3DCameraController::G3DCameraController() : DM_GraphicsViewCamera()
{
    m_camManipulator = nullptr;
    m_view = nullptr;
}

void G3DCameraController::setRealCameraManipulator(const GGraphicsViewImp::CameraType *manipulator)
{
    m_camManipulator = const_cast<GGraphicsViewImp::CameraType*>(manipulator);

    connect(m_camManipulator, &GGraphicsViewImp::CameraType::manipulated, this, &G3DCameraController::coordinatesChanged, Qt::DirectConnection);
}

AMKglViewer::CameraType *G3DCameraController::getRealCameraManipulator() const
{
    return m_camManipulator;
}

void G3DCameraController::setView(const GGraphicsViewImp *view)
{
    m_view = const_cast<GGraphicsViewImp*>(view);

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
    return m_camManipulator->sceneCenter().x();
}

double G3DCameraController::cy() const
{
    return m_camManipulator->sceneCenter().y();
}

double G3DCameraController::cz() const
{
    return m_camManipulator->sceneCenter().z();
}

double G3DCameraController::rx() const
{
    return m_camManipulator->orientation().x();
}

double G3DCameraController::ry() const
{
    return m_camManipulator->orientation().y();
}

double G3DCameraController::rz() const
{
    return m_camManipulator->orientation().z();
}

double G3DCameraController::rw() const
{
    return m_camManipulator->orientation().w();
}

float G3DCameraController::focusDistance() const
{
    return float(m_camManipulator->focusDistance());
}

CameraInterface::CameraType G3DCameraController::type() const
{
    return (m_camManipulator->projectionType() == GGraphicsViewImp::CameraType::PT_ORTHOGRAPHIC) ? CameraInterface::ORTHOGRAPHIC : CameraInterface::PERSPECTIVE;
}

const GLdouble* G3DCameraController::orientationMatrix() const
{
    m_camManipulator->getModelViewMatrix(const_cast<Eigen::Matrix4d&>(m_tmpMatrix).data());
    return m_tmpMatrix.data();
}

const GLdouble* G3DCameraController::orientationMatrixInversed() const
{
    m_camManipulator->getModelViewMatrix(const_cast<Eigen::Matrix4d&>(m_tmpMatrix).data());
    const_cast<Eigen::Matrix4d&>(m_tmpMatrix) = m_tmpMatrix.inverse();
    return m_tmpMatrix.data();
}

void G3DCameraController::modelViewMatrix(GLdouble m[16]) const
{
    m_camManipulator->getModelViewMatrix(m);
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
    return m_camManipulator->upVector();
}

Eigen::Vector3d G3DCameraController::rightVector() const
{
    return m_camManipulator->rightVector();
}

////////////// SLOTS ////////////////

void G3DCameraController::setX(double x)
{
    GGraphicsViewImp::VecType p = m_camManipulator->position();
    p.x() = x;

    m_camManipulator->setPosition(p);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setY(double y)
{
    GGraphicsViewImp::VecType p = m_camManipulator->position();
    p.y() = y;

    m_camManipulator->setPosition(p);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setZ(double z)
{
    GGraphicsViewImp::VecType p = m_camManipulator->position();
    p.z() = z;

    m_camManipulator->setPosition(p);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setCX(double cx)
{
    GGraphicsViewImp::VecType c = m_camManipulator->sceneCenter();
    c.x() = cx;

    m_camManipulator->setSceneCenter(c);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setCY(double cy)
{
    GGraphicsViewImp::VecType c = m_camManipulator->sceneCenter();
    c.y() = cy;

    m_camManipulator->setSceneCenter(c);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setCZ(double cz)
{
    GGraphicsViewImp::VecType c = m_camManipulator->sceneCenter();
    c.z() = cz;

    m_camManipulator->setSceneCenter(c);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setRX(double xRot)
{
    GGraphicsViewImp::QuatType quat = m_camManipulator->orientation();
    quat.x() = xRot;
    m_camManipulator->setOrientation(quat);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setRY(double yRot)
{
    GGraphicsViewImp::QuatType quat = m_camManipulator->orientation();
    quat.y() = yRot;
    m_camManipulator->setOrientation(quat);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setRZ(double zRot)
{
    GGraphicsViewImp::QuatType quat = m_camManipulator->orientation();
    quat.z() = zRot;
    m_camManipulator->setOrientation(quat);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setRW(double wRot)
{
    GGraphicsViewImp::QuatType quat = m_camManipulator->orientation();
    quat.w() = wRot;
    m_camManipulator->setOrientation(quat);

    emitCoordinatesChanged();
    redrawTheView();
}

void G3DCameraController::setType(CameraInterface::CameraType type)
{
    m_camManipulator->setProjectionType((type == CameraInterface::ORTHOGRAPHIC) ? GGraphicsViewImp::CameraType::PT_ORTHOGRAPHIC : GGraphicsViewImp::CameraType::PT_PERSPECTIVE);
}

void G3DCameraController::setPointOfView(double cx, double cy, double cz,
                                         double px, double py, double pz,
                                         double rx, double ry, double rz, double rw,
                                         bool redrawView)
{
    GGraphicsViewImp::QuatType orientation;
    orientation.x() = rx;
    orientation.y() = ry;
    orientation.z() = rz;
    orientation.w() = rw;
    m_camManipulator->setSceneCenter(GGraphicsViewImp::VecType(cx, cy, cz));
    m_camManipulator->setOrientation(orientation);
    m_camManipulator->setPosition(GGraphicsViewImp::VecType(px, py, pz));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::setPosition(double x, double y, double z, bool redrawView)
{
    m_camManipulator->setPosition(GGraphicsViewImp::VecType(x, y, z));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::setSceneCenter(double cx, double cy, double cz, bool redrawView)
{
    preventBugInQGLViewerCamera();
    m_camManipulator->setSceneCenter(GGraphicsViewImp::VecType(cx, cy, cz));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::setViewDirection(double rx, double ry, double rz, bool redrawView)
{
    m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(rx, ry, rz));

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::setOrientation(double q0, double q1, double q2, double q3, bool redrawView)
{
    GGraphicsViewImp::QuatType orientation;
    orientation.x() = q0;
    orientation.y() = q1;
    orientation.z() = q2;
    orientation.w() = q3;

    m_camManipulator->setOrientation(orientation);

    if(redrawView)
        redrawTheView();

    emitCoordinatesChanged();
}

void G3DCameraController::showEntireScene()
{
    m_camManipulator->showEntireScene();

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::syncWithCamera(const DM_GraphicsViewCamera *cam)
{
    const G3DCameraController *controller = dynamic_cast<const G3DCameraController*>(cam);

    if(controller != nullptr) {
        (*m_camManipulator) = *controller->getRealCameraManipulator();
        m_camManipulator->setScreenWidthAndHeight(m_view->width(), m_view->height());

        redrawTheView();
    }
}

void G3DCameraController::alignCameraToInvXAxis()
{
    GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    const double dist = m_camManipulator->distanceToSceneCenter();

    // vue de dos
    m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(0.0, -1.0, 0.0));
    m_camManipulator->setUpVector(GGraphicsViewImp::VecType(0.0, 0.0, 1.0));
    m_camManipulator->setTranslation(GGraphicsViewImp::VecType(center.x(), center.y()+dist, center.z()));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToInvYAxis()
{
    GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    const double dist = m_camManipulator->distanceToSceneCenter();

    // vue coté droit
    m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(-1.0, 0.0, 0.0));
    m_camManipulator->setUpVector(GGraphicsViewImp::VecType(0.0, 0.0, 1.0));
    m_camManipulator->setTranslation(GGraphicsViewImp::VecType(center.x()+dist, center.y(), center.z()));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToInvZAxis()
{
    GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    const double dist = m_camManipulator->distanceToSceneCenter();

    // vue de dessous
    m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(0.0, 0.0, 1.0));
    m_camManipulator->setUpVector(GGraphicsViewImp::VecType(0.0, -1.0, 0.0));
    m_camManipulator->setTranslation(GGraphicsViewImp::VecType(center.x(), center.y(), center.z()-dist));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToXAxis()
{
    GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    const double dist = m_camManipulator->distanceToSceneCenter();

    // vue de face
    m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(0.0, 1.0, 0.0));
    m_camManipulator->setUpVector(GGraphicsViewImp::VecType(0.0, 0.0, 1.0));
    m_camManipulator->setTranslation(GGraphicsViewImp::VecType(center.x(), center.y()-dist, center.z()));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToYAxis()
{
    GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    const double dist = m_camManipulator->distanceToSceneCenter();

    // vue coté gauche
    m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(1.0, 0.0, 0.0));
    m_camManipulator->setUpVector(GGraphicsViewImp::VecType(0.0, 0.0, 1.0));
    m_camManipulator->setTranslation(GGraphicsViewImp::VecType(center.x()-dist, center.y(), center.z()));

    redrawTheView();
    emitCoordinatesChanged();
}

void G3DCameraController::alignCameraToZAxis()
{
    GGraphicsViewImp::VecType center = m_camManipulator->sceneCenter();
    const double dist = m_camManipulator->distanceToSceneCenter();

    // vue de dessus
    m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(0.0, 0.0, -1.0));
    m_camManipulator->setUpVector(GGraphicsViewImp::VecType(0.0, 1.0, 0.0));
    m_camManipulator->setTranslation(GGraphicsViewImp::VecType(center.x(), center.y(), center.z()+dist));

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
        m_camManipulator->setSceneRadius(size);

        preventBugInQGLViewerCamera();

        m_camManipulator->fitBoundingBox(bot,
                                         top);

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
    m_camManipulator->setSceneRadius(1);
    m_camManipulator->setSceneCenter(GGraphicsViewImp::VecType(0,0,0));
    m_camManipulator->setViewDirection(GGraphicsViewImp::VecType(0.0, -1.0, 0.0));
    m_camManipulator->setUpVector(GGraphicsViewImp::VecType(0.0, 0.0, 1.0));
    m_camManipulator->setTranslation(GGraphicsViewImp::VecType(0, 1, 0));

    redrawTheView();
}

void G3DCameraController::preventBugInQGLViewerCamera()
{
    GGraphicsViewImp::CameraType::ProjectionType type = m_camManipulator->projectionType();

    // to correct a bug when the camera is in orthographic mode ; the attribute orthoCoef_ is not updated
    // but with this trick it will be
    // TODO AMKGL : remove this trick when refactor camera
    if(type == GGraphicsViewImp::CameraType::PT_ORTHOGRAPHIC) {
        m_camManipulator->setFieldOfView(M_PI / 2.0);
        m_camManipulator->setProjectionType(GGraphicsViewImp::CameraType::PT_PERSPECTIVE);
        m_camManipulator->setProjectionType(GGraphicsViewImp::CameraType::PT_ORTHOGRAPHIC);
    } else {
        m_camManipulator->setFieldOfView(M_PI / 2.0);
    }
}

void G3DCameraController::viewDrawBegin()
{
    m_camManipulator->getModelViewProjectionMatrix(m_cameraInfoBackup.modelViewProjectionMatrix.data());
}

void G3DCameraController::viewDrawFinished()
{
    Eigen::Matrix4d matrix;
    m_camManipulator->getModelViewProjectionMatrix(matrix.data());

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
