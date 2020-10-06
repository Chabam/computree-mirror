#ifndef CT_MATH2DLINES_H
#define CT_MATH2DLINES_H

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

#include "ctlibmath_global.h"

class CTLIBMATH_EXPORT CT_Math2DLines
{
public:

    inline static double distance2D(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2)
    {
        return sqrt( (p1(0) - p2(0))*(p1(0) - p2(0)) + (p1(1) - p2(1))*(p1(1) - p2(1)));
    }

    static bool intersectSegments(Eigen::Vector2d &a, Eigen::Vector2d &b, Eigen::Vector2d &c, Eigen::Vector2d &d, double &r, double &s, Eigen::Vector2d &intersection);

    static void computePerpendicularSegment(Eigen::Vector2d &a, Eigen::Vector2d &b, Eigen::Vector2d &c, Eigen::Vector2d &d, double length = -1, bool clockwise = false);

};

#endif // CT_MATH2DLINES_H
