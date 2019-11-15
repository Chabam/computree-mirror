#include "pb_steplooponfiles.h"

#include "pb_steppluginmanager.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"
#include "tools/pb_readerstools.h"
#include "ct_log/ct_logmanager.h"

#include <QMessageBox>
#include <QProgressDialog>
#include <QApplication>

PB_StepLoopOnFiles::PB_StepLoopOnFiles() : SuperClass(),
    mReader(nullptr),
    m_folderIterator(nullptr)
{
}

PB_StepLoopOnFiles::~PB_StepLoopOnFiles()
{
    delete mReader;
    delete m_folderIterator;
}

QString PB_StepLoopOnFiles::description() const
{
    return tr("5- Loops on files in selected directory");
}

CT_VirtualAbstractStep* PB_StepLoopOnFiles::createNewInstance() const
{
    return new PB_StepLoopOnFiles();
}

void PB_StepLoopOnFiles::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    const QStringList list_readersList = PB_ReadersTools::constructReadersUniqueNameList(pluginStaticCastT<PB_StepPluginManager>()->readersAvailable());

    preInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), "", list_readersList, m_readerSelectedUniqueName);
}

void PB_StepLoopOnFiles::finalizePreSettings()
{
    SuperClass::finalizePreSettings();

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

bool PB_StepLoopOnFiles::postInputConfigure()
{
    if(mReader == nullptr) {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun reader sélectionné"));
        return false;
    }

    QStringList folders;
    folders.append(m_folderPath);
    CT_GenericConfigurableWidget configDialog;
    configDialog.addFileChoice(tr("Choose directory containing files"), CT_FileChoiceButton::OneExistingFolder, "", folders);

    if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted)
    {
        if(folders.isEmpty()) {
            QMessageBox::critical(nullptr, tr("Erreur"), tr("Dossier vide"));
            return false;
        }

        const QString folderPath = folders.first();
        QDirIterator* filesIterator = createFilesIterator(folderPath);

        if(!filesIterator->hasNext()) {
            QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun fichier compatible dans le dossier"));
            return false;
        }

        const QString firstFilePath = filesIterator->next();
        QString fp = firstFilePath;

        int progress = 0;
        QProgressDialog progressD(tr("Vérification des fichiers en cours..."), tr("Annuler"), 0, countFiles());
        progressD.setValue(progress);
        progressD.show();

        while(!fp.isEmpty() && !progressD.wasCanceled())
        {
            if(!mReader->setFilePath(fp)) {
                delete filesIterator;
                QMessageBox::critical(nullptr, tr("Erreur"), tr("Fichier \"%1\" non accepté par le reader").arg(fp));
                return false;
            }

            if(filesIterator->hasNext())
                fp = filesIterator->next();
            else
                fp.clear();

            progressD.setValue(++progress);
            qApp->processEvents();
        }

        delete filesIterator;

        if(progressD.wasCanceled())
            return false;

        progressD.close();

        mReader->setFilePath(firstFilePath);
        mReader->setFilePathCanBeModified(false);
        const bool ok = mReader->configure();
        mReader->setFilePathCanBeModified(true);

        if(!ok)
            return false;

        m_folderPath = folderPath;
        setSettingsModified(true);

        return true;
    }

    return false;
}

void PB_StepLoopOnFiles::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    SuperClass::declareOutputModels(manager);

    manager.addResult(m_hOutResultLOF, tr("Liste de readers"));
    manager.setRootGroup(m_hOutResultLOF, m_hOutRootGroupLOF);
    manager.addGroup(m_hOutRootGroupLOF, m_hOutFileGroupLOF, tr("Fichiers"));

    // if one reader was selected
    if(mReader != nullptr)
    {
        manager.addItem(m_hOutFileGroupLOF, m_hOutReaderItemLOF, tr("Reader"), "", "", new CT_ReaderItem(mReader, false));

        // get the header
        CT_FileHeader* rHeader = mReader->createHeaderPrototype();

        if(rHeader != nullptr)
            manager.addItem(m_hOutFileGroupLOF, m_hOutFileHeaderLOF, tr("Entête"), "", "", rHeader);
    }
}

void PB_StepLoopOnFiles::compute()
{
    SuperClass::compute();

    if(_counter->currentTurn() == 1)
    {
        m_folderIterator = createFilesIterator(m_folderPath);
        _counter->setNTurns(countFiles());
    }

    addToLogCurrentTurnInformation();

    for(CT_ResultGroup* result : m_hOutResultLOF.iterateOutputs())
    {
        if(m_folderIterator->hasNext())
        {
            CT_StandardItemGroup* rootGroup = m_hOutRootGroupLOF.createInstance();
            result->addRootGroup(m_hOutRootGroupLOF, rootGroup);

            // for each current file in the list
            // copy the reader (copyFull = with configuration and models)
            CT_AbstractReader* readerCpy = mReader->copyFull();

            const QString filepath = m_folderIterator->next();

            // set the new filepath and check if it is valid
            if(readerCpy->setFilePath(filepath))
            {
                PS_LOG->addMessage(LogInterface::info, LogInterface::step, tr("Ajout du fichier %1").arg(filepath));

                // create the group that will contains header and reader (represent a File)
                CT_StandardItemGroup* grpHeader = new CT_StandardItemGroup();
                rootGroup->addGroup(m_hOutFileGroupLOF, grpHeader);

                // add the header
                if(m_hOutFileHeaderLOF.isValid()) {
                    CT_FileHeader* header = readerCpy->readHeader();
                    Q_ASSERT(header != nullptr);
                    grpHeader->addSingularItem(m_hOutFileHeaderLOF, header);
                }

                // add the reader item
                CT_ReaderItem* rItem = new CT_ReaderItem(readerCpy, true);
                grpHeader->addSingularItem(m_hOutReaderItemLOF, rItem);
            }
            else
            {
                PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 non valide").arg(filepath));
                delete readerCpy;
            }
        }

        if(isStopped())
            return;
    }
}

void PB_StepLoopOnFiles::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    writer.addParameterPath(this, "Directory", m_folderPath, tr("Chemin du dossier contenant les fichiers à traiter."));

    mReader->saveSettings(writer);
}

bool PB_StepLoopOnFiles::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    QString path;

    if(!reader.parameterPath(this, "Directory", path))
        return false;

    m_folderPath = path;

    if(mReader == nullptr)
        return false;

    /*QDirIterator* filesIterator = createFilesIterator(m_folderPath);

    while(filesIterator->hasNext())
    {
        const QString& fp = filesIterator->next();

        if(!mReader->setFilePath(fp)) {
            delete filesIterator;
            return false;
        }
    }

    delete filesIterator;*/

    return mReader->restoreSettings(reader);
}

int PB_StepLoopOnFiles::countFiles() const
{
    int nFiles = 0;
    QDirIterator* it = createFilesIterator(m_folderPath);

    while (it->hasNext()) { it->next(); ++nFiles;}
    delete it;

    return nFiles;
}

QDirIterator* PB_StepLoopOnFiles::createFilesIterator(const QString& dirPath) const
{
    if(mReader == nullptr)
        return nullptr;

    const QStringList fileFilters = PB_ReadersTools::constructStringListToFilterFiles(mReader);
    return new QDirIterator(dirPath, fileFilters, QDir::Files | QDir::NoSymLinks, QDirIterator::NoIteratorFlags);
}
