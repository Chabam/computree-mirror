#ifndef CT_GRID4D_DENSE_HPP
#define CT_GRID4D_DENSE_HPP

#include "ct_itemdrawable/ct_grid4d_dense.h"

#include <math.h>
#include <typeinfo>

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense() : SuperClass()
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense(const CT_Grid4D_Dense<DataT>& other) : SuperClass(other)
{
    _data = other._data.clone();
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense(double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        size_t dimw,
                                        size_t dimx,
                                        size_t dimy,
                                        size_t dimz,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : SuperClass(wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            dimw,
                                                                            dimx,
                                                                            dimy,
                                                                            dimz,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    this->_data.create(1, ncells);
    initGridWithValue(initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::CT_Grid4D_Dense(double wmin,
                                        double xmin,
                                        double ymin,
                                        double zmin,
                                        double wmax,
                                        double xmax,
                                        double ymax,
                                        double zmax,
                                        double resw,
                                        double resx,
                                        double resy,
                                        double resz,
                                        DataT na,
                                        DataT initValue) : SuperClass(wmin,
                                                                            xmin,
                                                                            ymin,
                                                                            zmin,
                                                                            wmax,
                                                                            xmax,
                                                                            ymax,
                                                                            zmax,
                                                                            resw,
                                                                            resx,
                                                                            resy,
                                                                            resz,
                                                                            na)
{
    int ncells[1];
    ncells[0] = this->nCells();
    initGridWithValue(initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>* CT_Grid4D_Dense<DataT>::createGrid4DFromWXYZCoords(const CT_OutAbstractSingularItemModel *model,
                                                                           const CT_AbstractResult *result,
                                                                           double wmin,
                                                                           double xmin,
                                                                           double ymin,
                                                                           double zmin,
                                                                           double wmax,
                                                                           double xmax,
                                                                           double ymax,
                                                                           double zmax,
                                                                           double resw,
                                                                           double resx,
                                                                           double resy,
                                                                           double resz,
                                                                           DataT na,
                                                                           DataT initValue)
{
    size_t dimw = ceil((wmax - wmin)/resw);
    size_t dimx = ceil((xmax - xmin)/resx);
    size_t dimy = ceil((ymax - ymin)/resy);
    size_t dimz = ceil((zmax - zmin)/resz);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (wmax >= (wmin + dimw * resw))
    {
        dimw++;
    }

    while (xmax >= (xmin + dimx * resx))
    {
        dimx++;
    }

    while (ymax >= (ymin + dimy * resy))
    {
        dimy++;
    }

    while (zmax >= (zmin + dimz * resz))
    {
        dimz++;
    }

    return new CT_Grid4D_Dense<DataT>(model, result, wmin, xmin, ymin, zmin, dimw, dimx, dimy, dimz, resw, resx, resy, resz, na, initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>* CT_Grid4D_Dense<DataT>::createGrid4DFromWXYZCoords(const QString &modelName,
                                                                           const CT_AbstractResult *result,
                                                                           double wmin,
                                                                           double xmin,
                                                                           double ymin,
                                                                           double zmin,
                                                                           double wmax,
                                                                           double xmax,
                                                                           double ymax,
                                                                           double zmax,
                                                                           double resw,
                                                                           double resx,
                                                                           double resy,
                                                                           double resz,
                                                                           DataT na,
                                                                           DataT initValue)
{
    size_t dimw = ceil((wmax - wmin)/resw);
    size_t dimx = ceil((xmax - xmin)/resx);
    size_t dimy = ceil((ymax - ymin)/resy);
    size_t dimz = ceil((zmax - zmin)/resz);

    // to ensure a point exactly on a maximum limit of the grid will be included in the grid
    while (wmax >= (wmin + dimw * resw))
    {
        dimw++;
    }

    while (xmax >= (xmin + dimx * resx))
    {
        dimx++;
    }

    while (ymax >= (ymin + dimy * resy))
    {
        dimy++;
    }

    while (zmax >= (zmin + dimz * resz))
    {
        dimz++;
    }

    return new CT_Grid4D_Dense<DataT>(modelName, result, wmin, xmin, ymin, zmin, dimw, dimx, dimy, dimz, resw, resx, resy, resz, na, initValue);
}

template< typename DataT>
CT_Grid4D_Dense<DataT>::~CT_Grid4D_Dense()
{
    _data.release();
}

template< typename DataT>
void CT_Grid4D_Dense<DataT>::initGridWithValue(const DataT val)
{
    for (size_t  i = 0 ; i < this->nCells() ; i++)
    {
        _data(i) = val;
    }
    this->_dataMin = val;
    this->_dataMax = val;
}

#endif // CT_GRID4D_DENSE_HPP
