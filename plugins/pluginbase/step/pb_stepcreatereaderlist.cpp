#include "pb_stepcreatereaderlist.h"

#include "pb_steppluginmanager.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"
#include "tools/pb_readerstools.h"

#include <QMessageBox>

// Constructor : initialization of parameters
PB_StepCreateReaderList::PB_StepCreateReaderList() : SuperClass(),
    mReader(nullptr)
{
}

PB_StepCreateReaderList::~PB_StepCreateReaderList()
{
    delete mReader;
}

// Step description (tooltip of contextual menu)
QString PB_StepCreateReaderList::description() const
{
    return tr("1- Créer une liste de readers");
}

// Step detailled description
QString PB_StepCreateReaderList::detailledDescription() const
{
    return tr("Cette étape créer une liste de reader (lecteur de fichier). Ces readers pourront par la suite être utilisés par d'autres étapes pour charger les fichiers.");
}

// Step copy method
CT_VirtualAbstractStep* PB_StepCreateReaderList::createNewInstance() const
{
    return new PB_StepCreateReaderList();
}

void PB_StepCreateReaderList::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    const QStringList list_readersList = PB_ReadersTools::constructReadersUniqueNameList(pluginStaticCastT<PB_StepPluginManager>()->readersAvailable());

    preInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), "", list_readersList, m_readerSelectedUniqueName);
}

void PB_StepCreateReaderList::finalizePreSettings()
{
    CT_AbstractReader* reader = pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedUniqueName);

    if(reader == nullptr) {
        delete mReader;
        mReader = nullptr;
        return;
    }

    if((mReader == nullptr) || (reader->uniqueName() != mReader->uniqueName()))
    {
        delete mReader;
        mReader = reader->copyFull();
    }
}

void PB_StepCreateReaderList::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.setNotNeedInputResult();
}

bool PB_StepCreateReaderList::postInputConfigure()
{
    const QString fileFilter = PB_ReadersTools::constructStringForFileDialog(mReader);

    if(fileFilter.isEmpty()) {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun reader sélectionné"));
        return false;
    }

    QStringList fileList = m_filepathCollection;

    CT_GenericConfigurableWidget configDialog;
    configDialog.addFileChoice(tr("Choisir les fichiers"), CT_FileChoiceButton::OneOrMoreExistingFiles, fileFilter, fileList);

    if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted) {

        if(fileList.isEmpty()) {
            QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun fichier sélectionné"));
            return false;
        }

        for(const QString& fp : fileList)
        {
            if(!mReader->setFilePath(fp)) {
                QMessageBox::critical(nullptr, tr("Erreur"), tr("Fichier \"%1\" non accepté par le reader").arg(fp));
                return false;
            }
        }

        mReader->setFilePath(fileList.first());
        mReader->setFilePathCanBeModified(false);
        const bool ok = mReader->configure();
        mReader->setFilePathCanBeModified(true);

        if(!ok)
            return false;

        m_filepathCollection = fileList;
        setSettingsModified(true);

        return true;
    }

    return false;
}

void PB_StepCreateReaderList::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    manager.addResult(m_hOutResult, tr("Liste de readers"));
    manager.setRootGroup(m_hOutResult, m_hOutRootGroup);
    manager.addGroup(m_hOutRootGroup, m_hOutFileGroup, tr("Fichier"));

    // if one reader was selected
    if(mReader != nullptr)
    {
        manager.addItem(m_hOutFileGroup, m_hOutReaderItem, tr("Reader"), "", "", new CT_ReaderItem(mReader, false));

        // get the header
        CT_FileHeader* rHeader = mReader->createHeaderPrototype();

        if(rHeader != nullptr)
            manager.addItem(m_hOutFileGroup, m_hOutFileHeader, tr("Entête"), "", "", rHeader);
    }
}

void PB_StepCreateReaderList::compute()
{
    for(CT_ResultGroup* result : m_hOutResult.iterateOutputs()) {

        CT_StandardItemGroup* rootGroup = m_hOutRootGroup.createInstance();
        result->addRootGroup(m_hOutRootGroup, rootGroup);

        // for each file in the list
        for(const QString& filePath : m_filepathCollection)
        {
            // copy the reader (copyFull = with configuration and models)
            CT_AbstractReader* readerCpy = mReader->copyFull();

            // set the new filepath and check if it is valid
            if (readerCpy->setFilePath(filePath))
            {
                // create the group that will contains header and reader (represent a File)
                CT_StandardItemGroup* grpHeader = new CT_StandardItemGroup();
                rootGroup->addGroup(m_hOutFileGroup, grpHeader);

                // add the header
                if(m_hOutFileHeader.isValid()) {
                    CT_FileHeader* header = readerCpy->readHeader();
                    Q_ASSERT(header != nullptr);
                    grpHeader->addSingularItem(m_hOutFileHeader, header);
                }

                // add the reader item
                CT_ReaderItem* rItem = new CT_ReaderItem(readerCpy, true);
                grpHeader->addSingularItem(m_hOutReaderItem, rItem);
            }
            else
            {
                STEP_LOG->addWarningMessage(tr("Fichier %1 inexistant ou non valide").arg(filePath));
                delete readerCpy;
            }

            if(isStopped())
                return;
        }
    }
}

void PB_StepCreateReaderList::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    for(const QString& filePath : m_filepathCollection) {
        writer.addParameterPath(this, "File", filePath);
    }

    mReader->saveSettings(writer);
}

bool PB_StepCreateReaderList::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    if(mReader == nullptr)
        return false;

    QString filePath;
    m_filepathCollection.clear();

    const int n = reader.parameterPathCount(this, "File");
    m_filepathCollection.reserve(n);

    for(int i=0 ; i<n; ++i) {
        if(reader.parameterPath(this, "File", filePath)) {

            if(!mReader->setFilePath(filePath))
                return false;

            m_filepathCollection.append(filePath);
        }
    }

    return mReader->restoreSettings(reader);
}