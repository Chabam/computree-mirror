#include "pb_steploadfilebyname.h"

#include "pb_steppluginmanager.h"

#include "ct_view/tools/ct_configurablewidgettodialog.h"
#include "ct_log/ct_logmanager.h"

#include "tools/ct_readerstools.h"

#include <QFile>
#include <QTextStream>
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
    return tr("Créer un sélecteur de fichier par attribut de nom");
}

QString PB_StepLoadFileByName::detailledDescription() const
{
    return tr("Cette étape charge l'entête d'un fichier, dont le nom est déterminé par un attribut choisi.<br>"
                "L'utilisateur doit choisir un fichier exemple, qui est utilisé pour définir le répertoire de recherche et vérifier le format utilisé.");
}

QString PB_StepLoadFileByName::inputDescription() const
{
    return SuperClass::inputDescription() + tr("<br><br>L'attibut sélectionné doit contenir le nom de base du fichier recherché. Si l'attribut contient un chemin complet, cette étape en extraiera uniquement le nom de base (sans chemin d'accès et sans extension).<br>Il s'agit souvent d'un attribut nommé \"filename\" ou \"filepath\"");
}

QString PB_StepLoadFileByName::outputDescription() const
{
    return SuperClass::outputDescription() + tr("<br>Cette étape charge uniquement l'entête du fichier. <strong>Il faudra ensuite charger les données du fichier</strong>, en général à l'aide de l'étape \"Charger les fichiers d'une liste\".");
}

QString PB_StepLoadFileByName::detailsDescription() const
{
    return tr("En général, cette étape est utilisée dans une boucle, afin de charger un fichier correspondant au tour de boucle en cours.<br>Par exemple, si on a une boucle qui charge tour à tour des nuages de points LIDAR, cette étape peut être utilisée pour identifier à chaque tour de boucle le fichier modèle numérique de terrain (MNT) ayant le même nom que le fichier LIDAR en cours, dans un répertoire dédié.");
}

CT_VirtualAbstractStep* PB_StepLoadFileByName::createNewInstance() const
{
    return new PB_StepLoadFileByName();
}

void PB_StepLoadFileByName::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    const QStringList list_readersList = CT_ReadersTools::constructReadersUniqueNameList(pluginStaticCastT<PB_StepPluginManager>()->readersAvailable());

    preInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), "", list_readersList, m_readerSelectedClassName);
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
    configDialog.addFileChoice(tr("Choisir un fichier exemple"), CT_FileChoiceButton::OneOrMoreExistingFiles, fileFilter, fileList, tr("Ce fichier exemple doit être l'un des fichiers potentiels à charger (peu importe lequel). Il est analysé pour vérifier le format utilisé, mais aussi pour définir dans quel répertoire il faut chercher les fichiers. "));
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
        // get the header
        CT_FileHeader* rHeader = mReader->createHeaderPrototype();

        if(rHeader != nullptr)
        {
            rHeader->setReader(mReader, false);
            manager.addItem(m_hInGroup, m_hOutFileHeader, tr("Entête"), "", "", rHeader);
        }
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
                    if (header == nullptr) {qDebug() << "PB_StepLoadFileByName::compute" << ", " << "header == nullptr";}

                    header->setReader(readerCpy, true);
                    group->addSingularItem(m_hOutFileHeader, header);
                }
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
