#include "pb_stepgenericexporter.h"

#include "ct_exporter/abstract/ct_abstractexporterattributesselection.h"

#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"

#include "ct_itemdrawable/model/inModel/ct_instdsingularitemmodel.h"

#include "ct_itemdrawable/model/outModel/ct_outstdsingularitemmodel.h"
#include "ct_itemdrawable/model/outModel/ct_outstdgroupmodel.h"
#include "ct_itemdrawable/ct_loopcounter.h"

#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/ct_outresultmodelgroupcopy.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_view/ct_stepconfigurabledialog.h"


#include "ct_step/ct_stepinitializedata.h"
#include "ct_step/abstract/ct_abstractsteploadfile.h"

#include "ct_model/tools/ct_modelsearchhelper.h"

#include "tools/pb_exportertools.h"

#include <QFileDialog>

#include <QDebug>

#define DEF_SearchInGroup           "g"
#define DEF_SearchInItemDrawable    "i"
#define DEF_SearchInItemDrawableAtt    "att"
#define DEF_SearchInItemWithName    "iname"
#define DEF_SearchInResultToExport  "r"

#define DEFin_resCounter "resCounter"
#define DEF_inGroupCounter "GroupCounter"
#define DEF_inCounter "counter"


PB_StepGenericExporter::PB_StepGenericExporter(CT_StepInitializeData &dataInit,
                                               CT_AbstractExporter *exporter) : CT_AbstractStep(dataInit)
{
    _adaptative = false;
    _multipleExport = false;
    _otherItem = false;
    _exporter = exporter;
    _exporter->setMyStep(this);

    CT_AbstractExporterAttributesSelection *ex = dynamic_cast<CT_AbstractExporterAttributesSelection*>(_exporter);

    if(ex != NULL)
        ex->setSearchOnlyModels(true);

    _exportPath = ".";

    setStepCustomName(_exporter->getExporterCustomName());

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

void PB_StepGenericExporter::init()
{
    _exporter->init();

    QSettings *settings = _data->getSettingsFile();

    if(settings != NULL)
    {
        settings->beginGroup(getStepName());

        _exportPath = settings->value("DefaultExportPath", ".").toString();

        settings->endGroup();
    }

    CT_AbstractStep::init();
}

QString PB_StepGenericExporter::getStepName() const
{
    return replaceBadCharacters(_exporter->getExporterName());
}

QString PB_StepGenericExporter::getStepDisplayableName() const
{
    return _exporter->getExporterName();
}

QString PB_StepGenericExporter::getStepDetailledDescription() const
{
    if(_exporter != NULL)
    {
        QString t = _exporter->toolTip();

        if(!t.isEmpty())
            return t;
    }

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

    return tr("Exporte un(des) fichier(s) de type :") + tmp;
}

QString PB_StepGenericExporter::getStepDescription() const
{
    return ((CT_AbstractExporter*)_exporter)->getExporterCustomName();
}

CT_VirtualAbstractStep* PB_StepGenericExporter::createNewInstance(CT_StepInitializeData &dataInit)
{
    // cree une copie de cette etape
    CT_VirtualAbstractStep* stepCopy = new PB_StepGenericExporter(dataInit, exporter()->copy());
    stepCopy->setStepCustomName(getStepCustomName());
    return stepCopy;
}

void PB_StepGenericExporter::saveInputSettings(SettingsWriterInterface &writer) const
{
    SuperClass::saveInputSettings(writer);

    _exporter->saveSettings(writer);
}

bool PB_StepGenericExporter::restoreInputSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restoreInputSettings(reader))
        return false;

    setItemsToExportFromModelsToExporter();

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

void PB_StepGenericExporter::createPreConfigurationDialog()
{
    CT_StepConfigurableDialog *configDialog = newStandardPreConfigurationDialog();

    configDialog->addBool(tr("Nom adaptatif (dans une boucle)"), "", "", _adaptative);
    configDialog->addBool(tr("Export Multiple"), "", "", _multipleExport);
    configDialog->addBool(tr("Nom de fichier dans un autre item (si export multiple)"), "", "", _otherItem);
}

//////////////////// PROTECTED //////////////////

void PB_StepGenericExporter::createInResultModelListProtected()
{
    CT_InResultModelGroup *resultModel = createNewInResultModel(DEF_SearchInResultToExport, tr("Résultat"), "", true);

    resultModel->setZeroOrMoreRootGroup();
    resultModel->addStdGroupModel("", DEF_SearchInGroup);

    if(!_exporter->exportOnlyGroup())
        resultModel->addStdItemModel(DEF_SearchInGroup, DEF_SearchInItemDrawable, CT_AbstractSingularItemDrawable::staticGetType(), tr("Item"));

    if (_multipleExport)
    {
        if (_otherItem)
        {
            resultModel->addStdItemModel(DEF_SearchInGroup, DEF_SearchInItemWithName, CT_AbstractSingularItemDrawable::staticGetType(), tr("ItemWithName"));
            resultModel->addStdItemAttributeModel(DEF_SearchInItemWithName, DEF_SearchInItemDrawableAtt, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::STRING, tr("Name"));
        } else {
            resultModel->addStdItemAttributeModel(DEF_SearchInItemDrawable, DEF_SearchInItemDrawableAtt, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::STRING, tr("Name"));
        }
    }

    if (_adaptative)
    {
        CT_InResultModelGroup* resCounter = createNewInResultModel(DEFin_resCounter, tr("Résultat compteur"), "", true);
        resCounter->setStdRootGroup(DEF_inGroupCounter);
        resCounter->addStdItemModel(DEF_inGroupCounter, DEF_inCounter, CT_LoopCounter::staticGetType(), tr("Compteur"));
    }
}

bool PB_StepGenericExporter::configureInputResult(bool forceReadOnly)
{
    if(forceReadOnly)
        return CT_VirtualAbstractStep::configureInputResult(forceReadOnly);

    if(CT_VirtualAbstractStep::configureInputResult(forceReadOnly))
        return configureExporter();

    return false;
}

bool PB_StepGenericExporter::postConfigure()
{
    // méthode appelée après avoir configuré les modèles d'entrées

    QString exportFileName;
    if (_adaptative || _multipleExport)
    {
        exportFileName = QFileDialog::getExistingDirectory(NULL, tr("Exporter dans..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename);
    } else {
        const QString extensions = PB_ExporterTools::constructStringForFileDialog(_exporter);
        exportFileName = QFileDialog::getSaveFileName(NULL, tr("Exporter sous..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename, extensions);
    }

    if(exportFileName.isEmpty())
        return false;

    _exportFilename = exportFileName;

    QFileInfo info(exportFileName);
    setDefaultExportPath(info.absoluteDir().absolutePath());

    _exporter->setExportFilePath(_exportFilename);

    return true;
}

void PB_StepGenericExporter::createOutResultModelListProtected()
{
}

void PB_StepGenericExporter::compute()
{
    CT_ResultGroup* resIn_Counter = NULL;
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

        if (group != NULL)
        {
            if (_exporter->exportOnlyGroup())
            {
                itemsToExport.append(group);
            } else
            {
                CT_AbstractItemDrawable *item = (CT_AbstractItemDrawable*) group->firstItemByINModelName(this, DEF_SearchInItemDrawable);

                CT_AbstractSingularItemDrawable *itemWithName = NULL;
                if (_multipleExport && _otherItem)
                {
                    itemWithName = (CT_AbstractSingularItemDrawable*) group->firstItemByINModelName(this, DEF_SearchInItemWithName);
                }

                if (item != NULL)
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
                CT_AbstractItemAttribute* att = NULL;
                if (itemWithName != NULL)
                {
                    att = itemWithName->firstItemAttributeByINModelName(inResult, this, DEF_SearchInItemDrawableAtt);
                }

                QString fileName = item->displayableName();

                if (att != NULL)
                {
                    fileName = att->toString(itemWithName, NULL);
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
                    //if(_exporterConfiguration != NULL)
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
                //if(_exporterConfiguration != NULL)
                //_exporter->loadExportConfiguration(_exporterConfiguration);

                // et on exporte....
                if(!_exporter->exportToFile()) {
                    PS_LOG->addErrorMessage(this, _exporter->errorMessage());
                    setErrorMessage(1, _exporter->errorMessage());
                    setErrorCode(1);
                }
            }
        }


    }
}


bool PB_StepGenericExporter::configureExporter()
{
    setItemsToExportFromModelsToExporter();

    if(_exporter->configureExport())
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepGenericExporter::setItemsToExportFromModelsToExporter()
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
        CT_InAbstractItemModel *inItemModelToExport = NULL;

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

    if(settings != NULL)
    {
        settings->beginGroup(getStepName());

        settings->setValue("DefaultExportPath", _exportPath);

        settings->endGroup();
    }
}

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

