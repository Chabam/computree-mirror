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

#include "ct_fileheader.h"

CT_DEFAULT_IA_INIT(CT_FileHeader)

void CT_FileHeader::setFilePath(const QString& filepath)
{
    m_fileInfo = QFileInfo(filepath);
}

QFileInfo CT_FileHeader::fileInfo() const
{
    return m_fileInfo;
}

QString CT_FileHeader::fileName() const
{
    return m_fileInfo.fileName();
}

QString CT_FileHeader::filePath() const
{
    return m_fileInfo.filePath();
}

QString CT_FileHeader::directoryPath() const
{
    return m_fileInfo.path();
}



