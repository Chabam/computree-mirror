/**
 * @author Michael Krebs (AMVALOR)
 * @date 25.01.2017
 * @version 1
 */
#ifndef CT_SHOT_H
#define CT_SHOT_H

#include "ctlibstructureaddon_global.h"

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

/**
 * @brief Represent a shot
 */
struct CTLIBSTRUCTUREADDON_EXPORT CT_Shot {
public:
    /**
     * @brief Construct a default shot
     * @param valid : if true set the direction to (1,0,0) otherwise to (0,0,0)
     */
    CT_Shot(bool valid = true);

    /**
     * @brief Construct a shot with an origin and a direction
     */
    CT_Shot(const Eigen::Vector3d& origin, const Eigen::Vector3d& direction);

    CT_Shot(const CT_Shot& other) = default;

    /**
     * @brief Set the origin of the shot
     */
    void setOrigin(const Eigen::Vector3d& origin);

    /**
     * @brief Returns the origin of the shot
     */
    const Eigen::Vector3d& origin() const;

    /**
     * @brief Set the direction of the shot
     */
    void setDirection(const Eigen::Vector3d& direction);

    /**
     * @brief Returns the direction of the shot
     */
    const Eigen::Vector3d& direction() const;

private:
    Eigen::Vector3d     m_origin;
    Eigen::Vector3d     m_direction;
};

#endif // CT_SHOT_H
