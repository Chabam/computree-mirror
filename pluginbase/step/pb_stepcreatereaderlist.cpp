#include "pb_stepcreatereaderlist.h"

#include "pb_steppluginmanager.h"

#include <QMessageBox>

#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/outModel/ct_outresultmodelgroup.h"
#include "ct_view/ct_genericconfigurablewidget.h"
#include "ct_abstractstepplugin.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "ct_view/ct_combobox.h"
#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_view/ct_stepconfigurabledialog.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"

#include "tools/pb_readerstools.h"

// Alias for indexing models
#define DEFout_res "res"
#define DEFout_grp "grp"
#define DEFout_grpHeader "grpHeader"
#define DEFout_header "header"
#define DEFout_reader "reader"

// Constructor : initialization of parameters
PB_StepCreateReaderList::PB_StepCreateReaderList(CT_StepInitializeData &dataInit) : CT_AbstractStepCanBeAddedFirst(dataInit)
{
}

// Step description (tooltip of contextual menu)
QString PB_StepCreateReaderList::getStepDescription() const
{
    return tr("1- Créer une liste de readers");
}

// Step detailled description
QString PB_StepCreateReaderList::getStepDetailledDescription() const
{
    return tr("Cette étape créer une liste de reader (lecteur de fichier). Ces readers pourront par la suite être utilisés par d'autres étapes pour charger les fichiers.");
}

// Step URL
QString PB_StepCreateReaderList::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStepCanBeAddedFirst::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepCreateReaderList::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepCreateReaderList(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepCreateReaderList::createInResultModelListProtected()
{
    // No in result is needed
    setNotNeedInputResult();
}

void PB_StepCreateReaderList::createPreConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();

    const QStringList list_readersList = PB_ReadersTools::constructReadersClassNameList(getPluginAs<PB_StepPluginManager>()->readersAvailable());

    configDialog->addStringChoice(tr("Choix du type de fichier"), "", list_readersList, m_readerSelectedClassName);
}

bool PB_StepCreateReaderList::postConfigure()
{
    CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);
    const QString fileFilter = PB_ReadersTools::constructStringForFileDialog(reader);

    if(fileFilter.isEmpty()) {
        QMessageBox::critical(NULL, tr("Erreur"), tr("Aucun reader sélectionné"));
        return false;
    }

    QStringList fileList = m_filepathCollection;

    CT_GenericConfigurableWidget configDialog;
    configDialog.addFileChoice(tr("Choisir les fichiers"), CT_FileChoiceButton::OneOrMoreExistingFiles, fileFilter, fileList);

    if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted) {

        if(fileList.isEmpty())
            return false;

        if(reader->setFilePath(fileList.first())) {
            reader->setFilePathCanBeModified(false);
            bool ok = reader->configure();
            reader->setFilePathCanBeModified(true);

            if(ok) {
                m_filepathCollection = fileList;
                setSettingsModified(true);
            }

            return ok;
        }
    }

    return false;
}

// Creation and affiliation of OUT models
void PB_StepCreateReaderList::createOutResultModelListProtected()
{
    // create a new result
    CT_OutResultModelGroup *outRes = createNewOutResultModel(DEFout_res, tr("Liste de readers"));

    // add a root group
    outRes->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));

    // get the reader selected
    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    // if one reader was selected and at least one file is defined
    if(reader != NULL && m_filepathCollection.size() > 0)
    {
        // get the header
        CT_FileHeader *rHeader = reader->createHeaderPrototype();

        if(rHeader != NULL) {
            // copy the reader (copyFull = with configuration and models)
            CT_AbstractReader* readerCpy = reader->copyFull();

            outRes->addGroupModel(DEFout_grp, DEFout_grpHeader, new CT_StandardItemGroup(), tr("Fichier"));
            outRes->addItemModel(DEFout_grpHeader, DEFout_reader, new CT_ReaderItem(NULL, NULL, readerCpy), tr("Reader"));
            outRes->addItemModel(DEFout_grpHeader, DEFout_header, rHeader, tr("Entête"));
        }
    }
}

void PB_StepCreateReaderList::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();

    // get the out result
    CT_ResultGroup* resultOut = outResultList.at(0);

    // search the model for headers
    CT_OutAbstractItemModel* headerModel = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(DEFout_header, resultOut);

    // create the root group and add it to result
    CT_StandardItemGroup* grp = new CT_StandardItemGroup(DEFout_grp, resultOut);
    resultOut->addGroup(grp);

    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(reader != NULL) {
        // for each file in the list
        for (int i = 0 ; i < m_filepathCollection.size() ; i++)
        {
            // copy the reader (copyFull = with configuration and models)
            CT_AbstractReader* readerCpy = reader->copyFull();

            // set the new filepath and check if it is valid
            if (readerCpy->setFilePath(m_filepathCollection.at(i)))
            {
                // create models of this reader
                if(readerCpy->outItemDrawableModels().isEmpty() && reader->outGroupsModel().isEmpty())
                    readerCpy->createOutItemDrawableModelList();

                // create the group that will contains header and reader (represent a File)
                CT_StandardItemGroup* grpHeader = new CT_StandardItemGroup(DEFout_grpHeader, resultOut);

                CT_FileHeader *header = readerCpy->readHeader();
                header->changeResult(resultOut);
                header->setModel(headerModel);

                // add the header
                grpHeader->addItemDrawable(header);

                // add the reader
                grpHeader->addItemDrawable(new CT_ReaderItem(DEFout_reader, resultOut, readerCpy));

                // add the group to the root
                grp->addGroup(grpHeader);
            }
            else
            {
                PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 inexistant ou non valide").arg(m_filepathCollection.at(i)));
                delete readerCpy;
            }
        }
    }
}

void PB_StepCreateReaderList::savePreSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePreSettings(writer);

    writer.addParameter(this, "ReaderClassName", m_readerSelectedClassName);

    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(reader != NULL)
        reader->saveSettings(writer);
}

bool PB_StepCreateReaderList::restorePreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePreSettings(reader))
        return false;

    QVariant value;
    if(!reader.parameter(this, "ReaderClassName", value))
        return false;

    m_readerSelectedClassName = value.toString();

    CT_AbstractReader *fReader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(fReader == NULL)
        return false;

    fReader->restoreSettings(reader);

    return true;
}

void PB_StepCreateReaderList::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    for(int i=0 ; i<m_filepathCollection.size(); ++i) {
        writer.addParameterPath(this, "File", m_filepathCollection.at(i));
    }
}

bool PB_StepCreateReaderList::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    QString path;
    m_filepathCollection.clear();

    const int n = reader.parameterPathCount(this, "File");

    for(int i=0 ; i<n; ++i) {
        if(reader.parameterPath(this, "File", path))
            m_filepathCollection.append(path);
    }

    CT_AbstractReader *fReader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    for(int i=0 ; i<m_filepathCollection.size(); ++i) {
        if(!fReader->setFilePath(m_filepathCollection.at(i)))
            return false;
    }

    return true;
}
