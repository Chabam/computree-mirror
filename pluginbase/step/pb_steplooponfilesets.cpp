#include "pb_steplooponfilesets.h"

#include "pb_steppluginmanager.h"

#include "ct_view/ct_genericconfigurablewidget.h"
#include "ct_abstractstepplugin.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "ct_view/ct_combobox.h"

#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_itemdrawable/ct_readeritem.h"
#include "ct_itemdrawable/ct_itemattributelist.h"
#include "ct_view/tools/ct_configurablewidgettodialog.h"

#include "tools/pb_readerstools.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

PB_StepLoopOnFileSets::PB_StepLoopOnFileSets() : CT_StepBeginLoop()
{
}

QString PB_StepLoopOnFileSets::description() const
{
    return tr("4- Loops on files sets defined in a text file");
}

QString PB_StepLoopOnFileSets::detailledDescription() const
{
    return tr("");
}

QString PB_StepLoopOnFileSets::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStepCanBeAddedFirst::getStepURL(); //by default URL of the plugin
}

CT_VirtualAbstractStep* PB_StepLoopOnFileSets::createNewInstance()
{
    return new PB_StepLoopOnFileSets();
}

void PB_StepLoopOnFileSets::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    writer.addParameterPath(this, "SetsFile", m_setsFilePath, tr("Chemin du fichier contenant en première colonne les noms et en deuxième colonne les chemins des fichiers à traiter (sans en-tête)."));
    writer.addParameterPath(this, "Directory", m_exampleFileFolderPath, tr("Chemin du dossier contenant les fichiers à traiter."));

    const CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(reader != nullptr)
        reader->saveSettings(writer);
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

    CT_AbstractReader* aReader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if((aReader == nullptr) || !aReader->restoreSettings(reader))
        return false;

    return true;
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepLoopOnFileSets::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();

    const QStringList list_readersList = PB_ReadersTools::constructReadersClassNameList(getPluginAs<PB_StepPluginManager>()->readersAvailable());

    postInputConfigDialog->addStringChoice(tr("Choose file type"), "", list_readersList, m_readerSelectedClassName);
}

bool PB_StepLoopOnFileSets::postConfigure()
{
    CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);
    const QString fileFilter = PB_ReadersTools::constructStringForFileDialog(reader);

    if(fileFilter.isEmpty()) {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun reader sélectionné"));
        return false;
    }

    QStringList fileList;
    QStringList setsList;

    CT_GenericConfigurableWidget configDialog;

    configDialog.addFileChoice(tr("File with sets"), CT_FileChoiceButton::OneExistingFile, tr("Fichier texte (*.txt) ; Fichier texte (*.*)"), setsList, tr("First column must contain set name, Second column must contain file path. No header."));

    configDialog.addEmpty();

    configDialog.addFileChoice(tr("Choisir un fichier exemple"), CT_FileChoiceButton::OneExistingFile, fileFilter, fileList);
    configDialog.addTitle(tr("Le fichier choisi doit :"));
    configDialog.addText("", tr("- Etre dans le répertoire des fichiers à charger"), "");
    configDialog.addText("", tr("- Avoir le même format que les fichiers à charger"), "");
    configDialog.addText("", tr("- Avoir la même structure / version que les fichiers à charger"), "");

    if(CT_ConfigurableWidgetToDialog::exec(&configDialog) == QDialog::Accepted) {

        if(fileList.isEmpty())
            return false;

        if(setsList.isEmpty())
            return false;

        if(reader->setFilePath(fileList.first())) {
            reader->setFilePathCanBeModified(false);
            bool ok = reader->configure();
            reader->setFilePathCanBeModified(true);

            if(ok) {
                m_setsFilePath = setsList.first();
                m_exampleFileFolderPath = QFileInfo(fileList.first()).path();
                setSettingsModified(true);
            }

            return ok;
        }
    }

    return false;
}

void PB_StepLoopOnFileSets::createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel)
{
    Q_UNUSED(firstResultModel);

    // create a new result
    CT_OutResultModelGroup *outRes = createNewOutResultModel(DEFout_res, tr("Liste de readers"));

    // add a root group
    outRes->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
    outRes->addItemModel(DEFout_grp, DEFout_plotname, new CT_ItemAttributeList(), tr("GroupName"));
    outRes->addItemAttributeModel(DEFout_plotname, DEFout_plotnameAtt, new CT_StdItemAttributeT<QString>(CT_AbstractCategory::DATA_VALUE), tr("Name"));

    // get the reader selected
    CT_AbstractReader *reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    // if one reader was selected and at least one file is defined
    if (reader != nullptr && !m_setsFilePath.isEmpty())
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

void PB_StepLoopOnFileSets::compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group)
{
    Q_UNUSED(outRes);
    Q_UNUSED(group);

    QList<CT_ResultGroup*> outResultList = getOutResultList();

    // get the out result
    CT_ResultGroup* resultOut = outResultList.at(1);

    const int currentTurn = int(_counter->getCurrentTurn());

    const CT_AbstractReader* reader = getPluginAs<PB_StepPluginManager>()->readerAvailableByClassName(m_readerSelectedClassName);

    if(reader == nullptr)
        return;

    if (currentTurn == 1)
    {
        if(m_setsFilePath.isEmpty()) {
            _counter->setNTurns(1);
            return;
        }

        int nturns = 0;

        if (!QFileInfo(m_setsFilePath).exists())
            return;

        QFile setFile(m_setsFilePath);
        if (setFile.exists() && setFile.open(QFile::Text | QFile::ReadOnly))
        {
            QTextStream stream(&setFile);

            while (!stream.atEnd())
            {
                QString line = stream.readLine();

                QStringList vals = line.split(QRegExp("[\t;,]"));
                if (vals.size() > 1)
                {
                    _sets.insert(vals.at(0), vals.at(1));
                    if (!_setKeys.contains(vals.at(0))) {_setKeys.append(vals.at(0)); nturns++;}
                }
            }
            setFile.close();
        }

        _counter->setNTurns(nturns);
    }

    // search the model for headers
    CT_OutAbstractItemModel* headerModel = (CT_OutAbstractItemModel*)PS_MODELS->searchModelForCreation(DEFout_header, resultOut);

    // create the root group and add it to result
    CT_StandardItemGroup* grp = new CT_StandardItemGroup(DEFout_grp, resultOut);
    resultOut->addGroup(grp);

    // for each current file in the list
    // copy the reader (copyFull = with configuration and models)

    if (_setKeys.size() >= _counter->getCurrentTurn())
    {
        QString grpKey = _setKeys.at((int)_counter->getCurrentTurn() - 1);
        QStringList filesToRead = _sets.values(grpKey);

        _counter->setTurnName(grpKey);

        CT_ItemAttributeList* attList = new CT_ItemAttributeList(DEFout_plotname, resultOut);
        attList->addItemAttribute(new CT_StdItemAttributeT<QString>(DEFout_plotnameAtt,
                                                                    CT_AbstractCategory::DATA_VALUE,
                                                                    resultOut,
                                                                    grpKey));
        grp->addItemDrawable(attList);

        for (int f = 0 ; f < filesToRead.size() ; f++)
        {
            CT_AbstractReader* readerCpy = reader->copyFull();
            const QList<FileFormat> &formats = readerCpy->readableFormats();

            QString filepath = filesToRead.at(f);

            if (!m_exampleFileFolderPath.isEmpty())
                filepath.push_front(QString("%1%2").arg(m_exampleFileFolderPath).arg("/"));

            if (formats.size() > 0)
            {
                QFileInfo filepathInfo(filepath);
                filepath = QString("%1/%2.%3").arg(filepathInfo.path()).arg(filepathInfo.baseName()).arg(formats.first().suffixes().first());
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
                PS_LOG->addMessage(LogInterface::warning, LogInterface::step, tr("Fichier %1 inexistant ou non valide").arg(filepath));
                delete readerCpy;
            }
        }
    }

}
