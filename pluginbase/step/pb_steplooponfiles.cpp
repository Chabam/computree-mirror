#include "pb_steplooponfiles.h"

#include "pb_steppluginmanager.h"

#include "ct_view/ct_genericconfigurablewidget.h"
#include "ct_abstractstepplugin.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "ct_view/ct_combobox.h"

#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_itemdrawable/ct_readeritem.h"

#include "ct_view/tools/ct_configurablewidgettodialog.h"

#include "tools/pb_readerstools.h"

#include <QDebug>

PB_StepLoopOnFiles::PB_StepLoopOnFiles() : CT_StepBeginLoop()
{
    m_folderIterator = nullptr;
}

PB_StepLoopOnFiles::~PB_StepLoopOnFiles()
{
    delete m_folderIterator;
}

QString PB_StepLoopOnFiles::description() const
{
    return tr("5- Loops on files in selected directory");
}

QString PB_StepLoopOnFiles::detailledDescription() const
{
    return tr("");
}

QString PB_StepLoopOnFiles::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStepCanBeAddedFirst::getStepURL(); //by default URL of the plugin
}

CT_VirtualAbstractStep* PB_StepLoopOnFiles::createNewInstance()
{
    return new PB_StepLoopOnFiles();
}

void PB_StepLoopOnFiles::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    writer.addParameterPath(this, "Directory", m_folderPath, tr("Chemin du dossier contenant les fichiers à traiter."));

    const CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(reader != nullptr)
        reader->saveSettings(writer);
}

bool PB_StepLoopOnFiles::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    QString path;

    if(!reader.parameterPath(this, "Directory", path))
        return false;

    m_folderPath = path;

    CT_AbstractReader* aReader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(aReader == nullptr)
        return false;

    if((aReader == nullptr) || !aReader->restoreSettings(reader))
        return false;

    return true;
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepLoopOnFiles::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();

    const QStringList list_readersList = PB_ReadersTools::constructReadersClassNameList(getPluginAs<PB_StepPluginManager>()->readersAvailable());

    postInputConfigDialog->addStringChoice(tr("Choose file type"), "", list_readersList, m_readerSelectedClassName);
}

bool PB_StepLoopOnFiles::postConfigure()
{
    m_folderPath.clear();

    QStringList folders;
    CT_GenericConfigurableWidget configDialog;
    configDialog.addFileChoice(tr("Choose directory containing files"), CT_FileChoiceButton::OneExistingFolder, "", folders);

    if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted)
    {
        if(folders.isEmpty())
            return false;

        m_folderPath = folders.first();

        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepLoopOnFiles::createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel)
{
    Q_UNUSED(firstResultModel);

    // create a new result
    CT_OutResultModelGroup *outRes = createNewOutResultModel(DEFout_res, tr("Liste de readers"));

    // add a root group
    outRes->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));

    // get the reader selected
    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    // if one reader was selected and at least one file is defined
    if (reader != nullptr && !m_folderPath.isEmpty())
    {
        // get the header
        CT_FileHeader *rHeader = reader->createHeaderPrototype();

        if(rHeader != nullptr) {
            // copy the reader (copyFull = with configuration and models)
            CT_AbstractReader* readerCpy = reader->copyFull();

            outRes->addGroupModel(DEFout_grp, DEFout_grpHeader, new CT_StandardItemGroup(), tr("File"));
            outRes->addItemModel(DEFout_grpHeader, DEFout_reader, new CT_ReaderItem(nullptr, nullptr, readerCpy), tr("Reader"));
            outRes->addItemModel(DEFout_grpHeader, DEFout_header, rHeader, tr("Header"));
        }
    }
}

void PB_StepLoopOnFiles::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{
    Q_UNUSED(outRes);
    Q_UNUSED(group);

    QList<CT_ResultGroup*> outResultList = getOutResultList();

    // get the out result
    CT_ResultGroup* resultOut = outResultList.at(1);

    int currentTurn = (int) _counter->getCurrentTurn();

    const CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if (currentTurn == 1)
    {
        if(m_folderPath.isEmpty()) {_counter->setNTurns(1); return;}

        const QStringList fileFilters = PB_ReadersTools::constructStringListToFilterFiles(reader);

        int nturns = 0;
        if(reader != nullptr)
        {
            m_folderIterator = new QDirIterator(m_folderPath, fileFilters ,QDir::Files | QDir::NoSymLinks, QDirIterator::NoIteratorFlags);

            while (m_folderIterator->hasNext()) {m_folderIterator->next(); ++nturns;}
            delete m_folderIterator;
            m_folderIterator = new QDirIterator(m_folderPath, fileFilters ,QDir::Files | QDir::NoSymLinks, QDirIterator::NoIteratorFlags);
        }

        _counter->setNTurns(nturns);
    }

    // search the model for headers
    CT_OutAbstractItemModel* headerModel = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(DEFout_header, resultOut);

    // create the root group and add it to result
    CT_StandardItemGroup* grp = new CT_StandardItemGroup(DEFout_grp, resultOut);
    resultOut->addGroup(grp);

    if(reader != nullptr)
    {
        // for each current file in the list
        // copy the reader (copyFull = with configuration and models)
        CT_AbstractReader* readerCpy = reader->copyFull();

        if (m_folderIterator->hasNext())
        {
            QString filepath = m_folderIterator->next();
            PS_LOG->addMessage(LogInterface::info, LogInterface::step, tr("Chargement du fichier %1").arg(filepath));

            // set the new filepath and check if it is valid
            if (readerCpy->setFilePath(filepath))
            {
                if (readerCpy->configure())
                {

                    // create models of this reader
                    if(readerCpy->outItemDrawableModels().isEmpty() && reader->outGroupsModel().isEmpty())
                    {
                        readerCpy->createOutItemDrawableModelList();
                    }

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
            }
            else
            {
                PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 inexistant ou non valide").arg(filepath));
                delete readerCpy;
            }
        }
    }
}
