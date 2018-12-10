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

#include "ct_readeritem.h"

CT_DEFAULT_IA_INIT(CT_ReaderItem)

CT_ReaderItem::CT_ReaderItem() : SuperClass(),
    m_reader(NULL),
    m_mustAutoDeleteReader(false)
{
}

CT_ReaderItem::CT_ReaderItem(CT_AbstractReader* reader,
                             bool mustAutoDeleteReader) : SuperClass(),
    m_reader(reader),
    m_mustAutoDeleteReader(mustAutoDeleteReader)
{
}

CT_ReaderItem::CT_ReaderItem(const CT_ReaderItem& other) : SuperClass(other),
    m_reader((m_reader == NULL) ? NULL : other.m_reader->copyFull()),
    m_mustAutoDeleteReader(true)
{
}

CT_ReaderItem::~CT_ReaderItem()
{
    if(m_mustAutoDeleteReader)
        delete m_reader;
}

CT_AbstractReader* CT_ReaderItem::reader()
{
    return m_reader;
}

bool CT_ReaderItem::mustAutoDeleteReader() const
{
    return m_mustAutoDeleteReader;
}

QString CT_ReaderItem::readerDisplayableName() const
{
    return m_reader->displayableName();
}

QString CT_ReaderItem::readerFilePath() const
{
    return m_reader->filepath();
}






