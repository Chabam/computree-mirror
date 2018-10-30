#ifndef CYLINDERGLRENDERER_H
#define CYLINDERGLRENDERER_H

#include <vector>
#include <QtOpenGL/QtOpenGL>

#include "Eigen/Core"

/**
 * @brief Cylinder renderer
 */
class CylinderGLRenderer
{
public:
    CylinderGLRenderer();
    CylinderGLRenderer(int nPoints);

    /**
     * @brief Init this renderer to draw a cylinder with a finite number of points
     */
    void init(int nPoints);

    /**
     * @brief Render a cylinder
     */
    void render();

    /**
     * @brief Returns the array that contains values
     */
    const std::vector<Eigen::Vector3f>& getValues() const;

    /**
     * @brief Returns the array that contains indices
     */
    const std::vector<GLuint>& getIndices() const;

private:
    std::vector<Eigen::Vector3f>    m_values;
    std::vector<GLuint>             m_indices;
};

#endif // CYLINDERGLRENDERER_H
