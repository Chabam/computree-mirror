#include "pb_steploadfilebyname.h"

#include "pb_steppluginmanager.h"

#include "ct_view/tools/ct_configurablewidgettodialog.h"
#include "ct_log/ct_logmanager.h"

#include "tools/ct_readerstools.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

PB_StepLoadFileByName::PB_StepLoadFileByName() : SuperClass(),
    mReader(nullptr)
{
}

PB_StepLoadFileByName::~PB_StepLoadFileByName()
{
    if (mReader != nullptr) {delete mReader;}
}

QString PB_StepLoadFileByName::description() const
{
    return tr("3- Create file reader from name");
}

QString PB_StepLoadFileByName::detailledDescription() const
{
    return tr("Create a file reader, using a base name obtained from checked field.\n You have to select a sample file, to allow reader configuration. ");
}

CT_VirtualAbstractStep* PB_StepLoadFileByName::createNewInstance() const
{
    return new PB_StepLoadFileByName();
}

void PB_StepLoadFileByName::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    const QStringList list_readersList = CT_ReadersTools::constructReadersUniqueNameList(pluginStaticCastT<PB_StepPluginManager>()->readersAvailable());

    preInputConfigDialog->addStringChoice(tr("Choose file type"), "", list_readersList, m_readerSelectedClassName);
}

void PB_StepLoadFileByName::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(m_hInResult, tr("Résultat"));
    manager.setZeroOrMoreRootGroup(m_hInResult, m_hInRootGroup);
    manager.addGroup(m_hInRootGroup, m_hInGroup);
    manager.addItem(m_hInGroup, m_hInItem, tr("Item"));
    manager.addItemAttribute(m_hInItem, m_hInItemAttribute, QStringList() << CT_AbstractCategory::DATA_FILE_NAME << CT_AbstractCategory::DATA_VALUE, tr("Nom"));
}

bool PB_StepLoadFileByName::postInputConfigure()
{
    CT_AbstractReader* reader = pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedClassName);
    const QString fileFilter = CT_ReadersTools::constructStringForFileDialog(reader);

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

        if((mReader == nullptr) || (reader->uniqueName() != mReader->uniqueName()))
        {
            if (mReader != nullptr) {delete mReader;}
            mReader = reader->copyFull();
        }

        if(mReader->setFilePath(fileList.first())) {
            mReader->setFilePathCanBeModified(false);
            bool ok = mReader->configure();
            mReader->setFilePathCanBeModified(true);

            if(ok) {
                m_folderPath = QFileInfo(fileList.first()).path();
                setSettingsModified(true);
            }

            return ok;
        }
    }

    return false;
}

void PB_StepLoadFileByName::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    manager.addResultCopy(m_hInResult);

    // if one reader was selected and at least one file is defined
    if((mReader != nullptr) && !m_folderPath.isEmpty())
    {
        manager.addItem(m_hInGroup, m_hOutReaderItem, tr("Reader"), "", "", new CT_ReaderItem(mReader, false));

        // get the header
        CT_FileHeader* rHeader = mReader->createHeaderPrototype();

        if(rHeader != nullptr)
            manager.addItem(m_hInGroup, m_hOutFileHeader, tr("Entête"), "", "", rHeader);
    }
}

void PB_StepLoadFileByName::compute()
{
    auto l = m_hInItem.iterateOutputs(m_hInResult);
    auto b = l.begin();
    auto e = l.end();

    const QList<FileFormat> &formats = mReader->readableFormats();

    while((b != e) && !isStopped())
    {
        CT_AbstractSingularItemDrawable* item = (*b);

        const CT_AbstractItemAttribute* att = item->itemAttribute(m_hInItemAttribute);

        if(att != nullptr)
        {
            CT_AbstractReader* readerCpy = mReader->copyFull();

            const QString filename = QFileInfo(att->toString(item, nullptr)).baseName();
            const QString filepath = QString("%1.%2").arg(QString("%1/%2").arg(m_folderPath).arg(filename)).arg(formats.first().suffixes().first());

            PS_LOG->addMessage(LogInterface::info, LogInterface::step, tr("Chargement du fichier %1").arg(filepath));

            // set the new filepath and check if it is valid
            if (readerCpy->setFilePath(filepath))
            {
                CT_StandardItemGroup* group = b.currentParent();

                // add the header
                if(m_hOutFileHeader.isValid()) {
                    CT_FileHeader* header = readerCpy->readHeader();
                    Q_ASSERT(header != nullptr);
                    group->addSingularItem(m_hOutFileHeader, header);
                }

                // add the reader item
                CT_ReaderItem* rItem = new CT_ReaderItem(readerCpy, true);
                group->addSingularItem(m_hOutReaderItem, rItem);
            }
            else
            {
                PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 inexistant ou non valide").arg(filepath));
                delete readerCpy;
            }
        }

        ++b;
    }
}

void PB_StepLoadFileByName::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    writer.addParameterPath(this, "Directory", m_folderPath);

    mReader->saveSettings(writer);
}

bool PB_StepLoadFileByName::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    if(!reader.parameterPath(this, "Directory", m_folderPath, ""))
        return false;

    CT_AbstractReader* fReader = pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedClassName);

    if(fReader == nullptr)
        return false;

    if (mReader != nullptr) {delete mReader;}
    mReader = fReader->copyFull();

    return mReader->restoreSettings(reader);
}
