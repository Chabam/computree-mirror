#include "ct_abstractexporter.h"

#include <QFileInfo>

#include "ct_model/inModel/tools/ct_inresultmodelconfigurationmanager.h"

#include "ct_model/inModel/views/ctg_inmodelpossibilitieschoice.h"
#include "ctlibmodelsextraviews/ctg_modelslinkconfigurationflowview.h"

#include "ct_abstractpiecebypieceexporter.h"

//#define InModelPossibilitiesChoiceWidgetType CTG_InModelPossibilitiesChoice
#define InModelPossibilitiesChoiceWidgetType CTG_ModelsLinkConfigurationFlowView

CT_AbstractExporter::CT_AbstractExporter(int subMenuLevel)
{
    m_stop = false;
    _filepath = ".";
    _progress = 0;
    _ePoints = false;
    _eFaces = false;
    _eEdges = false;
    m_subMenuLevel = subMenuLevel;

    m_inModelsStructureManager = nullptr;
    mNameOfTheFileContainsTurnNameFromACounter = false;
    mExportEachItemInSeparateFile = false;
    mFileNameUsedFromAttributeOfAnotherItem = false;
}

CT_AbstractExporter::CT_AbstractExporter(const CT_AbstractExporter &other) : QObject()
{
    _formats = other._formats;
    _errMsg = other._errMsg;
    _points = other._points;
    _faces = other._faces;
    _edges = other._edges;
    _filepath = other._filepath;
    _progress = other._progress;
    _ePoints = other._ePoints;
    _eFaces = other._eFaces;
    _eEdges = other._eEdges;
    m_stop = other.m_stop;
    m_tooltip = other.m_tooltip;
    m_subMenuLevel = other.m_subMenuLevel;

    m_inModelsStructureManager = nullptr;
    mNameOfTheFileContainsTurnNameFromACounter = false;
    mExportEachItemInSeparateFile = false;
    mFileNameUsedFromAttributeOfAnotherItem = false;
}

CT_AbstractExporter::~CT_AbstractExporter()
{
    delete m_inModelsStructureManager;
}

QString CT_AbstractExporter::displayableName() const
{
    return uniqueName();
}

QString CT_AbstractExporter::uniqueName() const
{
    return metaObject()->className();
}

void CT_AbstractExporter::setSubMenuLevel(int level)
{
    m_subMenuLevel = level;
}

int CT_AbstractExporter::subMenuLevel() const
{
    return m_subMenuLevel;
}

const QList<FileFormat>& CT_AbstractExporter::exportFormats() const
{
    return _formats;
}

bool CT_AbstractExporter::isExtensionPresentInReadableFormat(const QString& extension) const
{
    for(const FileFormat& ff : _formats) {
        if(ff.suffixes().contains(extension))
            return true;
    }

    return false;
}

bool CT_AbstractExporter::isExportError() const
{
    return !_errMsg.isEmpty();
}

QString CT_AbstractExporter::toolTip() const
{
    return m_tooltip;
}

void CT_AbstractExporter::setToolTip(const QString &t)
{
    m_tooltip = t;
}

bool CT_AbstractExporter::setPointsToExport(const QList<CT_AbstractCloudIndex *> &list)
{
    if(canExportPoints()) {
        _points = list;
        setMustUseModels(false);
    }

    return true;
}

bool CT_AbstractExporter::setFacesToExport(const QList<CT_AbstractCloudIndex *> &list)
{
    if(canExportFaces()) {
        _faces = list;
        setMustUseModels(false);
    }

    return true;
}

bool CT_AbstractExporter::setEdgesToExport(const QList<CT_AbstractCloudIndex *> &list)
{
    if(canExportEdges()) {
        _edges = list;
        setMustUseModels(false);
    }

    return true;
}

bool CT_AbstractExporter::canExportPoints() const
{
    return _ePoints;
}

bool CT_AbstractExporter::canExportEdges() const
{
    return _eEdges;
}

bool CT_AbstractExporter::canExportFaces() const
{
    return _eFaces;
}

bool CT_AbstractExporter::hasSomethingToExport() const
{
    return !_points.isEmpty() || !_faces.isEmpty() || !_edges.isEmpty();
}

bool CT_AbstractExporter::setFilePath(const QString &filepath)
{
    _filepath = filepath;

    return true;
}

void CT_AbstractExporter::setFileNamePrefix(const QString& fileNamePrefix)
{
    mFileNamePrefix = fileNamePrefix;
}

QString CT_AbstractExporter::filePath() const
{
    return _filepath;
}

void CT_AbstractExporter::saveSettings(SettingsWriterInterface &writer) const
{
    writer.addParameterPath(this, "File", _filepath);
}

bool CT_AbstractExporter::restoreSettings(SettingsReaderInterface &reader)
{
    QString path;
    if(!reader.parameterPath(this, "File", path))
        return false;

    return setFilePath(path);
}

void CT_AbstractExporter::declareInputModels(CT_InModelStructureManager& manager,
                                             bool nameOfTheFileContainsTurnNameFromACounter,
                                             bool exportEachItemInSeperateFile,
                                             bool fileNameUsedFromAttributeOfAnotherItem)
{
    setMustUseModels(true);

    mNameOfTheFileContainsTurnNameFromACounter = nameOfTheFileContainsTurnNameFromACounter;
    mExportEachItemInSeparateFile = exportEachItemInSeperateFile;
    mFileNameUsedFromAttributeOfAnotherItem = fileNameUsedFromAttributeOfAnotherItem;

    mInModelSManager.clearResults();
    delete m_inModelsStructureManager;
    m_inModelsStructureManager = nullptr;

    manager.addResult(m_handleResultExport, tr("Résultat (Exporter : %1)").arg(displayableName()), QString(), true);
    manager.setZeroOrMoreRootGroup(m_handleResultExport, m_handleZeroOrMoreGroupExport);

    CT_ExporterInModelStructureManager exporterManager(manager,
                                                       m_handleZeroOrMoreGroupExport);

    internalDeclareInputModels(exporterManager);

    if(mExportEachItemInSeparateFile)
    {
        if(mFileNameUsedFromAttributeOfAnotherItem || (exporterManager.m_hAbstractInItem == nullptr))
        {
            if(exporterManager.m_hAbstractInGroup != nullptr)
            {
                manager.addItem(*exporterManager.m_hAbstractInGroup, m_handleItemWithNameExport, tr("ItemWithName"));
                manager.addItemAttribute(m_handleItemWithNameExport, m_handleItemAttributeExport, CT_AbstractCategory::DATA_VALUE, tr("Name"));
            }
        }
        else if(exporterManager.m_hAbstractInItem != nullptr)
        {
            manager.addItemAttribute(*exporterManager.m_hAbstractInItem, m_handleItemAttributeExport, CT_AbstractCategory::DATA_VALUE, tr("Name"));
        }
    }

    if(mNameOfTheFileContainsTurnNameFromACounter)
    {
        manager.addResult(m_handleResultCounter, tr("Résultat compteur"), QString(), true);
        manager.setRootGroup(m_handleResultCounter, m_handleGroupCounter);
        manager.addItem(m_handleGroupCounter, m_handleLoopCounter);
    }
}

void CT_AbstractExporter::declareInputModels()
{
    setMustUseModels(true);

    mNameOfTheFileContainsTurnNameFromACounter = false;
    mExportEachItemInSeparateFile = false;
    mFileNameUsedFromAttributeOfAnotherItem = false;

    mInModelSManager.clearResults();
    mInModelSManager.addResult(m_handleResultExport);
    mInModelSManager.setZeroOrMoreRootGroup(m_handleResultExport, m_handleZeroOrMoreGroupExport);

    delete m_inModelsStructureManager;
    m_inModelsStructureManager = new CT_ExporterInModelStructureManager(mInModelSManager, m_handleZeroOrMoreGroupExport);

    internalDeclareInputModels(*m_inModelsStructureManager);
}

const CT_ExporterInModelStructureManager* CT_AbstractExporter::inputsModelManager() const
{
    return m_inModelsStructureManager;
}

bool CT_AbstractExporter::needInputs() const
{
    return m_inModelsStructureManager->needInputs();
}

bool CT_AbstractExporter::wereAllInputsFoundInOutputsOfThisManager(const CT_OutModelStructureManager& manager) const
{
    QVector<CT_InAbstractResultModel*> resModels(m_inModelsStructureManager->nResults());
    int currentIndex = 0;

    m_inModelsStructureManager->visitResults([&resModels, &currentIndex](const CT_InAbstractResultModel* inResultModel) -> bool {
        resModels[currentIndex++] = static_cast<CT_InAbstractResultModel*>(inResultModel->copy(false));
        return true;
    });

    const bool result = findInputsInOutputsOfThisManagerWithSpecifiedResultModels(manager, resModels.begin(), resModels.end());

    qDeleteAll(resModels);

    return result;
}

bool CT_AbstractExporter::findInputsInOutputsOfThisManager(const CT_OutModelStructureManager& manager)
{
    QVector<CT_InAbstractResultModel*> resModels(m_inModelsStructureManager->nResults());
    int currentIndex = 0;

    m_inModelsStructureManager->visitResults([&resModels, &currentIndex](const CT_InAbstractResultModel* inResultModel) -> bool {
        resModels[currentIndex++] = const_cast<CT_InAbstractResultModel*>(inResultModel);
        return true;
    });

    return findInputsInOutputsOfThisManagerWithSpecifiedResultModels(manager, resModels.begin(), resModels.end());
}

bool CT_AbstractExporter::showInputsConfigurationDialog()
{
    const QString extraTitle = QObject::tr(" de l'exporteur %1").arg(displayableName());
    CT_InResultModelConfigurationManager inModelConfigurationManager(mInModelSManager);
    const CT_InResultModelConfigurationManager::CreateDialogReturn crReturn = inModelConfigurationManager.createInResultModelConfigurationDialog<InModelPossibilitiesChoiceWidgetType>(extraTitle);

    // If it was an error (does not happen)
    if(crReturn == CT_InResultModelConfigurationManager::CreateError) {
        qFatal("Erreur lors de la creation de la fenetre de configuration des resultats d'entree");
        return false;
    }

    CT_InResultModelConfigurationManager::ConfigureReturn cReturn = inModelConfigurationManager.configureInResultModel(true);

    return (cReturn == CT_InResultModelConfigurationManager::ConfigureSuccess);
}

bool CT_AbstractExporter::exportToFile()
{
    clearErrorMessage();
    clearIterators();

    _progress = 0;
    m_stop = false;

    QString baseName;

    if(mustUseModels())
    {
        for(const CT_LoopCounter* counter : m_handleLoopCounter.iterateInputs(m_handleResultCounter)) {
            baseName = counter->turnName();
            baseName = QFileInfo(baseName).baseName();
        }
    }

    if(mustUseModels() && mExportEachItemInSeparateFile)
    {
        QStringList pathByItem;

        auto iterator = m_handleItemAttributeExport.iterateInputs(m_handleResultExport);
        auto begin = iterator.begin();
        auto end = iterator.end();

        while(begin != end)
        {
            const CT_AbstractItemAttributeT<QString>* itemAttribute = *begin;

            CT_AbstractItem* itemWithName = begin.currentParent();
            const QString fileName = QFileInfo(itemAttribute->toString(itemWithName, nullptr)).baseName();

            QString path = QFileInfo(filePath()).path();
            path.append("/");

            if(mNameOfTheFileContainsTurnNameFromACounter)
            {
                path.append(baseName);
                path.append("_");
            }

            path.append(mFileNamePrefix + fileName);

            pathByItem.append(path);

            ++begin;
        }

        mMaximumItemToExportCalled = false;
        const QString backupFilepath = filePath();

        for(const QString& path : pathByItem)
        {
            setFilePath(path);

            if(!internalExportToFile()) {
                setFilePath(backupFilepath);
                return false;
            }

            #define QT_FORCE_ASSERTS
            Q_ASSERT_X(mMaximumItemToExportCalled, "CT_AbstractExporter::exportToFile", "Developper of exporter doesn't call the method \"maximumItemToExportInFile\" !");
            #undef QT_FORCE_ASSERTS
        }

        setFilePath(backupFilepath);
    }
    else
    {
        QString finalFilepath;

        if (mNameOfTheFileContainsTurnNameFromACounter && !baseName.isEmpty())
            finalFilepath = QFileInfo(filePath()).path() + "/" + baseName;
        else
            finalFilepath = filePath();

        const QString backupFilepath = filePath();

        setFilePath(finalFilepath);

        if(!internalExportToFile()) {
            setFilePath(backupFilepath);
            return false;
        }

        setFilePath(backupFilepath);
    }

    setExportProgress(100);

    return true;
}

QString CT_AbstractExporter::errorMessage() const
{
    return _errMsg;
}

void CT_AbstractExporter::clearErrorMessage()
{
    _errMsg.clear();
}

bool CT_AbstractExporter::isStopped() const
{
    return m_stop;
}

const QList<CT_AbstractCloudIndex *> &CT_AbstractExporter::pointsToExport() const
{
    return _points;
}

const QList<CT_AbstractCloudIndex *> &CT_AbstractExporter::facesToExport() const
{
    return _faces;
}

const QList<CT_AbstractCloudIndex *> &CT_AbstractExporter::edgesToExport() const
{
    return _edges;
}

bool CT_AbstractExporter::canExportPieceByPiece() const
{
    return false;
}

CT_AbstractPieceByPieceExporter* CT_AbstractExporter::createPieceByPieceExporter(const QString&)
{
    return nullptr;
}

bool CT_AbstractExporter::exportOnePieceOfDataToFiles(const QList<CT_AbstractPieceByPieceExporter*>& pieceByPieceExporters)
{
    if(pieceByPieceExporters.isEmpty())
        return true;

    for(CT_AbstractPieceByPieceExporter* ex : pieceByPieceExporters)
    {
        if(!ex->createOrOpenFile())
            return false;
    }

    bool ok = internalExportOnePiece(pieceByPieceExporters);

    for(CT_AbstractPieceByPieceExporter* ex : pieceByPieceExporters)
    {
        ex->closeFile();
    }

    return ok;
}

bool CT_AbstractExporter::finalizePieceByPieceExport(const QList<CT_AbstractPieceByPieceExporter*>& pieceByPieceExporters, bool deleteItFromMemory)
{
    bool ok = true;

    for(CT_AbstractPieceByPieceExporter* ex : pieceByPieceExporters)
    {
        if(!ex->finalizeFile())
            ok = false;

        if(deleteItFromMemory)
            delete ex;
    }

    return ok;
}

void CT_AbstractExporter::addNewExportFormat(const FileFormat &format)
{
    _formats.append(format);
}

void CT_AbstractExporter::setExportProgress(int progress)
{
    if(_progress != progress)
    {
        _progress = progress;

        emit exportInProgress(progress);
    }
}

void CT_AbstractExporter::setErrorMessage(const QString &err)
{
    _errMsg = err;
}

void CT_AbstractExporter::setCanExportPoints(bool e)
{
    _ePoints = e;
}

void CT_AbstractExporter::setCanExportEdges(bool e)
{
    _eEdges = e;
}

void CT_AbstractExporter::setCanExportFaces(bool e)
{
    _eFaces = e;
}

void CT_AbstractExporter::setMustUseModels(bool useModels)
{
    mMustUseModels = useModels;
}

bool CT_AbstractExporter::mustUseModels() const
{
    return mMustUseModels;
}

int CT_AbstractExporter::maximumItemToExportInFile(const int nToExport) const
{
    const_cast<CT_AbstractExporter*>(this)->mMaximumItemToExportCalled = true;

    return mustUseModels() ? (mExportEachItemInSeparateFile ? 1 : nToExport) : nToExport;
}

bool CT_AbstractExporter::internalExportOnePiece(const QList<CT_AbstractPieceByPieceExporter*>&)
{
    return false;
}

bool CT_AbstractExporter::findInputsInOutputsOfThisManagerWithSpecifiedResultModels(const CT_OutModelStructureManager& manager,
                                                                                    QVector<CT_InAbstractResultModel*>::iterator begin,
                                                                                    QVector<CT_InAbstractResultModel*>::iterator end) const
{
    if(!needInputs())
        return true;

    const int nResults = std::distance(begin, end);

    QSet<CT_InAbstractResultModel*> modelsThatHaveFound;

    QVector<CT_InAbstractResultModel*>::iterator it = begin;

    while(it != end) {
        CT_InAbstractResultModel* inResultModel = *it;

        inResultModel->recursiveClearPossibilitiesSaved();

        manager.visitResults([&modelsThatHaveFound, &inResultModel](const CT_OutAbstractResultModel* outResultModel) -> bool {
            inResultModel->recursiveFindAllPossibilitiesInModel(*outResultModel, false);

            if(inResultModel->hasTheMinimumNumberOfPossibilityRequired())
                modelsThatHaveFound.insert(inResultModel);

            return true;
        });

        ++it;
    }

    return (modelsThatHaveFound.size() == nResults);
}

void CT_AbstractExporter::cancel()
{
    m_stop = true;

    emit mustCancel();
}

