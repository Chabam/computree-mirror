#include "pb_stepcreatereaderlist.h"

#include "pb_steppluginmanager.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"
#include "tools/ct_readerstools.h"

#include <QMessageBox>

// Constructor : initialization of parameters
PB_StepCreateReaderList::PB_StepCreateReaderList() : SuperClass(),
    mReader(nullptr)
{
    m_choiceMode = 0;
}

PB_StepCreateReaderList::~PB_StepCreateReaderList()
{
    delete mReader;
}

// Step description (tooltip of contextual menu)
QString PB_StepCreateReaderList::description() const
{
    return tr("Créer une liste de fichiers");
}

// Step detailled description
QString PB_StepCreateReaderList::detailledDescription() const
{
    return tr("Cette étape créer une liste fichiers. Cette étape ne charge pas les fichiers, elle créée juste les entêtes. Il faut ensuite ajouter l'étape \"Charger les fichiers d'une liste\" pour les charger, soit directement, soit après un début de boucle. ");
}

QString PB_StepCreateReaderList::outputDescription() const
{
    return SuperClass::outputDescription() + tr("<br><br>Cette étape génère un résultat contenant la liste des fichiers.<br>"
                                                "Pour chaque fichier deux éléments sont créés :<br>"
                                                "<ul>"
                                                "<li>Une entête contenant les informations générales sur le fichier, dépendant du format. A minima cette entête contient le nom et le chemin du fichier.</li>"
                                                "<li>Un \"reader\", qui est l'objet utilisé pour faire le chargement effectif du fichier dans la suite du script.</li>"
                                                "</ul>");
}

QString PB_StepCreateReaderList::detailsDescription() const
{
    return tr("L'utilisation la plus habituelle de cette étape est de traiter successivement une liste de fichiers, à l'aide d'une boucle ajoutée après cette étape. "
                "Dans ce cas, la structure du script est la suivante :"
                "<ol>"
                "<li><strong>Créer une liste de fichier</strong> (séléction de la liste des fichiers à parcourir)</li>"
                "<li>Boucle standard (début de la boucle)</li>"
                "<li>Charger les fichiers d'une liste (chargement du fichier correspondant au tour de boucle courant)</li>"
                "<li>... (étapes de traitement et d'export)</li>"
                "<li>Fin de boucle</li>"
                "</ol>");
}


// Step copy method
CT_VirtualAbstractStep* PB_StepCreateReaderList::createNewInstance() const
{
    return new PB_StepCreateReaderList();
}

void PB_StepCreateReaderList::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    const QStringList list_readersList = CT_ReadersTools::constructReadersUniqueNameList(pluginStaticCastT<PB_StepPluginManager>()->readersAvailable());

    preInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), "", list_readersList, m_readerSelectedUniqueName);

    CT_ButtonGroup &bg_choiceMode = preInputConfigDialog->addButtonGroup(m_choiceMode);

    preInputConfigDialog->addText(tr("Méthode de choix des fichiers :"),"", "");
    preInputConfigDialog->addExcludeValue("", "", tr("Sélection manuelle dans un répertoire"), bg_choiceMode, 0, tr("Si cette option est sélectionnée, l'utilisateur choisit manuellement les fichiers dans un répertoire."), true);
    preInputConfigDialog->addExcludeValue("", "", tr("Utilisation d'un fichier contenant les chemins"), bg_choiceMode, 1, tr("Si cette option est sélectionnée, l'utilisateur doit choisir un fichier texte, qui contient sur chaque ligne un chemin de fichier. Cela permet de préparer des lots de fichiers prédéfinis."), true);
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
    const QString fileFilter = CT_ReadersTools::constructStringForFileDialog(mReader);

    if(fileFilter.isEmpty()) {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun reader sélectionné"));
        return false;
    }

    QStringList fileList = m_filepathCollection;


    bool accepted = false;
    CT_GenericConfigurableWidget configDialog;
    if (m_choiceMode == 0)
    {
        configDialog.addFileChoice(tr("Choisir les fichiers"), CT_FileChoiceButton::OneOrMoreExistingFiles, fileFilter, fileList);

        if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted)
        {
            accepted = true;
        }
    } else {
        fileList.clear();

        QStringList textFile;
        configDialog.addFileChoice(tr("Choisir le fichier contenant les chemins"), CT_FileChoiceButton::OneExistingFile, tr("Fichier texte, *.*"), textFile);

        if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted)
        {
            accepted = true;
            if (textFile.size() > 0)
            {
                QFile ftxt(textFile.first());
                if (ftxt.exists() && ftxt.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream stream(&ftxt);
                    while (!stream.atEnd())
                    {
                        QString line = stream.readLine();
                        if (!line.isEmpty())
                        {
                            fileList.append(line);
                        }
                    }
                    ftxt.close();
                }
            }
        }
    }


    if (accepted)
    {
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
        // get the header
        CT_FileHeader* rHeader = mReader->createHeaderPrototype();
        rHeader->setReader(mReader, false);

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
                    if (header == nullptr) {qDebug() << "PB_StepCreateReaderList::compute" << ", " << "header == nullptr";}

                    header->setReader(readerCpy, true);
                    grpHeader->addSingularItem(m_hOutFileHeader, header);
                }
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
