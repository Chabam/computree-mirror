#include "pb_steplooponfilesets.h"

#include "pb_steppluginmanager.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"
#include "tools/ct_readerstools.h"
#include "ct_log/ct_logmanager.h"

#include <QMessageBox>
#include <QProgressDialog>
#include <QDir>
#include <QApplication>

PB_StepLoopOnFileSets::PB_StepLoopOnFileSets() : CT_StepBeginLoop(),
    mReader(nullptr)
{
}

PB_StepLoopOnFileSets::~PB_StepLoopOnFileSets()
{
    if (mReader != nullptr) {delete mReader;}
}

QString PB_StepLoopOnFileSets::description() const
{
    return tr("Boucle sur des groupes de fichiers");
}

QString PB_StepLoopOnFileSets::detailledDescription() const
{
    return tr("Cette étape démarre une boucle de script. "
        "Pour cela elle utilise un fichier de correspondance entre des noms de groupes et des fichiers."
        "A chaque tour de boucle, cette étape créée une liste des fichiers appartenant à l'un des groupes.<br>"
        "Les groupes sont définis dans un fichier de paramétrage ASCII (sans ligne d'entête) où :"
        "<ul>"
        "<li>La première colone indique le nom des groupes.</li>"
        "<li>La seconde le chemin des fichiers correspondants.</li>"
        "</ul>");
}

QString PB_StepLoopOnFileSets::outputDescription() const
{
    return  SuperClass::outputDescription() + tr("<br><br>Cette étape génère deux résultats :<br>"
                "<ul>"
                "<li>Un résultat Compteur, permettant la gestion de la boucle. Ce compteur est en particulier utilisé pour connaître le nom du tour courant.</li>"
                "<li>Un résultat contenant le nom du groupe, ainsi que la liste des fichiers de ce groupe (non chargés à ce stade)<./li>"
                "</ul>");
}

QString PB_StepLoopOnFileSets::detailsDescription() const
{
    return tr("Cette étape est utile pour charger des séries de fichiers devant être traités ensembles.<br>"
            "Le nom de groupe pourra également être utilisé, par exemple pour charger un autre fichier lié au groupe, en utilisant l'étape \"Créer un sélecteur de fichier par attribut de nom\".<br><br>"
            "Un exemple d'utilisation : charger un nuage de point d'une placette LIDAR, ainsi que les nuages de points des arbres de cette placette (préalablement segmentés).<br>"
            "Dans le fichier de correspondance on mettra<br>"
            "<ul>"
            "<li>Le nom dess placette en tant que noms de groupes en première colonne.</li>"
            "<li>Le nom de chaque fichier arbre en tant que noms de fichiers en seconde colonne.</li>"
            "</ul>"
            "Cette étape créera la liste des fichiers pour chacune des placettes à chacun des tours de boucle. Ils pourront ensuite être chargés dans la boucle à l'aide de l'étape \"Charger les fichiers d'une liste\".<br>"
            "Ensuite, en utilisant l'étape \"Créer un sélecteur de fichier par attribut de nom\", suivie de l'étape  \"Charger les fichiers d'une liste\", on pourra charger le fichier de la placette, pour peu que celui-ci soit nommé conformément au nom de placette utilisé comme nom de groupe. ");
}


CT_VirtualAbstractStep* PB_StepLoopOnFileSets::createNewInstance() const
{
    return new PB_StepLoopOnFileSets();
}

void PB_StepLoopOnFileSets::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    const QStringList list_readersList = CT_ReadersTools::constructReadersUniqueNameList(pluginStaticCastT<PB_StepPluginManager>()->readersAvailable());

    preInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), "", list_readersList, m_readerSelectedUniqueName, tr("Le type choisi doit correspondre aux fichiers listés dans le fichier de correspondance. "));
}

void PB_StepLoopOnFileSets::finalizePreSettings()
{
    SuperClass::finalizePreSettings();

    CT_AbstractReader* reader = pluginStaticCastT<PB_StepPluginManager>()->readerAvailableByUniqueName(m_readerSelectedUniqueName);

    if(reader == nullptr) {
        if (mReader != nullptr) {delete mReader;}
        mReader = nullptr;
        return;
    }

    if((mReader == nullptr) || (reader->uniqueName() != mReader->uniqueName()))
    {
        if (mReader != nullptr) {delete mReader;}
        mReader = reader->copyFull();
    }
}

bool PB_StepLoopOnFileSets::postInputConfigure()
{
    const QString fileFilter = CT_ReadersTools::constructStringForFileDialog(mReader);

    if(fileFilter.isEmpty()) {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun reader sélectionné"));
        return false;
    }

    QStringList folders;
    folders.append(m_exampleFileFolderPath);

    QStringList setsFilePath;
    setsFilePath.append(m_setsFilePath);

    CT_GenericConfigurableWidget configDialog;

    configDialog.addFileChoice(tr("Choix du fichier de correspondance entre groupes et fichiers"), CT_FileChoiceButton::OneExistingFile, tr("Fichier texte (*.txt) ; Fichier texte (*.*)"), setsFilePath, tr("La première colonne doit contenir le nom des groupes, la seconde colonne doit contenir le nom des fichiers du groupe (avec ou sans extension). Le fichier ne doit pas avoir de ligne d'en-tête."));
    configDialog.addEmpty();
    configDialog.addFileChoice(tr("Choix du dossier contenant les fichiers listés dans le fichier de correspondance"), CT_FileChoiceButton::OneExistingFolder, "", folders);

    if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted) {

        if(folders.isEmpty() || setsFilePath.isEmpty()) {
            QMessageBox::critical(nullptr, tr("Erreur"), tr("Fichier ou dossier non sélectionné"));
            return false;
        }

        const QString setFilePath = setsFilePath.first();
        const QString folderPath = folders.first();
        int nFiles;
        const QList<Set> sets = ReadSetsInFile(setFilePath, folderPath, mReader, &nFiles);

        if(sets.isEmpty()) {
            QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun lot trouvé dans le fichier \"%1\"").arg(setFilePath));
            return false;
        }

        int progress = 0;
        QProgressDialog progressD(tr("Vérification des fichiers en cours..."), tr("Annuler"), 0, nFiles);
        progressD.setValue(progress);
        progressD.show();

        QString firstFilePath;

        for(const Set& set : sets)
        {
            for(const QString& fp : set.mFilesPath)
            {
                if(!mReader->setFilePath(fp)) {
                    QMessageBox::critical(nullptr, tr("Erreur"), tr("Fichier \"%1\" non accepté par le reader").arg(fp));
                    return false;
                }

                if(firstFilePath.isEmpty())
                    firstFilePath = fp;

                progressD.setValue(++progress);
                qApp->processEvents();

                if(progressD.wasCanceled())
                    return false;
            }
        }

        progressD.close();

        mReader->setFilePath(firstFilePath);
        mReader->setFilePathCanBeModified(false);
        const bool ok = mReader->configure();
        mReader->setFilePathCanBeModified(true);

        if(!ok)
            return false;

        m_setsFilePath = setFilePath;
        m_exampleFileFolderPath = folderPath;
        setSettingsModified(true);

        return true;
    }

    return false;
}

void PB_StepLoopOnFileSets::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    SuperClass::declareOutputModels(manager);

    manager.addResult(m_hOutResultLOFS, tr("Liste de readers"));
    manager.setRootGroup(m_hOutResultLOFS, m_hOutRootGroupLOFS);
    manager.addGroup(m_hOutRootGroupLOFS, m_hOutSetGroupLOFS, tr("Groupes"));
    manager.addItem(m_hOutSetGroupLOFS, m_hOutSetItemLOFS, tr("Groupe"));
    manager.addItemAttribute(m_hOutSetItemLOFS, m_hOutSetItemAttributeLOFS, CT_AbstractCategory::DATA_VALUE, tr("Nom du Groupe"));
    manager.addGroup(m_hOutRootGroupLOFS, m_hOutFileGroupLOFS, tr("Fichiers du groupe"));

    // if one reader was selected
    if(mReader != nullptr)
    {
        // get the header
        CT_FileHeader* rHeader = mReader->createHeaderPrototype();

        if(rHeader != nullptr)
        {
            rHeader->setReader(mReader, false);
            manager.addItem(m_hOutFileGroupLOFS, m_hOutFileHeaderLOFS, tr("Entête"), "", "", rHeader);
        }
    }
}

void PB_StepLoopOnFileSets::compute()
{
    SuperClass::compute();

    if(_counter->currentTurn() == 1)
    {
        _sets = ReadSetsInFile(m_setsFilePath, m_exampleFileFolderPath, mReader);
        _counter->setNTurns(_sets.size());
    }

    const int index = _counter->currentTurn() - 1;

    if((index >= 0) && (index < _sets.size()))
    {
        const Set& set = _sets.at(index);
        _counter->setTurnName(set.mName);

        addToLogCurrentTurnInformation();

        for(CT_ResultGroup* result : m_hOutResultLOFS.iterateOutputs()) {

            CT_StandardItemGroup* rootGroup = m_hOutRootGroupLOFS.createInstance();
            result->addRootGroup(m_hOutRootGroupLOFS, rootGroup);

            CT_StandardItemGroup* setGroup = m_hOutSetGroupLOFS.createInstance();
            rootGroup->addGroup(m_hOutSetGroupLOFS, setGroup);

            CT_ItemAttributeList* attList = m_hOutSetItemLOFS.createInstance();
            setGroup->addSingularItem(m_hOutSetItemLOFS, attList);

            attList->addItemAttribute(m_hOutSetItemAttributeLOFS, new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_VALUE, set.mName));

            for(const QString& filepath : set.mFilesPath)
            {
                // for each current file in the list

                // set the new filepath and check if it is valid
                if(mReader->setFilePath(filepath))
                {
                    PS_LOG->addMessage(LogInterface::info, LogInterface::step, tr("Ajout du fichier %1 du lot %2").arg(filepath).arg(set.mName));

                    // create the group that will contains header and reader (represent a File)
                    CT_StandardItemGroup* grpHeader = new CT_StandardItemGroup();
                    rootGroup->addGroup(m_hOutFileGroupLOFS, grpHeader);

                    // add the header
                    if(m_hOutFileHeaderLOFS.isValid()) {
                        CT_FileHeader* header = mReader->readHeader();
                        Q_ASSERT(header != nullptr);
                        if (header == nullptr) {qDebug() << "PB_StepLoopOnFileSets::compute" << ", " << "header == nullptr";}

                        header->setReader(mReader, false);
                        grpHeader->addSingularItem(m_hOutFileHeaderLOFS, header);
                    }
                }
                else
                {
                    PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 non valide").arg(filepath));
                }

                if(isStopped())
                    return;
            }
        }
    }
}

void PB_StepLoopOnFileSets::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    writer.addParameterPath(this, "SetsFile", m_setsFilePath, tr("Chemin du fichier contenant en première colonne les noms des lots et en deuxième colonne les noms des fichiers à traiter (le fichier ne doit pas avoir d'en-tête)."));
    writer.addParameterPath(this, "Directory", m_exampleFileFolderPath, tr("Chemin du dossier contenant les fichiers à traiter."));

    mReader->saveSettings(writer);
}

bool PB_StepLoopOnFileSets::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    QString path;

    if(!reader.parameterPath(this, "SetsFile", path))
        return false;

    m_setsFilePath = path;

    if(!reader.parameterPath(this, "Directory", path))
        return false;

    m_exampleFileFolderPath = path;

    if(mReader == nullptr)
        return false;

    return mReader->restoreSettings(reader);
}

QList<PB_StepLoopOnFileSets::Set> PB_StepLoopOnFileSets::ReadSetsInFile(const QString& filepath, const QString& folderPath, CT_AbstractReader* reader, int* countFile)
{
    QList<Set> l;

    if(countFile != nullptr)
        (*countFile) = 0;

    QFile setFile(filepath);
    if (!setFile.exists() || !setFile.open(QFile::Text | QFile::ReadOnly))
        return l;

    QTextStream stream(&setFile);
    const QRegExp regExp("[\t;,]");

    QHash<QString, int> setsPos;

    QDir dir(folderPath);

    const QStringList fileFilters = CT_ReadersTools::constructStringListToFilterFiles(reader);
    const bool noFileFilter = fileFilters.contains("*.*");

    int nFile = 0;
    while(!stream.atEnd())
    {
        const QString line = stream.readLine();
        const QStringList vals = line.split(regExp);

        if(vals.size() > 1)
        {
            const QString filename = vals.at(1);
            const QFileInfoList entries = dir.entryInfoList(QStringList() << (filename + "*"), QDir::Files);

            if(!entries.isEmpty())
            {
                QFileInfo first = entries.first();

                if(noFileFilter || fileFilters.contains("*." + first.suffix()))
                {
                    const QString& absoluteFilepath = entries.first().absoluteFilePath();

                    const int pos = setsPos.value(vals.at(0), -1);

                    if(pos == -1) {
                        Set s;
                        s.mName = vals.at(0);
                        s.mFilesPath.append(absoluteFilepath);

                        setsPos.insert(s.mName, l.size());
                        l.append(s);
                    } else {
                        Set& s = l[pos];
                        s.mFilesPath.append(absoluteFilepath);
                    }

                    ++nFile;
                }
            }
        }
    }

    setFile.close();

    if(countFile != nullptr)
        (*countFile) = nFile;

    return l;
}
