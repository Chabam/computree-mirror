#include "ellipseglrenderer.h"

#if !defined (_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include <QtMath>

#include <QtOpenGL/QtOpenGL>

EllipseGLRenderer::EllipseGLRenderer()
{
}

EllipseGLRenderer::EllipseGLRenderer(int nPoints)
{
    init(nPoints);
}

void EllipseGLRenderer::init(int nPoints)
{
    m_values.resize(nPoints);

    float inc = M_PI*2/((double)nPoints);
    float a = 0;

    for(int i=0; i<nPoints; ++i)
    {
        Eigen::Vector3f &pair = m_values[i];
        pair[0] = std::cos(a);
        pair[1] = 0;
        pair[2] = std::sin(a);

        a += inc;
    }
}

void EllipseGLRenderer::render()
{
    const size_t& size = m_values.size();

    for(int i=0; i<size; ++i)
        glVertex3fv(&m_values[i][0]);
}

const std::vector<Eigen::Vector3f> &EllipseGLRenderer::getValues() const
{
    return m_values;
}
