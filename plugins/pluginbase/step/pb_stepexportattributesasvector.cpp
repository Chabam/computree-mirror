#include "pb_stepexportattributesasvector.h"
#include "ct_log/ct_logmanager.h"
#include "tools/pb_tools.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <math.h>
#include <QDir>

#define DEF_ESRI_SHP "GDAL ESRI Shapefile"

PB_StepExportAttributesAsVector::PB_StepExportAttributesAsVector() : SuperClass()
{
    _replaceSpecialCharacters = true;
    _shortenNames = true;

    _vectorPrefix = "";

#ifdef USE_GDAL
    _vectorDriverName = DEF_ESRI_SHP;
    GDALDriverManager* const driverManager = GetGDALDriverManager();

    const int count = driverManager->GetDriverCount();

    for(int i = 0 ; i < count ; ++i)
    {
        GDALDriver *driver = driverManager->GetDriver(i);
        QString name = CT_GdalTools::staticGdalDriverName(driver);

        if(!name.isEmpty() && driver->GetMetadataItem(GDAL_DCAP_VECTOR) != nullptr && driver->GetMetadataItem(GDAL_DCAP_CREATE) != nullptr) {
            _gdalVectorDrivers.insert(name, driver);
        }
    }
#endif
}

QString PB_StepExportAttributesAsVector::description() const
{
    return tr("Export d'attributs - Vectoriel");
}
QString PB_StepExportAttributesAsVector::detailledDescription() const
{
    return tr("Cette étape permet d'exporter des attributs d'items situés dans un même groupe. Cette étape fonctionne également au sein d'une boucle, en prenant en compte les tours surccessifs. Tout attribut de n'importe quel item peut être exporté.<br>"
                "L'export est réalisé sous forme de vecteur (points en 2D avec attributs), produisant un fichier vecteur (par tour de boucle le cas échéant), avec une ligne par item, et une colonne par attribut. Cela nécessite que parmi les attributs figurent des coordonnées (x;y), auquelles assosicer les autres attributs.");
}

QString PB_StepExportAttributesAsVector::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>Il faut sélectionner les attributs à exporter.<br>"
                                               "Ces attributs peuvent appartenir à plusieurs items, tant que ces items sont dans un même groupe.<br>"
                                               "Il faut également séléctionner des attributs pour les coordonnées (X,Y) auxquelles les attributs seront associés spatialement.<br>"
                                               "Enfin, lorsque l'export est au sein d'une boucle, il faut sélectionner le compteur de boucle.");
}

QString PB_StepExportAttributesAsVector::outputDescription() const
{
    return SuperClass::outputDescription() + tr("Cette étape ne génère pas de nouvelles données.");
}

QString PB_StepExportAttributesAsVector::detailsDescription() const
{
    return tr("N.B. : les noms d'attributs sont simplifiés : suppression des accents, remplacement des espaces et caractères spéciaux par \"_\".");
}


CT_VirtualAbstractStep* PB_StepExportAttributesAsVector::createNewInstance() const
{
    return new PB_StepExportAttributesAsVector();
}

//////////////////// PROTECTED METHODS //////////////////


void PB_StepExportAttributesAsVector::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Résultat"), "", true);
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroupMain);

    manager.addItem(mInGroupMain, mInItemWithXY, tr("Item de position (avec XY)"));
    manager.addItemAttribute(mInItemWithXY, mInItemAttributeX, CT_AbstractCategory::DATA_X, tr("X"));
    manager.addItemAttribute(mInItemWithXY, mInItemAttributeY, CT_AbstractCategory::DATA_Y, tr("Y"));

    manager.addItem(mInGroupMain, mInItemWithAttribute, tr("Item avec des attributs"));
    manager.addItemAttribute(mInItemWithAttribute, mInItemAttribute, CT_AbstractCategory::DATA_VALUE, tr("Attribut à exporter"));

    manager.addResult(mInResultCounter, tr("Résultat compteur"), QString(), true);
    manager.setRootGroup(mInResultCounter, mInGroupCounter);
    manager.addItem(mInGroupCounter, mInLoopCounter, tr("Compteur"));
}

void PB_StepExportAttributesAsVector::declareOutputModels(CT_StepOutModelStructureManager&)
{
}

void PB_StepExportAttributesAsVector::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{        
    QStringList driversV;
#ifdef USE_GDAL
    driversV.append(_gdalVectorDrivers.keys());
#endif

    postInputConfigDialog->addEmpty();
    postInputConfigDialog->addTitle(tr("Export vectoriel (1 fichier / tour)"));
    postInputConfigDialog->addString(tr("Prefixe pour les fichiers exportés"), "", _vectorPrefix, tr("Un prefixe optionnel peut être ajouté à tous les noms de fichier, pour par exemple identifier différents lancements du script, exportant dans un même répertoire."));
    postInputConfigDialog->addStringChoice(tr("Choix du format d'export"), "", driversV, _vectorDriverName, tr("Format vecteur à utiliser pour les exports."));
    postInputConfigDialog->addFileChoice(tr("Répertoire d'export (vide de préférence)"), CT_FileChoiceButton::OneExistingFolder, "", _outVectorFolder, tr("Le contenu du dossier sélectionné ne sera pas effacé. Cependant pour plus de clarté il est préférable de choisir un dossier vide."));

    postInputConfigDialog->addEmpty();
    postInputConfigDialog->addBool(tr("Supprimer les caractères spéciaux dans les noms de champs"), "", "", _replaceSpecialCharacters, tr("Si cette case est cochée tous les caractères accentués seront remplacés par leur version non accentuée, et tous les caractères spéciaux seront remplacés par \"_\", dans les noms de champs"));
    postInputConfigDialog->addBool(tr("Raccourcir les noms de champs (à 10 caractères)"), "", "", _shortenNames, tr("Si cette case est cochée les noms de champs sont raccourcis si nécessaire pour ne pas dépasser 10 caractères. Dans ce cas un fichier texte est produit contenant les correspondances entre noms complets et noms raccourcis. Cette option est fortement recommandée en cas d'export au format ESRI Shapefile. "));
}

void PB_StepExportAttributesAsVector::compute()
{
    bool firstTurnFromCounter = true;

    const QString exportBaseName = createExportBaseName(firstTurnFromCounter);

    if (firstTurnFromCounter)
    {
        _names.clear();
        _modelsKeys.clear();

        computeModelsKeysAndNamesAndOgrTypes();
        PB_Tools::renameDuplicates(_names, _itemNames);

        if (_shortenNames) {_shortNames =  PB_Tools::computeShortNames(_names);}

        if (_replaceSpecialCharacters)
        {
            replaceSpecialCharacters(_names);
            if (_shortenNames) {replaceSpecialCharacters(_shortNames);}
        }

        if (_shortenNames && !_outVectorFolder.isEmpty())
        {
            PB_Tools::createCorrespondanceFile(QString("%1/vector").arg(_outVectorFolder.first()), _names, _shortNames);
        }

        std::sort(_modelsKeys.begin(), _modelsKeys.end());

        if (isStopped()) {return;}
    }


    if(isStopped())
        return;

#ifdef USE_GDAL
    QScopedPointer<GDALDataset, GDalDatasetScopedPointerCustomDeleter> vectorDataSet;
    OGRLayer* vectorLayer = nullptr;

    if(isStopped())
        return;

    preExportVectorIfNecessary(exportBaseName, vectorDataSet, vectorLayer);
#endif

    // IN results browsing
    for(const CT_StandardItemGroup* grp : mInGroupMain.iterateInputs(mInResult))
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
            }
#endif

            for (int i = 0 ; i < _modelsKeys.size() ; i++)
            {
                const QString key = _modelsKeys.at(i);

                const auto pair = indexedAttributes.value(key);

#ifdef USE_GDAL
                if (vectorLayer != nullptr && pair.first != nullptr && pair.second != nullptr)
                {
                    std::string fieldName;
                    if (_shortenNames) {fieldName = _shortNames.value(key).toStdString();}
                    else {fieldName = _names.value(key).toStdString();}

                    if (_ogrTypes.value(key) == OFTBinary)
                    {
                        vectorFeature->SetField(fieldName.data(), pair.second->toInt(pair.first, nullptr));
                    }
                    else if (_ogrTypes.value(key) == OFTString)
                    {
                        const std::string text = PB_Tools::replaceAccentCharacters(pair.second->toString(pair.first, nullptr)).toStdString();
                        vectorFeature->SetField(fieldName.data(), text.data());
                    }
                    else if (_ogrTypes.value(key) == OFTInteger)
                    {
                        vectorFeature->SetField(fieldName.data(), pair.second->toInt(pair.first, nullptr));
                        //                        }
                        //                        else if (_ogrTypes.value(key) == OFTInteger64)
                        //                        {
                        //                            vectorFeature->SetField(fieldName.data(), pair.second->toInt(pair.first, nullptr));
                    }
                    else
                    {
                        vectorFeature->SetField(fieldName.data(), pair.second->toDouble(pair.first, nullptr));
                    }
                }
#endif                
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
        }
    }
}

QString PB_StepExportAttributesAsVector::createExportBaseName(bool& first) const
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

    return QString("noName");
}

void PB_StepExportAttributesAsVector::computeModelsKeysAndNamesAndOgrTypes()
{
    // Iterate over models and not over items because it can be possible to have a model that doesn't have
    // an item at least.

    computeModelsKeysAndNamesAndOgrTypes(mInItemAttributeX, false);
    computeModelsKeysAndNamesAndOgrTypes(mInItemAttributeY, false);

    computeModelsKeysAndNamesAndOgrTypes(mInItemAttribute, true);
}

void PB_StepExportAttributesAsVector::computeModelsKeysAndNamesAndOgrTypesForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractItemAttributeModel* attModel, bool isNotXOrYAttribute)
{
    const QString itemDN = itemModel->displayableName();
    const QString attrDN = attModel->displayableName();

    const QString key = computeKeyForModels(itemModel, attModel);

    if(!_modelsKeys.contains(key))
    {
        _modelsKeys.append(key);

        if(isNotXOrYAttribute)
            _modelsKeysWithoutXOrYAttribute.append(key);

        _names.insert(key, attrDN);
        _itemNames.insert(key, itemDN);

#ifdef USE_GDAL
        if (!_outVectorFolder.isEmpty())
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

QString PB_StepExportAttributesAsVector::computeKeyForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractModel* attModel) const
{
    return QString("ITEM_%1_ATTR_%2").arg(size_t(itemModel->recursiveOriginalModel())).arg(size_t(attModel->recursiveOriginalModel()));
}


#ifdef USE_GDAL
void PB_StepExportAttributesAsVector::preExportVectorIfNecessary(const QString& exportBaseName, QScopedPointer<GDALDataset, GDalDatasetScopedPointerCustomDeleter>& vectorDataSet, OGRLayer*& vectorLayer)
{
    GDALDriver* driverVector = _gdalVectorDrivers.value(_vectorDriverName, nullptr);

    if (driverVector != nullptr && _outVectorFolder.size() > 0)
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

                std::string fieldName;
                if (_shortenNames) {fieldName = _shortNames.value(key).toStdString();}
                else {fieldName = _names.value(key).toStdString();}

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

void PB_StepExportAttributesAsVector::addToIndexedAttributesCollection(const CT_AbstractSingularItemDrawable* item, const CT_AbstractItemAttribute* attribute, QMap<QString, QPair<const CT_AbstractSingularItemDrawable*, const CT_AbstractItemAttribute*> >& indexedAttributes) const
{
    indexedAttributes.insert(computeKeyForModels(item->model(), attribute->model()), qMakePair(item, attribute));
}

void PB_StepExportAttributesAsVector::replaceSpecialCharacters(QMap<QString, QString> &names) const
{
    QMutableMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        it.setValue(PB_Tools::replaceSpecialCharacters(it.value()));
    }
}

