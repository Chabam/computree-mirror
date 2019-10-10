#include "pb_stepexportattributesinloop.h"

#ifdef USE_OPENCV
#include "ct_itemdrawable/ct_image2d.h"
#include "exporters/grid2d/pb_grid2dexporter.h"
#endif

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <math.h>
#include <QDir>

#define EPSILON_LIMITS 0.0000001

PB_StepExportAttributesInLoop::PB_StepExportAttributesInLoop() : SuperClass()
{
    _asciiExport = true;
    _vectorExport = false;
    _rasterExport = false;
    _exportInLoop = true;

    _vectorPrefix = "";
    _rasterPrefix = "";

    _subFolders = true;

    _rasterDriverName = DEF_ESRI_ASCII_Grid;
    _vectorDriverName = DEF_ESRI_SHP;

#ifdef USE_GDAL
    //GDALAllRegister();
    GDALDriverManager* const driverManager = GetGDALDriverManager();

    //CPLSetConfigOption("GDAL_DATA", "C:/xDONNEES/Programmation/computree64/computreeDependencies/gdal_bin/data");

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

void PB_StepExportAttributesInLoop::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_InResultModelGroup *resIn = createNewInResultModel(DEFin_res, tr("Résultat"));
    resIn->setZeroOrMoreRootGroup();
    resIn->addStdGroupModel("", DEFin_grpMain, CT_StandardItemGroup::staticGetType(), tr("Groupe"));

#ifdef USE_OPENCV
    if (_rasterExport)
    {
        resIn->addStdItemModel(DEFin_grpMain, DEFin_plotListInGrid, CT_PlotListInGrid::staticGetType(), tr("Grille de placettes"), "", CT_InAbstractModel::C_ChooseOneIfMultiple, CT_InAbstractModel::F_IsObligatory);
    }
#endif

    resIn->addStdGroupModel(DEFin_grpMain, DEFin_grp, CT_StandardItemGroup::staticGetType(), tr("Groupe"));
    resIn->addStdItemModel(DEFin_grp, DEFin_itemWithXY, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item (avec XY)"), "", CT_InAbstractModel::C_ChooseOneIfMultiple);
    resIn->addStdItemAttributeModel(DEFin_itemWithXY, DEFin_Xattribute, QList<QString>() << CT_AbstractCategory::DATA_X, CT_AbstractCategory::DOUBLE, tr("X"), "", CT_InAbstractModel::C_ChooseOneIfMultiple);
    resIn->addStdItemAttributeModel(DEFin_itemWithXY, DEFin_Yattribute, QList<QString>() << CT_AbstractCategory::DATA_Y, CT_AbstractCategory::DOUBLE, tr("Y"), "", CT_InAbstractModel::C_ChooseOneIfMultiple);
    resIn->addStdItemAttributeModel(DEFin_itemWithXY, DEFin_attributeInItemXY, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Attribut Item (avec XY)"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple, CT_InAbstractModel::F_IsOptional);

    resIn->addStdItemModel(DEFin_grp, DEFin_itemWithAttribute, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple, CT_InAbstractModel::F_IsOptional);
    resIn->addStdItemAttributeModel(DEFin_itemWithAttribute, DEFin_attribute, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Attribut Item"), "", CT_InAbstractModel::C_ChooseMultipleIfMultiple, CT_InAbstractModel::F_IsOptional);

    if (_exportInLoop)
    {
        CT_InResultModelGroup* res_counter = createNewInResultModel(DEF_inResultCounter, tr("Résultat compteur"), "", true);
        res_counter->setStdRootGroup(DEF_inGroupCounter);
        res_counter->addStdItemModel(DEF_inGroupCounter, DEF_inCounter, CT_LoopCounter::staticGetType(), tr("Compteur"));
        res_counter->setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(0);
    }
}

void PB_StepExportAttributesInLoop::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    //createNewOutResultModel(DEFout_res, tr("Resultat vide"));
}

void PB_StepExportAttributesInLoop::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    preInputConfigDialog->addBool(tr("Activer export ASCII tabulaire (1 fichier en tout)"), "", tr("Activer"), _asciiExport);

#ifdef USE_OPENCV
    preInputConfigDialog->addEmpty();
    preInputConfigDialog->addBool(tr("Activer export raster (1 fichier / tour / métrique)"), "", tr("Activer"), _rasterExport);
    preInputConfigDialog->addTitle(tr("L'export raster nécessite une grille de placettes (désactiver si pas de résultat valide)"));
#endif

#ifdef USE_GDAL
    preInputConfigDialog->addEmpty();
    preInputConfigDialog->addBool(tr("Activer export vectoriel (1 fichier / tour)"), "", tr("Activer"), _vectorExport);
#endif

    preInputConfigDialog->addEmpty();
    preInputConfigDialog->addBool(tr("Export dans une boucle (cas normal)"), "", tr("Activer"), _exportInLoop);

}

void PB_StepExportAttributesInLoop::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
    if (_asciiExport)
    {
        postInputConfigDialog->addTitle(tr("Export ASCII tabulaire (1 fichier en tout)"));
        postInputConfigDialog->addFileChoice(tr("Choix du fichier"), CT_FileChoiceButton::OneNewFile, tr("Fichier texte (*.txt)"), _outASCIIFileName);
    }

#ifdef USE_OPENCV
    if (_rasterExport)
    {
        QStringList driversR;
        driversR.append(DEF_ESRI_ASCII_Grid);

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
#endif

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
    bool firstTurn = true;
    _modelsKeys.clear();
    _names.clear();

    QFile* fileASCII = nullptr;
    QTextStream* streamASCII = nullptr;
    bool first = true;

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn = inResultList.at(0);

    QString exportBaseName = "noName";
    if (_exportInLoop && inResultList.size() > 1)
    {
        CT_ResultGroup* resCounter = inResultList.at(1);
        CT_ResultItemIterator itCounter(resCounter, this, DEF_inCounter);
        if (itCounter.hasNext())
        {
            const CT_LoopCounter* counter = (const CT_LoopCounter*) itCounter.next();

            if (counter != nullptr)
            {
                QFileInfo fileinfo(counter->getTurnName());
                if (fileinfo.exists())
                {
                    exportBaseName = fileinfo.baseName();
                } else {
                    exportBaseName = counter->getTurnName();
                }

                if (counter->getCurrentTurn() > 1)
                {
                    first = false;
                }
            }
        }
    }

    CT_ModelSearchHelper::SplitHash hash;
    QString xKey = "";
    QString yKey = "";

    CT_ModelSearchHelper::SplitHash hash1 = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_attribute, DEFin_itemWithAttribute, resIn->model(), this);
    QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> ith1(hash1);
    while (ith1.hasNext())
    {
        ith1.next();
        hash.insert((CT_OutAbstractSingularItemModel*) (ith1.key()->rootOriginalModel()), ith1.value());
    }

    CT_ModelSearchHelper::SplitHash hash2 = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_attributeInItemXY, DEFin_itemWithXY, resIn->model(), this);
    QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> ith2(hash2);
    while (ith2.hasNext())
    {
        ith2.next();
        hash.insert((CT_OutAbstractSingularItemModel*) (ith2.key()->rootOriginalModel()), ith2.value());
    }

    CT_ModelSearchHelper::SplitHash hashX = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_Xattribute, DEFin_itemWithXY, resIn->model(), this);
    QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> ithX(hashX);
    while (ithX.hasNext())
    {
        ithX.next();
        hash.insert((CT_OutAbstractSingularItemModel*) (ithX.key()->rootOriginalModel()), ithX.value());

        CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*) (ithX.key()->rootOriginalModel());
        CT_OutAbstractItemAttributeModel *attrModel = ithX.value();
        if (attrModel->isADefaultItemAttributeModel() && attrModel->rootOriginalModel() != nullptr) {attrModel = (CT_OutAbstractItemAttributeModel*) (attrModel->rootOriginalModel());}
        xKey = QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrModel->uniqueName());
    }

    CT_ModelSearchHelper::SplitHash hashY = PS_MODELS->splitSelectedAttributesModelBySelectedSingularItemModel(DEFin_Yattribute, DEFin_itemWithXY, resIn->model(), this);
    QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> ithY(hashY);
    while (ithY.hasNext())
    {
        ithY.next();
        hash.insert((CT_OutAbstractSingularItemModel*) (ithY.key()->rootOriginalModel()), ithY.value());

        CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*) (ithY.key()->rootOriginalModel());
        CT_OutAbstractItemAttributeModel *attrModel = ithY.value();
        if (attrModel->isADefaultItemAttributeModel() && attrModel->rootOriginalModel() != nullptr) {attrModel = (CT_OutAbstractItemAttributeModel*) (attrModel->rootOriginalModel());}
        yKey = QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrModel->uniqueName());
    }

    if (firstTurn)
    {
        QHashIterator<CT_OutAbstractSingularItemModel *, CT_OutAbstractItemAttributeModel *> itModels(hash);
        while (itModels.hasNext())
        {
            itModels.next();

            CT_OutAbstractSingularItemModel  *itemModel = itModels.key();
            CT_OutAbstractItemAttributeModel *attrModel = itModels.value();

            QString itemDN = itemModel->displayableName();
            QString itemUN = itemModel->uniqueName();

            QString attrDN = attrModel->displayableName();
            QString attrUN = attrModel->uniqueName();

            if (attrModel->isADefaultItemAttributeModel() && attrModel->rootOriginalModel() != nullptr) {attrUN = attrModel->rootOriginalModel()->uniqueName();}

            QString key = QString("ITEM_%1_ATTR_%2").arg(itemUN).arg(attrUN);
            _modelsKeys.append(key);

            _names.insert(key, QString("%2_%1").arg(itemDN).arg(attrDN));

#ifdef USE_GDAL
            if (_vectorExport && _outVectorFolder.size() > 0)
            {
                CT_AbstractCategory::ValueType type = attrModel->itemAttribute()->type();

                if      (type == CT_AbstractCategory::BOOLEAN) {_ogrTypes.insert(key, OFTInteger);}
                else if (type == CT_AbstractCategory::STRING)  {_ogrTypes.insert(key, OFTString);}
                else if (type == CT_AbstractCategory::STRING)  {_ogrTypes.insert(key, OFTString);}
                else if (type == CT_AbstractCategory::INT8)    {_ogrTypes.insert(key, OFTInteger);}
                else if (type == CT_AbstractCategory::UINT8)   {_ogrTypes.insert(key, OFTInteger);}
                else if (type == CT_AbstractCategory::INT16)   {_ogrTypes.insert(key, OFTInteger);}
                else if (type == CT_AbstractCategory::UINT16)  {_ogrTypes.insert(key, OFTInteger);}
                else if (type == CT_AbstractCategory::INT32)   {_ogrTypes.insert(key, OFTInteger);}
                //                else if (type == CT_AbstractCategory::UINT32)  {_ogrTypes.insert(key, OFTInteger64);}
                //                else if (type == CT_AbstractCategory::INT64)   {_ogrTypes.insert(key, OFTInteger64);}
                //                else if (type == CT_AbstractCategory::INT32)   {_ogrTypes.insert(key, OFTInteger64);}
                else                                           {_ogrTypes.insert(key, OFTReal);}

            }
#endif
        }
        replaceBadCharacters(_names);
        qSort(_modelsKeys.begin(), _modelsKeys.end());

        if (_vectorExport && _outVectorFolder.size() > 0)
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

        if (_asciiExport && _outASCIIFileName.size() > 0)
        {
            fileASCII = new QFile(_outASCIIFileName.first());
            streamASCII = new QTextStream(fileASCII);

            if (first)
            {
                if (fileASCII->open(QIODevice::WriteOnly | QIODevice::Text))
                {
                    (*streamASCII) << "Name\t";

                    for (int i = 0 ; i < _modelsKeys.size() ; i++)
                    {
                        (*streamASCII) << _names.value(_modelsKeys.at(i));
                        if (i < _modelsKeys.size() - 1) {(*streamASCII) << "\t";} else {(*streamASCII) << "\n";}
                    }

                } else {
                    delete streamASCII; streamASCII = nullptr;
                    delete fileASCII; fileASCII = nullptr;
                    PS_LOG->addMessage(LogInterface::error, LogInterface::step, getStepCustomName() + tr("Impossible de créer le fichier d'export ASCII. Arrêt des traitements."));
                    stop();
                    return;
                }
            } else {
                if (!fileASCII->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
                {
                    delete streamASCII; streamASCII = nullptr;
                    delete fileASCII; fileASCII = nullptr;
                    PS_LOG->addMessage(LogInterface::error, LogInterface::step, getStepCustomName() + tr("Impossible d'ouvrir le fichier d'export ASCII. Arrêt des traitements."));
                    stop();
                    return;
                }
            }
        }
    }

#ifdef USE_GDAL
        GDALDataset *vectorDataSet = nullptr;
        OGRLayer *vectorLayer = nullptr;

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

            vectorDataSet = driverVector->Create(outFileName.toLatin1(), 0, 0, 0, GDT_Unknown, nullptr );

            if (vectorDataSet != nullptr)
            {
                vectorLayer = vectorDataSet->CreateLayer( "point", nullptr, wkbPoint, nullptr );
            } else {
                PS_LOG->addMessage(LogInterface::error, LogInterface::step, getStepCustomName() + tr("Impossible d'utiliser le format d'export Vectoriel choisi."));
            }

            for (int i = 0 ; vectorLayer != nullptr && i < _modelsKeys.size() ; i++)
            {
                QString key = _modelsKeys.at(i);
                if (_ogrTypes.contains(key))
                {
                    OGRFieldType ogrType = _ogrTypes.value(key);

                    QByteArray fieldNameBA = _shortNames.value(key).toLatin1();
                    const char* fieldName = fieldNameBA;

                    OGRFieldDefn oField(fieldName, ogrType );

                    if (vectorLayer->CreateField( &oField ) != OGRERR_NONE)
                    {
                        //  erreur
                    }
                }
            }
        }
#endif

    // IN results browsing
    CT_ResultGroupIterator itIn_grpMain(resIn, this, DEFin_grpMain);
    while (itIn_grpMain.hasNext() && !isStopped())
    {
        const CT_StandardItemGroup* grpMain = (CT_StandardItemGroup*) itIn_grpMain.next();

#ifdef USE_OPENCV
        QMap<QString, CT_Image2D<double>*> rasters;
        if (_rasterExport)
        {
            CT_PlotListInGrid* plotListInGrid = (CT_PlotListInGrid*) grpMain->firstItemByINModelName(this, DEFin_plotListInGrid);

            Eigen::Vector2d min, max;
            plotListInGrid->boundingBox2D(min, max);
            double resolution = plotListInGrid->getSpacing();

            if (plotListInGrid != nullptr)
            {
                for (int i = 0 ; i < _modelsKeys.size() ; i++)
                {
                    QString key = _modelsKeys.at(i);

                    if (key != xKey && key != yKey)
                    {
                        rasters.insert(key, CT_Image2D<double>::createImage2DFromXYCoords(nullptr, nullptr, min(0), min(1), max(0) - EPSILON_LIMITS, max(1) - EPSILON_LIMITS, resolution, 0, DEF_NA, DEF_NA));
                    }
                }
            }
        }
#endif

        CT_GroupIterator itIn_grp(grpMain, this, DEFin_grp);
        while (itIn_grp.hasNext() && !isStopped())
        {
            const CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itIn_grp.next();

            QMap<QString, QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*> > indexedAttributes;

            double x = std::numeric_limits<double>::max();
            double y = std::numeric_limits<double>::max();

            CT_AbstractSingularItemDrawable* itemXY = (CT_AbstractSingularItemDrawable*) grp->firstItemByINModelName(this, DEFin_itemWithXY);
            if (itemXY != nullptr)
            {
                CT_AbstractItemAttribute* attX = itemXY->firstItemAttributeByINModelName(resIn, this, DEFin_Xattribute);
                CT_AbstractItemAttribute* attY = itemXY->firstItemAttributeByINModelName(resIn, this, DEFin_Yattribute);

                if (attX != nullptr) {x = attX->toDouble(itemXY, nullptr);}
                if (attY != nullptr) {y = attY->toDouble(itemXY, nullptr);}

                CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*)itemXY->model();

                QList<CT_OutAbstractItemAttributeModel *> attributesModel = hash.values(itemModel);
                QList<CT_AbstractItemAttribute *> attributes = itemXY->itemAttributes(attributesModel);

                for (int i = 0 ; i < attributes.size() ; i++)
                {
                    CT_AbstractItemAttribute* attribute = attributes.at(i);
                    if (attribute != nullptr)
                    {
                        CT_OutAbstractItemAttributeModel* attrModel = (CT_OutAbstractItemAttributeModel*) attribute->model();

                        QString attrUN = attrModel->uniqueName();

                        indexedAttributes.insert(QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrUN), QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*>(itemXY, attribute));
                    }
                }
            }

            CT_ItemIterator itItem(grp, this, DEFin_itemWithAttribute);
            while (itItem.hasNext())
            {
                CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) itItem.next();

                if (item != nullptr)
                {
                    CT_OutAbstractSingularItemModel  *itemModel = (CT_OutAbstractSingularItemModel*)item->model();
                    QList<CT_OutAbstractItemAttributeModel *> attributesModel = hash.values(itemModel);
                    QList<CT_AbstractItemAttribute *> attributes = item->itemAttributes(attributesModel);

                    for (int i = 0 ; i < attributes.size() ; i++)
                    {
                        CT_AbstractItemAttribute* attribute = attributes.at(i);
                        if (attribute != nullptr)
                        {
                            CT_OutAbstractItemAttributeModel* attrModel = (CT_OutAbstractItemAttributeModel*) attribute->model();

                            QString attrUN = attrModel->uniqueName();

                            indexedAttributes.insert(QString("ITEM_%1_ATTR_%2").arg(itemModel->uniqueName()).arg(attrUN), QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*>(item, attribute));
                        }
                    }
                }
            }

            bool hasMetricsToExport = !(indexedAttributes.isEmpty());

#ifdef USE_GDAL
            OGRFeature *vectorFeature = nullptr;
            if (_vectorExport && hasMetricsToExport && vectorLayer != nullptr)
            {
                vectorFeature = OGRFeature::CreateFeature(vectorLayer->GetLayerDefn());
                OGRPoint pt;
                pt.setX(x);
                pt.setY(y);
                vectorFeature->SetGeometry(&pt);
            }
#endif
            if (_asciiExport && streamASCII != nullptr)
            {
                (*streamASCII) << exportBaseName << "\t";
            }

            for (int i = 0 ; i < _modelsKeys.size() ; i++)
            {
                QString key = _modelsKeys.at(i);

                const QPair<CT_AbstractSingularItemDrawable*, CT_AbstractItemAttribute*> &pair = indexedAttributes.value(key);

#ifdef USE_OPENCV
                CT_Image2D<double>* raster = rasters.value(key, nullptr);
#endif

                if (pair.first != nullptr && pair.second != nullptr)
                {
                    if (hasMetricsToExport && _asciiExport && streamASCII != nullptr)
                    {
                        (*streamASCII) << pair.second->toString(pair.first, nullptr);
                    }
#ifdef USE_GDAL
                    if (_vectorExport && vectorLayer != nullptr)
                    {

                        QByteArray fieldNameBA = _shortNames.value(key).toLatin1();
                        const char* fieldName = fieldNameBA;

                        if      (_ogrTypes.value(key) == OFTBinary)
                        {
                            vectorFeature->SetField(fieldName, pair.second->toInt(pair.first, nullptr));
                        } else if (_ogrTypes.value(key) == OFTString)
                        {
                            //QString text = replaceAccentCharacters(pair.second->toString(pair.first, nullptr));
                            QString text = pair.second->toString(pair.first, nullptr);
                            QByteArray textBA = text.toLatin1();
                            const char* textChar = textBA;
                            vectorFeature->SetField(fieldName, textChar);
                        } else if (_ogrTypes.value(key) == OFTInteger)
                        {
                            vectorFeature->SetField(fieldName, pair.second->toInt(pair.first, nullptr));
//                        } else if (_ogrTypes.value(key) == OFTInteger64)
//                        {
//                            vectorFeature->SetField(fieldName, pair.second->toInt(pair.first, nullptr));
                        } else
                        {
                            vectorFeature->SetField(fieldName, pair.second->toDouble(pair.first, nullptr));
                        }

                    }
#endif

#ifdef USE_OPENCV
                    if (_rasterExport && raster != nullptr)
                    {
                        double val = pair.second->toDouble(pair.first, nullptr);
                        if (std::isnan(val)) {val = DEF_NA;}
                        raster->setValueAtCoords(x, y, val);
                    }
#endif
                }

                if (hasMetricsToExport && _asciiExport && streamASCII != nullptr)
                {
                    if(i < _modelsKeys.size() - 1) {(*streamASCII) << "\t";} else {(*streamASCII) << "\n";}
                }

            }

#ifdef USE_GDAL
            if (_vectorExport && vectorLayer != nullptr)
            {
                if (vectorLayer->CreateFeature(vectorFeature) != OGRERR_NONE)
                {
                    OGRFeature::DestroyFeature(vectorFeature);
                }
            }
#endif

        }

#ifdef USE_OPENCV
        if (_rasterExport)
        {
            QMapIterator<QString, CT_Image2D<double>*> itRaster(rasters);
            while (itRaster.hasNext())
            {
                itRaster.next();
                QString key = itRaster.key();
                QList<CT_AbstractItemDrawable* > rasterList;
                rasterList.append(itRaster.value());

                QString metricName = _names.value(key);
                QString fileName = QString("%1/%2%3_%4").arg(_outRasterFolder.first()).arg(_rasterPrefix).arg(metricName).arg(exportBaseName);

                if (_subFolders) {
                    QDir dir(QString("%1/%2%3").arg(_outRasterFolder.first()).arg(_rasterPrefix).arg(metricName));
                    if (!dir.exists()) {dir.mkdir(".");}

                    fileName = QString("%1/%2%3/%2%3_%4").arg(_outRasterFolder.first()).arg(_rasterPrefix).arg(metricName).arg(exportBaseName);
                }

                if (_rasterDriverName == DEF_ESRI_ASCII_Grid)
                {
                    PB_Grid2DExporter exporter;
                    exporter.init();

                    if (exporter.setExportFilePath(fileName))
                    {
                        exporter.setItemDrawableToExport(rasterList);
                        exporter.exportToFile();
                    }

                } else {
#ifdef USE_GDAL
                    GDALDriver* driver = _gdalRasterDrivers.value(_rasterDriverName, nullptr);

                    if (driver != nullptr)
                    {
                        PB_GDALExporter exporter(driver);
                        exporter.init();

                        if (exporter.setExportFilePath(fileName))
                        {
                            exporter.setItemDrawableToExport(rasterList);
                            exporter.exportToFile();
                        }
                    }
#endif
                }
            }
        }
#endif

    }

    if (fileASCII != nullptr) {fileASCII->close(); delete fileASCII;}
    if (streamASCII != nullptr) {delete streamASCII;}
#ifdef USE_GDAL
    if (_vectorExport && vectorDataSet != nullptr)
    {
        GDALClose(vectorDataSet);
    }
#endif

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
