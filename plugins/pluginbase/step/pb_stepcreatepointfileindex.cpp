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
    manager.addItem(_inGrpArea, _inArea, tr("Emprise"));

    manager.addResult(_inResultReader,  tr("Fichiers"), tr(""), true);
    manager.setZeroOrMoreRootGroup(_inResultReader, _inZeroOrMoreRootGroupReader);
    manager.addGroup(_inZeroOrMoreRootGroupReader, _inGrpReader, tr("Groupe Fichier"));
    manager.addItem(_inGrpReader, _inReader, tr("Fichier de points"));
}

void PB_StepCreatePointFileIndex::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
}

void PB_StepCreatePointFileIndex::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
   postInputConfigDialog->addFileChoice(tr("Répertoire d'export des fichiers d'index (vide)"), CT_FileChoiceButton::OneExistingFolder, "", _outFolder);
}

void PB_StepCreatePointFileIndex::compute()
{
    // verify if all input readers are in the same format
    int filecount = 0;
    QString formatCode;
    QString basePath;
    for (const CT_IndexablePointFileHeader* readerHeader : _inReader.iterateInputs(_inResultReader))
    {
        CT_AbstractReader* reader = const_cast<CT_IndexablePointFileHeader*>(readerHeader)->reader();
        CT_IndexablePointsReader* ireader = const_cast<CT_IndexablePointFileHeader*>(readerHeader)->indexablePointReader();

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

    QList<AreaIndexFile*> indexFiles;
    // Create one index file per area2D
    for (const CT_AbstractAreaShape2D* areaItem : _inArea.iterateInputs(_inResultArea))
    {
        indexFiles.append(new AreaIndexFile(areaItem, folder, formatCode));
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


    int fileNumber = 0;
    // Loop on readers to add indices to index files
    for (const CT_IndexablePointFileHeader* readerHeader : _inReader.iterateInputs(_inResultReader))
    {
        CT_IndexablePointsReader* reader = const_cast<CT_IndexablePointFileHeader*>(readerHeader)->indexablePointReader();

        //if (isStopped()) {return;}

        for (AreaIndexFile* file : indexFiles)
        {
            qint64 lastIncludedIndex;
            QList<qint64> indicesAfterLastIncludedIndex;

            bool all;

            if (reader->getPointIndicesInside2DShape(file->_areaData, all, lastIncludedIndex, indicesAfterLastIncludedIndex))
            {
                file->writeFileIndices(readerHeader->fileName(), all, lastIncludedIndex, indicesAfterLastIncludedIndex);
            }
        }

        setProgress(100.0f*(float(++fileNumber) / float(filecount)));
    }

    qDeleteAll(indexFiles);
}

void PB_StepCreatePointFileIndex::AreaIndexFile::writeAreaShape(QDataStream &outStream, const CT_AreaShape2DData *areaData)
{
    Eigen::Vector3d min, max;
    areaData->getBoundingBox(min, max);

    outStream << min(0) << max(0) << min(1) << max(1);


    const CT_Box2DData* boxData = dynamic_cast<const CT_Box2DData*>(areaData);
    if (boxData != nullptr)
    {
        outStream << QString("BOX");
    }

    const CT_Circle2DData* circleData = dynamic_cast<const CT_Circle2DData*>(areaData);
    if (circleData != nullptr)
    {
        outStream << QString("CIRCLE");

        Eigen::Vector2d center = circleData->getCenter();
        outStream << center(0) << center(1) << circleData->getRadius();
    }

    const CT_Polygon2DData* polyData = dynamic_cast<const CT_Polygon2DData*>(areaData);
    if (polyData != nullptr)
    {
        outStream << QString("POLYGON");
        outStream << polyData->getVerticesNumber();

        for (Eigen::Vector2d vert : polyData->getVertices())
        {
            outStream << vert(0) << vert(1);
        }
    }
}

void PB_StepCreatePointFileIndex::AreaIndexFile::writeFileIndices(QString name, bool all, qint64 &lastIncludedIndex, QList<qint64> &indicesAfterLastIncludedIndex)
{
    if (_file.open(QIODevice::Append | QIODevice::Text))
    {
        QDataStream outStream(&_file);

        qint64 n = lastIncludedIndex + 1 + indicesAfterLastIncludedIndex.size();

        if (n > 0)
        {
            _pointCount += n;

            outStream << name;
            outStream << all;
            outStream << n;

            if (!all)
            {
                if (lastIncludedIndex > -1)
                {
                    for (qint64 i = 0 ; i <= lastIncludedIndex ; i++)
                    {
                        outStream << i;
                    }
                }

                for (qint64 i : indicesAfterLastIncludedIndex)
                {
                    outStream << i;
                }
            }

            _file.seek(0);
            outStream << _pointCount;
        }

        _file.close();
    }
}
