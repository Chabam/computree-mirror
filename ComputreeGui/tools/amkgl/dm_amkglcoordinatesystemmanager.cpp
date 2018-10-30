#include "dm_amkglcoordinatesystemmanager.h"

#include "ct_global/ct_context.h"

DM_AMKglCoordinateSystemManager::DM_AMKglCoordinateSystemManager()
{
    m_coordinateSystemManager = PS_COORDINATES_SYS_MANAGER;
    m_coordinateSystemIndexesCollection = m_coordinateSystemManager->indexCloudOfCoordinateSystemOfPoints();
}

void DM_AMKglCoordinateSystemManager::getCoordinateSystemInfos(const GLuint &coordinateSystemIndex,
                                                               uint &uniqueKey,
                                                               Eigen::Vector3d &offset)
{
    CT_AbstractCoordinateSystem *cs = m_coordinateSystemManager->coordinateSystemAt(coordinateSystemIndex);
    cs->offset(offset);

    uniqueKey = cs->uniqueKey();
}

const GLuint& DM_AMKglCoordinateSystemManager::getCoordinateSystemIndexOfPointAt(const size_t &globalIndex)
{
    return m_coordinateSystemIndexesCollection->constValueAt(globalIndex);
}

void DM_AMKglCoordinateSystemManager::getCoordinateSystemInfosForCoordinates(const double &x,
                                                                             const double &y,
                                                                             const double &z,
                                                                             uint &uniqueKey,
                                                                             Eigen::Vector3d &offset)
{
    uniqueKey = m_coordinateSystemManager->computeUniqueKeyForCoordinatesAndFillTab(x,
                                                                                    y,
                                                                                    z,
                                                                                    m_tab);

    m_coordinateSystemManager->computeOffsetForTab(m_tab,
                                                   offset);
}
