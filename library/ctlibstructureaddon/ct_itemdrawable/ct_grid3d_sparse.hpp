/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forets (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
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

#include "ct_itemdrawable/ct_grid3d_sparse.h"
#include "ct_itemdrawable/tools/drawmanager/ct_standardgrid3d_sparsedrawmanager.h"

#include <math.h>
#include <typeinfo>
#include <limits>

template< typename DataT>
const CT_StandardGrid3D_SparseDrawManager<DataT> CT_Grid3D_Sparse<DataT>::GRID3D_SPARSE_DRAW_MANAGER;

template< typename DataT>
CT_Grid3D_Sparse<DataT>::CT_Grid3D_Sparse() : SuperClass()
{
    _NAdata = -1;
    _initData = -1;
    _dataMax = -1;
    _dataMin = -1;

    int ncells[1];
    ncells[0] = 1;
    this->_data.create(1, ncells);

    setBaseDrawManager(&GRID3D_SPARSE_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid3D_Sparse<DataT>::CT_Grid3D_Sparse(const CT_Grid3D_Sparse<DataT>& other) : SuperClass(other)
{
    _NAdata = other._NAdata;
    _initData = other._initData;
    _dataMax = other._dataMax;
    _dataMin = other._dataMin;
    _data = other._data.clone();
    _colorMap = other._colorMap;
    _defaultColor = other._defaultColor;
}

template< typename DataT>
CT_Grid3D_Sparse<DataT>::CT_Grid3D_Sparse(double xmin,
                                          double ymin,
                                          double zmin,
                                          size_t dimx,
                                          size_t dimy,
                                          size_t dimz,
                                          double resolution,
                                          DataT na,
                                          DataT initValue) : SuperClass(xmin, ymin, zmin, dimx, dimy, dimz, resolution)
{
    _NAdata = na;
    _initData = initValue;
    _dataMax = initValue;
    _dataMin = initValue;

    int ncells[1];
    ncells[0] = 1;
    this->_data.create(1, ncells);

    setBaseDrawManager(&GRID3D_SPARSE_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid3D_Sparse<DataT>::CT_Grid3D_Sparse(double xmin,
                                          double ymin,
                                          double zmin,
                                          double xmax,
                                          double ymax,
                                          double zmax,
                                          double resolution,
                                          DataT na,
                                          DataT initValue) : SuperClass(xmin, ymin, zmin, xmax, ymax, zmax, resolution)
{
    _NAdata = na;
    _initData = initValue;
    _dataMax = initValue;
    _dataMin = initValue;

    int ncells[1];
    ncells[0] = 1;
    this->_data.create(1, ncells);

    setBaseDrawManager(&GRID3D_SPARSE_DRAW_MANAGER);
}

template< typename DataT>
CT_Grid3D_Sparse<DataT>* CT_Grid3D_Sparse<DataT>::createGrid3DFromXYZCoords(double xmin,
                                                                            double ymin,
                                                                            double zmin,
                                                                            double xmax,
                                                                            double ymax,
                                                                            double zmax,
                                                                            double resolution,
                                                                            DataT na,
                                                                            DataT initValue,
                                                                            bool extends)
{    
    return CreateGrid3DTFromXYZCoords<CT_Grid3D_Sparse<DataT>>(xmin, ymin, zmin, xmax, ymax, zmax, resolution, extends, na, initValue);
}

template< typename DataT>
CT_Grid3D_Sparse<DataT>::~CT_Grid3D_Sparse()
{
    _data.release();
}

template< typename DataT>
void CT_Grid3D_Sparse<DataT>::setValueAtIndexFromDouble(const size_t &index, const double &value)
{
    setValueAtIndex(index, DataT(value));
}

template< typename DataT>
void CT_Grid3D_Sparse<DataT>::computeMinMax()
{  
    _dataMin = _initData;
    _dataMax = _initData;

    bool first = true;
    cv::SparseMatConstIterator it = _data.begin(), it_end = _data.end();
    for( ; it != it_end; ++it )
    {
        DataT val = it.value<DataT>();
        if (first)
        {
            _dataMin = val;
            _dataMax = val;
            first = false;
        } else {
            if (val < _dataMin) {_dataMin = val;}
            if (val > _dataMax) {_dataMax = val;}
        }
    }
}

template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::setValueAtIndex(const size_t index, const DataT value)
{
    if (index >= nCells()) {return false;}
    _data.ref(int(index)) = value;

    return true;
}

template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::setValue(const size_t colx, const size_t liny, const size_t levz, const DataT value)
{
    size_t i;
    if (index(colx, liny, levz, i))
    {
        return setValueAtIndex(i, value);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::setValueAtXYZ(const double x, const double y, const double z, const DataT value)
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return setValueAtIndex(i, value);
    }
    return false;
}


template< typename DataT>
DataT CT_Grid3D_Sparse<DataT>::valueAtIndex(const size_t index) const
{
    if (index >= nCells()) {return NA();}

    const DataT* val = _data.template find<DataT>(int(index));
    if (val == nullptr) {return _initData;}

    return _data(int(index));
}

template< typename DataT>
double CT_Grid3D_Sparse<DataT>::ratioValueAtIndex(const size_t index) const
{
    if (_dataMax <= _dataMin) {return 1;}
    DataT value = valueAtIndex(index);
    if (value == NA()) {return -1;}
    return double((value - _dataMin)/(_dataMax - _dataMin));
}

template< typename DataT>
double CT_Grid3D_Sparse<DataT>::valueAtIndexAsDouble(const size_t index) const
{
    DataT value = valueAtIndex(index);
    if (value == NA()) {return std::numeric_limits<DataT>::quiet_NaN();}
    return value;
}

template< typename DataT>
QString CT_Grid3D_Sparse<DataT>::valueAtIndexAsString(const size_t index) const
{
    DataT value = valueAtIndex(index);
    return QVariant(value).toString();
}

template< typename DataT>
QString CT_Grid3D_Sparse<DataT>::NAAsString() const
{
    return QVariant(NA()).toString();
}

template< typename DataT>
DataT CT_Grid3D_Sparse<DataT>::value(const size_t colx, const size_t liny, const size_t levz) const
{
    size_t i;
    if (index(colx, liny, levz, i))
    {
        return valueAtIndex(i);
    }
    return NA();
}

template< typename DataT>
DataT CT_Grid3D_Sparse<DataT>::valueAtXYZ(const double x, const double y, const double z) const
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return valueAtIndex(i);
    }
    return false;
}


template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::setMaxValueAtIndex(const size_t index, const DataT value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data(index);

    if ((currentValue == NA()) || (value > currentValue)) {
        return setValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::setMaxValueAtXYZ(const double x, const double y, const double z, const DataT value)
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return setMaxValueAtIndex(i, value);
    }
    return false;
}


template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::setMinValueAtIndex(const size_t index, DataT value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data(index);

    if ((currentValue == NA()) || (value < currentValue)) {
        return setValueAtIndex(index, value);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::setMinValueAtXYZ(const double x, const double y, const double z, const DataT value)
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return setMinValueAtIndex(i, value);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::addValueAtIndex(const size_t index, DataT value)
{
    if (index >= nCells()) {return false;}

    DataT currentValue = _data(index);

    if (currentValue == NA())
    {
        return setValueAtIndex(index, value);
    } else if (value != NA())
    {
        return setValueAtIndex(index, value + currentValue);
    }
    return false;
}

template< typename DataT>
bool CT_Grid3D_Sparse<DataT>::addValueAtXYZ(const double x, const double y, const double z, const DataT value)
{
    size_t i;
    if (indexAtXYZ(x, y, z, i))
    {
        return addValueAtIndex(i, value);
    }
    return false;
}

template< typename DataT>
QList<DataT> CT_Grid3D_Sparse<DataT>::neighboursValues(const size_t colx, const size_t liny, const size_t levz, const size_t distance, const bool keepNAs, const CenterMode centermode) const
{
    QList<DataT> liste;

    if (distance < 1) {return liste;}
    if (colx >= _dimx) {return liste;}
    if (liny >= _dimy) {return liste;}
    if (levz >= _dimz) {return liste;}

    size_t firstColx = colx-distance;
    size_t lastColx = colx+distance;
    size_t firstLiny = liny-distance;
    size_t lastLiny = liny+distance;
    size_t firstLinz = levz-distance;
    size_t lastLinz = levz+distance;

    if (firstColx >= _dimx) {firstColx = 0;}
    if (lastColx >= _dimx) {lastColx = _dimx - 1;}
    if (firstLiny >= _dimy) {firstLiny = 0;}
    if (lastLiny >= _dimy) {lastLiny = _dimy - 1;}
    if (firstLinz >= _dimz) {firstLinz = 0;}
    if (lastLinz >= _dimz) {lastLinz = _dimz - 1;}


    for (size_t xx = firstColx ; xx <= lastColx ; xx++)
    {
        for (size_t yy = firstLiny ; yy <= lastLiny ; yy++)
        {
            for (size_t zz = firstLinz ; zz <= lastLinz ; zz++)
            {

                DataT val = value(xx, yy, zz);
                if ((xx == colx) && (yy == liny) && (zz == levz)) {
                    if (centermode == CM_KeepCenter) {
                        if ((val != NA()) || keepNAs) {liste.append(val);}
                    } else if (centermode == CM_NAasCenter) {
                        liste.append(NA());
                    }
                } else {
                    if ((val != NA()) || keepNAs) {liste.append(val);}
                }
            }
        }
    }

    return liste;
}

template< typename DataT>
void CT_Grid3D_Sparse<DataT>::getIndicesWithData(QList<size_t> &list) const
{
    cv::SparseMatConstIterator it = _data.begin(), it_end = _data.end();
    for( ; it != it_end; ++it )
    {
        size_t ind = size_t(it.node()->idx[0]);
        list.append(ind);
    }
}
