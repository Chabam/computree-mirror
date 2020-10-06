/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
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

#ifndef CT_SHAPE2DDATA_H
#define CT_SHAPE2DDATA_H

#include "ctlibstructureaddon_global.h"

#if defined(_WIN32) && defined(_MSC_VER) // Microsoft Visual Studio Compiler
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__) // GNU Compiler (gcc,g++) for Linux, Unix, and MinGW (Windows)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic ignored "-Wdeprecated-copy"
#else
#pragma GCC diagnostic ignored "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#elif defined(__APPLE__) // Clang Compiler (Apple)
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wint-in-bool-context"
#endif
#include "Eigen/Core"
#if defined(_WIN32) && defined(_MSC_VER)
#elif (defined(__linux__) || defined(_WIN32)) && defined(__GNUC__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#if __GNUC__ > 7
#pragma GCC diagnostic warning "-Wdeprecated-copy"
#else
#pragma GCC diagnostic warning "-Wattributes" // See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89325
#endif
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#elif defined(__APPLE__)
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wint-in-bool-context"
#endif

#define CT_SHAPEDATA2D_CLONE_IMP(argClass) CT_Shape2DData* copy() const override { return clone(); } \
                                           argClass* clone() const { return new argClass(*this); }

/**
  * Represent data of 2D geometrical item
  */
class CTLIBSTRUCTUREADDON_EXPORT CT_Shape2DData
{
public:
    CT_Shape2DData() = default;
    CT_Shape2DData(const Eigen::Vector2d& center);
    CT_Shape2DData(const double& x, const double& y);
    CT_Shape2DData(const CT_Shape2DData& other) = default;
    virtual ~CT_Shape2DData() {}

    const Eigen::Vector2d& getCenter() const;

    void setCenterX(const double& x);
    void setCenterY(const double& y);
    void setCenter(const Eigen::Vector2d& center);
    void setCenter(const Eigen::Vector3d& center);

    virtual void getBoundingBox(Eigen::Vector3d& min, Eigen::Vector3d& max) const = 0;

    virtual CT_Shape2DData* copy() const = 0;

protected:
    Eigen::Vector2d   _center;
};

#endif // CT_SHAPE2DDATA_H
