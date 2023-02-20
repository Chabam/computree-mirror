/****************************************************************************
 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                         All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

 This file is part of PluginONF library.

 PluginONF is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginONF is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginONF.  If not, see <http://www.gnu.org/licenses/lgpl.html>.
*****************************************************************************/

#ifndef ONF_STEPCREATEPOINTFILEINDEX_H
#define ONF_STEPCREATEPOINTFILEINDEX_H

#include "ct_step/abstract/ct_abstractstep.h"

#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_reader/extensions/ct_indexablepointsreader.h"

#include <QFile>
#include <QDataStream>


class PB_StepCreatePointFileIndex: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:

    PB_StepCreatePointFileIndex();

    QString description() const override;
    QString detailledDescription() const override;
    QString inputDescription() const override;
    QString outputDescription() const override;
    QString detailsDescription() const override;

    QString URL() const override;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    struct AreaIndexFile
    {
        AreaIndexFile(const CT_AbstractAreaShape2D* areaItem, QString outFolder, QString fileFormat)
        {
            _pointCount = 0;
            _areaItem = areaItem;

            QString name = areaItem->name();
            QString path = outFolder + "/" + name + ".cti";

            _areaData = dynamic_cast<const CT_AreaShape2DData*>(areaItem->getPointerData());

            _file.setFileName(path);
            if (_file.open(QIODevice :: WriteOnly))
            {
                QDataStream outStream(&_file);
                outStream << _pointCount;
                outStream << fileFormat;

                writeAreaShape(outStream, _areaData);

                _file.close();
            }
        }

        const CT_AbstractAreaShape2D*   _areaItem;
        const CT_AreaShape2DData*       _areaData;
        QFile                           _file;
        quint64                         _pointCount;

        void writeAreaShape(QDataStream& outStream, const CT_AreaShape2DData *areaData);
        void writeFileIndices(QString name, bool all, size_t &lastIncludedIndex, QList<size_t> &indicesAfterLastIncludedIndex);

    };

    QStringList                                                                         _outFolder;

    CT_HandleInResultGroup<>                                                            _inResultArea;
    CT_HandleInStdZeroOrMoreGroup                                                       _inZeroOrMoreRootGroupArea;
    CT_HandleInStdGroup<>                                                               _inGrpArea;
    CT_HandleInSingularItem<CT_AbstractAreaShape2D>                                     _inArea;

    CT_HandleInResultGroup<>                                                            _inResultReader;
    CT_HandleInStdZeroOrMoreGroup                                                       _inZeroOrMoreRootGroupReader;
    CT_HandleInStdGroup<>                                                               _inGrpReader;
    CT_HandleInSingularItem<CT_ReaderItem>                                              _inReader;



};

#endif // ONF_STEPCREATEPOINTFILEINDEX_H
