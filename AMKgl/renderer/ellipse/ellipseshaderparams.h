#ifndef ELLIPSESHADERPARAMS_H
#define ELLIPSESHADERPARAMS_H

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

struct EllipseShaderParams {
    Eigen::Vector3f majorAxisDirection;
    Eigen::Vector3f minorAxisDirection;
    Eigen::Vector3f center;

    static inline GLenum staticGetGLMode() { return GL_LINE_LOOP; }

    static inline EllipseShaderParams staticConstructFromValues(const double& cx, const double& cy, const double &cz,
                                                                const float& majorAxisRadius, const float& minorAxisRadius,
                                                                const Eigen::Vector3f& majorAxisDirection,
                                                                const Eigen::Vector3f& normal,
                                                                const Eigen::Vector3d& offset) {
        Eigen::Vector3f major = majorAxisDirection.normalized();
        Eigen::Vector3f normalN = normal.normalized();
        Eigen::Vector3f minorAxisDirection = major.cross(normalN);

        EllipseShaderParams params;
        params.majorAxisDirection = (major * majorAxisRadius);
        params.minorAxisDirection = (minorAxisDirection * minorAxisRadius);
        params.center = Eigen::Vector3f(cx - offset[0], cy - offset[1], cz - offset[2]);

        return params;
    }

    inline void getMatrix4f(Eigen::Matrix4f &matrix4f) const {
        // Tangent  = +X axis = majorAxisDirection
        // Normal   = +Y axis = normal
        // Binormal = +Z axis = minorAxisDirection

        Eigen::Vector3f normal = majorAxisDirection.cross(minorAxisDirection);

        matrix4f = Eigen::Matrix4f::Identity();
        matrix4f.block(0,3,3,1) = center;
        matrix4f(0,0) = majorAxisDirection[0];    // Xx
        matrix4f(1,0) = majorAxisDirection[1];    // Xy
        matrix4f(2,0) = majorAxisDirection[2];    // Xz
        matrix4f(0,1) = normal[0]; // Yx
        matrix4f(1,1) = normal[1]; // Yy
        matrix4f(2,1) = normal[2]; // Yz
        matrix4f(0,2) = minorAxisDirection[0];    // Zx
        matrix4f(1,2) = minorAxisDirection[1];    // Zy
        matrix4f(2,2) = minorAxisDirection[2];    // Zz
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
            return majorAxisDirection(0);
        else if(index == 1)
            return minorAxisDirection(0);

        return center(0);
    }

    inline const float& operator()(const int &index) const {
        if(index == 0)
            return majorAxisDirection(0);
        else if(index == 1)
            return minorAxisDirection(0);

        return center(0);
    }
};


#endif // ELLIPSESHADERPARAMS_H
