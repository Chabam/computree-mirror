#ifndef QGLVIEWERTOOLS_H
#define QGLVIEWERTOOLS_H

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
#include "qglviewer.h"
#include "Eigen/Core"
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

class QGLViewerTools {
public:
    static inline Eigen::Vector3d vecToEigen(const qglviewer::Vec& v) {
        return Eigen::Vector3d(v.x, v.y, v.z);
    }

    static inline qglviewer::Vec eigenToVec(const Eigen::Vector3d& v) {
        return qglviewer::Vec(v[0], v[1], v[2]);
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
