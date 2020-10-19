#ifndef QUADSGLRENDERER_H
#define QUADSGLRENDERER_H

#include <vector>

#include <Eigen/Core>

/**
 * @brief Quads renderer
 */
class QuadsGLRenderer
{
public:
    QuadsGLRenderer();

    /**
     * @brief Init this renderer to draw a quads
     */
    void init();

    /**
     * @brief Render a quads
     */
    void render();

    /**
     * @brief Returns the array that contains points
     */
    const std::vector<Eigen::Vector3f>& getValues() const;

private:
    std::vector<Eigen::Vector3f> m_values;
};

#endif // QUADSGLRENDERER_H
