/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

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

#include "ct_pointcluster.h"

#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_cloudindex/registered/ct_standardmodifiablecloudindexregisteredt.h"
#include "ct_accessor/ct_pointaccessor.h"
#include "ct_iterator/ct_pointiterator.h"

const CT_StandardPointClusterDrawManager CT_PointCluster::POINTCLUSTER_DRAW_MANAGER;

CT_PointCluster::CT_PointCluster(bool prototype) : SuperClass()
{
    m_pIndex = nullptr;
    _barycenter.reset();

    if(!prototype) {
        CT_PointCloudIndexVector* cloudIndex = new CT_PointCloudIndexVector();
        m_pIndex = cloudIndex;

        cloudIndex->setSortType(CT_AbstractPointCloudIndex::NotSorted); // Important to be able to store Polylines
        setPointCloudIndexRegistered(PS_REPOSITORY->registerPointCloudIndex(cloudIndex));
    }

    setBaseDrawManager(&POINTCLUSTER_DRAW_MANAGER);
}

bool CT_PointCluster::addPoint(size_t index, bool verifyIfExist, bool firstPosition)
{
    if(verifyIfExist && m_pIndex->contains(index))
        return false;

    CT_PointAccessor pAccess;
    const CT_Point& point = pAccess.constPointAt(index);

    if (firstPosition)
        m_pIndex->push_front(index);
    else
        m_pIndex->addIndex(index);

    _barycenter.addPoint(point);

    Eigen::Vector3d min, max;
    boundingBox(min, max);

    min(0) = qMin(point(0), min(0));
    min(1) = qMin(point(1), min(1));
    min(2) = qMin(point(2), min(2));

    max(0) = qMax(point(0), max(0));
    max(1) = qMax(point(1), max(1));
    max(2) = qMax(point(2), max(2));

    setBoundingBox(min, max);

    return true;
}

const CT_PointClusterBarycenter& CT_PointCluster::getBarycenter() const
{
    return _barycenter;
}

CT_PointCluster* CT_PointCluster::merge(CT_PointCluster& pCLuster1, CT_PointCluster& pCLuster2, bool verifyDuplicated)
{
    CT_PointCluster* pMerged = new CT_PointCluster(false);

    const CT_AbstractPointCloudIndex* pIndex1 = pCLuster1.pointCloudIndex();
    const CT_AbstractPointCloudIndex* pIndex2 = pCLuster2.pointCloudIndex();

    size_t size = pIndex1->size();
    size_t index;

    for(size_t i=0; i<size; ++i)
    {
        pIndex1->indexAt(i, index);
        pMerged->addPoint(index);
    }

    size = pIndex2->size();

    for(size_t i=0; i<size; ++i)
    {
        pIndex2->indexAt(i, index);
        pMerged->addPoint(index, verifyDuplicated);
    }

    return pMerged;
}

void CT_PointCluster::initBarycenter()
{
    _barycenter.reset();

    CT_PointIterator it(pointCloudIndex());

    while(it.hasNext())
    {
        it.next();
        _barycenter.addPoint(it.currentPoint());
    }

    _barycenter.compute();
}
