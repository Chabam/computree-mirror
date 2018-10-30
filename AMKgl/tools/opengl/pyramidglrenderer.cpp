#include "pyramidglrenderer.h"

PyramidGLRenderer::PyramidGLRenderer()
{
    init();
}

void PyramidGLRenderer::init()
{
    m_values.resize(5);

    // base
    m_values[0][0] = -1;
    m_values[0][1] = 1;
    m_values[0][2] = -1;

    m_values[1][0] = -1;
    m_values[1][1] = 1;
    m_values[1][2] = 1;

    m_values[2][0] = 1;
    m_values[2][1] = 1;
    m_values[2][2] = 1;

    m_values[3][0] = 1;
    m_values[3][1] = 1;
    m_values[3][2] = -1;

    // top
    m_values[4][0] = 0;
    m_values[4][1] = -1;
    m_values[4][2] = 0;

    // indices to draw with GL_TRIANGLES command
    m_indices.resize(18);

    size_t i = 0;

    // base
    m_indices[i++] = 0;
    m_indices[i++] = 1;
    m_indices[i++] = 3;

    m_indices[i++] = 3;
    m_indices[i++] = 2;
    m_indices[i++] = 1;

    // side
    m_indices[i++] = 1;
    m_indices[i++] = 4;
    m_indices[i++] = 2;

    m_indices[i++] = 2;
    m_indices[i++] = 3;
    m_indices[i++] = 4;

    m_indices[i++] = 4;
    m_indices[i++] = 3;
    m_indices[i++] = 0;

    m_indices[i++] = 0;
    m_indices[i++] = 1;
    m_indices[i++] = 4;
}

void PyramidGLRenderer::render()
{
    const size_t& size = m_indices.size();

    for(int i=0; i<size; ++i)
        glVertex3fv(&m_values[m_indices[i]][0]);
}

const std::vector<Eigen::Vector3f> &PyramidGLRenderer::getValues() const
{
    return m_values;
}

const std::vector<GLuint> &PyramidGLRenderer::getIndices() const
{
    return m_indices;
}
