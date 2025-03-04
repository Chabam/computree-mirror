/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "ct_scanpath.h"

#include <Eigen/src/Core/DenseStorage.h>
#include <Eigen/src/Core/Matrix.h>
#include <algorithm>
#include <limits>

const CT_StandardScanPathDrawManager CT_ScanPath::SCANPATH_DRAW_MANAGER;

CT_TYPE_IMPL_INIT_MACRO(CT_ScanPath)

CT_ScanPath::CT_ScanPath()
{
    _minGPSTime = std::numeric_limits<double>::max();
    _maxGPSTime = -std::numeric_limits<double>::max();
}

CT_ScanPath::CT_ScanPath(QString name) :
    SuperClass(),
    _minGPSTime(std::numeric_limits<double>::max()),
    _maxGPSTime(-std::numeric_limits<double>::max())
{
    _name = name;
    setDisplayableName(_name);
    setBoundingBox(std::numeric_limits<double>::max(),
                   std::numeric_limits<double>::max(),
                   std::numeric_limits<double>::max(),
                   -std::numeric_limits<double>::max(),
                   -std::numeric_limits<double>::max(),
                   -std::numeric_limits<double>::max());

    setBaseDrawManager(&SCANPATH_DRAW_MANAGER);
}

CT_ScanPath::CT_ScanPath(const CT_ScanPath &other)
{
    _name = other._name;
    setDisplayableName(_name);

    _pathPoints = other._pathPoints;
    _minGPSTime = other._minGPSTime;
    _maxGPSTime = other._maxGPSTime;

    Eigen::Vector3d min, max;
    other.boundingBox(min, max);

    setBoundingBox(min(0), min(1), min(2), max(0), max(1), max(2));

    setBaseDrawManager(&SCANPATH_DRAW_MANAGER);
}

QString CT_ScanPath::getPathName() const
{
    return _name;
}

void CT_ScanPath::addPathPoint(double gpsTime, double x, double y, double z, double h, double r, double p)
{
    QList<PathPoint>::iterator it = _pathPoints.begin();
    for (; it != _pathPoints.end(); ++it)
    {
        if (it->_gpsTime > gpsTime)
            break;
    }
    _pathPoints.insert(it, PathPoint(gpsTime, Eigen::Vector3d(x,y,z), Eigen::Vector3d(h,r,p)));

    Eigen::Vector3d min, max;
    boundingBox(min, max);

    if (x < min(0)) {min(0) = x;}
    if (y < min(1)) {min(1) = y;}
    if (z < min(2)) {min(2) = z;}
    if (gpsTime < _minGPSTime) {_minGPSTime = gpsTime;}

    if (x > max(0)) {max(0) = x;}
    if (y > max(1)) {max(1) = y;}
    if (z > max(2)) {max(2) = z;}
    if (gpsTime > _maxGPSTime) {_maxGPSTime = gpsTime;}

    setBoundingBox(min, max);
}

void CT_ScanPath::addPathPoint(double gpsTime, const Eigen::Vector3d &position)
{
    addPathPoint(gpsTime, position(0), position(1), position(2), 0, 0, 0);
}

void CT_ScanPath::addPathPoint(double gpsTime, const Eigen::Vector3d &position, const Eigen::Vector3d &orientation)
{
    addPathPoint(gpsTime, position(0), position(1), position(2), orientation(0), orientation(1), orientation(2));
}

bool CT_ScanPath::isInScanPath(double gpsTime) const
{
    if (gpsTime < _minGPSTime) {return false;}
    if (gpsTime > _maxGPSTime) {return false;}
    return true;
}

Eigen::Vector3d CT_ScanPath::getPathPointForGPSTime(double gpsTime) const
{
    // Returns the first point that is **not less** than the given gps time
    QList<PathPoint>::const_iterator pathPoint = std::lower_bound(
        _pathPoints.cbegin(),
        _pathPoints.cend(),
        gpsTime,
        [](const PathPoint& point, double bestGpsTime)
        {
            return point._gpsTime < bestGpsTime;

        });

    // There was no point found for this gps time.
    if (pathPoint == _pathPoints.cend())
        return Eigen::Vector3d(0,0,0);

    // If the point was the first one we can't do the ratio with the
    // previous point. There is no previous point.
    if (pathPoint == _pathPoints.cbegin())
        return pathPoint->_position;

    // Otherwise we do the ratio
    QList<PathPoint>::const_iterator previousPathPoint = pathPoint - 1;

    const Eigen::Vector3d currentPoint = pathPoint->_position;
    const double currentGPSTime = pathPoint->_gpsTime;
    const Eigen::Vector3d lastPoint = previousPathPoint->_position;
    const double lastGPSTime = previousPathPoint->_gpsTime;

    const Eigen::Vector3d direction = currentPoint - lastPoint;
    const double ratio = (gpsTime - lastGPSTime) / (currentGPSTime - lastGPSTime);

    return lastPoint + direction * ratio;
}
