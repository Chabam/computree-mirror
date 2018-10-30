#ifndef G3DCAMERACONTROLLER_H
#define G3DCAMERACONTROLLER_H

#include "dm_graphicsview.h"

#include "view/DocumentView/GraphicsViews/3D/ggraphicsviewimp.h"

class G3DCameraController : public DM_GraphicsViewCamera
{
    Q_OBJECT

public:
    G3DCameraController();

    void setRealCameraManipulator(const GGraphicsViewImp::CameraType *manipulator);
    GGraphicsViewImp::CameraType* getRealCameraManipulator() const;
    void setView(const GGraphicsViewImp *view);

    double x() const;
    double y() const;
    double z() const;
    double cx() const;
    double cy() const;
    double cz() const;
    double rx() const;
    double ry() const;
    double rz() const;
    double rw() const;

    CameraInterface::CameraType type() const;

    float focusDistance() const;

    const GLdouble* orientationMatrix() const;
    const GLdouble* orientationMatrixInversed() const;

    void modelViewMatrix(GLdouble m[16]) const;

    void getOrientation(double &q0, double &q1, double &q2, double &q3) const;

    Eigen::Vector3d upVector() const;
    Eigen::Vector3d rightVector() const;

    void setLastItemSelectedCameraCenter(double x, double y, double z);

private:

    struct CameraInfo {
        Eigen::Matrix4d modelViewProjectionMatrix;
    };

    GGraphicsViewImp::CameraType*           m_camManipulator;
    GGraphicsViewImp*                       m_view;

    Eigen::Matrix4d                         m_tmpMatrix;
    Eigen::Vector3d                         m_lastItemSelectedCenter;

    CameraInfo                              m_cameraInfoBackup;

    void emitCoordinatesChanged();
    void redrawTheView();

public slots:

    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setCX(double cx);
    void setCY(double cy);
    void setCZ(double cz);
    void setRX(double xRot);
    void setRY(double yRot);
    void setRZ(double zRot);
    void setRW(double wRot);

    void setType(CameraInterface::CameraType type);
    void setPointOfView(double cx, double cy, double cz, double px, double py, double pz, double rx, double ry, double rz, double rw, bool redrawView = true);
    void setPosition(double x, double y, double z, bool redrawView = true);
    void setSceneCenter(double cx, double cy, double cz, bool redrawView = true);
    void setViewDirection(double rx, double ry, double rz,  bool redrawView = true);
    void setOrientation(double q0, double q1, double q2, double q3, bool redrawView = true);

    //void centerScene();
    void showEntireScene();
    void syncWithCamera(const DM_GraphicsViewCamera *cam);
    void alignCameraToInvXAxis();
    void alignCameraToInvYAxis();
    void alignCameraToInvZAxis();
    void alignCameraToXAxis();
    void alignCameraToYAxis();
    void alignCameraToZAxis();
    void initCameraCenter();
    void fixCameraCenterToItemsBarycenter();
    void fixCameraCenterToSelectedItemsBarycenter();
    void fitCameraToVisibleItems();
    void fitToSpecifiedBox(const Eigen::Vector3d &bot, const Eigen::Vector3d &top);
    void setCenterToSpecifiedBox(const Eigen::Vector3d &min, const Eigen::Vector3d &max, bool redrawView = true);

    void homePosition();

    void preventBugInQGLViewerCamera();

private slots:
    void viewDrawBegin();
    void viewDrawFinished();
};

#endif // G3DCAMERACONTROLLER_H
