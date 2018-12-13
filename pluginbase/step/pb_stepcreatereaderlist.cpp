#include "pb_stepcreatereaderlist.h"

#include "pb_steppluginmanager.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"
#include "tools/pb_readerstools.h"

#include <QMessageBox>

// Constructor : initialization of parameters
PB_StepCreateReaderList::PB_StepCreateReaderList() : SuperClass()
{
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

void PB_StepCreateReaderList::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.setNotNeedInputResult();
}

bool PB_StepCreateReaderList::postInputConfigure()
{
    CT_AbstractReader* reader = pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedUniqueName);
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
            const bool ok = reader->configure();
            reader->setFilePathCanBeModified(true);

            if(!ok)
                return false;

            m_filepathCollection = fileList;
            setSettingsModified(true);

            return true;
        }
    }

    return false;
}

void PB_StepCreateReaderList::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    manager.addResult(m_hOutResult, tr("Liste de readers"));
    manager.setRootGroup(m_hOutResult, m_hOutRootGroup);

    CT_AbstractReader* reader = pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedUniqueName);

    // if one reader was selected and at least one file is defined
    if((reader != NULL)
            && (m_filepathCollection.size() > 0))
    {
        manager.addGroup(m_hOutRootGroup, m_hOutFileGroup, tr("Fichier"));
        manager.addItem(m_hOutRootGroup, m_hOutReaderItem, tr("Reader"), "", "", new CT_ReaderItem(reader, false));

        // get the header
        CT_FileHeader* rHeader = reader->createHeaderPrototype();

        if(rHeader != NULL)
            manager.addItem(m_hOutRootGroup, m_hOutFileHeader, tr("Entête"), "", "", rHeader);
    }
}

void PB_StepCreateReaderList::compute()
{
    for(CT_ResultGroup* result : m_hOutResult.iterateOutputs()) {

        CT_StandardItemGroup* rootGroup = m_hOutRootGroup.createInstance();
        result->addRootGroup(m_hOutRootGroup, rootGroup);

        CT_AbstractReader* reader = pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedUniqueName);

        if(reader != NULL) {

            // for each file in the list
            for(const QString& filePath : m_filepathCollection)
            {
                // copy the reader (copyFull = with configuration and models)
                CT_AbstractReader* readerCpy = reader->copyFull();

                // set the new filepath and check if it is valid
                if (readerCpy->setFilePath(filePath))
                {
                    // create the group that will contains header and reader (represent a File)
                    CT_StandardItemGroup* grpHeader = new CT_StandardItemGroup();
                    rootGroup->addGroup(m_hOutFileGroup, grpHeader);

                    // add the header
                    if(m_hOutFileHeader.isValid()) {
                        CT_FileHeader* header = readerCpy->readHeader();
                        Q_ASSERT(header != NULL);
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
            }
        }
    }
}

void PB_StepCreateReaderList::savePreSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePreSettings(writer);

    writer.addParameter(this, "ReaderClassName", m_readerSelectedUniqueName);
}

bool PB_StepCreateReaderList::restorePreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePreSettings(reader))
        return false;

    QVariant value;
    if(!reader.parameter(this, "ReaderClassName", value))
        return false;

    m_readerSelectedUniqueName = value.toString();

    return (pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedUniqueName) != NULL);
}

void PB_StepCreateReaderList::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    for(const QString& filePath : m_filepathCollection) {
        writer.addParameterPath(this, "File", filePath);
    }
}

bool PB_StepCreateReaderList::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    CT_AbstractReader* fReader = pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedUniqueName);

    QString filePath;
    m_filepathCollection.clear();

    const int n = reader.parameterPathCount(this, "File");
    m_filepathCollection.reserve(n);

    for(int i=0 ; i<n; ++i) {
        if(reader.parameterPath(this, "File", filePath)) {

            if(!fReader->setFilePath(filePath))
                return false;

            m_filepathCollection.append(filePath);
        }
    }

    return true;
}
