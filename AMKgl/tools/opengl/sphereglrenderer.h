#ifndef SPHEREGLRENDERER_H
#define SPHEREGLRENDERER_H

#include <QtOpenGL>
#include <vector>

#include "Eigen/Core"

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
