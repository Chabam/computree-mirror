#include "pb_stepexportitemlist.h"

#include "pb_steppluginmanager.h"
#include "ct_log/ct_logmanager.h"

PB_StepExportItemList::PB_StepExportItemList() : SuperClass(),
    mExporter(nullptr)
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

void PB_StepExportItemList::saveInputSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveInputSettings(writer);

    if(mExporter != nullptr)
        mExporter->saveSettings(writer);
}

bool PB_StepExportItemList::restoreInputSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreInputSettings(reader))
        return false;

    if(mExporter != nullptr)
        return mExporter->restoreSettings(reader);

    return true;
}

CT_VirtualAbstractStep* PB_StepExportItemList::createNewInstance() const
{
    return new PB_StepExportItemList();
}

void PB_StepExportItemList::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    QStringList list_exportersList = pluginStaticCastT<PB_StepPluginManager>()->exportersAvailable().keys();

    if(list_exportersList.isEmpty())
        list_exportersList.append(tr("ERREUR : aucun exporter disponible"));

    preInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), "", list_exportersList, m_exporterSelectedKey);
}

void PB_StepExportItemList::finalizePreSettings()
{
    delete mExporter;
    CT_AbstractExporter* exporter = pluginStaticCastT<PB_StepPluginManager>()->exportersAvailable().value(m_exporterSelectedKey);

    if(exporter != nullptr)
        mExporter = exporter->copy();
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepExportItemList::declareInputModels(CT_StepInModelStructureManager& manager)
{
    if(mExporter != nullptr)
        mExporter->declareInputModels(manager, CT_AbstractExporter::EachItemInSeparateFile_AttributeFromAnotherItem_InternalCounterIfSameFileName, true);
}

void PB_StepExportItemList::declareOutputModels(CT_StepOutModelStructureManager& /*manager*/)
{
}

void PB_StepExportItemList::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
    postInputConfigDialog->addFileChoice(tr("Répertoire d'export"), CT_FileChoiceButton::OneExistingFolder, "", _dir);
    postInputConfigDialog->addString(tr("Préfixe de nom de fichier"), "", _prefixFileName);
}

void PB_StepExportItemList::compute()
{
    if(mExporter == nullptr)
        return;

    mExporter->setFilePath(_dir.first());
    mExporter->setFileNamePrefix(_prefixFileName);

    if(!mExporter->exportToFile())
    {
        PS_LOG->addErrorMessage(LogInterface::step, mExporter->errorMessage());
        setErrorMessage(1, mExporter->errorMessage());
        setErrorCode(1);
    }
}
