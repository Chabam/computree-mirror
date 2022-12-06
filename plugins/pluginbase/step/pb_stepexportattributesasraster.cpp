#include "pb_stepexportattributesasraster.h"

#include "ct_itemdrawable/ct_image2d.h"
#include "exporters/grid2d/pb_grid2dexporter.h"
#include "ct_log/ct_logmanager.h"
#include "tools/pb_tools.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <math.h>
#include <QDir>

#define DEF_ESRI_SHP "GDAL ESRI Shapefile"

#define EPSILON_LIMITS 0.0000001
#define DEF_NA -9999999

PB_StepExportAttributesAsRaster::PB_StepExportAttributesAsRaster() : SuperClass()
{
    _replaceSpecialCharacters = true;
    _shortenNames = false;

    _rasterPrefix = "";

    _subFolders = true;

    mGrid2DExporterUniqueName = PB_Grid2DExporter().uniqueName();
    _rasterDriverName = mGrid2DExporterUniqueName;

#ifdef USE_GDAL
    GDALDriverManager* const driverManager = GetGDALDriverManager();

    const int count = driverManager->GetDriverCount();

    for(int i = 0 ; i < count ; ++i)
    {
        GDALDriver *driver = driverManager->GetDriver(i);
        QString name = CT_GdalTools::staticGdalDriverName(driver);

        if(!name.isEmpty() && driver->GetMetadataItem(GDAL_DCAP_RASTER) != nullptr && driver->GetMetadataItem(GDAL_DCAP_CREATE) != nullptr) {
            _gdalRasterDrivers.insert(name, driver);
        }
    }
#endif
}

QString PB_StepExportAttributesAsRaster::description() const
{
    return tr("Export d'attributs dans une boucle");
}
QString PB_StepExportAttributesAsRaster::detailledDescription() const
{
    return tr("Cette étape permet d'exporter des données au sein d'une boucle, en prenant en compte les tours surccessifs. Tout attribut de n'importe quel item peut être exporté.<br>"
                "Cette étape propose 3 types d'exports différents, pouvant être réalisés en parallèle ou non :<br>"
                "<ul>"
                "<li>Un export sous forme de fichier texte, avec une ligne par item et tour de boucle, et une colonne par attribut. Un seul fichier est produit, regroupant les données de tous les tours de boucle.</li>"
                "<li>Un export sous forme de raster, produisant un fichier raster par attribut et par tour de boucle. Pour que cet export puisse être utilisé, il faut que les données soient organisées sous forme de grille spatiale.</li>"
                "<li>Un export sous forme de vecteur (points en 2D avec attributs), produisant un fichier vecteur par tour de boucle, avec une ligne par item, et une colonne par attribut. Cela nécessite que parmi les attributs figurent des coordonnées (x;y), auquelles assosicer les autres attributs.</li>"
                "</ul>"
                "Il est possible d'utiliser cette étape en dehors d'une boucle, même si ce n'est pas son usage prévu, en sélectionnant l'option correspondante. ");
}

QString PB_StepExportAttributesAsRaster::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>Le type de données d'entrée nécessaire dépend des exports activés.<br>"
                                               "Dans tous les cas il faut sélectionner les attributs à exporter.<br>"
                                               "Pour les exports raster, il faut sélectionner un objet \"Grille de placettes\", par exemple généré par l'étape \"Créer une grille de placettes sur l'emprise\".<br>"
                                               "Pour les exports vecteurs (et aussi raster), il faut également séléctionner des attributs pour les coordonnées (x;y) auxquelles les attributs seront associés spatialement.<br><br>"
                                               "Comme il s'agit d'un export au sein d'une boucle, il faut également sélectionner le compteur de boucle (sauf si l'option d'export hors boucle a été choisi).");
}

QString PB_StepExportAttributesAsRaster::outputDescription() const
{
    return SuperClass::outputDescription() + tr("Cette étape ne génère pas de nouvelles données.");
}

QString PB_StepExportAttributesAsRaster::detailsDescription() const
{
    return tr("A noter que les trois types d'exports sont indépendants, même s'ils exportent les mêmes attributs.");
}


CT_VirtualAbstractStep* PB_StepExportAttributesAsRaster::createNewInstance() const
{
    return new PB_StepExportAttributesAsRaster();
}

//////////////////// PROTECTED METHODS //////////////////


void PB_StepExportAttributesAsRaster::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Résultat"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroupMain);

    manager.addItem(mInGroupMain, mInPlotListInGrid, tr("Grille de placettes"));

    manager.addGroup(mInGroupMain, mInGroupChild);

    manager.addItem(mInGroupChild, mInItemWithXY, tr("Item de position (avec XY)"));
    manager.addItemAttribute(mInItemWithXY, mInItemAttributeX, CT_AbstractCategory::DATA_X, tr("X"));
    manager.addItemAttribute(mInItemWithXY, mInItemAttributeY, CT_AbstractCategory::DATA_Y, tr("Y"));

    manager.addItem(mInGroupChild, mInItemWithAttribute, tr("Item avec des attributs"));
    manager.addItemAttribute(mInItemWithAttribute, mInItemAttribute, CT_AbstractCategory::DATA_VALUE, tr("Attribut à exporter"));

    manager.addResult(mInResultCounter, tr("Résultat compteur"), QString(), true);
    manager.setRootGroup(mInResultCounter, mInGroupCounter);
    manager.addItem(mInGroupCounter, mInLoopCounter, tr("Compteur"));

}

void PB_StepExportAttributesAsRaster::declareOutputModels(CT_StepOutModelStructureManager&)
{
}

void PB_StepExportAttributesAsRaster::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
    QStringList driversR;
    driversR.append(mGrid2DExporterUniqueName);
#ifdef USE_GDAL
    driversR.append(_gdalRasterDrivers.keys());
#endif

    postInputConfigDialog->addTitle(tr("Export raster (1 fichier / tour / métrique)"));
    postInputConfigDialog->addString(tr("Prefixe pour les fichiers exportés"), "", _rasterPrefix, tr("Un prefixe optionnel peut être ajouté à tous les noms de fichier, pour par exemple identifier différents lancements du script, exportant dans un même répertoire."));
    postInputConfigDialog->addStringChoice(tr("Choix du format d'export"), "", driversR, _rasterDriverName, tr("Format raster à utiliser pour les exports."));
    postInputConfigDialog->addFileChoice(tr("Répertoire d'export (vide de préférence)"), CT_FileChoiceButton::OneExistingFolder, "", _outRasterFolder, tr("Le contenu du dossier sélectionné ne sera pas effacé. Cependant pour plus de clarté il est préférable de choisir un dossier vide."));
    postInputConfigDialog->addBool(tr("Créer un sous-dossier par métrique"), "", "", _subFolders, tr("Si cette case est cochée, un dossier séparé sera créé pour achaque attribut à exporter, contenant chacun un raster par tour de boucle."));

    postInputConfigDialog->addEmpty();
    postInputConfigDialog->addBool(tr("Supprimer les caractères spéciaux dans les noms de champs"), "", "", _replaceSpecialCharacters, tr("Si cette case est cochée tous les caractères accentués seront remplacés par leur version non accentuée, et tous les caractères spéciaux seront remplacés par \"_\", dans les noms de métriques"));
    postInputConfigDialog->addBool(tr("Raccourcir les noms de métriques (à 10 caractères)"), "", "", _shortenNames, tr("Si cette case est cochée les noms de métriques sont raccourcis si nécessaire pour ne pas dépasser 10 caractères. Dans ce cas un fichier texte est produit contenant les correspondances entre noms complets et noms raccourcis. "));
}

void PB_StepExportAttributesAsRaster::compute()
{
    bool firstTurnFromCounter = true;

    const QString exportBaseName = createExportBaseName(firstTurnFromCounter);

    if (firstTurnFromCounter)
    {
        _names.clear();
        _modelsKeys.clear();
        computeModelsKeysAndNamesAndOgrTypes();

        if(isStopped()) {return;}

        if (_shortenNames)
        {
            _shortNames =  PB_Tools::computeShortNames(_names);

            if (!_outRasterFolder.isEmpty())
            {
                PB_Tools::createCorrespondanceFile(_outRasterFolder.first(), _shortNames);
            }
        }
    }


    // IN results browsing
    for(const CT_StandardItemGroup* grpMain : mInGroupMain.iterateInputs(mInResult))
    {
        if(isStopped())
            return;

        RastersMap rasters;

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

            for (int i = 0 ; i < _modelsKeys.size() ; i++)
            {
                const QString key = _modelsKeys.at(i);

                const auto pair = indexedAttributes.value(key);


                CT_Image2D<double>* raster = rasters.value(key, nullptr);

                const double val = pair.second->toDouble(pair.first, nullptr);

                if (std::isnan(val))
                    raster->setValueAtCoords(x, y, DEF_NA);
                else
                    raster->setValueAtCoords(x, y, val);
            }
        }

        QMapIterator<QString, CT_Image2D<double>*> itRaster(rasters);
        while (itRaster.hasNext())
        {
            itRaster.next();
            QString key = itRaster.key();
            QList<const CT_AbstractImage2D* > rasterList;
            rasterList.append(itRaster.value());

            QString metricName;

            if (_shortenNames) {metricName = _shortNames.value(key);}
                          else {metricName = _names.value(key);}


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

QString PB_StepExportAttributesAsRaster::createExportBaseName(bool& first) const
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

void PB_StepExportAttributesAsRaster::computeModelsKeysAndNamesAndOgrTypes()
{
    // Iterate over models and not over items because it can be possible to have a model that doesn't have
    // an item at least.

    computeModelsKeysAndNamesAndOgrTypes(mInItemAttribute, true);

    if (_replaceSpecialCharacters)
    {
        replaceSpecialCharacters(_names);
    }

    std::sort(_modelsKeys.begin(), _modelsKeys.end());
}

void PB_StepExportAttributesAsRaster::computeModelsKeysAndNamesAndOgrTypesForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractItemAttributeModel* attModel, bool isNotXOrYAttribute)
{
    const QString attrDN = attModel->displayableName();

    const QString key = computeKeyForModels(itemModel, attModel);

    if(!_modelsKeys.contains(key))
    {
        _modelsKeys.append(key);

        if(isNotXOrYAttribute)
            _modelsKeysWithoutXOrYAttribute.append(key);

        _names.insert(key, attrDN);
    }
}

QString PB_StepExportAttributesAsRaster::computeKeyForModels(const CT_OutAbstractModel* itemModel, const CT_OutAbstractModel* attModel) const
{
    return QString("ITEM_%1_ATTR_%2").arg(size_t(itemModel->recursiveOriginalModel())).arg(size_t(attModel->recursiveOriginalModel()));
}



void PB_StepExportAttributesAsRaster::addToIndexedAttributesCollection(const CT_AbstractSingularItemDrawable* item, const CT_AbstractItemAttribute* attribute, QMap<QString, QPair<const CT_AbstractSingularItemDrawable*, const CT_AbstractItemAttribute*> >& indexedAttributes) const
{
    indexedAttributes.insert(computeKeyForModels(item->model(), attribute->model()), qMakePair(item, attribute));
}

void PB_StepExportAttributesAsRaster::replaceSpecialCharacters(QMap<QString, QString> &names) const
{
    QMutableMapIterator<QString, QString> it(names);
    while (it.hasNext())
    {
        it.next();
        it.setValue(PB_Tools::replaceSpecialCharacters(it.value()));
    }
}

