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

#include "pb_stepcreatepointfileindex.h"
#include "ct_log/ct_logmanager.h"

#include "ct_shape2ddata/ct_box2ddata.h"
#include "ct_shape2ddata/ct_circle2ddata.h"
#include "ct_shape2ddata/ct_polygon2ddata.h"

#include <QDir>

PB_StepCreatePointFileIndex::PB_StepCreatePointFileIndex() : SuperClass()
{
}

QString PB_StepCreatePointFileIndex::description() const
{
    return tr("Créer des fichiers d'index de points");
}

QString PB_StepCreatePointFileIndex::detailledDescription() const
{
    return tr("");
}

QString PB_StepCreatePointFileIndex::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>");
}

QString PB_StepCreatePointFileIndex::outputDescription() const
{
    return SuperClass::outputDescription() + tr("<br><br>");
}

QString PB_StepCreatePointFileIndex::detailsDescription() const
{
    return tr("");
}

QString PB_StepCreatePointFileIndex::URL() const
{
    //return tr("STEP URL HERE");
    return SuperClass::URL(); //by default URL of the plugin
}

CT_VirtualAbstractStep* PB_StepCreatePointFileIndex::createNewInstance() const
{
    return new PB_StepCreatePointFileIndex();
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepCreatePointFileIndex::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(_inResultArea,  tr("Emprises"), tr(""), true);
    manager.setZeroOrMoreRootGroup(_inResultArea, _inZeroOrMoreRootGroupArea);
    manager.addGroup(_inZeroOrMoreRootGroupArea, _inGrpArea, tr("Groupe Emprise"));
    manager.addItem(_inGrpArea, _inAreaSmall, tr("Emprise Réduite"));
    manager.addItem(_inGrpArea, _inArea, tr("Emprise Complète"));
    manager.addItemAttribute(_inArea, _inID, CT_AbstractCategory::DATA_VALUE, tr("Identifiant"));

    manager.addResult(_inResultReader,  tr("Fichiers"), tr(""), true);
    manager.setZeroOrMoreRootGroup(_inResultReader, _inZeroOrMoreRootGroupReader);
    manager.addGroup(_inZeroOrMoreRootGroupReader, _inGrpReader, tr("Groupe Fichier"));
    manager.addItem(_inGrpReader, _inReader, tr("Fichier de points"));
}

void PB_StepCreatePointFileIndex::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    Q_UNUSED(manager)
}

void PB_StepCreatePointFileIndex::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
   postInputConfigDialog->addFileChoice(tr("Répertoire d'export des fichiers d'index (vide)"), CT_FileChoiceButton::OneExistingFolder, "", _outFolder);
}

void PB_StepCreatePointFileIndex::compute()
{
    // verify if all input readers are in the same format
    PS_LOG->addInfoMessage(LogInterface::step, tr("Vérification des fichiers."));
    int filecount = 0;
    QString formatCode;
    QString basePath;
    size_t maxNPoints = 0;
    for (const CT_IndexablePointFileHeader* readerHeader : _inReader.iterateInputs(_inResultReader))
    {
        CT_AbstractReader* reader = const_cast<CT_IndexablePointFileHeader*>(readerHeader)->reader();
        CT_IndexablePointsReader* ireader = const_cast<CT_IndexablePointFileHeader*>(readerHeader)->indexablePointReader();

        size_t nPoints = readerHeader->nPoints();
        if (nPoints > maxNPoints) {maxNPoints = nPoints;}

        ++filecount;

        if (formatCode.isEmpty())
        {
            formatCode = ireader->getFormatCode();
            basePath = QFileInfo(reader->filepath()).path();
        } else {
            if (formatCode != ireader->getFormatCode())
            {
                PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr("Tous les fichiers d'entrée n'ont pas le même format. Arrêt de l'indexation."));
                return;
            }

            if (basePath != QFileInfo(reader->filepath()).path())
            {
                PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr("Tous les fichiers d'entrée ne sont pas dans le même dossier. Arrêt de l'indexation."));
                return;
            }
        }
    }

    bool bit64 = (maxNPoints >= std::numeric_limits<qint32>::max());

    if (_outFolder.isEmpty())
    {
        PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr("Repertoire d'export non choisi. Arrêt de l'indexation."));
        return;
    }

    QString folder = _outFolder.first();
    QDir folderDir(folder);

    if (!folderDir.exists())
    {
        PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr("Repertoire d'export inexistant. Arrêt de l'indexation."));
        return;
    }

    PS_LOG->addInfoMessage(LogInterface::step, tr("Initialisation des fichiers d'index."));
    QList<AreaIndexFile*> indexFiles;
    // Create one index file per area2D
    for (const CT_StandardItemGroup* group : _inGrpArea.iterateInputs(_inResultArea))
    {
        const CT_AbstractAreaShape2D* areaItem = group->singularItem(_inArea);
        const CT_AbstractAreaShape2D* areaSmallItem = group->singularItem(_inAreaSmall);

        if (areaItem != nullptr)
        {
            QString id = areaItem->displayableName();
            const CT_AbstractItemAttribute* attID = areaItem->itemAttribute(_inID);
            if (attID != nullptr)
            {
                bool ok;
                id = attID->toString(areaItem, &ok);
                if (!ok) {id = areaItem->displayableName();}

                QFileInfo info(id);
                id = info.baseName();
            }

            if (areaSmallItem == nullptr) {areaSmallItem = areaItem;}
            indexFiles.append(new AreaIndexFile(id, bit64, areaItem, areaSmallItem, folder, formatCode));
        }
    }

    // Write side File containing source path
    QString sideFileName = folder + "/" + "sourcePath.txt";
    QFile fp(sideFileName);

    if(fp.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream(&fp);

        stream << basePath;
        fp.close();
    }


    PS_LOG->addInfoMessage(LogInterface::step, tr("Création des fichiers d'index (%1 fichiers à analyser)").arg(filecount));
    int fileNumber = 1;
    // Loop on readers to add indices to index files
    for (const CT_IndexablePointFileHeader* readerHeader : _inReader.iterateInputs(_inResultReader))
    {       
        PS_LOG->addInfoMessage(LogInterface::step, tr("Analyse du fichier %1 (%2/%3)").arg(readerHeader->fileName()).arg(fileNumber).arg(filecount));

        CT_IndexablePointsReader* ireader = const_cast<CT_IndexablePointFileHeader*>(readerHeader)->indexablePointReader();

        Eigen::Vector3d min, max;
        readerHeader->boundingBox(min, max);

        //if (isStopped()) {return;}

        QList<CT_IndexablePointsReader::CandidateShape2D> candidateShapes;
        QMap<const CT_AreaShape2DData*, AreaIndexFile*> corresp;

        for (AreaIndexFile* file : indexFiles)
        {
            qint64 lastIncludedIndex;
            std::list<qint64> indicesAfterLastIncludedIndex;
            bool all;

            Eigen::Vector3d minArea, maxArea;
            file->_areaData->getBoundingBox(minArea, maxArea);

            // If area2D is a 2DBox, it is possible to check if all data is included in area2D directly from extent
            const CT_Box2DData* boxData = dynamic_cast<const CT_Box2DData*>(file->_areaData);
            if (boxData != nullptr)
            {
                if (min(0) >= minArea(0) && max(0) <= maxArea(0) && min(1) >= minArea(1) && max(1) <= maxArea(1))
                {
                    all = true;
                    lastIncludedIndex = readerHeader->nPoints() - 1;
                    file->writeFileIndices(readerHeader->fileName(), bit64, all, lastIncludedIndex, indicesAfterLastIncludedIndex);
                }
            }

            // If bounding boxes not overlapping => no index
            if (minArea(0) <= max(0) && minArea(1) <= max(1) && maxArea(0) >= min(0) && maxArea(1) >= min(1))
            {
                candidateShapes.append(CT_IndexablePointsReader::CandidateShape2D(file->_areaData));
                corresp.insert(file->_areaData, file);
            }
        }

        if (candidateShapes.size() > 0)
        {
            ireader->getPointIndicesInside2DShape(candidateShapes);
            for (CT_IndexablePointsReader::CandidateShape2D& sh : candidateShapes)
            {
                if (sh._asPointsInside)
                {
                    AreaIndexFile* file = corresp.value(sh._area2D);
                    if (file != nullptr)
                    {
                        file->writeFileIndices(readerHeader->fileName(), bit64, sh._all, sh._lastIncludedIndex, sh._indicesAfterLastIncludedIndex);
                    }
                }
            }
        }

        setProgress(100.0f*(float(fileNumber++) / float(filecount)));
    }

    qDeleteAll(indexFiles);
}

void PB_StepCreatePointFileIndex::AreaIndexFile::writeExtent(QDataStream &outStream, const CT_AreaShape2DData *areaData, const CT_AreaShape2DData *areaSmallData)
{
    Eigen::Vector3d min1, max1, min2, max2;
    areaData->getBoundingBox(min1, max1);
    areaSmallData->getBoundingBox(min2, max2);

    if (min2(0) < min1(0)) {min1(0) = min2(0);}
    if (min2(1) < min1(1)) {min1(1) = min2(1);}

    if (max2(0) > max1(0)) {max1(0) = max2(0);}
    if (max2(1) > max1(1)) {max1(1) = max2(1);}

    outStream << min1(0) << max1(0) << min1(1) << max1(1);
}


void PB_StepCreatePointFileIndex::AreaIndexFile::writeAreaShape(QDataStream &outStream, const CT_AreaShape2DData *areaData)
{
    const CT_Circle2DData* circleData = dynamic_cast<const CT_Circle2DData*>(areaData);
    const CT_Polygon2DData* polyData = dynamic_cast<const CT_Polygon2DData*>(areaData);

     if (circleData != nullptr)
    {
        outStream << QString("CIRCLE");

        Eigen::Vector2d center = circleData->getCenter();
        outStream << center(0) << center(1) << circleData->getRadius();
    } else if (polyData != nullptr)
    {
        outStream << QString("POLYGON");
        outStream << polyData->getVerticesNumber();

        for (Eigen::Vector2d vert : polyData->getVertices())
        {
            outStream << vert(0) << vert(1);
        }
    } else {
        outStream << QString("BOX");
        Eigen::Vector3d min, max;
        areaData->getBoundingBox(min, max);
        outStream << min(0) << min(1) << max(0) << max(1);
    }

}

void PB_StepCreatePointFileIndex::AreaIndexFile::writeFileIndices(QString name, bool bit64, bool all, qint64 &lastIncludedIndex, std::list<qint64> &indicesAfterLastIncludedIndex)
{
    if (_file.open(QIODevice::Append))
    {
        QDataStream outStream(&_file);

        qint64 n = lastIncludedIndex + 1 + indicesAfterLastIncludedIndex.size();

        if (n > 0)
        {
            outStream << name;
            outStream << all;
            outStream << n;

            if (!all)
            {
                if (lastIncludedIndex > -1)
                {
                    for (qint64 i = 0 ; i <= lastIncludedIndex ; i++)
                    {
                        if (bit64)
                        {
                            outStream << i;
                        } else {
                            qint32 i32 = qint32(i);
                            outStream << i32;
                        }
                    }
                }

                for (qint64 i : indicesAfterLastIncludedIndex)
                {
                    if (bit64)
                    {
                        outStream << i;
                    } else {
                        qint32 i32 = qint32(i);
                        outStream << i32;
                    }
                }
            }
        }
        _file.close();
    }
}
