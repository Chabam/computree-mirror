#include "amkglcamera.h"

#include <Eigen/Geometry>
#include <gl/GLU.h>
#include <limits>

#include "tools/math.h"
#include "tools/amkgldomutils.h"

AMKglCamera::AMKglCamera() :
    m_useNormalCamera(true),
    m_fieldOfView(M_PI / 4.0),
    m_pivotPoint(0.0, 0.0, 0.0),
    m_rotationConstraintEnabled(false),
    m_modelViewMatrixIsUpToDate(false),
    m_projectionMatrixIsUpToDate(false),
    m_position(0.0, 0.0, 0.0),
    m_orientation(1.0, 0.0, 0.0, 0.0),
    m_sceneUpVector(0.0, 1.0, 0.0),
    m_rotatesAroundUpVector(false),
    m_zoomsOnPivotPoint(false),
    m_currentAction(A_NoAction),
    m_manipulated(false)
{
    setRotationSensitivity(1.0);
    setTranslationSensitivity(1.0);
    setWheelSensitivity(1.0);
    setZoomSensitivity(1.0);

    setSceneRadius(1.0);
    m_orthoCoef = tan(fieldOfView() / 2.0);
    setSceneCenter(Eigen::Vector3d(0.0, 0.0, 0.0));
    setProjectionType(PT_ORTHOGRAPHIC);
    setZNearCoefficient(0.005);
    setZClippingCoefficient(sqrt(3.0));
    setScreenWidthAndHeight(600, 400);

    for (unsigned short j = 0; j < 16; ++j)
    {
        m_modelViewMatrix[j] = ((j % 5 == 0) ? 1.0 : 0.0);
        m_projectionMatrix[j] = 0.0;
    }

    computeProjectionMatrix();
}

AMKglCamera::AMKglCamera(const AMKglCamera& other)
{
    for (unsigned short j = 0; j < 16; ++j)
    {
        m_modelViewMatrix[j] = ((j % 5 == 0) ? 1.0 : 0.0);
        m_projectionMatrix[j] = 0.0;
    }

    (*this) = other;
}

AMKglCamera& AMKglCamera::operator=(const AMKglCamera& other)
{
    setScreenWidthAndHeight(other.screenWidth(), other.screenHeight());
    setFieldOfView(other.fieldOfView());
    setSceneRadius(other.sceneRadius());
    setSceneCenter(other.sceneCenter());
    setZNearCoefficient(other.zNearCoefficient());
    setZClippingCoefficient(other.zClippingCoefficient());
    setProjectionType(other.projectionType());

    m_orthoCoef = other.orthographicCoefficient();
    m_projectionMatrixIsUpToDate = false;

    setPosition(other.position());
    setOrientation(other.orientation());

    computeProjectionMatrix();
    computeModelViewMatrix();

    return *this;
}

void AMKglCamera::enableRotationConstraint(bool e)
{
    m_rotationConstraintEnabled = e;
}

bool AMKglCamera::isRotationConstraintEnabled() const
{
    return m_rotationConstraintEnabled;
}

Eigen::Vector3d AMKglCamera::position() const
{
    return m_position;
}

Eigen::Vector3d AMKglCamera::translation() const
{
    return position();
}

Eigen::Vector3d AMKglCamera::upVector() const
{
    return localInverseTransformOf(Eigen::Vector3d(0.0, 1.0, 0.0));
}

Eigen::Vector3d AMKglCamera::viewDirection() const
{
    return localInverseTransformOf(Eigen::Vector3d(0.0, 0.0, -1.0));
}

Eigen::Vector3d AMKglCamera::rightVector() const
{
    return localInverseTransformOf(Eigen::Vector3d(1.0, 0.0, 0.0));
}

Eigen::Quaterniond AMKglCamera::orientation() const
{
    return m_orientation;
}

void AMKglCamera::setPosition(const Eigen::Vector3d& pos)
{
    m_position = pos;
    modified();
}

void AMKglCamera::setTranslation(const Eigen::Vector3d& pos)
{
    setPosition(pos);
}

void AMKglCamera::setOrientation(const Eigen::Quaterniond& q)
{
    m_orientation = q;
    modified();
    updateSceneUpVector();
}

void AMKglCamera::setOrientation(double theta, double phi)
{
    Eigen::Vector3d axis(0.0, 1.0, 0.0);
    const Eigen::Quaterniond rot1(Eigen::AngleAxisd(theta, axis.normalized()));
    axis = Eigen::Vector3d(-cos(theta), 0.0, sin(theta));
    const Eigen::Quaterniond rot2(Eigen::AngleAxisd(phi, axis.normalized()));
    setOrientation(rot1 * rot2);
}

void AMKglCamera::setUpVector(const Eigen::Vector3d& up, bool noMove)
{
    Eigen::Quaterniond q = Math::CreateQuaternionThatRotateFromTo(Eigen::Vector3d(0.0, 1.0, 0.0), localTransformOf(up));

    if(!noMove)
        setPosition(pivotPoint() - Math::Rotate(orientation() * q, localCoordinatesOf(pivotPoint())));

    rotate(q);
    updateSceneUpVector();
}

void AMKglCamera::setViewDirection(const Eigen::Vector3d& direction)
{
    if (direction.squaredNorm() < 1E-10)
      return;

    Eigen::Vector3d xAxis = direction.cross(upVector());
    if (xAxis.squaredNorm() < 1E-10) {
      // rotation around X axis
      xAxis = localInverseTransformOf(Eigen::Vector3d(1.0, 0.0, 0.0));
    }

    Eigen::Quaterniond q = Math::CreateQuaternionFromRotatedBasis(xAxis, xAxis.cross(direction), -direction);
    setOrientationWithConstraint(q);
}

void AMKglCamera::lookAt(const Eigen::Vector3d& target)
{
    setViewDirection(target - position());
}

void AMKglCamera::showEntireScene()
{
    fitSphere(sceneCenter(), sceneRadius());
}

void AMKglCamera::fitSphere(const Eigen::Vector3d& center, double radius)
{
    double distance = 0.0;
    switch (projectionType()) {
    case PT_PERSPECTIVE: {
        const double yview = radius / sin(fieldOfView() / 2.0);
        const double xview = radius / sin(horizontalFieldOfView() / 2.0);
        distance = qMax(xview, yview);
        break;
    }
    case PT_ORTHOGRAPHIC: {
        distance = ((center - pivotPoint()).dot(viewDirection())) + (radius / m_orthoCoef);
        break;
    }
    }
    Eigen::Vector3d newPos(center - distance * viewDirection());
    setPositionWithConstraint(newPos);
}

void AMKglCamera::fitBoundingBox(const Eigen::Vector3d& min, const Eigen::Vector3d& max)
{
    double diameter = qMax(fabs(max[1] - min[1]), fabs(max[0] - min[0]));
    diameter = qMax(fabs(max[2] - min[2]), diameter);
    fitSphere(0.5 * (min + max), 0.5 * diameter);
}

void AMKglCamera::centerScene()
{
    projectOnLine(sceneCenter(), viewDirection());
}

double AMKglCamera::fieldOfView() const
{
    return m_fieldOfView;
}

double AMKglCamera::horizontalFieldOfView() const
{
    return 2.0 * atan(tan(fieldOfView() / 2.0) * aspectRatio());
}

double AMKglCamera::aspectRatio() const
{
    return m_screenWidth / double(m_screenHeight);
}

int AMKglCamera::screenWidth() const
{
    return m_screenWidth;
}

int AMKglCamera::screenHeight() const
{
    return m_screenHeight;
}

void AMKglCamera::getViewport(GLint viewport[4]) const
{
    viewport[0] = 0;
    viewport[1] = screenHeight();
    viewport[2] = screenWidth();
    viewport[3] = -screenHeight();
}

double AMKglCamera::pixelGLRatio(const Eigen::Vector3d& position) const
{
    switch (projectionType()) {
    case PT_PERSPECTIVE:
        return 2.0 * fabs((localCoordinatesOf(position)).z()) * tan(fieldOfView() / 2.0) / screenHeight();
    case PT_ORTHOGRAPHIC: {
        GLdouble w, h;
        getOrthoWidthHeight(w, h);
        return 2.0 * h / screenHeight();
    }
    }
    return 1.0;
}

double AMKglCamera::zNearCoefficient() const
{
    return m_zNearCoef;
}

double AMKglCamera::zClippingCoefficient() const
{
    return m_zClippingCoef;
}

double AMKglCamera::orthographicCoefficient() const
{
    return m_orthoCoef;
}

double AMKglCamera::zNear() const
{
    const double zNearScene = zClippingCoefficient() * sceneRadius();
    double z = distanceToSceneCenter() - zNearScene;

    // Avoid zNear <=0
    const double zMin = zNearCoefficient() * zNearScene;
    if (z < zMin)
    {
        switch (projectionType()) {
        case PT_PERSPECTIVE:
            return zMin;
        case PT_ORTHOGRAPHIC:
            return 0.0;
        }
    }

    return z;
}

double AMKglCamera::zFar() const
{
    return distanceToSceneCenter() + zClippingCoefficient() * sceneRadius();
}

void AMKglCamera::getOrthoWidthHeight(GLdouble& halfWidth, GLdouble& halfHeight) const
{
    const double dist = m_orthoCoef * fabs(cameraCoordinatesOf(pivotPoint()).z());

    halfWidth = dist * ((aspectRatio() < 1.0) ? 1.0 : aspectRatio());
    halfHeight = dist * ((aspectRatio() < 1.0) ? 1.0 / aspectRatio() : 1.0);
}

void AMKglCamera::getFrustumPlanesCoefficients(GLdouble coef[6][4]) const
{
    const Eigen::Vector3d pos = position();
    const Eigen::Vector3d viewDir = viewDirection();
    const Eigen::Vector3d up = upVector();
    const Eigen::Vector3d right = rightVector();
    const double posViewDir = pos.dot(viewDir);

    static Eigen::Vector3d normal[6];
    static GLdouble dist[6];

    switch (projectionType()) {
    case PT_PERSPECTIVE: {
        const double hhfov = horizontalFieldOfView() / 2.0;
        const double chhfov = cos(hhfov);
        const double shhfov = sin(hhfov);
        normal[0] = -shhfov * viewDir;
        normal[1] = normal[0] + chhfov * right;
        normal[0] = normal[0] - chhfov * right;

        normal[2] = -viewDir;
        normal[3] = viewDir;

        const double hfov = fieldOfView() / 2.0;
        const double chfov = cos(hfov);
        const double shfov = sin(hfov);
        normal[4] = -shfov * viewDir;
        normal[5] = normal[4] - chfov * up;
        normal[4] = normal[4] + chfov * up;

        for (int i = 0; i < 2; ++i)
            dist[i] = pos.dot(normal[i]);
        for (int j = 4; j < 6; ++j)
            dist[j] = pos.dot(normal[j]);

        // Optimisation for :
        // dist[0,1,4,5] = pos * normal[0,1,4,5];
        // dist[2] = (pos + zNear() * viewDir) * normal[2];
        // dist[3] = (pos + zFar()  * viewDir) * normal[3];

        const double posRightCosHH = chhfov * pos.dot(right);
        dist[0] = -shhfov * posViewDir;
        dist[1] = dist[0] + posRightCosHH;
        dist[0] = dist[0] - posRightCosHH;
        const double posUpCosH = chfov * pos.dot(up);
        dist[4] = -shfov * posViewDir;
        dist[5] = dist[4] - posUpCosH;
        dist[4] = dist[4] + posUpCosH;

        break;
    }
    case PT_ORTHOGRAPHIC:
        normal[0] = -right;
        normal[1] = right;
        normal[4] = up;
        normal[5] = -up;

        GLdouble hw, hh;
        getOrthoWidthHeight(hw, hh);
        dist[0] = (pos - hw * right).dot(normal[0]);
        dist[1] = (pos + hw * right).dot(normal[1]);
        dist[4] = (pos + hh * up).dot(normal[4]);
        dist[5] = (pos - hh * up).dot(normal[5]);
        break;
    }

    // Front and Back planes are the same for Ortho and perspective
    normal[2] = -viewDir;
    normal[3] = viewDir;
    dist[2] = -posViewDir - zNear();
    dist[3] = posViewDir + zFar();

    for (int i = 0; i < 6; ++i) {
        coef[i][0] = GLdouble(normal[i].x());
        coef[i][1] = GLdouble(normal[i].y());
        coef[i][2] = GLdouble(normal[i].z());
        coef[i][3] = dist[i];
    }
}

AMKglCamera::ProjectionType AMKglCamera::projectionType() const
{
    return m_projectionType;
}

void AMKglCamera::setProjectionType(AMKglCamera::ProjectionType type)
{
    if ((type == PT_ORTHOGRAPHIC) && (m_projectionType == PT_PERSPECTIVE))
      m_orthoCoef = tan(fieldOfView() / 2.0);
    m_projectionType = type;
    m_projectionMatrixIsUpToDate = false;
}

void AMKglCamera::setFieldOfView(double fov)
{
    m_fieldOfView = fov;
    setFocusDistance(sceneRadius() / m_orthoCoef);
    m_projectionMatrixIsUpToDate = false;
}

void AMKglCamera::setHorizontalFieldOfView(double hfov)
{
    setFieldOfView(2.0 * atan(tan(hfov / 2.0) / aspectRatio()));
}

void AMKglCamera::setFOVToFitScene()
{
    if (distanceToSceneCenter() > sqrt(2.0) * sceneRadius())
        setFieldOfView(2.0 * asin(sceneRadius() / distanceToSceneCenter()));
    else
        setFieldOfView(M_PI / 2.0);
}

void AMKglCamera::setAspectRatio(double aspect)
{
    setScreenWidthAndHeight(int(100.0 * aspect), 100);
}

void AMKglCamera::setScreenWidthAndHeight(int width, int height)
{
    m_screenWidth = qMax(width, 1);
    m_screenHeight = qMax(height, 1);
    m_projectionMatrixIsUpToDate = false;
}

void AMKglCamera::setZNearCoefficient(double coef)
{
    m_zNearCoef = coef;
    m_projectionMatrixIsUpToDate = false;
}

void AMKglCamera::setZClippingCoefficient(double coef)
{
    m_zClippingCoef = coef;
    m_projectionMatrixIsUpToDate = false;
}

void AMKglCamera::setSceneRadius(double radius)
{
    if (radius <= 0.0)
      return;

    m_sceneRadius = radius;
    m_projectionMatrixIsUpToDate = false;

    setFocusDistance(sceneRadius() / tan(fieldOfView() / 2.0));
}

double AMKglCamera::sceneRadius() const
{
    return m_sceneRadius;
}

void AMKglCamera::setSceneCenter(const Eigen::Vector3d& center)
{
    m_sceneCenter = center;
    setPivotPoint(sceneCenter());
    m_projectionMatrixIsUpToDate = false;
}

Eigen::Vector3d AMKglCamera::sceneCenter() const
{
    return m_sceneCenter;
}

double AMKglCamera::distanceToSceneCenter() const
{
    return fabs((localCoordinatesOf(sceneCenter())).z());
}

void AMKglCamera::setSceneBoundingBox(const Eigen::Vector3d& min, const Eigen::Vector3d& max)
{
    setSceneCenter((min + max) / 2.0);
    setSceneRadius(0.5 * (max - min).norm());
}

void AMKglCamera::setPivotPoint(const Eigen::Vector3d& point)
{
    const double prevDist = fabs(cameraCoordinatesOf(pivotPoint()).z());

    // If frame's RAP is set directly, projectionMatrixIsUpToDate_ should also be
    // set to false to ensure proper recomputation of the ORTHO projection matrix.
    m_pivotPoint = point;

    // orthoCoef_ is used to compensate for changes of the pivotPoint, so that the
    // image does not change when the pivotPoint is changed in ORTHOGRAPHIC mode.
    const double newDist = fabs(cameraCoordinatesOf(pivotPoint()).z());
    // Prevents division by zero when rap is set to camera position
    if ((prevDist > 1E-9) && (newDist > 1E-9))
        m_orthoCoef *= prevDist / newDist;
    m_projectionMatrixIsUpToDate = false;
}

Eigen::Vector3d AMKglCamera::pivotPoint() const
{
    return m_pivotPoint;
}

void AMKglCamera::loadProjectionMatrix(bool reset) const
{
    glMatrixMode(GL_PROJECTION);

    if (reset)
      glLoadIdentity();

    computeProjectionMatrix();

    glMultMatrixd(m_projectionMatrix);
}

void AMKglCamera::loadModelViewMatrix(bool reset) const
{
    if(m_useNormalCamera) {
        glMatrixMode(GL_MODELVIEW);
        computeModelViewMatrix();
        if (reset)
          glLoadMatrixd(m_modelViewMatrix);
        else
          glMultMatrixd(m_modelViewMatrix);
        return;
    }

    glMatrixMode(GL_MODELVIEW);

    if (reset)
    {
        // Identity
        Eigen::Matrix4d m = Eigen::Matrix4d::Identity();

        glLoadMatrixd(m.data());
    }
}

void AMKglCamera::computeProjectionMatrix() const
{
    if (m_projectionMatrixIsUpToDate)
      return;

    const double ZNear = zNear();
    const double ZFar = zFar();

    switch (projectionType()) {
    case PT_PERSPECTIVE: {
      const double f = 1.0 / tan(fieldOfView() / 2.0);
      m_projectionMatrix[0] = f / aspectRatio();
      m_projectionMatrix[5] = f;
      m_projectionMatrix[10] = (ZNear + ZFar) / (ZNear - ZFar);
      m_projectionMatrix[11] = -1.0;
      m_projectionMatrix[14] = 2.0 * ZNear * ZFar / (ZNear - ZFar);
      m_projectionMatrix[15] = 0.0;
      // equiv to gluPerspective( 180.0*fieldOfView()/M_PI, aspectRatio(), zNear(), zFar() );
      break;
    }
    case PT_ORTHOGRAPHIC: {
      GLdouble w, h;
      getOrthoWidthHeight(w, h);
      m_projectionMatrix[0] = 1.0 / w;
      m_projectionMatrix[5] = 1.0 / h;
      m_projectionMatrix[10] = -2.0 / (ZFar - ZNear);
      m_projectionMatrix[11] = 0.0;
      m_projectionMatrix[14] = -(ZFar + ZNear) / (ZFar - ZNear);
      m_projectionMatrix[15] = 1.0;
      // equiv to glOrtho( -w, w, -h, h, zNear(), zFar() );
      break;
    }
    }

    m_projectionMatrixIsUpToDate = true;
}

void AMKglCamera::computeModelViewMatrix() const
{
    if (m_modelViewMatrixIsUpToDate)
      return;

    const Eigen::Quaterniond q = orientation();

    const double q00 = 2.0 * q.x() * q.x();
    const double q11 = 2.0 * q.y() * q.y();
    const double q22 = 2.0 * q.z() * q.z();

    const double q01 = 2.0 * q.x() * q.y();
    const double q02 = 2.0 * q.x() * q.z();
    const double q03 = 2.0 * q.x() * q.w();

    const double q12 = 2.0 * q.y() * q.z();
    const double q13 = 2.0 * q.y() * q.w();

    const double q23 = 2.0 * q.z() * q.w();

    m_modelViewMatrix[0] = 1.0 - q11 - q22;
    m_modelViewMatrix[1] = q01 - q23;
    m_modelViewMatrix[2] = q02 + q13;
    m_modelViewMatrix[3] = 0.0;

    m_modelViewMatrix[4] = q01 + q23;
    m_modelViewMatrix[5] = 1.0 - q22 - q00;
    m_modelViewMatrix[6] = q12 - q03;
    m_modelViewMatrix[7] = 0.0;

    m_modelViewMatrix[8] = q02 - q13;
    m_modelViewMatrix[9] = q12 + q03;
    m_modelViewMatrix[10] = 1.0 - q11 - q00;
    m_modelViewMatrix[11] = 0.0;

    const Eigen::Vector3d t = Math::Rotate(q.inverse(), position());

    m_modelViewMatrix[12] = -t.x();
    m_modelViewMatrix[13] = -t.y();
    m_modelViewMatrix[14] = -t.z();
    m_modelViewMatrix[15] = 1.0;

    m_modelViewMatrixIsUpToDate = true;
}

void AMKglCamera::getProjectionMatrix(GLdouble m[]) const
{
    computeProjectionMatrix();
    for (unsigned short i = 0; i < 16; ++i)
        m[i] = m_projectionMatrix[i];
}

void AMKglCamera::getProjectionMatrix(Eigen::Matrix4d& m) const
{
    static GLdouble mat[16];
    getProjectionMatrix(mat);
    for (unsigned short i = 0; i < 16; ++i)
      m(i) = mat[i];
}

void AMKglCamera::getModelViewMatrix(GLdouble m[]) const
{
    computeModelViewMatrix();
    for (unsigned short i = 0; i < 16; ++i)
        m[i] = m_modelViewMatrix[i];
}

void AMKglCamera::getModelViewMatrix(Eigen::Matrix4d& m) const
{
    static GLdouble mat[16];
    getModelViewMatrix(mat);
    for (unsigned short i = 0; i < 16; ++i)
      m(i) = mat[i];
}

void AMKglCamera::getModelViewProjectionMatrix(GLdouble m[]) const
{
    GLdouble mv[16];
    GLdouble proj[16];
    getModelViewMatrix(mv);
    getProjectionMatrix(proj);

    for (unsigned short i = 0; i < 4; ++i) {
        for (unsigned short j = 0; j < 4; ++j) {
            double sum = 0.0;
            for (unsigned short k = 0; k < 4; ++k)
                sum += proj[i + 4 * k] * mv[k + 4 * j];
            m[i + 4 * j] = sum;
        }
    }
}

void AMKglCamera::getModelViewProjectionMatrix(Eigen::Matrix4d& m) const
{
    static GLdouble mat[16];
    getModelViewProjectionMatrix(mat);
    for (unsigned short i = 0; i < 16; ++i)
        m(i) = mat[i];
}

Eigen::Vector3d AMKglCamera::cameraCoordinatesOf(const Eigen::Vector3d& src) const
{
    return localCoordinatesOf(src);
}

Eigen::Vector3d AMKglCamera::worldCoordinatesOf(const Eigen::Vector3d& src) const
{
    return localInverseCoordinatesOf(src);
}

void AMKglCamera::getCameraCoordinatesOf(const double src[], double res[]) const
{
    const Eigen::Vector3d r = cameraCoordinatesOf(Eigen::Vector3d(src));
    for (int i = 0; i < 3; ++i)
        res[i] = r[i];
}

void AMKglCamera::getWorldCoordinatesOf(const double src[], double res[]) const
{
    const Eigen::Vector3d r = worldCoordinatesOf(Eigen::Vector3d(src));
    for (int i = 0; i < 3; ++i)
        res[i] = r[i];
}

Eigen::Vector3d AMKglCamera::projectedCoordinatesOf(const Eigen::Vector3d& src) const
{
    GLdouble x, y, z;
    static GLint viewport[4];
    getViewport(viewport);

    gluProject(src.x(), src.y(), src.z(), m_modelViewMatrix, m_projectionMatrix,
               viewport, &x, &y, &z);

    return Eigen::Vector3d(x, y, z);
}

Eigen::Vector3d AMKglCamera::unprojectedCoordinatesOf(const Eigen::Vector3d& src) const
{
    GLdouble x, y, z;
    static GLint viewport[4];
    getViewport(viewport);

    gluUnProject(src.x(), src.y(), src.z(), m_modelViewMatrix, m_projectionMatrix,
                 viewport, &x, &y, &z);

    return Eigen::Vector3d(x, y, z);
}

void AMKglCamera::getProjectedCoordinatesOf(const double src[], double res[]) const
{
    const Eigen::Vector3d r = projectedCoordinatesOf(Eigen::Vector3d(src));
    for (int i = 0; i < 3; ++i)
      res[i] = r[i];
}

void AMKglCamera::getUnprojectedCoordinatesOf(const double src[], double res[]) const
{
    const Eigen::Vector3d r = unprojectedCoordinatesOf(Eigen::Vector3d(src));
    for (int i = 0; i < 3; ++i)
      res[i] = r[i];
}

void AMKglCamera::convertClickToLine(const QPoint& pixel, Eigen::Vector3d& orig, Eigen::Vector3d& dir) const
{
    switch (projectionType()) {
    case PT_PERSPECTIVE:
        orig = position();
        dir = Eigen::Vector3d(((2.0 * pixel.x() / screenWidth()) - 1.0) *
                              tan(fieldOfView() / 2.0) * aspectRatio(),
                              ((2.0 * (screenHeight() - pixel.y()) / screenHeight()) - 1.0) *
                              tan(fieldOfView() / 2.0),
                              -1.0);
        dir = worldCoordinatesOf(dir) - orig;
        dir.normalize();
        break;

    case PT_ORTHOGRAPHIC: {
        GLdouble w, h;
        getOrthoWidthHeight(w, h);
        orig = Eigen::Vector3d((2.0 * pixel.x() / screenWidth() - 1.0) * w,
                               -(2.0 * pixel.y() / screenHeight() - 1.0) * h, 0.0);
        orig = worldCoordinatesOf(orig);
        dir = viewDirection();
        break;
    }
    }
}

Eigen::Vector3d AMKglCamera::pointUnderPixel(const QPoint& pixel, bool& found) const
{
    float depth;
    // Qt use upper corner as origin while GL uses lower corner
    glReadPixels(pixel.x(), screenHeight() - 1 - pixel.y(), 1, 1,
                 GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    found = static_cast<double>(depth)< 1.0;
    const Eigen::Vector3d point(pixel.x(), pixel.y(), static_cast<double>(depth));

    return unprojectedCoordinatesOf(point);
}

double AMKglCamera::focusDistance() const
{
    return m_focusDistance;
}

void AMKglCamera::setFocusDistance(double distance)
{
    m_focusDistance = distance;
}

bool AMKglCamera::isManipulated() const
{
    return m_manipulated;
}

void AMKglCamera::align()
{
    Eigen::Quaterniond fOrientation(1, 0, 0, 0);
    Eigen::Vector3d fPosition = m_pivotPoint;

    Eigen::Vector3d directions[2][3];
    for (unsigned short d = 0; d < 3; ++d) {
        Eigen::Vector3d dir((d == 0) ? 1.0 : 0.0, (d == 1) ? 1.0 : 0.0, (d == 2) ? 1.0 : 0.0);

        directions[0][d] = Math::Rotate(fOrientation, dir);
        directions[1][d] = localInverseTransformOf(dir);
    }

    double maxProj = 0.0;
    double proj;
    unsigned short index[2];
    index[0] = index[1] = 0;
    for (unsigned short i = 0; i < 3; ++i)
        for (unsigned short j = 0; j < 3; ++j)
            if ((proj = qFabs(directions[0][i].dot(directions[1][j]))) >= maxProj) {
                index[0] = i;
                index[1] = j;
                maxProj = proj;
            }

    Eigen::Quaterniond fOldOrientation = orientation();
    Eigen::Vector3d fOldPosition = translation();

    double coef = directions[0][index[0]].dot(directions[1][index[1]]);
    if (fabs(coef) >= 0) {
        const Eigen::Vector3d axis = directions[0][index[0]].cross(directions[1][index[1]]);
        double angle = asin(axis.norm());
        if (coef >= 0.0)
            angle = -angle;
        rotate(orientation().inverse() * Eigen::Quaterniond(Eigen::AngleAxisd(angle, axis.normalized())) * orientation());

        // Try to align an other axis direction
        unsigned short d = (index[1] + 1) % 3;
        Eigen::Vector3d dir((d == 0) ? 1.0 : 0.0, (d == 1) ? 1.0 : 0.0, (d == 2) ? 1.0 : 0.0);
        dir = localInverseTransformOf(dir);

        double max = 0.0;
        for (unsigned short i = 0; i < 3; ++i) {
            double proj = qFabs(directions[0][i].dot(dir));
            if (proj > max) {
                index[0] = i;
                max = proj;
            }
        }

        if (max >= 0) {
            const Eigen::Vector3d axis = directions[0][index[0]].cross(dir);
            double angle = asin(axis.norm());
            if (directions[0][index[0]].dot(dir) >= 0.0)
                angle = -angle;
            rotate(orientation().inverse() * Eigen::Quaterniond(Eigen::AngleAxisd(angle, axis.normalized())) * orientation());
        }
    }

    Eigen::Vector3d center;
    center = fPosition;

    translate(center - Math::Rotate(orientation(), Math::Rotate(fOldOrientation.conjugate(), center - fOldPosition)) - translation());
}

void AMKglCamera::setRotationSensitivity(double sensitivity)
{
    m_rotationSensitivity = sensitivity;
}

void AMKglCamera::setTranslationSensitivity(double sensitivity)
{
    m_translationSensitivity = sensitivity;
}

void AMKglCamera::setWheelSensitivity(double sensitivity)
{
    m_wheelSensitivity = sensitivity;
}

void AMKglCamera::setZoomSensitivity(double sensitivity)
{
    m_zoomSensitivity = sensitivity;
}

QDomElement AMKglCamera::domElement(const QString& name, QDomDocument& document) const
{
    QDomElement de = document.createElement(name);

    QDomElement paramNode = document.createElement("Parameters");
    paramNode.setAttribute("fieldOfView", QString::number(fieldOfView()));
    paramNode.setAttribute("zNearCoefficient", QString::number(zNearCoefficient()));
    paramNode.setAttribute("zClippingCoefficient", QString::number(zClippingCoefficient()));
    paramNode.setAttribute("orthoCoef", QString::number(orthographicCoefficient()));
    paramNode.setAttribute("sceneRadius", QString::number(sceneRadius()));
    paramNode.appendChild(AMKglDomUtils::VecDomElement(sceneCenter(), "SceneCenter", document));

    switch (projectionType()) {
    case PT_PERSPECTIVE: paramNode.setAttribute("Type", "PERSPECTIVE"); break;
    case PT_ORTHOGRAPHIC: paramNode.setAttribute("Type", "ORTHOGRAPHIC"); break;
    }
    de.appendChild(paramNode);

    QDomElement e = document.createElement("ManipulatedCameraFrame");
    e.appendChild(AMKglDomUtils::VecDomElement(position(), "position", document));
    e.appendChild(AMKglDomUtils::QuaternionDomElement(orientation(), "orientation", document));

    QDomElement mp = document.createElement("ManipulatedParameters");
    mp.setAttribute("rotSens", QString::number(m_rotationSensitivity));
    mp.setAttribute("transSens", QString::number(m_translationSensitivity));
    mp.setAttribute("wheelSens", QString::number(m_wheelSensitivity));
    mp.setAttribute("zoomSens", QString::number(m_zoomSensitivity));
    e.appendChild(mp);

    QDomElement mcp = document.createElement("ManipulatedCameraParameters");
    AMKglDomUtils::setBoolAttribute(mcp, "rotatesAroundUpVector", m_rotatesAroundUpVector);
    AMKglDomUtils::setBoolAttribute(mcp, "zoomsOnPivotPoint", m_zoomsOnPivotPoint);
    mcp.appendChild(AMKglDomUtils::VecDomElement(m_sceneUpVector, "sceneUpVector", document));
    e.appendChild(mcp);

    de.appendChild(e);

    return de;
}

void AMKglCamera::initFromDOMElement(const QDomElement& element)
{
    QDomElement cameraChild = element.firstChild().toElement();

    while(!cameraChild.isNull())
    {
        if(cameraChild.tagName() == "Parameters")
        {
            setFieldOfView(AMKglDomUtils::doubleFromDom(cameraChild, "fieldOfView", M_PI / 4.0));
            setZNearCoefficient(AMKglDomUtils::doubleFromDom(cameraChild, "zNearCoefficient", 0.005));
            setZClippingCoefficient(AMKglDomUtils::doubleFromDom(cameraChild, "zClippingCoefficient", sqrt(3.0)));
            m_orthoCoef = AMKglDomUtils::doubleFromDom(cameraChild, "orthoCoef", std::numeric_limits<double>::quiet_NaN());

            if(std::isnan(m_orthoCoef))
                m_orthoCoef = tan(fieldOfView() / 2.0);

            const double orthoCoefBackup = m_orthoCoef;

            setSceneRadius(AMKglDomUtils::doubleFromDom(cameraChild, "sceneRadius", sceneRadius()));

            setProjectionType(PT_PERSPECTIVE);
            const QString type = cameraChild.attribute("Type", "PERSPECTIVE");
            if(type == "PERSPECTIVE")
                setProjectionType(PT_PERSPECTIVE);
            else if(type == "ORTHOGRAPHIC")
                setProjectionType(PT_ORTHOGRAPHIC);

            QDomElement child2 = cameraChild.firstChild().toElement();
            while(!child2.isNull())
            {
                if (child2.tagName() == "SceneCenter")
                {
                    setSceneCenter(AMKglDomUtils::VecFromDom(child2));
                    break;
                }

                child2 = child2.nextSibling().toElement();
            }

            m_orthoCoef = orthoCoefBackup;
        }

        if (cameraChild.tagName() == "ManipulatedCameraFrame")
        {
            QDomElement frameChild = cameraChild.firstChild().toElement();
            while (!frameChild.isNull()) {
                if (frameChild.tagName() == "position")
                    setPosition(AMKglDomUtils::VecFromDom(frameChild));
                if (frameChild.tagName() == "orientation")
                    setOrientation(AMKglDomUtils::QuaternionFromDom(frameChild).normalized());

                frameChild = frameChild.nextSibling().toElement();
            }

            QDomElement manipulatedFrameChild = cameraChild.firstChild().toElement();
            while (!manipulatedFrameChild.isNull()) {
                if (manipulatedFrameChild.tagName() == "ManipulatedParameters") {
                    setRotationSensitivity(AMKglDomUtils::doubleFromDom(manipulatedFrameChild, "rotSens", 1.0));
                    setTranslationSensitivity(AMKglDomUtils::doubleFromDom(manipulatedFrameChild, "transSens", 1.0));
                    setWheelSensitivity(AMKglDomUtils::doubleFromDom(manipulatedFrameChild, "wheelSens", 1.0));
                    setZoomSensitivity(AMKglDomUtils::doubleFromDom(manipulatedFrameChild, "zoomSens", 1.0));
                }
                manipulatedFrameChild = manipulatedFrameChild.nextSibling().toElement();
            }

            QDomElement manipulatedCameraFrameChild = cameraChild.firstChild().toElement();
            while (!manipulatedCameraFrameChild.isNull()) {
                if (manipulatedCameraFrameChild.tagName() == "ManipulatedCameraParameters") {
                    m_rotatesAroundUpVector = AMKglDomUtils::boolFromDom(manipulatedCameraFrameChild, "rotatesAroundUpVector", false);
                    m_zoomsOnPivotPoint = AMKglDomUtils::boolFromDom(manipulatedCameraFrameChild, "zoomsOnPivotPoint", false);

                    QDomElement schild = manipulatedCameraFrameChild.firstChild().toElement();
                    while (!schild.isNull()) {
                        if (schild.tagName() == "sceneUpVector")
                            m_sceneUpVector = AMKglDomUtils::VecFromDom(schild);

                        schild = schild.nextSibling().toElement();
                    }
                }
                manipulatedCameraFrameChild = manipulatedCameraFrameChild.nextSibling().toElement();
            }
        }

        cameraChild = cameraChild.nextSibling().toElement();
    }
}

void AMKglCamera::setUseNormalMode(bool normal)
{
    if(m_useNormalCamera == normal)
        return;

    m_useNormalCamera = normal;
    modified();
}

Eigen::Vector3d AMKglCamera::localInverseTransformOf(const Eigen::Vector3d& src) const
{
    return Math::Rotate(m_orientation, src);
}

Eigen::Vector3d AMKglCamera::localTransformOf(const Eigen::Vector3d& src) const
{
    return Math::Rotate(m_orientation.conjugate(), src);
}

Eigen::Vector3d AMKglCamera::localCoordinatesOf(const Eigen::Vector3d& src) const
{
    return Math::Rotate(m_orientation.conjugate(), src - translation());
}

Eigen::Vector3d AMKglCamera::localInverseCoordinatesOf(const Eigen::Vector3d& src) const
{
    return Math::Rotate(m_orientation, src) + translation();
}

void AMKglCamera::modified()
{
    m_projectionMatrixIsUpToDate = false;
    m_modelViewMatrixIsUpToDate = false;
}

void AMKglCamera::updateSceneUpVector()
{
    m_sceneUpVector = localInverseTransformOf(Eigen::Vector3d(0.0, 1.0, 0.0));
}

void AMKglCamera::rotate(const Eigen::Quaterniond& q)
{
    Eigen::Quaterniond qNotConst = q;
    rotate(qNotConst);
}

void AMKglCamera::rotate(Eigen::Quaterniond& q)
{
    if (m_rotationConstraintEnabled)
      q = Eigen::Quaterniond::Identity();

    m_orientation *= q;
    m_orientation.normalize();
    modified();
}

void AMKglCamera::rotateAroundPoint(const Eigen::Quaterniond& rotation, const Eigen::Vector3d& point)
{
    Eigen::Quaterniond rotationNotConst = rotation;
    rotateAroundPoint(rotationNotConst, point);
}

void AMKglCamera::rotateAroundPoint(Eigen::Quaterniond& rotation, const Eigen::Vector3d& point)
{
    if (m_rotationConstraintEnabled)
      rotation = Eigen::Quaterniond::Identity();

    m_orientation *= rotation;
    m_orientation.normalize();

    Eigen::AngleAxisd aa;
    aa = rotation;

    const Eigen::Vector3d trans = point + Math::Rotate(Eigen::Quaterniond(Eigen::AngleAxisd(aa.angle(), localInverseTransformOf(aa.axis()).normalized())), (position() - point)) - position();

    m_position += trans;
    modified();
}

void AMKglCamera::translate(const Eigen::Vector3d& t)
{
    Eigen::Vector3d tNotConst = t;
    translate(tNotConst);
}

void AMKglCamera::translate(Eigen::Vector3d& t)
{
    m_position += t;
    modified();
}

void AMKglCamera::zoom(double delta)
{
    const double sceneRadius = this->sceneRadius();
    if (m_zoomsOnPivotPoint)
    {
        const Eigen::Vector3d direction = position() - pivotPoint();
        if (direction.norm() > 0.02 * sceneRadius || delta > 0.0)
            translate(delta * direction);
    }
    else
    {
        const double coef = qMax(fabs((localCoordinatesOf(pivotPoint())).z()), double(0.2) * sceneRadius);
        const Eigen::Vector3d trans(0.0, 0.0, -coef * delta);
        translate(localInverseTransformOf(trans));
    }
}

void AMKglCamera::setOrientationWithConstraint(Eigen::Quaterniond& q)
{
    Eigen::Quaterniond deltaQ = m_rotationConstraintEnabled ? Eigen::Quaterniond::Identity() : m_orientation.inverse() * q;

    deltaQ.normalize();

    setOrientation(m_orientation * deltaQ);
    m_orientation.normalize();
    q = m_orientation;
}

void AMKglCamera::setPositionWithConstraint(Eigen::Vector3d& pos)
{
    Eigen::Vector3d deltaT = pos - translation();

    setTranslation(translation() + deltaT);
    pos = translation();
}

void AMKglCamera::projectOnLine(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction)
{
    const Eigen::Vector3d shift = origin - position();
    Eigen::Vector3d proj = shift;
    proj = ((proj.dot(direction)) / direction.squaredNorm()) * direction;
    translate(shift - proj);
}

void AMKglCamera::mousePressEvent(QMouseEvent* e)
{
    if(m_currentAction != A_NoAction)
        return;

    m_manipulated = false;

    if(e->buttons().testFlag(Qt::LeftButton))
    {
        m_currentAction = A_Rotate;
        m_constrainedRotationIsReversed = localTransformOf(m_sceneUpVector).y() < 0.0;
    }
    else if(e->buttons().testFlag(Qt::RightButton))
    {
        m_currentAction = A_Translate;
    }
    else if(e->buttons().testFlag(Qt::MiddleButton))
    {
        m_currentAction = A_Zoom;
    }

    m_mousePreviousPos = m_mousePressPos = e->pos();
}

void AMKglCamera::mouseMoveEvent(QMouseEvent* e)
{
    if(m_currentAction == A_Translate)
    {
        const QPoint delta = m_mousePreviousPos - e->pos();
        Eigen::Vector3d trans(delta.x(), -delta.y(), 0.0);
        // Scale to fit the screen mouse displacement
        switch (projectionType())
        {
        case PT_PERSPECTIVE:
            trans *= 2.0 * tan(fieldOfView() / 2.0) *
                    fabs((localCoordinatesOf(pivotPoint())).z()) /
                    screenHeight();
            break;
        case PT_ORTHOGRAPHIC: {
            GLdouble w, h;
            getOrthoWidthHeight(w, h);
            trans[0] *= 2.0 * w / screenWidth();
            trans[1] *= 2.0 * h / screenHeight();
            break;
        }
        }
        translate(localInverseTransformOf(m_translationSensitivity * trans));
        m_manipulated = true;
        emit manipulated();
        m_mousePreviousPos = e->pos();
        return;
    }

    if(m_currentAction == A_Rotate)
    {
        Eigen::Quaterniond rot;
        if (m_rotatesAroundUpVector)
        {
            // Multiply by 2.0 to have the same speed as with the deformed ball
            double dx = 2.0 * m_rotationSensitivity * (m_mousePreviousPos.x() - e->x()) / screenWidth();
            const double dy = 2.0 * m_rotationSensitivity * (m_mousePreviousPos.y() - e->y()) / screenHeight();
            if (m_constrainedRotationIsReversed)
                dx = -dx;
            const Eigen::Vector3d verticalAxis = localTransformOf(m_sceneUpVector);
            rot = Eigen::Quaterniond(Eigen::AngleAxisd(dx, verticalAxis.normalized())) * Eigen::Quaterniond(Eigen::AngleAxisd(dy, Eigen::Vector3d(1.0, 0.0, 0.0).normalized()));
        }
        else
        {
            const Eigen::Vector3d trans = projectedCoordinatesOf(pivotPoint());
            rot = deformedBallQuaternion(e->x(), e->y(), trans[0], trans[1]);
        }

        rotateAroundPoint(rot, m_pivotPoint);
        m_manipulated = true;
        emit manipulated();
        m_mousePreviousPos = e->pos();
        return;
    }

    if(m_currentAction == A_Zoom)
    {
        zoom(deltaWithPrevPos(e));
        m_manipulated = true;
        emit manipulated();
        m_mousePreviousPos = e->pos();
        return;
    }
}

void AMKglCamera::mouseReleaseEvent(QMouseEvent* /*e*/)
{
    m_currentAction = A_NoAction;
    m_manipulated = false;
}

void AMKglCamera::mouseDoubleClickEvent(QMouseEvent* e)
{
    Q_UNUSED(e);
    m_manipulated = true;
    align();
    emit manipulated();
    m_manipulated = false;
}

void AMKglCamera::wheelEvent(QWheelEvent* e)
{
    m_manipulated = true;
    zoom(wheelDelta(e));
    emit manipulated();
    m_manipulated = false;

    m_currentAction = A_NoAction;
}

void AMKglCamera::keyPressEvent(QKeyEvent* e)
{
    Q_UNUSED(e);
}

void AMKglCamera::keyReleaseEvent(QKeyEvent* e)
{
    Q_UNUSED(e);
}

double AMKglCamera::deltaWithPrevPos(const QMouseEvent* event) const
{
    const double dx = double(event->x() - m_mousePreviousPos.x()) / screenWidth();
    const double dy = double(event->y() - m_mousePreviousPos.y()) / screenHeight();

    double value = fabs(dx) > fabs(dy) ? dx : dy;
    return value * m_zoomSensitivity;
}

double AMKglCamera::wheelDelta(const QWheelEvent* event) const
{
    static const double WHEEL_SENSITIVITY_COEF = 8E-4;
    return event->delta() * m_wheelSensitivity * WHEEL_SENSITIVITY_COEF;
}

double AMKglCamera::ProjectOnBall(double x, double y)
{
    // If you change the size value, change angle computation in deformedBallQuaternion().
    const double size = 1.0;
    const double size2 = size * size;
    const double size_limit = size2 * 0.5;

    const double d = x * x + y * y;
    return d < size_limit ? sqrt(size2 - d) : size_limit / sqrt(d);
}

Eigen::Quaterniond AMKglCamera::deformedBallQuaternion(int x, int y, double cx, double cy) const {
    // Points on the deformed ball
    const double px = m_rotationSensitivity * (m_mousePreviousPos.x() - cx) / screenWidth();
    const double py = m_rotationSensitivity * (cy - m_mousePreviousPos.y()) / screenHeight();
    const double dx = m_rotationSensitivity * (x - cx) / screenWidth();
    const double dy = m_rotationSensitivity * (cy - y) / screenHeight();

    const Eigen::Vector3d p1(px, py, ProjectOnBall(px, py));
    const Eigen::Vector3d p2(dx, dy, ProjectOnBall(dx, dy));
    // Approximation of rotation angle
    // Should be divided by the projectOnBall size, but it is 1.0
    const Eigen::Vector3d axis = p2.cross(p1);
    const double angle = 5.0 * asin(sqrt(axis.squaredNorm() / p1.squaredNorm() / p2.squaredNorm()));
    return Eigen::Quaterniond(Eigen::AngleAxisd(angle, axis.normalized()));
}
