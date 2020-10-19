#ifndef PYRAMIDGLRENDERER_H
#define PYRAMIDGLRENDERER_H

#include <vector>
#include <QtOpenGL/QtOpenGL>

#include <Eigen/Core>

/**
 * @brief Pyramid renderer
 */
class PyramidGLRenderer
{
public:
    PyramidGLRenderer();

    /**
     * @brief Init this renderer to draw a pyramid
     */
    void init();

    /**
     * @brief Render a pyramid
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

#endif // PYRAMIDGLRENDERER_H
