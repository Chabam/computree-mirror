#include "pb_stepgenericexporter.h"

#include "tools/pb_exportertools.h"
#include "ct_view/ct_checkbox.h"
#include "loginterface.h"

#include <QFileDialog>

PB_StepGenericExporter::PB_StepGenericExporter(CT_AbstractExporter *exporter) : SuperClass()
{
    _adaptative = false;
    _multipleExport = false;
    _otherItem = false;
    _exporter = exporter;
    _exportPath = ".";

    connect(_exporter, SIGNAL(exportInProgress(int)), this, SLOT(exportProgressChanged(int)));
    connect(this, SIGNAL(stopped()), _exporter, SLOT(cancel()), Qt::DirectConnection);
}

PB_StepGenericExporter::~PB_StepGenericExporter()
{
    delete _exporter;
}

const CT_AbstractExporter *PB_StepGenericExporter::exporter() const
{
    return _exporter;
}

QString PB_StepGenericExporter::name() const
{
    return _exporter->uniqueName();
}

QString PB_StepGenericExporter::displayableName() const
{
    return _exporter->displayableName();
}

QString PB_StepGenericExporter::description() const
{
    return tr("%1 : %2").arg(displayableName()).arg(createExportExtensionString());
}

QString PB_StepGenericExporter::detailledDescription() const
{
    QString t = _exporter->toolTip();

    if(!t.isEmpty())
        return t;

    return tr("Exporte un(des) fichier(s) de type :") + createExportExtensionString();
}

/*void PB_StepGenericExporter::init()
{
    QSettings *settings = _data->getSettingsFile();

    if(settings != nullptr)
    {
        settings->beginGroup(getStepName());

        _exportPath = settings->value("DefaultExportPath", ".").toString();

        settings->endGroup();
    }

    SuperClass::init();
}*/

void PB_StepGenericExporter::saveInputSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveInputSettings(writer);

    _exporter->saveSettings(writer);
}

bool PB_StepGenericExporter::restoreInputSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreInputSettings(reader))
        return false;

    return _exporter->restoreSettings(reader);
}

void PB_StepGenericExporter::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    writer.addParameterPath(this, "ExportPath", _exportFilename);
}

bool PB_StepGenericExporter::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    if(!reader.parameterPath(this, "ExportPath", _exportFilename))
        return false;

    return true;
}

CT_VirtualAbstractStep* PB_StepGenericExporter::createNewInstance() const
{
    return new PB_StepGenericExporter(_exporter->copy());
}

QString PB_StepGenericExporter::createExportExtensionString() const
{
    QString tmp;

    const QList<FileFormat> &ext = _exporter->exportFormats();
    QListIterator<FileFormat> it(ext);

    while(it.hasNext())
    {
        FileFormat ff = it.next();
        QListIterator<QString> itS(ff.suffixes());
        while(itS.hasNext())
        {
            tmp += "*." + itS.next();
            if (itS.hasNext()) {tmp += " ";}
        }
    }

    return tmp;
}

void PB_StepGenericExporter::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    preInputConfigDialog->addBool(tr("Nom du fichier = nom du compteur de boucle"), "", "", _adaptative, tr("Si coché vous devrez choisir un item de type \"compteur\" et le nom du fichier contiendra le nom du tour courant."));
    CT_CheckBox* cbEM = preInputConfigDialog->addBool(tr("Un item par fichier"), "", "", _multipleExport, tr("Si coché : \r- vous devrez choisir un attribut d'item contenant une chaine de caractère\r- chaque item sera exporté dans un fichier différent\r - le nom du fichier utilisera la chaine de caractère contenue dans l'attribut."));
    CT_CheckBox* cbOI = preInputConfigDialog->addBool(tr("Nom du fichier contenu dans un attribut d'un autre item"), "", "", _otherItem, tr("Si coché : \r- L'attribut correspondra à l'item que vous devrez coché et non pas à l'item exporté"));

    connect(cbEM, &CT_CheckBox::valueChanged, cbOI, &CT_CheckBox::setEnabled);
}

void PB_StepGenericExporter::declareInputModels(CT_StepInModelStructureManager& manager)
{
    _exporter->declareInputModels(manager, _adaptative, _multipleExport, _otherItem);
}

bool PB_StepGenericExporter::postInputConfigure()
{
    QString exportFileName;

    if (_adaptative || _multipleExport)
        exportFileName = QFileDialog::getExistingDirectory(nullptr, tr("Exporter dans..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename);
    else
        exportFileName = QFileDialog::getSaveFileName(nullptr, tr("Exporter sous..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename, PB_ExporterTools::constructStringForFileDialog(_exporter));

    if(exportFileName.isEmpty())
        return false;

    if(!_exporter->configure())
        return false;

    QFileInfo info(exportFileName);
    //setDefaultExportPath(info.absoluteDir().absolutePath());
    _exportFilename = exportFileName;
    _exporter->setFilePath(_exportFilename);

    setSettingsModified(true);

    return true;
}

void PB_StepGenericExporter::declareOutputModels(CT_StepOutModelStructureManager&)
{
    // no output !
}

void PB_StepGenericExporter::compute()
{
    if(!_exporter->exportToFile())
    {
        PS_LOG->addErrorMessage(LogInterface::step, _exporter->errorMessage());
        setErrorMessage(1, _exporter->errorMessage());
        setErrorCode(1);
    }
    /*CT_ResultGroup* resIn_Counter = nullptr;
    QString baseName = "";

    if (_adaptative)
    {
        QList<CT_ResultGroup*> inResultList = getInputResults();

        resIn_Counter = inResultList.at(1);
        CT_ResultItemIterator it(resIn_Counter, this, DEF_inCounter);
        if (it.hasNext())
        {
            CT_LoopCounter* counter = (CT_LoopCounter*) it.next();
            baseName = counter->getTurnName();
            baseName = QFileInfo(baseName).baseName();
        }
    }

    QList<CT_AbstractItemDrawable*> itemsToExport;
    QList<CT_AbstractSingularItemDrawable*> itemsWithNames;

    CT_ResultGroup *inResult = getInputResults().first();
    CT_ResultGroupIterator it(inResult, this, DEF_SearchInGroup);
    while(!isStopped() && it.hasNext())
    {
        CT_StandardItemGroup *group = (CT_StandardItemGroup*) it.next();

        if (group != nullptr)
        {
            if (_exporter->exportOnlyGroup())
            {
                itemsToExport.append(group);
            } else
            {
                CT_AbstractItemDrawable *item = (CT_AbstractItemDrawable*) group->firstItemByINModelName(this, DEF_SearchInItemDrawable);

                CT_AbstractSingularItemDrawable *itemWithName = nullptr;
                if (_multipleExport && _otherItem)
                {
                    itemWithName = (CT_AbstractSingularItemDrawable*) group->firstItemByINModelName(this, DEF_SearchInItemWithName);
                }

                if (item != nullptr)
                {
                    itemsToExport.append(item);

                    if (_multipleExport)
                    {
                        if (_otherItem)
                        {
                            itemsWithNames.append(itemWithName);
                        } else {
                            itemsWithNames.append((CT_AbstractSingularItemDrawable*)item);
                        }
                    }
                }
            }
        }
    }


    // une fois la liste constituée
    if(!itemsToExport.isEmpty())
    {
        _exporter->clearErrorMessage();

        if (_multipleExport)
        {
            for (int i = 0 ; i < itemsToExport.size() ; i++)
            {
                CT_AbstractItemDrawable* item = (CT_AbstractItemDrawable*) itemsToExport.at(i);
                CT_AbstractSingularItemDrawable* itemWithName = (CT_AbstractSingularItemDrawable*) itemsWithNames.at(i);
                CT_AbstractItemAttribute* att = nullptr;
                if (itemWithName != nullptr)
                {
                    att = itemWithName->firstItemAttributeByINModelName(inResult, this, DEF_SearchInItemDrawableAtt);
                }

                QString fileName = item->displayableName();

                if (att != nullptr)
                {
                    fileName = att->toString(itemWithName, nullptr);
                    fileName = QFileInfo(fileName).baseName();
                }

                QString path = _exportFilename;
                path.append("/");

                if (_adaptative)
                {
                    path.append(baseName);
                    path.append("_");
                }

                path.append(fileName);
                _exporter->setExportFilePath(path);

                QList<CT_AbstractItemDrawable*> listTmp;
                listTmp.append(item);

                // on la donne à l'exportateur
                if(!_exporter->setItemDrawableToExport(listTmp))
                {
                    PS_LOG->addErrorMessage(this, _exporter->errorMessage());
                    setErrorMessage(1, _exporter->errorMessage());
                    setErrorCode(1);
                } else
                {
                    //if(_exporterConfiguration != nullptr)
                    //_exporter->loadExportConfiguration(_exporterConfiguration);

                    // et on exporte....
                    if(!_exporter->exportToFile()) {
                        PS_LOG->addErrorMessage(this, _exporter->errorMessage());
                        setErrorMessage(1, _exporter->errorMessage());
                        setErrorCode(1);
                    }
                }
            }


        } else {

            if (_adaptative)
            {
                QString path = _exportFilename;
                path.append("/");
                path.append(baseName);
                _exporter->setExportFilePath(path);

            } else {
                _exporter->setExportFilePath(_exportFilename);
            }

            // on la donne à l'exportateur
            if(!_exporter->setItemDrawableToExport(itemsToExport))
            {
                PS_LOG->addErrorMessage(this, _exporter->errorMessage());
                setErrorMessage(1, _exporter->errorMessage());
                setErrorCode(1);
            } else
            {
                //if(_exporterConfiguration != nullptr)
                //_exporter->loadExportConfiguration(_exporterConfiguration);

                // et on exporte....
                if(!_exporter->exportToFile()) {
                    PS_LOG->addErrorMessage(this, _exporter->errorMessage());
                    setErrorMessage(1, _exporter->errorMessage());
                    setErrorCode(1);
                }
            }
        }


    }*/
}

/*void PB_StepGenericExporter::setItemsToExportFromModelsToExporter()
{
    // on récupère le modèle d'entrée qu'on avait défini
    CT_InResultModelGroup *resModel = (CT_InResultModelGroup*)getInResultModel(DEF_SearchInResultToExport);

    // on récupère les possibilités pour ce résultat (il n'y en a qu'une puisque un résultat d'entrée = un résultat de sortie)
    QList<CT_InStdModelPossibility*> possibilities = resModel->getPossibilitiesSelected();

    QListIterator<CT_InStdModelPossibility*> it(possibilities);

    QList<CT_AbstractItemDrawable*> eItems;

    while(it.hasNext())
    {
        // on récupère le modèle d'entrée qu'on avait défini (celui à exporter)
        CT_InAbstractItemModel *inItemModelToExport = nullptr;

        if(_exporter->exportOnlyGroup())
            inItemModelToExport = (CT_InAbstractItemModel*)PS_MODELS->searchModel(DEF_SearchInGroup, (CT_OutAbstractResultModel*)it.next()->outModel(), this);
        else
            inItemModelToExport = (CT_InAbstractItemModel*)PS_MODELS->searchModel(DEF_SearchInItemDrawable, (CT_OutAbstractResultModel*)it.next()->outModel(), this);

        // on récupère sa possibilité (il n'y en a qu'une puisqu'on a mis CT_InStandardItemDrawableModel::C_ChooseOneIfMultiple)
        CT_InStdModelPossibility *possibility = inItemModelToExport->getPossibilitiesSelected().first();

        // on récupère l'ItemDrawable choisi
        eItems.append(((CT_OutAbstractItemModel*)possibility->outModel())->itemDrawable());
    }

    _exporter->clearErrorMessage();

    // on les donne à l'exporter
    _exporter->setItemDrawableToExport(eItems);
}

void PB_StepGenericExporter::setDefaultExportPath(const QString &path)
{
    _exportPath = path;

    QSettings *settings = _data->getSettingsFile();

    if(settings != nullptr)
    {
        settings->beginGroup(getStepName());

        settings->setValue("DefaultExportPath", _exportPath);

        settings->endGroup();
    }
}*/

void PB_StepGenericExporter::exportProgressChanged(int progress)
{
    setProgress(progress);
}

QString PB_StepGenericExporter::replaceBadCharacters(const QString &name) const
{
    QString value = name;
    value.replace(QRegExp("[àáâãäå]"), "a");
    value.replace(QRegExp("[ÀÁÂÃÄÅ]"), "A");
    value.replace(QRegExp("[éèëê]"), "e");
    value.replace(QRegExp("[ÈÉÊË]"), "E");
    value.replace(QRegExp("[ìíîï]"), "i");
    value.replace(QRegExp("[ÌÍÎÏ]"), "I");
    value.replace(QRegExp("[òóôõöø]"), "o");
    value.replace(QRegExp("[ÒÓÔÕÖØ]"), "O");
    value.replace(QRegExp("[ùúûü]"), "u");
    value.replace(QRegExp("[ÙÚÛÜ]"), "U");
    value.replace(QRegExp("[ñ]"), "n");
    value.replace(QRegExp("[Ñ]"), "N");
    value.replace(QRegExp("[ç]"), "c");
    value.replace(QRegExp("[Ç]"), "C");
    value.replace(QRegExp("[\\W]"), "_");
    return value;
}

