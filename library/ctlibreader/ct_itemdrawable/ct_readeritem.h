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

#ifndef CT_READERITEM_H
#define CT_READERITEM_H

#include "ctlibreader_global.h"
#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"
#include "ct_reader/abstract/ct_abstractreader.h"

/**
  * Représente un header de fichier
  */
class CTLIBREADER_EXPORT CT_ReaderItem : public CT_AbstractSingularItemDrawable
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_ReaderItem, CT_AbstractSingularItemDrawable, Reader item)

    using SuperClass = CT_AbstractSingularItemDrawable;

public:
    CT_ReaderItem();
    CT_ReaderItem(CT_AbstractReader* reader,
                  bool mustAutoDeleteReader);

    /**
     * @brief Copy constructor.
     *
     *        What is copied :
     *          - Pointer of the result and model of the original item.
     *          - Unique ID
     *          - Pointer of base and alternative draw manager
     *          - Displayable name
     *          - Center coordinates
     *          - Default Color
     *          - Reader is full copied
     *
     *        What is initialized differently :
     *          - Parent is set to NULL
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     *          - Auto delete reader is set to true
     */
    CT_ReaderItem(const CT_ReaderItem& other);
    ~CT_ReaderItem() override;

    /**
     * @brief Returns the reader
     */
    CT_AbstractReader* reader();

    /**
     * @brief Returns true if we must auto delete the reader
     */
    bool mustAutoDeleteReader() const;

    /**
     * @brief Returns the filepath used by the reader
     */
    QString readerFilePath() const;

    /**
     * @brief Returns the displayable name used by the reader
     */
    QString readerDisplayableName() const;

    CT_ITEM_COPY_IMP(CT_ReaderItem)

private:
    CT_AbstractReader*  m_reader;
    bool                m_mustAutoDeleteReader;

    CT_DEFAULT_IA_BEGIN(CT_ReaderItem)
    CT_DEFAULT_IA_V2(CT_ReaderItem, CT_AbstractCategory::staticInitDataDisplayableName(), &CT_ReaderItem::readerDisplayableName, QObject::tr("ReaderType"))
    CT_DEFAULT_IA_V2(CT_ReaderItem, CT_AbstractCategory::staticInitDataFileName(), &CT_ReaderItem::readerFilePath, QObject::tr("FilePath"))
    CT_DEFAULT_IA_END(CT_ReaderItem)

};

#endif // CT_READERITEM_H
