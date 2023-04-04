#include "pb_stepgenericexporter.h"

#include "tools/ct_exportertools.h"
#include "ct_view/ct_checkbox.h"
#include "loginterface.h"

#include <QFileDialog>

PB_StepGenericExporter::PB_StepGenericExporter(CT_AbstractExporter *exporter) : SuperClass()
{
    _adaptative = false;
    _multipleExport = 0;
    _multipleExportChoice = CT_AbstractExporter::EachItemInSeparateFile_AttributeFromAnotherItem_InternalCounterIfSameFileName;
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
    // "EXPORT" is important because an exporter can have the same name than a reader !
    return "EXPORT " + _exporter->uniqueName();
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

    const QString multiExportDesc = tr("Chaque item sera exporté dans un fichier différent\nLe nom du fichier à utiliser sera fonction du choix que vous allez faire ci-dessous.");

    if(_exporter->isExportEachItemInSeparateFileOptionnal())
    {

        CT_ButtonGroup& bgex = preInputConfigDialog->addButtonGroup(_multipleExport);

        preInputConfigDialog->addExcludeValue("", "", tr("Exporter un fichier unique"), bgex, 0, tr("Si coché : \nUn unique ficher de sortie sera créé, contenant toutes le données. "), true);
        preInputConfigDialog->addExcludeValue("", "", tr("Exporter un fichier par item"), bgex, 1, tr("Si coché : \r%1").arg(multiExportDesc), true);

        preInputConfigDialog->addEmpty();
        preInputConfigDialog->addText(tr("Dans le cas de l'export d'un fichier par item, comment déterminer le nom du fichier ?"), "", "");

    }
    else
    {
        preInputConfigDialog->addText(tr("Un fichier sera créé pour chaque item. Comment déterminer le nom du fichier ?"), "", "");

        _multipleExport = 1;
    }

    CT_ButtonGroup& bg = preInputConfigDialog->addButtonGroup(_multipleExportChoice);
    preInputConfigDialog->addExcludeValue("", "", tr("Nom du fichier contenu dans un attribut d'un autre item"), bg, CT_AbstractExporter::EachItemInSeparateFile_AttributeFromAnotherItem_InternalCounterIfSameFileName, tr("Si coché : \nL'attribut à utiliser appartiendra à un autre item que celui à exporter"), true);
    preInputConfigDialog->addExcludeValue("", "", tr("Nom du fichier contenu dans un attribut de l'item"), bg, CT_AbstractExporter::EachItemInSeparateFile_AttributeOfItem_InternalCounterIfSameFileName, tr("Si coché : \nL'attribut à utiliser appartiendra à l'item à exporter"), true);
    preInputConfigDialog->addExcludeValue("", "", tr("Choisir un nom pour le fichier"), bg, CT_AbstractExporter::EachItemInSeparateFile_OriginalName_InternalCounter, tr("Saisie manuelle d'un nom de base.\nChaque item génére un fichier nommé NomDeBase_xx, avec xx allant de 0 à n. "), true);

    preInputConfigDialog->addEmpty();
    preInputConfigDialog->addBool("", "", tr("Utiliser le nom du tour courant comme nom de fichier (boucles uniquement), ou comme préfixe si export d'un fichier par item"), _adaptative, tr("Si coché vous devrez choisir un item de type \"compteur\" et le nom du fichier contiendra le nom du tour courant."));

}

void PB_StepGenericExporter::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_AbstractExporter::ExportWithModelsOption option = CT_AbstractExporter::AllItemsInSameFile_OriginalName;

    if(_multipleExport == 1)
        option = CT_AbstractExporter::ExportWithModelsOption(_multipleExportChoice);

    _exporter->declareInputModels(manager, option, _adaptative);
}

bool PB_StepGenericExporter::postInputConfigure()
{
    QString exportFileName;
    const bool isDir = _adaptative || (_multipleExport == 1 && (_multipleExportChoice != CT_AbstractExporter::EachItemInSeparateFile_OriginalName_InternalCounter));

    if(isDir)
        exportFileName = QFileDialog::getExistingDirectory(nullptr, tr("Exporter dans..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename);
    else
        exportFileName = QFileDialog::getSaveFileName(nullptr, tr("Exporter sous..."), _exportFilename.isEmpty() ? _exportPath : _exportFilename, CT_ExporterTools::constructStringForFileDialog(_exporter));

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

