/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Rudy MARTY (CS GROUP)

 This file is part of CT_Buffer library 1.0.

 CT_Buffer is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 CT_Buffer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with CT_Buffer.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#ifndef CTLIBBUFFER_GLOBAL_H
#define CTLIBBUFFER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CTLIBBUFFER_LIBRARY)
#  define CTLIBBUFFER_EXPORT Q_DECL_EXPORT
#else
#  define CTLIBBUFFER_EXPORT Q_DECL_IMPORT
#endif


#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
#define QT_SKIP_EMPTY_PARTS Qt::SkipEmptyParts
#else
#define QT_SKIP_EMPTY_PARTS QString::SkipEmptyParts
#endif

#endif // CTLIBBUFFER_GLOBAL_H
