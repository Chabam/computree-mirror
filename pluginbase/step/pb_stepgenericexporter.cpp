#include "pb_stepgenericexporter.h"

#include "tools/pb_exportertools.h"
#include "ct_view/ct_checkbox.h"
#include "loginterface.h"

#include <QFileDialog>

PB_StepGenericExporter::PB_StepGenericExporter(CT_AbstractExporter *exporter) : SuperClass()
{
    _adaptative = false;
    _multipleExport = false;
    _multipleExportChoice = CT_AbstractExporter::EachItemInSeparateFile_OriginalName_InternalCounter;
    _exporter = exporter;
    _exportPath = ".";

    connect(_exporter, SIGNAL(exportInProgress(int)), this, SLOT(exportProgressChanged(int)));
    connect(this, SIGNAL(stopped()), _exporter, SLOT(cancel()), Qt::DirectConnection);
}

PB_StepGenericExporter::~PB_StepGenericExporter()
{
    delete _exporter;
    _exporter = nullptr;
}

const CT_AbstractExporter* PB_StepGenericExporter::exporter() const
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
    preInputConfigDialog->addBool(tr("Nom du fichier = nom du tour courant + nom du fichier"), "", "", _adaptative, tr("Si coché vous devrez choisir un item de type \"compteur\" et le nom du fichier contiendra le nom du tour courant."));

    const QString multiExportDesc = tr("Chaque item sera exporté dans un fichier différent\r - le nom du fichier à utiliser sera fonction du choix que vous allez faire ci-dessous.");

    if(_exporter->isExportEachItemInSeparateFileOptionnal())
    {
        preInputConfigDialog->addBool(tr("Un item par fichier"), "", "", _multipleExport, tr("Si coché : \r%1").arg(multiExportDesc));
    }
    else
    {
        preInputConfigDialog->addText(tr("Un item par fichier obligatoire (l'exportateur ne peut pas faire autrement) :"), "", "", multiExportDesc);
        _multipleExport = true;
    }

    CT_ButtonGroup& bg = preInputConfigDialog->addButtonGroup(_multipleExportChoice);
    preInputConfigDialog->addExcludeValue(tr("Nom du fichier original"), "", "", bg, CT_AbstractExporter::EachItemInSeparateFile_OriginalName_InternalCounter);
    preInputConfigDialog->addExcludeValue(tr("Nom du fichier contenu dans un attribut"), "", "", bg, CT_AbstractExporter::EachItemInSeparateFile_AttributeOfItem_InternalCounterIfSameFileName, tr("Si coché : \r- L'attribut à relier sera un fils de l'item à exporter"));
    preInputConfigDialog->addExcludeValue(tr("Nom du fichier contenu dans un attribut d'un autre item"), "", "", bg, CT_AbstractExporter::EachItemInSeparateFile_AttributeFromAnotherItem_InternalCounterIfSameFileName, tr("Si coché : \r- L'attribut à relier sera un fils d'un autre item que celui à exporter"));
}

void PB_StepGenericExporter::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_AbstractExporter::ExportWithModelsOption option = CT_AbstractExporter::AllItemsInSameFile_OriginalName;

    if(_multipleExport)
        option = CT_AbstractExporter::ExportWithModelsOption(_multipleExportChoice);

    _exporter->declareInputModels(manager, option, _adaptative);
}

bool PB_StepGenericExporter::postInputConfigure()
{
    QString exportFileName;
    const bool isDir = (_multipleExport && (_multipleExportChoice != CT_AbstractExporter::EachItemInSeparateFile_OriginalName_InternalCounter));

    if(isDir)
        exportFileName = QFileDialog::getExistingDirectory(nullptr, tr("Exporter dans..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename);
    else
        exportFileName = QFileDialog::getSaveFileName(nullptr, tr("Exporter sous..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename, PB_ExporterTools::constructStringForFileDialog(_exporter));

    if(exportFileName.isEmpty())
        return false;

    if(isDir)
        exportFileName = QDir::toNativeSeparators(exportFileName + "/");

    if(!_exporter->configure())
        return false;

    QFileInfo info(exportFileName);
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
}

void PB_StepGenericExporter::exportProgressChanged(int progress)
{
    setProgress(progress);
}

