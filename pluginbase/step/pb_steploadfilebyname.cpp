#include "pb_steploadfilebyname.h"

#include "pb_steppluginmanager.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_reader/ct_standardreaderseparator.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_abstractstepplugin.h"


#include "ct_global/ct_context.h"
#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"

#include "tools/pb_readerstools.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

// Alias for indexing models
#define DEF_inResult "inResult"
#define DEF_inGroup  "inGroup"
#define DEF_inItem   "inItem"
#define DEF_inAttName   "inAtt"


// Constructor : initialization of parameters
PB_StepLoadFileByName::PB_StepLoadFileByName(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepLoadFileByName::getStepDescription() const
{
    return tr("3- Create file reader from name");
}

// Step detailled description
QString PB_StepLoadFileByName::getStepDetailledDescription() const
{
    return tr("Create a file reader, using a base name obtained from checked field.\n You have to select a sample file, to allow reader configuration. ");
}

// Step URL
QString PB_StepLoadFileByName::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepLoadFileByName::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepLoadFileByName(dataInit);
}

void PB_StepLoadFileByName::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    writer.addParameterPath(this, "Directory", m_folderPath);
    writer.addParameter(this, "ReaderType", m_readerSelectedClassName);

    const CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(reader != nullptr)
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

    if(aReader != nullptr)
        return aReader->restoreSettings(reader);

    return true;
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepLoadFileByName::createPreConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();

    const QStringList list_readersList = PB_ReadersTools::constructReadersClassNameList(getPluginAs<PB_StepPluginManager>()->readersAvailable());

    configDialog->addStringChoice(tr("Choose file type"), "", list_readersList, m_readerSelectedClassName);
}

//void PB_StepLoadFileByName::createPostConfigurationDialog()
//{
//    CT_StepConfigurableDialog *configDialog = newStandardPostConfigurationDialog();

//    configDialog->addFileChoice(tr("Source directory"), CT_FileChoiceButton::OneExistingFolder, "", _directory);
//}

bool PB_StepLoadFileByName::postConfigure()
{
    CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);
    const QString fileFilter = PB_ReadersTools::constructStringForFileDialog(reader);

    if(fileFilter.isEmpty()) {
        QMessageBox::critical(NULL, tr("Erreur"), tr("Aucun reader sélectionné"));
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

void PB_StepLoadFileByName::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resultModel = createNewInResultModelForCopy(DEF_inResult, tr("Résultat"));
    resultModel->setZeroOrMoreRootGroup();
    resultModel->addStdGroupModel("", DEF_inGroup, CT_StandardItemGroup::staticGetType(), tr("Groupe"), "", CT_InAbstractGroupModel::CG_ChooseOneIfMultiple);
    resultModel->addStdItemModel(DEF_inGroup, DEF_inItem, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));
    resultModel->addStdItemAttributeModel(DEF_inItem, DEF_inAttName, QList<QString>() << CT_AbstractCategory::DATA_FILE_NAME << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Nom"));
}

// Creation and affiliation of OUT models
void PB_StepLoadFileByName::createOutResultModelListProtected()
{
    // get the reader selected
    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    CT_OutResultModelGroupToCopyPossibilities *resultModel = createNewOutResultModelToCopy(DEF_inResult);

    if (resultModel != NULL)
        if (reader != NULL && !m_folderPath.isEmpty())
        {
            // get the header
            CT_FileHeader *rHeader = reader->createHeaderPrototype();

            if(rHeader != NULL)
            {
                // copy the reader (copyFull = with configuration and models)
                CT_AbstractReader* readerCpy = reader->copyFull();

                resultModel->addItemModel(DEF_inGroup, _outReaderModelName, new CT_ReaderItem(NULL, NULL, readerCpy), tr("Reader"));
                resultModel->addItemModel(DEF_inGroup, _outHeaderModelName, rHeader, tr("Header"));
            }
        }
}


void PB_StepLoadFileByName::compute()
{
    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);
    if(reader == NULL) {return;}

    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resultOut = outResultList.at(0);

    CT_ResultGroupIterator it(resultOut, this, DEF_inGroup);
    while(!isStopped() && it.hasNext())
    {
        CT_StandardItemGroup *group = (CT_StandardItemGroup*) it.next();

        if (group != NULL)
        {
            CT_AbstractSingularItemDrawable* item = (CT_AbstractSingularItemDrawable*) group->firstItemByINModelName(this, DEF_inItem);
            if (item != NULL)
            {
                CT_AbstractItemAttribute* att = item->firstItemAttributeByINModelName(resultOut, this, DEF_inAttName);
                if (att !=  NULL)
                {
                    CT_AbstractReader* readerCpy = reader->copyFull();
                    const QList<FileFormat> &formats = readerCpy->readableFormats();

                    QString filename = QFileInfo(att->toString(item, NULL)).baseName();

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
