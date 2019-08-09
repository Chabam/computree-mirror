#include "pb_steploadfilebyname.h"

#include "pb_steppluginmanager.h"

#include "ct_reader/ct_standardreaderseparator.h"

#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_abstractstepplugin.h"

#include "ct_itemdrawable/ct_readeritem.h"

#include "ct_view/tools/ct_configurablewidgettodialog.h"

#include "tools/pb_readerstools.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

PB_StepLoadFileByName::PB_StepLoadFileByName() : SuperClass()
{
}

QString PB_StepLoadFileByName::description() const
{
    return tr("3- Create file reader from name");
}

QString PB_StepLoadFileByName::detailledDescription() const
{
    return tr("Create a file reader, using a base name obtained from checked field.\n You have to select a sample file, to allow reader configuration. ");
}

QString PB_StepLoadFileByName::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

CT_VirtualAbstractStep* PB_StepLoadFileByName::createNewInstance()
{
    return new PB_StepLoadFileByName();
}

void PB_StepLoadFileByName::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    writer.addParameterPath(this, "Directory", m_folderPath);
    writer.addParameter(this, "ReaderType", m_readerSelectedClassName);

    const CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(reader != nullptrptr)
        reader->saveSettings(writer);
}

bool PB_StepLoadFileByName::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    if(!reader.parameterPath(this, "Directory", m_folderPath, ""))
        return false;

    QVariant value;

    if(!reader.parameter(this, "ReaderType", value))
        return false;

    m_readerSelectedClassName = value.toString();

    CT_AbstractReader *aReader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(aReader != nullptrptr)
        return aReader->restoreSettings(reader);

    return true;
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepLoadFileByName::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();

    const QStringList list_readersList = PB_ReadersTools::constructReadersClassNameList(getPluginAs<PB_StepPluginManager>()->readersAvailable());

    postInputConfigDialog->addStringChoice(tr("Choose file type"), "", list_readersList, m_readerSelectedClassName);
}

//void PB_StepLoadFileByName::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
//{
//

//    postInputConfigDialog->addFileChoice(tr("Source directory"), CT_FileChoiceButton::OneExistingFolder, "", _directory);
//}

bool PB_StepLoadFileByName::postConfigure()
{
    CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);
    const QString fileFilter = PB_ReadersTools::constructStringForFileDialog(reader);

    if(fileFilter.isEmpty()) {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun reader sélectionné"));
        return false;
    }

    QStringList fileList;

    CT_GenericConfigurableWidget configDialog;
    configDialog.addFileChoice(tr("Choisir un fichier exemple"), CT_FileChoiceButton::OneOrMoreExistingFiles, fileFilter, fileList);
    configDialog.addTitle(tr("Le fichier choisi doit :"));
    configDialog.addText("", tr("- Etre dans le répertoire des fichiers à charger"), "");
    configDialog.addText("", tr("- Avoir le même format que les fichiers à charger"), "");
    configDialog.addText("", tr("- Avoir la même structure / version que les fichiers à charger"), "");

    if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted) {

        if(fileList.isEmpty())
            return false;

        if(reader->setFilePath(fileList.first())) {
            reader->setFilePathCanBeModified(false);
            bool ok = reader->configure();
            reader->setFilePathCanBeModified(true);

            if(ok) {
                m_folderPath = QFileInfo(fileList.first()).path();
                setSettingsModified(true);
            }

            return ok;
        }
    }

    return false;
}

void PB_StepLoadFileByName::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_InResultModelGroupToCopy *resultModel = createNewInResultModelForCopy(DEF_inResult, tr("Résultat"));
    resultModel->setZeroOrMoreRootGroup();
    resultModel->addStdGroupModel("", DEF_inGroup, CT_StandardItemGroup::staticGetType(), tr("Groupe"), "", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple);
    resultModel->addStdItemModel(DEF_inGroup, DEF_inItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));
    resultModel->addStdItemAttributeModel(DEF_inItem, DEF_inAttName, QList<QString>() << CT_AbstractCategory::DATA_FILE_NAME << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Nom"));
}

void PB_StepLoadFileByName::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    // get the reader selected
    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    CT_OutResultModelGroupToCopyPossibilities *resultModel = createNewOutResultModelToCopy(DEF_inResult);

    if (resultModel != nullptr)
        if (reader != nullptr && !m_folderPath.isEmpty())
        {
            // get the header
            CT_FileHeader *rHeader = reader->createHeaderPrototype();

            if(rHeader != nullptr)
            {
                // copy the reader (copyFull = with configuration and models)
                CT_AbstractReader* readerCpy = reader->copyFull();

                resultModel->addItemModel(DEF_inGroup, _outReaderModelName, new CT_ReaderItem(nullptr, nullptr, readerCpy), tr("Reader"));
                resultModel->addItemModel(DEF_inGroup, _outHeaderModelName, rHeader, tr("Header"));
            }
        }
}

void PB_StepLoadFileByName::compute()
{
    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);
    if(reader == nullptr) {return;}

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resultOut = outResultList.at(0);

    CT_ResultGroupIterator it(resultOut, this, DEF_inGroup);
    while(!isStopped() && it.hasNext())
    {
        CT_StandardItemGroup *group = (CT_StandardItemGroup*) it.next();

        if (group != nullptr)
        {
            CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) group->firstItemByINModelName(this, DEF_inItem);
            if (item != nullptr)
            {
                CT_AbstractItemAttribute* att = item->firstItemAttributeByINModelName(resultOut, this, DEF_inAttName);
                if (att !=  nullptr)
                {
                    CT_AbstractReader* readerCpy = reader->copyFull();
                    const QList<FileFormat> &formats = readerCpy->readableFormats();

                    QString filename = QFileInfo(att->toString(item, nullptr)).baseName();

                    QString filepath = QString("%1/%2").arg(m_folderPath).arg(filename);
                    if (formats.size() > 0)
                    {
                        filepath = QString("%1.%2").arg(filepath).arg(formats.first().suffixes().first());
                    }

                    PS_LOG->addMessage(LogInterface::info, LogInterface::step, tr("Chargement du fichier %1").arg(filepath));

                    // set the new filepath and check if it is valid
                    if (readerCpy->setFilePath(filepath))
                    {
                        // create models of this reader
                        if(readerCpy->outItemDrawableModels().isEmpty() && reader->outGroupsModel().isEmpty())
                        {
                            readerCpy->createOutItemDrawableModelList();
                        }

                        CT_OutAbstractItemModel* headerModel = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(_outHeaderModelName.completeName(), resultOut);

                        CT_FileHeader *header = readerCpy->readHeader();

                        header->changeResult(resultOut);
                        header->setModel(headerModel);

                        // add the header
                        group->addItemDrawable(header);

                        // add the reader
                        group->addItemDrawable(new CT_ReaderItem(_outReaderModelName.completeName(), resultOut, readerCpy));
                    } else
                    {
                        PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 inexistant ou non valide").arg(filepath));
                        delete readerCpy;
                    }
                }
            }
        }
    }
}
