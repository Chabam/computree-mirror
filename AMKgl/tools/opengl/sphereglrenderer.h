#ifndef SPHEREGLRENDERER_H
#define SPHEREGLRENDERER_H

#include <QtOpenGL>
#include <vector>

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
 * @brief Sphere renderer
 */
class SphereGlRenderer
{
public:
    SphereGlRenderer();
    SphereGlRenderer(int nPoints);

    /**
     * @brief Init this renderer to draw a sphere with a finite number of points
     */
    void init(int nPoints);

    /**
     * @brief Render a sphere
     */
    void render();

    /**
     * @brief Returns the array that contains values
     */
    const std::vector<Eigen::Vector3f>& getValues() const;

    /**
     * @brief Returns the array that contains theta phi values of each vertex
     */
    const std::vector<Eigen::Vector2f>& getThetaPhiValues() const;

    /**
     * @brief Returns the array that contains indices
     */
    const std::vector<GLuint>& getIndices() const;


private:
    std::vector<Eigen::Vector3f>    m_values;
    std::vector<Eigen::Vector2f>    m_thetaPhiValues;
    std::vector<GLuint>             m_indices;

    void initWithGlLineStrip(int nPoints);
};

#endif // SPHEREGLRENDERER_H
