#ifndef QGLVIEWERTOOLS_H
#define QGLVIEWERTOOLS_H

#include <Eigen/Core>
#include <QGL>

class QGLViewerTools {
public:
    static inline Eigen::Vector3d vecToEigen(const QVector3D& v) {
        return Eigen::Vector3d(v.x(), v.y(), v.z());
    }

    static inline QVector3D eigenToVec(const Eigen::Vector3d& v) {
        return QVector3D(v[0], v[1], v[2]);
    }

    static inline Eigen::Matrix4d doubleMatrixToEigen(GLdouble modelViewMatrix[16]) {
        Eigen::Matrix4d matrix;
        matrix << modelViewMatrix[0], modelViewMatrix[4], modelViewMatrix[8], modelViewMatrix[12],
                  modelViewMatrix[1], modelViewMatrix[5], modelViewMatrix[9], modelViewMatrix[13],
                  modelViewMatrix[2], modelViewMatrix[6], modelViewMatrix[10], modelViewMatrix[14],
                  modelViewMatrix[3], modelViewMatrix[7], modelViewMatrix[11], modelViewMatrix[15];

        return matrix;
    }
};

#endif // QGLVIEWERTOOLS_H
