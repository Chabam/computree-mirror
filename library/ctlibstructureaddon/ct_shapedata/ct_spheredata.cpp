#include "ct_spheredata.h"

CT_SphereData::CT_SphereData() : SuperClass()
{
    m_radius = 0;
}

CT_SphereData::CT_SphereData(const Eigen::Vector3d& center, double radius) : SuperClass(center, Eigen::Vector3d(0, 0, 0))
{
    m_radius = radius;
}

double CT_SphereData::getRadius() const
{
    return m_radius;
}
