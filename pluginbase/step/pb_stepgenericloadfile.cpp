#include "pb_stepgenericloadfile.h"

#include "ct_result/ct_resultgroup.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

PB_StepGenericLoadFile::PB_StepGenericLoadFile(CT_AbstractReader* reader) : SuperClass()
{
    m_reader = reader;

    connect(m_reader, SIGNAL(progressChanged(int)), this, SLOT(readerProgressChanged(int)), Qt::DirectConnection);
    connect(m_reader, SIGNAL(filePathModified()), this, SLOT(readerFilePathModified()), Qt::DirectConnection);
    connect(this, SIGNAL(stopped()), m_reader, SLOT(cancel()), Qt::DirectConnection);
}

PB_StepGenericLoadFile::~PB_StepGenericLoadFile()
{
    delete m_reader;
}

QString PB_StepGenericLoadFile::name() const
{
    // "IMPORT" is important because an exporter can have the same name than a reader !
    return "IMPORT " + m_reader->uniqueName();
}

QString PB_StepGenericLoadFile::displayableName() const
{
    return m_reader->displayableName();
}

QString PB_StepGenericLoadFile::description() const
{
    return tr("%1 : %2").arg(displayableName()).arg(createAcceptedExtensionString(" "));
}

QString PB_StepGenericLoadFile::detailledDescription() const
{
    const QString t = m_reader->toolTip();

    if(!t.isEmpty())
        return t;

    return tr("Charge un fichier du type") + createAcceptedExtensionString(" ");
}

QList<FileFormat> PB_StepGenericLoadFile::fileExtensionAccepted() const
{
    return m_reader->readableFormats();
}

void PB_StepGenericLoadFile::savePreSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePreSettings(writer);

    writer.addParameter(this, "ReaderUniqueName", m_reader->uniqueName());
}

bool PB_StepGenericLoadFile::restorePreSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePreSettings(reader))
        return false;

    QVariant value;
    if(!reader.parameter(this, "ReaderUniqueName", value))
        return false;

    if(value.toString() != m_reader->uniqueName())
        return false;

    return true;
}

void PB_StepGenericLoadFile::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    m_reader->saveSettings(writer);
}

bool PB_StepGenericLoadFile::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    if(!m_reader->restoreSettings(reader))
        return false;

    setFilePath(m_reader->filepath());

    return true;
}

bool PB_StepGenericLoadFile::setFilePath(const QString& newFilePath)
{
    const QString fp = filePath();

    if(SuperClass::setFilePath(newFilePath))
    {
        if(m_reader->setFilePath(filePath()))
            return true;

        // undo
        SuperClass::setFilePath(fp);
        setSettingsModified(false);
    }

    return false;
}

CT_VirtualAbstractStep* PB_StepGenericLoadFile::createNewInstance() const
{
    return new PB_StepGenericLoadFile(m_reader->copyFull());
}

bool PB_StepGenericLoadFile::preInputConfigure()
{
    if(SuperClass::preInputConfigure())
    {
        if(m_reader->filepath().isEmpty() || m_reader->configure()) {
            setSettingsModified(true);
            return true;
        }
    }

    return false;
}

void PB_StepGenericLoadFile::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.setNotNeedInputResult();
}

bool PB_StepGenericLoadFile::postInputConfigure()
{
    if(SuperClass::postInputConfigure())
    {
        if(m_reader->configure()) {
            setSettingsModified(true);
            return true;
        }
    }

    return false;
}

void PB_StepGenericLoadFile::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    manager.addResult(m_hOutResult);
    manager.setRootGroup(m_hOutResult, m_hOutRootGroup);
    m_reader->declareOutputModelsInGroupWithHeader(manager, m_hOutRootGroup);
}

void PB_StepGenericLoadFile::compute()
{
    for(CT_ResultGroup* outRes : m_hOutResult.iterateOutputs()) {

        CT_StandardItemGroup* rootGroup = new CT_StandardItemGroup();
        outRes->addRootGroup(m_hOutRootGroup, rootGroup);

        m_reader->readFile(rootGroup);
    }
}

void PB_StepGenericLoadFile::readerProgressChanged(int progress)
{
    setProgress(progress);
}

void PB_StepGenericLoadFile::readerFilePathModified()
{
    SuperClass::setFilePath(m_reader->filepath());
}
