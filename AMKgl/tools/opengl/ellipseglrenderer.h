#ifndef ELLIPSEGLRENDERER_H
#define ELLIPSEGLRENDERER_H

#include <vector>

#include "Eigen/Core"

/**
 * @brief Ellipse renderer
 */
class EllipseGLRenderer
{
public:
    EllipseGLRenderer();
    EllipseGLRenderer(int nPoints);

    /**
     * @brief Init this renderer to draw an ellipse with a finite number of points
     */
    void init(int nPoints);

    /**
     * @brief Render an ellipse
     */
    void render();

    /**
     * @brief Returns the array that contains values
     */
    const std::vector<Eigen::Vector3f>& getValues() const;

private:
    std::vector<Eigen::Vector3f> m_values;

};

#endif // ELLIPSEGLRENDERER_H
