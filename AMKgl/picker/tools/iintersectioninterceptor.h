#ifndef IINTERSECTIONINTERCEPTOR_H
#define IINTERSECTIONINTERCEPTOR_H

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

/**
 * @brief Inherit this class if you want to intercept all intersection point discovered
 */
class IIntersectionInterceptor {
public:
    virtual ~IIntersectionInterceptor() {}

    /**
     * @brief Called when a intersection point is detected
     */
    virtual void addIntersectionPoint(const Eigen::Vector3d& p, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) = 0;

    /**
     * @brief Called when a intersection line is detected
     */
    virtual void addIntersectionLine(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) = 0;

    /**
     * @brief Called when a intersection segment is detected
     */
    virtual void addIntersectionSegment(const Eigen::Vector3d& p1, const Eigen::Vector3d& p2, const Eigen::Vector3f& color = Eigen::Vector3f(1,1,1)) = 0;
};

#endif // IINTERSECTIONINTERCEPTOR_H
