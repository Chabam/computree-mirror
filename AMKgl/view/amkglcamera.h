#ifndef AMKGLCAMERA_H
#define AMKGLCAMERA_H

#include <QtOpenGL>
#include <QPoint>
#include <QDomDocument>

#include <Eigen/Dense>

/**
 * @brief The AMKglCamera class represent a 3D camera orthographic or perspective for
 *        an opengl view.
 */
class AMKglCamera : public QObject
{
    Q_OBJECT

public:
    enum ProjectionType
    {
        PT_PERSPECTIVE,
        PT_ORTHOGRAPHIC
    };

    AMKglCamera();
    AMKglCamera(const AMKglCamera& other);
    AMKglCamera& operator=(const AMKglCamera& other);

    void enableRotationConstraint(bool e);
    bool isRotationConstraintEnabled() const;

    Eigen::Vector3d position() const;
    Eigen::Vector3d translation() const;
    Eigen::Vector3d upVector() const;
    Eigen::Vector3d viewDirection() const;
    Eigen::Vector3d rightVector() const;
    Eigen::Quaterniond orientation() const;

    void setPosition(const Eigen::Vector3d& pos);
    void setTranslation(const Eigen::Vector3d& pos);
    void setOrientation(const Eigen::Quaterniond& q);
    void setOrientation(double theta, double phi);
    void setUpVector(const Eigen::Vector3d& up, bool noMove = true);
    void setViewDirection(const Eigen::Vector3d& direction);

    void lookAt(const Eigen::Vector3d& target);
    void showEntireScene();
    void fitSphere(const Eigen::Vector3d& center, double radius);
    void fitBoundingBox(const Eigen::Vector3d& min, const Eigen::Vector3d& max);
    void centerScene();

    double fieldOfView() const;
    double horizontalFieldOfView() const;
    double aspectRatio() const;
    int screenWidth() const;
    int screenHeight() const;
    void getViewport(GLint viewport[4]) const;
    double pixelGLRatio(const Eigen::Vector3d& position) const;
    double zNearCoefficient() const;
    double zClippingCoefficient() const;
    double orthographicCoefficient() const;
    double zNear() const;
    double zFar() const;
    void getOrthoWidthHeight(GLdouble &halfWidth,
                             GLdouble &halfHeight) const;
    void getFrustumPlanesCoefficients(GLdouble coef[6][4]) const;

    ProjectionType projectionType() const;
    void setProjectionType(ProjectionType type);
    void setFieldOfView(double fov);
    void setHorizontalFieldOfView(double hfov);
    void setFOVToFitScene();
    void setAspectRatio(double aspect);
    void setScreenWidthAndHeight(int width, int height);
    void setZNearCoefficient(double coef);
    void setZClippingCoefficient(double coef);

    void setSceneRadius(double radius);
    double sceneRadius() const;

    void setSceneCenter(const Eigen::Vector3d& center);
    Eigen::Vector3d sceneCenter() const;
    double distanceToSceneCenter() const;

    void setSceneBoundingBox(const Eigen::Vector3d& min, const Eigen::Vector3d& max);

    void setPivotPoint(const Eigen::Vector3d& point);
    Eigen::Vector3d pivotPoint() const;

    void loadProjectionMatrix(bool reset = true) const;
    void loadModelViewMatrix(bool reset = true) const;
    void computeProjectionMatrix() const;
    void computeModelViewMatrix() const;

    void getProjectionMatrix(GLdouble m[16]) const;
    void getProjectionMatrix(Eigen::Matrix4d& m) const;
    void getModelViewMatrix(GLdouble m[16]) const;
    void getModelViewMatrix(Eigen::Matrix4d& m) const;
    void getModelViewProjectionMatrix(GLdouble m[16]) const;
    void getModelViewProjectionMatrix(Eigen::Matrix4d& m) const;

    Eigen::Vector3d cameraCoordinatesOf(const Eigen::Vector3d& src) const;
    Eigen::Vector3d worldCoordinatesOf(const Eigen::Vector3d& src) const;
    void getCameraCoordinatesOf(const double src[3], double res[3]) const;
    void getWorldCoordinatesOf(const double src[3], double res[3]) const;

    Eigen::Vector3d projectedCoordinatesOf(const Eigen::Vector3d& src) const;
    Eigen::Vector3d unprojectedCoordinatesOf(const Eigen::Vector3d& src) const;
    void getProjectedCoordinatesOf(const double src[3], double res[3]) const;
    void getUnprojectedCoordinatesOf(const double src[3], double res[3]) const;

    void convertClickToLine(const QPoint &pixel, Eigen::Vector3d& orig, Eigen::Vector3d& dir) const;
    Eigen::Vector3d pointUnderPixel(const QPoint &pixel, bool &found) const;

    double focusDistance() const;
    void setFocusDistance(double distance);

    bool isManipulated() const;

    void align();

    /**
     * @brief Defines the rotation sensitivity
     */
    void setRotationSensitivity(double sensitivity);

    /**
     * @brief Defines the translation sensitivity
     */
    void setTranslationSensitivity(double sensitivity);

    /**
     * @brief Defines the wheel sensitivity
     */
    void setWheelSensitivity(double sensitivity);

    /**
     * @brief Defines the zoom sensitivity
     */
    void setZoomSensitivity(double sensitivity);

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

    void setUseNormalMode(bool normal);

protected:
    friend class AMKglViewer;

    enum Action {
        A_NoAction,
        A_Rotate,
        A_Translate,
        A_Zoom
    };

    bool                m_useNormalCamera;
    int                 m_screenWidth;
    int                 m_screenHeight;
    double              m_fieldOfView;              // in radians
    Eigen::Vector3d     m_sceneCenter;
    double              m_sceneRadius; // OpenGL units
    double              m_focusDistance;
    double              m_zNearCoef;
    double              m_zClippingCoef;
    double              m_orthoCoef;
    ProjectionType      m_projectionType;
    Eigen::Vector3d     m_pivotPoint;
    bool                m_rotationConstraintEnabled;
    mutable GLdouble    m_modelViewMatrix[16]; // Buffered model view matrix.
    mutable bool        m_modelViewMatrixIsUpToDate;
    mutable GLdouble    m_projectionMatrix[16]; // Buffered projection matrix.
    mutable bool        m_projectionMatrixIsUpToDate;

    Eigen::Vector3d     m_position;
    Eigen::Quaterniond  m_orientation;

    double              m_rotationSensitivity;
    double              m_translationSensitivity;
    double              m_wheelSensitivity;
    double              m_zoomSensitivity;

    Eigen::Vector3d     m_sceneUpVector;

    QPoint              m_mousePreviousPos;
    QPoint              m_mousePressPos;
    bool                m_rotatesAroundUpVector;
    // Inverse the direction of an horizontal mouse motion. Depends on the
    // projected screen orientation of the vertical axis when the mouse button is
    // pressed.
    bool                m_constrainedRotationIsReversed;
    bool                m_zoomsOnPivotPoint;
    Action              m_currentAction;

    bool                m_manipulated;

    Eigen::Vector3d localInverseTransformOf(const Eigen::Vector3d& src) const;
    Eigen::Vector3d localTransformOf(const Eigen::Vector3d& src) const;
    Eigen::Vector3d localCoordinatesOf(const Eigen::Vector3d& src) const;
    Eigen::Vector3d localInverseCoordinatesOf(const Eigen::Vector3d& src) const;

    void modified();
    void updateSceneUpVector();
    void rotate(const Eigen::Quaterniond& q);
    void rotate(Eigen::Quaterniond& q);
    void rotateAroundPoint(const Eigen::Quaterniond& rotation, const Eigen::Vector3d& point);
    void rotateAroundPoint(Eigen::Quaterniond& rotation, const Eigen::Vector3d& point);
    void translate(const Eigen::Vector3d& t);
    void translate(Eigen::Vector3d& t);
    void zoom(double delta);
    void setOrientationWithConstraint(Eigen::Quaterniond& q);
    void setPositionWithConstraint(Eigen::Vector3d& pos);
    void projectOnLine(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

    double deltaWithPrevPos(const QMouseEvent* event) const;
    double wheelDelta(const QWheelEvent* event) const;

    static double ProjectOnBall(double x, double y);
    Eigen::Quaterniond deformedBallQuaternion(int x, int y, qreal cx, qreal cy) const;

signals:
    void manipulated();
};

#endif // AMKGLCAMERA_H
