#include "quadsglrenderer.h"

#include <QtOpenGL/QtOpenGL>

QuadsGLRenderer::QuadsGLRenderer()
{
    init();
}

void QuadsGLRenderer::init()
{
    m_values.resize(4);

    m_values[0][0] = -1;
    m_values[0][1] = 0;
    m_values[0][2] = -1;

    m_values[1][0] = -1;
    m_values[1][1] = 0;
    m_values[1][2] = 1;

    m_values[2][0] = 1;
    m_values[2][1] = 0;
    m_values[2][2] = 1;

    m_values[3][0] = 1;
    m_values[3][1] = 0;
    m_values[3][2] = -1;
}

void QuadsGLRenderer::render()
{
    const size_t& size = m_values.size();

    for(int i=0; i<static_cast<int>(size); ++i)
        glVertex3fv(&m_values[i][0]);
}

const std::vector<Eigen::Vector3f>& QuadsGLRenderer::getValues() const
{
    return m_values;
}
