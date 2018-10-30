#include "boxglrenderer.h"

BoxGLRenderer::BoxGLRenderer()
{
    init();
}

void BoxGLRenderer::init()
{
    m_values.resize(8);

    // front
    m_values[0][0] = -1;
    m_values[0][1] = -1;
    m_values[0][2] = -1;

    m_values[1][0] = -1;
    m_values[1][1] = 1;
    m_values[1][2] = -1;

    m_values[2][0] = 1;
    m_values[2][1] = 1;
    m_values[2][2] = -1;

    m_values[3][0] = 1;
    m_values[3][1] = -1;
    m_values[3][2] = -1;

    int i;
    // back
    for(i=4; i<8; ++i) {
        m_values[i][0] = m_values[i-4][0];
        m_values[i][1] = m_values[i-4][1];
        m_values[i][2] = 1;
    }

    // indices to draw with GL_QUADS command
    m_indices.resize(24);

    i = 0;
    // front
    m_indices[i++] = 0;
    m_indices[i++] = 1;
    m_indices[i++] = 2;
    m_indices[i++] = 3;

    // left
    m_indices[i++] = 0;
    m_indices[i++] = 1;
    m_indices[i++] = 5;
    m_indices[i++] = 4;

    // back
    m_indices[i++] = 4;
    m_indices[i++] = 5;
    m_indices[i++] = 6;
    m_indices[i++] = 7;

    // right
    m_indices[i++] = 2;
    m_indices[i++] = 6;
    m_indices[i++] = 7;
    m_indices[i++] = 3;

    // top
    m_indices[i++] = 1;
    m_indices[i++] = 5;
    m_indices[i++] = 6;
    m_indices[i++] = 2;

    // bottom
    m_indices[i++] = 0;
    m_indices[i++] = 4;
    m_indices[i++] = 7;
    m_indices[i++] = 3;
}

void BoxGLRenderer::render()
{
    const size_t& size = m_indices.size();

    for(int i=0; i<size; ++i)
        glVertex3fv(&m_values[m_indices[i]][0]);
}

const std::vector<Eigen::Vector3f> &BoxGLRenderer::getValues() const
{
    return m_values;
}

const std::vector<GLuint> &BoxGLRenderer::getIndices() const
{
    return m_indices;
}
