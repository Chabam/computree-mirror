#ifndef SPHERESHADERPARAMS
#define SPHERESHADERPARAMS

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#else
#pragma GCC diagnostic ignored "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "Eigen/Dense"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic warning "-Wdeprecated-copy"
#else
#pragma GCC diagnostic warning "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#endif

#include <QtOpenGL/QtOpenGL>

struct SphereShaderParams {
    Eigen::Vector3f center;
    Eigen::Vector2f startEndThetaInRadians;
    Eigen::Vector2f startEndPhiInRadians;
    float           radius;

    static inline GLenum staticGetGLMode() { return GL_LINE_STRIP; }

    static inline SphereShaderParams staticConstructFromValues(const double& cx, const double& cy, const double &cz,
                                                               const double& radius,
                                                               const double &initTheta, const double &endTheta,
                                                               const double &initPhi, const double &endPhi,
                                                               bool phiThetaInRadians,
                                                               const Eigen::Vector3d& offset) {

        SphereShaderParams params;
        params.center = Eigen::Vector3f(cx - offset[0], cy - offset[1], cz - offset[2]);
        params.startEndThetaInRadians.x() = phiThetaInRadians ? initTheta : qDegreesToRadians(initTheta);
        params.startEndThetaInRadians.y() = phiThetaInRadians ? endTheta : qDegreesToRadians(endTheta);
        params.startEndPhiInRadians.x() = phiThetaInRadians ? initPhi : qDegreesToRadians(initPhi);
        params.startEndPhiInRadians.y() = phiThetaInRadians ? endPhi : qDegreesToRadians(endPhi);
        params.radius = radius;

        while(params.startEndPhiInRadians.x() < 0
                || params.startEndPhiInRadians.y() < 0) {
            params.startEndPhiInRadians.x() += (float)M_PI;
            params.startEndPhiInRadians.y() += (float)M_PI;
        }

        while(params.startEndThetaInRadians.x() < 0
                || params.startEndThetaInRadians.y() < 0) {
            params.startEndThetaInRadians.x() += (float)M_PI;
            params.startEndThetaInRadians.y() += (float)M_PI;
        }

        return params;
    }

    inline void getMatrix4f(Eigen::Matrix4f &matrix4f) const {
        matrix4f = Eigen::Matrix4f::Identity();
        matrix4f.block(0,3,3,1) = center;
        matrix4f(0,0) = radius;     // Xx
        matrix4f(1,1) = radius;     // Yy
        matrix4f(2,2) = radius;     // Zz
    }

    inline void transform(const Eigen::Matrix4f& trMatrix) {
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(trMatrix)
    // TODO
#endif
    }

    inline void translate(const Eigen::Vector3f& translation) {
        center[0] += translation[0];
        center[1] += translation[1];
        center[2] += translation[2];
    }

    inline void rotate(const Eigen::AngleAxis<float>& rotation) {
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(rotation)
    // TODO
#endif
    }

    inline void rotate(const Eigen::Quaternion<float>& rotation) {
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(rotation)
    // TODO
#endif
    }

    inline void scale(const Eigen::Vector3f& scaling) {
#ifdef AMKGL_NO_TODO_WARNINGS
    Q_UNUSED(scaling)
    // TODO
#endif
    }

    inline float& operator()(const int &index) {
        if(index == 0)
            return center(0);
        else if(index == 1)
            return startEndThetaInRadians(0);

        return startEndPhiInRadians(1);
    }

    inline const float& operator()(const int &index) const {
        if(index == 0)
            return center(0);
        else if(index == 1)
            return startEndThetaInRadians(0);

        return startEndPhiInRadians(1);
    }
};

#endif // SPHERESHADERPARAMS

