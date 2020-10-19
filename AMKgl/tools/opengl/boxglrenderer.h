#ifndef BOXGLRENDERER_H
#define BOXGLRENDERER_H

#include <vector>
#include <QtOpenGL/QtOpenGL>

#include <Eigen/Core>

/**
 * @brief Box renderer
 */
class BoxGLRenderer
{
public:
    BoxGLRenderer();

    /**
     * @brief Init this renderer to draw a box
     */
    void init();

    /**
     * @brief Render a box
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

#endif // BOXGLRENDERER_H
