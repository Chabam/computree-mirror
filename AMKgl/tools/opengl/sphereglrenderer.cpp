#include "sphereglrenderer.h"

#if !defined (_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include <QtMath>

#include <QtOpenGL/QtOpenGL>

SphereGlRenderer::SphereGlRenderer()
{
}

SphereGlRenderer::SphereGlRenderer(int nPoints)
{
    init(nPoints);
}

void SphereGlRenderer::init(int nPoints)
{
    initWithGlLineStrip(nPoints);
}

void SphereGlRenderer::render()
{
    if(!m_indices.empty()) {
        const size_t& size = m_indices.size();

        for(int i=0; i<size; ++i)
            glVertex3fv(&m_values[m_indices[i]][0]);

    } else {

        const size_t& size = m_values.size();

        for(int i=0; i<size; ++i)
            glVertex3fv(&m_values[i][0]);
    }
}

const std::vector<Eigen::Vector3f> &SphereGlRenderer::getValues() const
{
    return m_values;
}

const std::vector<Eigen::Vector2f> &SphereGlRenderer::getThetaPhiValues() const
{
    return m_thetaPhiValues;
}

const std::vector<GLuint> &SphereGlRenderer::getIndices() const
{
    return m_indices;
}

void SphereGlRenderer::initWithGlLineStrip(int nPoints)
{
    int nPointsPhi = nPoints;
    int nPointsTheta = nPoints;

    int endPhiSecondPart = nPointsPhi;

    int firstPart = nPointsTheta*nPointsPhi;
    int secondPart = ((endPhiSecondPart-1)*nPointsTheta)+endPhiSecondPart-1;

    m_values.resize(firstPart+1);
    m_thetaPhiValues.resize(m_values.size());
    m_indices.resize(m_values.size()+secondPart);

    float finalTheta = (float)(M_PI*2.0f);
    float finalPhi = finalTheta;

    float stepTheta = finalTheta / ((float)nPointsTheta);
    float stepPhi = finalPhi / ((float)nPointsPhi);

    float cosPhi, sinPhi, cosTheta, sinTheta, lastPhi;

    float currentTheta = 0;

    size_t currentIndex = 0;

    bool useReverse = true;
    bool reverse = false;

    for ( int i=0; i<nPointsTheta; ++i )
    {
        sinTheta = std::sin(currentTheta);
        cosTheta = std::cos(currentTheta);

        float currentPhi = 0;

        if(reverse)
            currentPhi = finalPhi;

        for (int j=0; j<nPointsPhi; ++j)
        {
            sinPhi = std::sin(currentPhi);
            cosPhi = std::cos(currentPhi);

            m_values[currentIndex] = Eigen::Vector3f(sinPhi*cosTheta,
                                                     sinPhi*sinTheta,
                                                     cosPhi);

            m_thetaPhiValues[currentIndex] = Eigen::Vector2f(currentTheta, currentPhi);

            if(!m_indices.empty())
                m_indices[currentIndex] = (GLuint)currentIndex;

            if(reverse) {
                if(j == 0)
                    currentPhi = lastPhi;
                else
                    currentPhi -= stepPhi;
            } else {
                if(j < nPointsPhi)
                    lastPhi = currentPhi;

                currentPhi += stepPhi;
            }

            ++currentIndex;
        }

        //if(i < 1)
            currentTheta += stepTheta;

        if(useReverse)
            reverse = !reverse;
    }

    m_values[currentIndex] = Eigen::Vector3f(0,0,1);
    m_thetaPhiValues[currentIndex] = Eigen::Vector2f(0, 0);

    if(!m_indices.empty())
        m_indices[currentIndex++] = (GLuint)currentIndex;

    if(!m_indices.empty()) {
        for(int i=1; i<endPhiSecondPart; ++i) {

            reverse = false;
            int newIndex = i;
            int normalStep = (i*2);
            int reverseStep = (nPointsPhi*2) - normalStep;

            for(int j=0; j<nPointsTheta; ++j) {
                m_indices[currentIndex++] = newIndex;

                if(useReverse) {
                    reverse = !reverse;

                    if(reverse)
                        newIndex += reverseStep;
                    else
                        newIndex += normalStep;

                } else {
                    newIndex += nPointsPhi;
                }
            }

            m_indices[currentIndex++] = i;
        }
    }
}
