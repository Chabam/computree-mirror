#include "pb_stepexportitemlist.h"

#include "pb_steppluginmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "ct_abstractstepplugin.h"
#include "ct_exporter/ct_standardexporterseparator.h"

#include "ct_itemdrawable/ct_profile.h"
#include "exporters/profile/pb_profileexporter.h"

#include "ct_itemdrawable/ct_loopcounter.h"

PB_StepExportItemList::PB_StepExportItemList() : SuperClass()
{
}

QString PB_StepExportItemList::description() const
{
    return tr("Export avec nom de fichier adaptatif (DEPRECATED)");
}

QString PB_StepExportItemList::detailledDescription() const
{
    return tr("Permet un export avec nom de fichier adaptatif.<br>"
              "Cette étape peut utiliser n'importe quel exporter.<br>"
              "Le nom du fichier de sorti, est déterminé à partir du compteur de boucle spécifié.");
}

QString PB_StepExportItemList::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

CT_VirtualAbstractStep* PB_StepExportItemList::createNewInstance()
{
    return new PB_StepExportItemList();
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepExportItemList::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_InResultModelGroup *resIn = createNewInResultModel(DEFin_res, tr("Résultat"));
    resIn->setZeroOrMoreRootGroup();
    resIn->addStdGroupModel("", DEFin_grp, CT_StandardItemGroup::staticGetType(), tr("Groupe"));
    resIn->addStdItemModel(DEFin_grp, DEFin_item, CT_AbstractItemDrawable::staticGetType(), tr("Item à exporter"));

    CT_InResultModelGroup* resCounter = createNewInResultModel(DEFin_resCounter, tr("Résultat compteur"), "", true);
    resCounter->setStdRootGroup(DEF_inGroupCounter);
    resCounter->addStdItemModel(DEF_inGroupCounter, DEF_inCounter, CT_LoopCounter::staticGetType(), tr("Compteur"));

    CT_InResultModelGroup* baseNameRes = createNewInResultModel(DEF_inresName, tr("Nom de base (optionnel)"), "", true);
    baseNameRes->setZeroOrMoreRootGroup();
    baseNameRes->addStdGroupModel("", DEF_inGroupName, CT_StandardItemGroup::staticGetType(), tr("Groupe"));
    baseNameRes->addStdItemModel(DEF_inGroupName, DEF_inName, CT_AbstractSingularItemDrawable::staticGetType(), tr("Base Name"));
    baseNameRes->addStdItemAttributeModel(DEF_inName, DEF_inNameAtt, QList<QString>() << CT_AbstractCategory::DATA_FILE_NAME << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Name"));
    baseNameRes->setMinimumNumberOfPossibilityThatMustBeSelectedForOneTurn(0);
}

//bool PB_StepExportItemList::configureInputResult()
//{
//    if(CT_VirtualAbstractStep::configureInputResult())
//        return configureExporter();

//    return false;
//}

//bool PB_StepExportItemList::configureExporter()
//{
//    configureExporterFromModel();

//    if(_exporterConfiguration != nullptr)
//        _exporter->loadExportConfiguration(_exporterConfiguration);

//    if(_exporter->configureExport())
//    {
//        clearExporterConfiguration();
//        _exporterConfiguration = _exporter->saveExportConfiguration();

//        setSettingsModified(true);

//        return true;
//    }

//    return false;
//}

//void PB_StepExportItemList::configureExporterFromModel()
//{
//    // on se place au premier tour (il n'y en a qu'un de toute façon)
//    getInTurnManager()->getTurnIndexManager()->resetTurnIndex();

//    // on demande au gestionnaire de tour de créer la liste des modèles d'entrée pour le premier tour
//    getInTurnManager()->createSearchModelListForCurrentTurn();

//    // on récupère le modèle d'entrée qu'on avait défini
//    CT_InResultModelGroup *resModel = (CT_InResultModelGroup*)getInResultModel(DEFin_res);

//    // on récupère les possibilités pour ce résultat (il n'y en a qu'une puisque un résultat d'entrée = un résultat de sortie)
//    QList<CT_InStdModelPossibility*> possibilities = resModel->getPossibilitiesSavedSelected();

//    QListIterator<CT_InStdModelPossibility*> it(possibilities);

//    QList<CT_AbstractItemDrawable*> eItems;

//    while(it.hasNext())
//    {
//        // on récupère le modèle d'entrée qu'on avait défini (celui à exporter)
//        CT_InAbstractItemModel *inItemModelToExport = nullptr;

//        if(_exporter->exportOnlyGroup())
//            inItemModelToExport = (CT_InAbstractItemModel*)PS_MODELS->searchModel(DEFin_grp, (CT_OutAbstractResultModel*)it.next()->outModel(), this);
//        else
//            inItemModelToExport = (CT_InAbstractItemModel*)PS_MODELS->searchModel(DEFin_item, (CT_OutAbstractResultModel*)it.next()->outModel(), this);

//        // on récupère sa possibilité (il n'y en a qu'une puisqu'on a mis CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple)
//        CT_InStdModelPossibility *possibility = inItemModelToExport->getPossibilitiesSavedSelected().first();

//        // on récupère l'ItemDrawable choisi
//        eItems.append(((CT_OutAbstractItemModel*)possibility->outModel())->itemDrawable());
//    }

//    _exporter->clearErrorMessage();

//    // on les donne à l'exporter
//    _exporter->setItemDrawableToExport(eItems);
//}

void PB_StepExportItemList::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    CT_OutResultModelGroup *res_res = createNewOutResultModel(DEFout_res, tr("Résultat"));
    res_res->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
}

void PB_StepExportItemList::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{

    postInputConfigDialog->addFileChoice(tr("Répertoire d'export"), CT_FileChoiceButton::OneExistingFolder, "", _dir);
    postInputConfigDialog->addString(tr("Préfixe de nom de fichier"), "", _prefixFileName);

    QStringList list_exportersList = getPluginAs<PB_StepPluginManager>()->exportersAvailable().keys();

    if(list_exportersList.isEmpty())
        list_exportersList.append(tr("ERREUR : aucun exporter disponible"));

    postInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), "", list_exportersList, m_exporterSelectedKey);
}

void PB_StepExportItemList::compute()
{

    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resIn_Item = inResultList.at(0);
    CT_ResultGroup* resIn_Counter = inResultList.at(1);
    CT_ResultGroup* resIn_BaseName = nullptr;

    QString rootBaseName = "";
    if (inResultList.size() > 2)
    {
        resIn_BaseName = inResultList.at(2);

        CT_ResultItemIterator it0(resIn_BaseName, this, DEF_inName);
        if (it0.hasNext())
        {
            const CT_AbstractSingularItemDrawable* item = it0.next();
            CT_AbstractItemAttribute* att = item->firstItemAttributeByINModelName(resIn_BaseName, this, DEF_inNameAtt);
            QString tmp = att->toString(item, nullptr);
            rootBaseName = QFileInfo(tmp).baseName();
        }
    }

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* res_res = outResultList.at(0);

    QString baseName = "";
    CT_ResultItemIterator it(resIn_Counter, this, DEF_inCounter);
    if (it.hasNext())
    {
        CT_LoopCounter* counter = (CT_LoopCounter*) it.next();
        baseName = counter->getTurnName();
        baseName = QFileInfo(baseName).baseName();
    }

    // IN results browsing
    CT_ResultGroupIterator itIn_grp(resIn_Item, this, DEFin_grp);
    while (itIn_grp.hasNext() && !isStopped())
    {
        const CT_StandardItemGroup* grpIn = (CT_StandardItemGroup*) itIn_grp.next();
        CT_AbstractItemDrawable* item = (CT_AbstractItemDrawable*)grpIn->firstItemByINModelName(this, DEFin_item);

        if (item != nullptr)
        {
            QList<CT_AbstractItemDrawable*> list;
            list.append(item);

            QString path = "";
            if (_dir.size() > 0)
            {
                path.append(_dir.first());
                path.append("/");
                path.append(rootBaseName);
                path.append(_prefixFileName);
            }

            path.append(baseName);

            CT_AbstractExporter* exporter = getPluginAs<PB_StepPluginManager>()->exportersAvailable().value(m_exporterSelectedKey);

            exporter->setExportFilePath(path);

            if (exporter->setItemDrawableToExport(list))
                exporter->exportToFile();
        }

    }


    // OUT results creation
    CT_StandardItemGroup* grpOut= new CT_StandardItemGroup(DEFout_grp, res_res);
    res_res->addGroup(grpOut);

}
