#ifndef DM_AMKGLCOORDINATESYSTEMMANAGER_H
#define DM_AMKGLCOORDINATESYSTEMMANAGER_H

#include "interfaces/icoordinatesystemmanager.h"

#include "ct_coordinates/tools/ct_coordinatesystemmanager.h"

class DM_AMKglCoordinateSystemManager : public ICoordinateSystemManager
{
public:
    DM_AMKglCoordinateSystemManager();

    void getCoordinateSystemInfos(const GLuint &coordinateSystemIndex,
                                  uint& uniqueKey,
                                  Eigen::Vector3d& offset);

    const GLuint& getCoordinateSystemIndexOfPointAt(const size_t &globalIndex);

    void getCoordinateSystemInfosForCoordinates(const double& x,
                                                const double& y,
                                                const double& z,
                                                uint& uniqueKey,
                                                Eigen::Vector3d& offset);

private:
    CT_CoordinateSystemManager*     m_coordinateSystemManager;
    CT_CoordinateSystemCloudIndex*  m_coordinateSystemIndexesCollection;
    int                             m_tab[3];
};

#endif // DM_AMKGLCOORDINATESYSTEMMANAGER_H
