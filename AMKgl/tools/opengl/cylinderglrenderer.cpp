#include "cylinderglrenderer.h"

CylinderGLRenderer::CylinderGLRenderer()
{

}

CylinderGLRenderer::CylinderGLRenderer(int nPoints)
{
    init(nPoints);
}

void CylinderGLRenderer::init(int nPoints)
{
    int fNPoints = (2 * (nPoints + 1)) + 2;

    m_values.resize(fNPoints);

    float inc = M_PI*2/((double)nPoints);
    float a = 0;
    int j;

    for (int i=0 ; i<=nPoints ; ++i)
    {
        j = 2 * i;
        double dx = cos(a);
        double dz = sin(a);

        Eigen::Vector3f &coords = m_values[j];
        coords[0] = dx; coords[1] = -0.5; coords[2] = dz;

        Eigen::Vector3f &coords2 = m_values[j+1];
        coords2[0] = dx; coords2[1] = 0.5; coords2[2] = dz;

        a += inc;
    }

    Eigen::Vector3f &coords = m_values[fNPoints-2];
    coords[0] = 0; coords[1] = 0.5; coords[2] = 0;

    Eigen::Vector3f &coords2 = m_values[fNPoints-1];
    coords2[0] = 0; coords2[1] = -0.5; coords2[2] = 0;

    m_indices.resize(nPoints*8 + 1);

    j = 0;

    for(int i=0; i<nPoints; ++i) {
        m_indices[j] = fNPoints-1;
        m_indices[j+1] = i*2;
        m_indices[j+2] = (i*2)+2;
        j+=3;
    }

    int lastPoint = (2 * nPoints)-1;
    for(int i=0; i<nPoints*2; ++i) {
        m_indices[j] = lastPoint--;
        ++j;
    }

    m_indices[j++] = (nPoints*2)-1;

    for(int i=0; i<nPoints; ++i) {
        m_indices[j] = fNPoints-2;
        m_indices[j+1] = i*2+1;
        m_indices[j+2] = (i*2)+3;
        j+=3;
    }
}

void CylinderGLRenderer::render()
{
    const size_t& size = m_indices.size();

    for(int i=0; i<size; ++i)
        glVertex3fv(&m_values[m_indices[i]][0]);
}

const std::vector<Eigen::Vector3f> &CylinderGLRenderer::getValues() const
{
    return m_values;
}

const std::vector<GLuint> &CylinderGLRenderer::getIndices() const
{
    return m_indices;
}
