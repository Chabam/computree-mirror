#if !defined (_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <QtMath>

#include "ct_thetaphishootingpattern.h"
#include "ct_itemdrawable/ct_scanner.h"

CT_ThetaPhiShootingPattern::CT_ThetaPhiShootingPattern() :
    CT_ThetaPhiShootingPattern(Eigen::Vector3d::Zero(),
                               0,
                               0,
                               0,
                               0,
                               0,
                               0,
                               Eigen::Vector3d(0, 0, 1),
                               true,
                               false)
{
}

CT_ThetaPhiShootingPattern::CT_ThetaPhiShootingPattern(const Eigen::Vector3d &origin,
                                                       double hFov,
                                                       double vFov,
                                                       double hRes,
                                                       double vRes,
                                                       double initTheta,
                                                       double initPhi,
                                                       const Eigen::Vector3d &zVector,
                                                       bool clockWise,
                                                       bool radians) :
    SuperClass()
{
    m_origin = origin;
    m_zVector = zVector;
    m_clockWise = clockWise;

    m_hFov = hFov;
    m_vFov = vFov;
    m_hRes = hRes;
    m_vRes = vRes;

    m_initTheta = (m_clockWise ? -initTheta : initTheta);
    m_initPhi = initPhi;

    if(!radians )
    {
        m_hFov = qDegreesToRadians(m_hFov);
        m_vFov = qDegreesToRadians(m_vFov);
        m_hRes = qDegreesToRadians(m_hRes);
        m_vRes = qDegreesToRadians(m_vRes);
        m_initTheta = qDegreesToRadians(m_initTheta);
        m_initPhi = qDegreesToRadians(m_initPhi);
    }


    computeShots();

}

size_t CT_ThetaPhiShootingPattern::numberOfShots() const
{
    return m_shots.size();
}

CT_Shot CT_ThetaPhiShootingPattern::shotAt(const size_t& index) const
{
   return m_shots.at(index);
}

CT_Shot CT_ThetaPhiShootingPattern::shotForPoint(const CT_Point& pt) const
{
    return CT_Shot(m_origin, pt - m_origin);
}

CT_ShootingPattern* CT_ThetaPhiShootingPattern::clone() const
{
    return new CT_ThetaPhiShootingPattern(*this);
}

void CT_ThetaPhiShootingPattern::computeShots()
{
    m_nHRays = 0;
    m_nVRays = 0;
    m_shots.clear();

    // Calculates the number of horizontal and vertical rays
    if (!qFuzzyIsNull(m_hRes))
    {
        m_nHRays = int(ceil(fabs(m_hFov/m_hRes)));
    }

    if (!qFuzzyIsNull(m_vRes))
    {
        m_nVRays = int(ceil(fabs(m_vFov/m_vRes)));
    }


    for (size_t i = 0; i < m_nHRays; ++i)
    {
        double theta = m_initTheta + (i * m_hRes);

        if (isClockWise()) {
            theta = -theta;
        }

        const double cosTheta = std::cos(theta);
        const double sinTheta = std::sin(theta);
        for (size_t j = 0; j < m_nVRays; ++j)
        {
            const double phi = m_initPhi + (j * m_vRes);

            const double sinPhi = std::sin(phi);
            const double cosPhi = std::cos(phi);

            // The direction is calculated using spherical coordinates
            Eigen::Vector3d direction;
            direction.x() = sinPhi * cosTheta;
            direction.y() = sinPhi * sinTheta;
            direction.z() = cosPhi;
            m_shots.append(CT_Shot(m_origin, direction));
        }
    }
}
