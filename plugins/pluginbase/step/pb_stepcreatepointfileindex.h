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
#include "ct_itemdrawable/ct_indexablepointfileheader.h"
#include "ct_reader/extensions/ct_indexablepointsreader.h"

#include <QFile>
#include <QDataStream>
#include <QTextStream>


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
        AreaIndexFile(QString id, const CT_AbstractAreaShape2D* areaItem, const CT_AbstractAreaShape2D* areaSmallItem, QString outFolder, QString fileFormat)
        {
            _areaItem = areaItem;
            _areaSmallItem = areaSmallItem;

            QString path = outFolder + "/" + id + ".cti";
            _path = path;

            _areaData = dynamic_cast<const CT_AreaShape2DData*>(areaItem->getPointerData());
            _areaSmallData = dynamic_cast<const CT_AreaShape2DData*>(_areaSmallItem->getPointerData());

            _file.setFileName(path);

            if (_file.open(QIODevice :: WriteOnly))
            {
                QDataStream outStream(&_file);

                outStream << fileFormat;

                writeExtent(outStream, _areaData, _areaSmallData);
                writeAreaShape(outStream, _areaData);
                writeAreaShape(outStream, _areaSmallData);

                _file.close();
            }
        }

        const CT_AbstractAreaShape2D*   _areaItem;
        const CT_AbstractAreaShape2D*   _areaSmallItem;
        const CT_AreaShape2DData*       _areaData;
        const CT_AreaShape2DData*       _areaSmallData;
        QFile                           _file;
        QString                         _path;

        void writeExtent(QDataStream &outStream, const CT_AreaShape2DData *areaData, const CT_AreaShape2DData *areaSmallData);
        void writeAreaShape(QDataStream& outStream, const CT_AreaShape2DData *areaData);
        void writeFileIndices(QString name, bool all, qint64 &lastIncludedIndex, std::list<qint64> &indicesAfterLastIncludedIndex);

    };

    QStringList                                                                         _outFolder;

    CT_HandleInResultGroup<>                                                            _inResultArea;
    CT_HandleInStdZeroOrMoreGroup                                                       _inZeroOrMoreRootGroupArea;
    CT_HandleInStdGroup<>                                                               _inGrpArea;
    CT_HandleInSingularItem<CT_AbstractAreaShape2D>                                     _inArea;
    CT_HandleInStdItemAttribute<CT_AbstractCategory::ANY>                               _inID;
    CT_HandleInSingularItem<CT_AbstractAreaShape2D, 0, 1>                               _inAreaSmall;

    CT_HandleInResultGroup<>                                                            _inResultReader;
    CT_HandleInStdZeroOrMoreGroup                                                       _inZeroOrMoreRootGroupReader;
    CT_HandleInStdGroup<>                                                               _inGrpReader;
    CT_HandleInSingularItem<CT_IndexablePointFileHeader>                                _inReader;



};

#endif // ONF_STEPCREATEPOINTFILEINDEX_H
