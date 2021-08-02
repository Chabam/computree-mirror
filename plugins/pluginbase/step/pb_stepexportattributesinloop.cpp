#include "pb_stepexportattributesinloop.h"

#include "ct_itemdrawable/ct_image2d.h"
#include "exporters/grid2d/pb_grid2dexporter.h"
#include "ct_log/ct_logmanager.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <math.h>
#include <QDir>

#define DEF_ESRI_SHP "GDAL ESRI Shapefile"

#define EPSILON_LIMITS 0.0000001
#define DEF_NA -9999999

PB_StepExportAttributesInLoop::PB_StepExportAttributesInLoop() : SuperClass()
{
    _asciiExport = true;
    _vectorExport = false;
    _rasterExport = false;
    _exportInLoop = true;

    _vectorPrefix = "";
    _rasterPrefix = "";

    _subFolders = true;

    mGrid2DExporterUniqueName = PB_Grid2DExporter().uniqueName();
    _rasterDriverName = mGrid2DExporterUniqueName;

#ifdef USE_GDAL
    _vectorDriverName = DEF_ESRI_SHP;
    GDALDriverManager* const driverManager = GetGDALDriverManager();

    const int count = driverManager->GetDriverCount();

    for(int i = 0 ; i < count ; ++i)
    {
        GDALDriver *driver = driverManager->GetDriver(i);
        QString name = CT_GdalTools::staticGdalDriverName(driver);

        if(!name.isEmpty() && driver->GetMetadataItem(GDAL_DCAP_RASTER) != nullptr && driver->GetMetadataItem(GDAL_DCAP_CREATE) != nullptr) {
            _gdalRasterDrivers.insert(name, driver);
        }

        if(!name.isEmpty() && driver->GetMetadataItem(GDAL_DCAP_VECTOR) != nullptr && driver->GetMetadataItem(GDAL_DCAP_CREATE) != nullptr) {
            _gdalVectorDrivers.insert(name, driver);
        }
    }
#endif
}

QString PB_StepExportAttributesInLoop::description() const
{
    return tr("Export d'attributs dans une boucle");
}

CT_VirtualAbstractStep* PB_StepExportAttributesInLoop::createNewInstance() const
{
    return new PB_StepExportAttributesInLoop();
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepExportAttributesInLoop::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    preInputConfigDialog->addBool(tr("Activer export ASCII tabulaire (1 fichier en tout)"), "", tr("Activer"), _asciiExport);

    preInputConfigDialog->addEmpty();
    preInputConfigDialog->addBool(tr("Activer export raster (1 fichier / tour / métrique)"), "", tr("Activer"), _rasterExport);
    preInputConfigDialog->addTitle(tr("L'export raster nécessite une grille de placettes (désactiver si pas de résultat valide)"));

#ifdef USE_GDAL
    preInputConfigDialog->addEmpty();
    preInputConfigDialog->addBool(tr("Activer export vectoriel (1 fichier / tour)"), "", tr("Activer"), _vectorExport);
#endif

    preInputConfigDialog->addEmpty();
    preInputConfigDialog->addBool(tr("Export dans une boucle (cas normal)"), "", tr("Activer"), _exportInLoop);

}

void PB_StepExportAttributesInLoop::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Résultat"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroupMain);

    if (_rasterExport)
        manager.addItem(mInGroupMain, mInPlotListInGrid, tr("Grille de placettes"));

    manager.addGroup(mInGroupMain, mInGroupChild);

    manager.addItem(mInGroupChild, mInItemWithXY, tr("Item (avec XY)"));
    manager.addItemAttribute(mInItemWithXY, mInItemAttributeX, CT_AbstractCategory::DATA_X, tr("X"));
    manager.addItemAttribute(mInItemWithXY, mInItemAttributeY, CT_AbstractCategory::DATA_Y, tr("Y"));
    manager.addItemAttribute(mInItemWithXY, mInItemAttributeXY, CT_AbstractCategory::DATA_VALUE, tr("Attribut Item (avec XY)"));

    manager.addItem(mInGroupChild, mInItemWithAttribute, tr("Item"));
    manager.addItemAttribute(mInItemWithAttribute, mInItemAttribute, CT_AbstractCategory::DATA_VALUE, tr("Attribut Item"));

    if(_exportInLoop)
    {
        manager.addResult(mInResultCounter, tr("Résultat compteur"), QString(), true);
        manager.setRootGroup(mInResultCounter, mInGroupCounter);
        manager.addItem(mInGroupCounter, mInLoopCounter, tr("Compteur"));
    }
}

void PB_StepExportAttributesInLoop::declareOutputModels(CT_StepOutModelStructureManager&)
{
}

void PB_StepExportAttributesInLoop::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
    if (_asciiExport)
    {
        postInputConfigDialog->addTitle(tr("Export ASCII tabulaire (1 fichier en tout)"));
        postInputConfigDialog->addFileChoice(tr("Choix du fichier"), CT_FileChoiceButton::OneNewFile, tr("Fichier texte (*.txt)"), _outASCIIFileName);
    }

    if (_rasterExport)
    {
        QStringList driversR;
        driversR.append(mGrid2DExporterUniqueName);

#ifdef USE_GDAL
        driversR.append(_gdalRasterDrivers.keys());
#endif

        postInputConfigDialog->addEmpty();
        postInputConfigDialog->addTitle(tr("Export raster (1 fichier / tour / métrique)"));
        postInputConfigDialog->addString(tr("Prefixe pour les fichiers exportés"), "", _rasterPrefix);
        postInputConfigDialog->addStringChoice(tr("Choix du format d'export"), "", driversR, _rasterDriverName);
        postInputConfigDialog->addFileChoice(tr("Répertoire d'export (vide de préférence)"), CT_FileChoiceButton::OneExistingFolder, "", _outRasterFolder);
        postInputConfigDialog->addBool(tr("Créer un sous-dossier par métrique"), "", "", _subFolders);
    }

#ifdef USE_GDAL
    if (_vectorExport)
    {
        QStringList driversV;
        driversV.append(_gdalVectorDrivers.keys());

        postInputConfigDialog->addEmpty();
        postInputConfigDialog->addTitle(tr("Export vectoriel (1 fichier / tour)"));
        postInputConfigDialog->addString(tr("Prefixe pour les fichiers exportés"), "", _vectorPrefix);
        postInputConfigDialog->addStringChoice(tr("Choix du format d'export"), "", driversV, _vectorDriverName);
        postInputConfigDialog->addFileChoice(tr("Répertoire d'export (vide de préférence)"), CT_FileChoiceButton::OneExistingFolder, "", _outVectorFolder);
    }
#endif
}

void PB_StepExportAttributesInLoop::compute()
{

    QScopedPointer<QFile> fileASCII;
    QScopedPointer<QTextStream> streamASCII;
    bool firstTurnFromCounter = true;

    const QString exportBaseName = createExportBaseName(firstTurnFromCounter);

    if (firstTurnFromCounter)
    {
        _names.clear();
        _modelsKeys.clear();
        computeModelsKeysAndNamesAndOgrTypes();

        if(isStopped())
            return;

        createFieldsNamesFileForVectorsIfNecessary();
    }


    if(isStopped() || !exportInAsciiIfNecessary(fileASCII, streamASCII, firstTurnFromCounter))
        return;

#ifdef USE_GDAL
    QScopedPointer<GDALDataset, GDalDatasetScopedPointerCustomDeleter> vectorDataSet;
    OGRLayer* vectorLayer = nullptr;

    if(isStopped())
        return;

    preExportVectorIfNecessary(exportBaseName, vectorDataSet, vectorLayer);
#endif

    // IN results browsing
    for(const CT_StandardItemGroup* grpMain : mInGroupMain.iterateInputs(mInResult))
    {
        if(isStopped())
            return;

        RastersMap rasters;
        if (_rasterExport)
        {
            const CT_PlotListInGrid* plotListInGrid = grpMain->singularItem(mInPlotListInGrid);

            if (plotListInGrid != nullptr)
            {
                Eigen::Vector2d min, max;
                plotListInGrid->getBoundingBox2D(min, max);
                const double resolution = plotListInGrid->getSpacing();

                for (int i = 0 ; i < _modelsKeysWithoutXOrYAttribute.size() ; i++)
                {
                    const QString key = _modelsKeysWithoutXOrYAttribute.at(i);
                    rasters.insert(key, CT_Image2D<double>::createImage2DFromXYCoords(min(0), min(1), max(0) - EPSILON_LIMITS, max(1) - EPSILON_LIMITS, resolution, 0, DEF_NA, DEF_NA));
                }
            }
        }

        auto iteratorGroupsChild = grpMain->groups(mInGroupChild);

        for(const CT_StandardItemGroup* grp : iteratorGroupsChild)
        {
            if(isStopped())
                return;

            QMap<QString, QPair<const CT_AbstractSingularItemDrawable*, const CT_AbstractItemAttribute*> > indexedAttributes;

            double x = std::numeric_limits<double>::max();
            double y = std::numeric_limits<double>::max();

            const CT_AbstractSingularItemDrawable* itemXY = grp->singularItem(mInItemWithXY);

            if (itemXY != nullptr)
            {
                const CT_AbstractItemAttribute* attX = itemXY->itemAttribute(mInItemAttributeX);
                const CT_AbstractItemAttribute* attY = itemXY->itemAttribute(mInItemAttributeY);

                if (attX != nullptr) {x = attX->toDouble(itemXY, nullptr); addToIndexedAttributesCollection(itemXY, attX, indexedAttributes); }
                if (attY != nullptr) {y = attY->toDouble(itemXY, nullptr); addToIndexedAttributesCollection(itemXY, attY, indexedAttributes);}

                auto iteratorAttributesXY = itemXY->itemAttributesByHandle(mInItemAttributeXY);

                for(const CT_AbstractItemAttribute* attr : iteratorAttributesXY)
                {
                    addToIndexedAttributesCollection(itemXY, attr, indexedAttributes);
                }
            }

            auto iteratorItemWithAttribute = grp->singularItems(mInItemWithAttribute);

            for(const CT_AbstractSingularItemDrawable* item : iteratorItemWithAttribute)
            {
                auto iteratorAttributes = item->itemAttributesByHandle(mInItemAttribute);

                for(const CT_AbstractItemAttribute* attr : iteratorAttributes)
                {
                    addToIndexedAttributesCollection(item, attr, indexedAttributes);
                }
            }

            const bool hasMetricsToExport = !(indexedAttributes.isEmpty());

#ifdef USE_GDAL
            OGRFeature *vectorFeature = nullptr;
            if (hasMetricsToExport && (vectorLayer != nullptr))
            {
                vectorFeature = OGRFeature::CreateFeature(vectorLayer->GetLayerDefn());
                OGRPoint pt;
                pt.setX(x);
                pt.setY(y);
                vectorFeature->SetGeometry(&pt);

                qDebug() << "01";
            }
#endif

            if (!streamASCII.isNull())
            {
                (*streamASCII.data()) << exportBaseName << "\t";
            }

            for (int i = 0 ; i < _modelsKeys.size() ; i++)
            {
                const QString key = _modelsKeys.at(i);

                const auto pair = indexedAttributes.value(key);

                if(pair.first == nullptr)
                    continue;

                if (hasMetricsToExport && !streamASCII.isNull())
                {
                    (*streamASCII.data()) << pair.second->toString(pair.first, nullptr);
                }

#ifdef USE_GDAL
                if (vectorLayer != nullptr)
                {
                    const std::string fieldName = _shortNames.value(key).toStdString();
                    qDebug() << "02";

                    if (_ogrTypes.value(key) == OFTBinary)
                    {
                        vectorFeature->SetField(fieldName.data(), pair.second->toInt(pair.first, nullptr));
                        qDebug() << "03";
                    }
                    else if (_ogrTypes.value(key) == OFTString)
                    {
                        const std::string text = replaceAccentCharacters(pair.second->toString(pair.first, nullptr)).toStdString();
                        vectorFeature->SetField(fieldName.data(), text.data());
                        qDebug() << "04";
                    }
                    else if (_ogrTypes.value(key) == OFTInteger)
                    {
                        vectorFeature->SetField(fieldName.data(), pair.second->toInt(pair.first, nullptr));
                        qDebug() << "05";
//                        }
//                        else if (_ogrTypes.value(key) == OFTInteger64)
//                        {
//                            vectorFeature->SetField(fieldName.data(), pair.second->toInt(pair.first, nullptr));
                    }
                    else
                    {
                        vectorFeature->SetField(fieldName.data(), pair.second->toDouble(pair.first, nullptr));
                        qDebug() << "06";
                    }
                    qDebug() << "07";

                }
#endif

                CT_Image2D<double>* raster = rasters.value(key, nullptr);

                if (_rasterExport && raster != nullptr)
                {
                    const double val = pair.second->toDouble(pair.first, nullptr);

                    if (std::isnan(val))
                        raster->setValueAtCoords(x, y, DEF_NA);
                    else
                        raster->setValueAtCoords(x, y, val);
                }
                if (hasMetricsToExport && !streamASCII.isNull())
                {
                    if(i < _modelsKeys.size() - 1) {(*streamASCII.data()) << "\t";} else {(*streamASCII.data()) << "\n";}
                }
            }

#ifdef USE_GDAL
            if(vectorLayer != nullptr)
            {
                if( vectorLayer->CreateFeature(vectorFeature) != OGRERR_NONE )
                {
                    //  erreur
                }

                OGRFeature::DestroyFeature(vectorFeature);
            }
#endif
            qDebug() << "08";

        }

        if (_rasterExport)
        {
            QMapIterator<QString, CT_Image2D<double>*> itRaster(rasters);
            while (itRaster.hasNext())
            {
                itRaster.next();
                QString key = itRaster.key();
                QList<const CT_AbstractImage2D* > rasterList;
                rasterList.append(itRaster.value());

                QString metricName = _names.value(key);
                QString fileName = QString("%1/%2%3_%4").arg(_outRasterFolder.first()).arg(_rasterPrefix).arg(metricName).arg(exportBaseName);

                if (_subFolders) {
                    QDir dir(QString("%1/%2%3").arg(_outRasterFolder.first()).arg(_rasterPrefix).arg(metricName));
                    if (!dir.exists()) {dir.mkdir(".");}

                    fileName = QString("%1/%2%3/%2%3_%4").arg(_outRasterFolder.first()).arg(_rasterPrefix).arg(metricName).arg(exportBaseName);
                }

                if (_rasterDriverName == mGrid2DExporterUniqueName)
                {
                    PB_Grid2DExporter exporter;

                    if (exporter.setFilePath(fileName))
                    {
                        exporter.setItemsToExport(rasterList);
                        exporter.exportToFile();
                    }

                } else {
#ifdef USE_GDAL
                    GDALDriver* driver = _gdalRasterDrivers.value(_rasterDriverName, nullptr);

                    if (driver != nullptr)
                    {
                        PB_GDALExporter exporter(driver);

                        if (exporter.setFilePath(fileName))
                        {
                            exporter.setRastersToExport(rasterList);
                            exporter.exportToFile();
                        }
                    }
#endif
                }
            }
        }

    }
}

QString PB_StepExportAttributesInLoop::createExportBaseName(bool& first) const
{
    if(_exportInLoop)
    {
        for(const CT_LoopCounter* counter : mInLoopCounter.iterateInputs(mInResultCounter))
        {
            if (counter->currentTurn() > 1)
                first = false;

            QFileInfo fileinfo(counter->turnName());

            if (fileinfo.exists())
                return fileinfo.baseName();

            return counter->turnName();
        }
    }

    return QString("noName");
}

void PB_StepExportAttributesInLoop::computeModelsKeysAndNamesAndOgrTypes()
{
    // Iterate over models and not over items because it can be possible to have a model that doesn't have
    // an item at least.
    computeModelsKeysAndNamesAndOgrTypes(mInItemAttributeXY, true);
    computeModelsKeysAndNamesAndOgrTypes(mInItemAttributeX, false);
    computeModelsKeysAndNamesAndOgrTypes(mInItemAttributeY, false);
    computeModelsKeysAndNamesAndOgrTypes(mInItemAttribute, true);

    replaceBadCharacters(_names);
    std::sort(_modelsKeys.begin(), _modelsKeys.end());
}

void PB_StepExportAttributesInLoop::computeModelsKeysAndNamesAndOgrTypesForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractItemAttributeModel* attModel, bool isNotXOrYAttribute)
{
    const QString attrDN = attModel->displayableName();

    const QString key = computeKeyForModels(itemModel, attModel);

    if(!_modelsKeys.contains(key))
    {
        _modelsKeys.append(key);

        if(isNotXOrYAttribute)
            _modelsKeysWithoutXOrYAttribute.append(key);

        _names.insert(key, attrDN);

#ifdef USE_GDAL
        if (_vectorExport && !_outVectorFolder.isEmpty())
        {
            const CT_AbstractCategory::ValueType type = CT_AbstractCategory::ValueType(attModel->itemAttribute()->itemAttributeToolForModel()->valueType());

            if      (type == CT_AbstractCategory::BOOLEAN) {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::STRING)  {_ogrTypes.insert(key, OFTString);}
            else if (type == CT_AbstractCategory::STRING)  {_ogrTypes.insert(key, OFTString);}
            else if (type == CT_AbstractCategory::INT8)    {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::UINT8)   {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::INT16)   {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::UINT16)  {_ogrTypes.insert(key, OFTInteger);}
            else if (type == CT_AbstractCategory::INT32)   {_ogrTypes.insert(key, OFTInteger);}
            //                else if (type == CT_AbstractCategory::UINT32)  {ogrTypes.insert(key, OFTInteger64);}
            //                else if (type == CT_AbstractCategory::INT64)   {ogrTypes.insert(key, OFTInteger64);}
            //                else if (type == CT_AbstractCategory::INT32)   {ogrTypes.insert(key, OFTInteger64);}
            else                                           {_ogrTypes.insert(key, OFTReal);}
        }
#endif
    }
}

QString PB_StepExportAttributesInLoop::computeKeyForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractModel* attModel) const
{
    return QString("ITEM_%1_ATTR_%2").arg(size_t(itemModel->recursiveOriginalModel())).arg(size_t(attModel->recursiveOriginalModel()));
}

void PB_StepExportAttributesInLoop::createFieldsNamesFileForVectorsIfNecessary()
{
    if (_vectorExport && !_outVectorFolder.isEmpty())
    {
        _shortNames = computeShortNames(_names);

        QFile ffields(QString("%1/fields_names.txt").arg(_outVectorFolder.first()));
        QTextStream fstream(&ffields);
        if (ffields.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMapIterator<QString, QString> itF(_shortNames);
            while (itF.hasNext())
            {
                itF.next();
                QString key = itF.key();
                QString shortName = itF.value();
                QString longName = _names.value(key);
                fstream << shortName << "\t";
                fstream << longName << "\n";
            }
            ffields.close();
        }
    }
}

bool PB_StepExportAttributesInLoop::exportInAsciiIfNecessary(QScopedPointer<QFile>& fileASCII, QScopedPointer<QTextStream>& streamASCII, const bool firstTurnFromCounter)
{
    if (_asciiExport && !_outASCIIFileName.isEmpty())
    {
        fileASCII.reset(new QFile(_outASCIIFileName.first()));

        if (firstTurnFromCounter)
        {
            if (fileASCII->open(QIODevice::WriteOnly | QIODevice::Text))
            {
                streamASCII.reset(new QTextStream(fileASCII.data()));
                (*streamASCII) << "Name\t";

                for (int i = 0 ; i < _modelsKeys.size() ; i++)
                {
                    (*streamASCII) << _names.value(_modelsKeys.at(i));
                    if (i < _modelsKeys.size() - 1) {(*streamASCII) << "\t";} else {(*streamASCII) << "\n";}
                }

            } else {
                fileASCII.reset(nullptr);
                PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr(" : Impossible de créer le fichier d'export ASCII. Arrêt des traitements."));
                stop();
                return false;
            }
        } else {
            if (!fileASCII->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
            {
                fileASCII.reset(nullptr);
                PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr(" : Impossible d'ouvrir le fichier d'export ASCII. Arrêt des traitements."));
                stop();
                return false;
            }

            streamASCII.reset(new QTextStream(fileASCII.data()));
        }
    }

    return true;
}

#ifdef USE_GDAL
void PB_StepExportAttributesInLoop::preExportVectorIfNecessary(const QString& exportBaseName, QScopedPointer<GDALDataset, GDalDatasetScopedPointerCustomDeleter>& vectorDataSet, OGRLayer*& vectorLayer)
{
    GDALDriver* driverVector = _gdalVectorDrivers.value(_vectorDriverName, nullptr);

    if (_vectorExport && driverVector != nullptr && _outVectorFolder.size() > 0)
    {
        QString outFileName = (QString("%1/%2").arg(_outVectorFolder.first()).arg(exportBaseName));
        QStringList ext = CT_GdalTools::staticGdalDriverExtension(driverVector);
        if (ext.size() > 0)
        {
            outFileName.append(".");
            outFileName.append(ext.first());
        }

        vectorDataSet.reset(driverVector->Create(outFileName.toLatin1(), 0, 0, 0, GDT_Unknown, nullptr));

        if (vectorDataSet.isNull())
        {
            PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr(" : Impossible d'utiliser le format d'export Vectoriel choisi."));
            return;
        }

        vectorLayer = vectorDataSet->CreateLayer("point", nullptr, wkbPoint, nullptr);

        if (vectorLayer == nullptr)
        {
            vectorDataSet.reset(nullptr);
            PS_LOG->addErrorMessage(LogInterface::step, displayableCustomName() + tr(" : Impossible de créer la couche \"point\"."));
            return;
        }

        for (int i = 0 ; i < _modelsKeys.size() && !isStopped() ; i++)
        {
            QString key = _modelsKeys.at(i);
            if (_ogrTypes.contains(key))
            {
                OGRFieldType ogrType = _ogrTypes.value(key);

                const std::string fieldName = _shortNames.value(key).toStdString();

                OGRFieldDefn oField(fieldName.data(), ogrType );

                if (vectorLayer->CreateField( &oField ) != OGRERR_NONE)
                {
                    //  erreur
                }
            }
        }
    }
}

#endif

void PB_StepExportAttributesInLoop::addToIndexedAttributesCollection(const CT_AbstractSingularItemDrawable* item, const CT_AbstractItemAttribute* attribute, QMap<QString, QPair<const CT_AbstractSingularItemDrawable*, const CT_AbstractItemAttribute*> >& indexedAttributes) const
{
    indexedAttributes.insert(computeKeyForModels(item->model(), attribute->model()), qMakePair(item, attribute));
}

void PB_StepExportAttributesInLoop::replaceBadCharacters(QMap<QString, QString> &names) const
{
    QMutableMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        it.setValue(replaceBadCharacters(it.value()));
    }
}

QString PB_StepExportAttributesInLoop::replaceBadCharacters(const QString &name) const
{
        QString value = name;
        value.replace(QRegExp("[àáâãäå]"), "a");
        value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
        value.replace(QRegExp("[éèëê]"), "e");
        value.replace(QRegExp("[ÈÉÊË]"), "E");
        value.replace(QRegExp("[ìíîï]"), "i");
        value.replace(QRegExp("[ÌÍÎÏ]"), "I");
        value.replace(QRegExp("[òóôõöø]"), "o");
        value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
        value.replace(QRegExp("[ùúûü]"), "u");
        value.replace(QRegExp("[ÙÚÛÜ]"), "U");
        value.replace(QRegExp("[ñ]"), "n");
        value.replace(QRegExp("[Ñ]"), "N");
        value.replace(QRegExp("[ç]"), "c");
        value.replace(QRegExp("[Ç]"), "C");
        value.replace(QRegExp("[\\W]"), "_");
        return value;
}

QString PB_StepExportAttributesInLoop::replaceAccentCharacters(const QString &name) const
{
        QString value = name;
        value.replace(QRegExp("[àáâãäå]"), "a");
        value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
        value.replace(QRegExp("[éèëê]"), "e");
        value.replace(QRegExp("[ÈÉÊË]"), "E");
        value.replace(QRegExp("[ìíîï]"), "i");
        value.replace(QRegExp("[ÌÍÎÏ]"), "I");
        value.replace(QRegExp("[òóôõöø]"), "o");
        value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
        value.replace(QRegExp("[ùúûü]"), "u");
        value.replace(QRegExp("[ÙÚÛÜ]"), "U");
        value.replace(QRegExp("[ñ]"), "n");
        value.replace(QRegExp("[Ñ]"), "N");
        value.replace(QRegExp("[ç]"), "c");
        value.replace(QRegExp("[Ç]"), "C");
        return value;
}

QMap<QString, QString> PB_StepExportAttributesInLoop::computeShortNames(const QMap<QString, QString> &names) const
{
   QMap<QString, QString> shortNames;
   QList<QString> existing;

   QMapIterator<QString, QString> it(names);
   while (it.hasNext())
   {
       it.next();
       QString key = it.key();
       QString value = it.value();

       if (value.size() <= 10)
       {
           shortNames.insert(key, value);
           existing.append(value.toLower());
       } else {
           QString newValue = value.left(10);
           int cpt = 2;
           while (existing.contains(newValue.toLower()))
           {
               QString number = QVariant(cpt++).toString();
               newValue = QString("%1%2").arg(value.left(10 - number.length())).arg(number);
           }
           shortNames.insert(key, newValue);
           existing.append(newValue.toLower());
       }
   }

   return shortNames;
}
