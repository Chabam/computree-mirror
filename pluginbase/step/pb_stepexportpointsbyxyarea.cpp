#include "pb_stepexportpointsbyxyarea.h"

#include "pb_steppluginmanager.h"
#include "ct_log/ct_logmanager.h"

#include <QMessageBox>
#include <QDir>

PB_StepExportPointsByXYArea::PB_StepExportPointsByXYArea() : SuperClass()
{
    m_exporter = nullptr;
    _nameByCoordinates = false;
    _nameByCoordinatesOffset = 0;
}

PB_StepExportPointsByXYArea::~PB_StepExportPointsByXYArea()
{
    qDeleteAll(_areas);
    delete m_exporter;
}

QString PB_StepExportPointsByXYArea::description() const
{
    return tr("Redallage+Export de points dans une boucle");
}

QString PB_StepExportPointsByXYArea::detailledDescription() const
{
    return tr("Parmi les points de la scène d'entrée, séléctionne pour chaque emprise la liste des points à exporter."
              "Cet étape fonctionne dans une boucle, gardant le même fichier de sortie pour chaque emprise, pour toutes les scènes d'entrée (une par tour de boucle).");
}

CT_VirtualAbstractStep* PB_StepExportPointsByXYArea::createNewInstance() const
{
    return new PB_StepExportPointsByXYArea();
}

void PB_StepExportPointsByXYArea::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(m_hInCounterResult, tr("Résultat compteur"), QString(), true);
    manager.setRootGroup(m_hInCounterResult, m_hInCounterRootGroup);
    manager.addItem(m_hInCounterRootGroup, m_hInLoopCounter, tr("Compteur"));

    manager.addResult(m_hInSceneResult, tr("Résultat points"));
    manager.setRootGroup(m_hInSceneResult, m_hInSceneRootGroup);
    manager.addItem(m_hInSceneRootGroup, m_hInScene, tr("Scène"));

    manager.addResult(m_hInAreaResult, tr("Résultat emprise"), QString(), true);
    manager.setRootGroup(m_hInAreaResult, m_hInAreaRootGroup);
    manager.addItem(m_hInAreaRootGroup, m_hInArea, tr("Emprise"));
    manager.addItemAttribute(m_hInArea, m_hInAreaAttribute, CT_AbstractCategory::DATA_VALUE, tr("Nom"));
}

void PB_StepExportPointsByXYArea::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
    postInputConfigDialog->addFileChoice(tr("Répertoire d'export"), CT_FileChoiceButton::OneExistingFolder, QString(), _dir);
    postInputConfigDialog->addString(tr("Suffixe de nom de fichier"), QString(), _suffixFileName);

    const QStringList list_exportersList = pluginStaticCastT<PB_StepPluginManager>()->exportersForPointsWithPieceByPieceAvailable().keys();
    postInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), QString(), list_exportersList, m_exporterSelectedClassName);

    postInputConfigDialog->addEmpty();
    postInputConfigDialog->addBool(tr("Nommage selon coordonnées : Xmin_Ymin"), QString(), QString(), _nameByCoordinates);
    postInputConfigDialog->addDouble(tr("Offset sur les coodonnées Xmin et Ymin"), "m", -99999, 99999, 2, _nameByCoordinatesOffset);
}

void PB_StepExportPointsByXYArea::declareOutputModels(CT_StepOutModelStructureManager& /*manager*/)
{
    // no output
}

bool PB_StepExportPointsByXYArea::postInputConfigure()
{
    if(pluginStaticCastT<PB_StepPluginManager>()->exportersForPointsWithPieceByPieceAvailable().isEmpty())
    {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun exporter disponible"));
        return false;
    }

    if(SuperClass::postInputConfigure())
        return configureExporter();

    return false;
}

void PB_StepExportPointsByXYArea::refreshExporterToUse()
{
    if(m_lastExporterSelectedClassName != m_exporterSelectedClassName)
    {
        delete m_exporter;
        m_exporter = pluginStaticCastT<PB_StepPluginManager>()->exportersForPointsWithPieceByPieceAvailable().value(m_exporterSelectedClassName, nullptr);

        m_lastExporterSelectedClassName = m_exporterSelectedClassName;

        if(m_exporter != nullptr) {
            m_exporter = m_exporter->createInstance();

            /*CT_AbstractExporterAttributesSelection* exp = dynamic_cast<CT_AbstractExporterAttributesSelection*>(m_exporter);
            if (exp != nullptr)
                exp->setSearchOnlyModels(true);*/
        }
    }
}

bool PB_StepExportPointsByXYArea::configureExporter()
{
    refreshExporterToUse();

    if((m_exporter != nullptr) && m_exporter->configure())
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepExportPointsByXYArea::compute()
{
    if(m_exporter == nullptr)
    {
        qDeleteAll(_areas);
        _areas.clear();
        return;
    }

    auto itCounter = m_hInLoopCounter.iterateInputs(m_hInCounterResult);
    auto beginCounter = itCounter.begin();
    auto endCounter = itCounter.end();

    if(beginCounter == endCounter)
    {
        qDeleteAll(_areas);
        _areas.clear();

        return;
    }

    setProgress(5);

    const CT_LoopCounter* counter = (*beginCounter);

    if(counter->currentTurn() == 1)
    {
        qDeleteAll(_areas);
        _areas.clear();

        for(const CT_AbstractAreaShape2D* area : m_hInArea.iterateInputs(m_hInAreaResult))
        {
            CT_AbstractExporter* exporterCpy = m_exporter->copy();

            QString path = _dir.isEmpty() ? QString() : (_dir.first() + "/");

            QString name;

            if (_nameByCoordinates)
            {
                Eigen::Vector3d min, max;
                area->boundingBox(min, max);

                name = QString("%1_%2").arg(QString::number(min(0) + _nameByCoordinatesOffset, 'f', 0)).arg(QString::number(min(1) + _nameByCoordinatesOffset, 'f', 0));
            }
            else
            {
                const CT_AbstractItemAttribute* areaName = area->itemAttribute(m_hInAreaAttribute);

                if (areaName != nullptr)
                    name = areaName->toString(area, nullptr);
                else
                    name = area->displayableName();
            }

            path.append(name);
            path.append(_suffixFileName);

            if (exporterCpy->setFilePath(path) && exporterCpy->createExportFileForPieceByPieceExport())
            {
                CT_AreaShape2DData* areaBox2D = static_cast<CT_AreaShape2DData*>(area->getPointerData()->copy());
                AreaData* areaData = new AreaData(areaBox2D, exporterCpy);
                _areas.append(areaData);
            }
            else
            {
                delete exporterCpy;
            }
        }
    }

    const int nAreas = _areas.size();

    for (int i = 0 ; i < nAreas; ++i)
    {
        AreaData* areaData = _areas.at(i);
        areaData->setPointCloudIndex(new CT_PointCloudIndexVector());
    }

    setProgress(10);

    QList<AreaData*> allSelectedAreas;

    for(const CT_AbstractItemDrawableWithPointCloud* scene : m_hInScene.iterateInputs(m_hInSceneResult))
    {
        Eigen::Vector3d minSc, maxSc;
        scene->boundingBox(minSc, maxSc);

        QList<AreaData*> selectedAreas;

        for (int i = 0 ; i < nAreas; ++i)
        {
            AreaData* areaData = _areas.at(i);
            CT_AreaShape2DData* area = areaData->_area;

            Eigen::Vector3d minAr, maxAr;
            area->getBoundingBox(minAr, maxAr);

            if (maxSc(0) >= minAr(0) && minSc(0) <= maxAr(0) && maxSc(1) >= minAr(1) && minSc(1) <= maxAr(1))
            {
                selectedAreas.append(areaData);
                allSelectedAreas.append(areaData);
            }
        }

        const CT_AbstractPointCloudIndex* inCloudIndex = scene->pointCloudIndex();

        CT_PointIterator itP(inCloudIndex);
        const size_t nPoints = itP.size();
        size_t currentPointIndex = 0;

        while(itP.hasNext() && (!isStopped()))
        {
            const CT_Point &point = itP.next().currentPoint();
            size_t index = itP.currentGlobalIndex();

            for (int i = 0 ; i < selectedAreas.size() ; i++)
            {
                AreaData* areaData = selectedAreas.at(i);

                CT_AreaShape2DData* area = areaData->_area;

                if (area->contains(point(0), point(1)))
                {
                    CT_PointCloudIndexVector* outCloudIndex = areaData->_cloudIndex;
                    outCloudIndex->addIndex(index);
                }
            }

            ++currentPointIndex;
            setProgress(10 + (currentPointIndex*10)/nPoints);
        }
    }

    setProgress(20);

    m_progressRangeForExporter = 70.0/double(_areas.size());

    for (m_currentAreaIndex = 0 ; m_currentAreaIndex < allSelectedAreas.size() ; ++m_currentAreaIndex)
    {
        AreaData* areaData = allSelectedAreas.at(m_currentAreaIndex);

        CT_PointCloudIndexVector* outCloudIndex = areaData->_cloudIndex;
        CT_AbstractExporter* exporter = areaData->_exporter;

        QList<CT_AbstractCloudIndex *> list;
        list.append(outCloudIndex);

        connect(exporter, SIGNAL(exportInProgress(int)), this, SLOT(exporterProgressChanged(int)), Qt::DirectConnection);

        exporter->setPointsToExport(list);
        exporter->exportOnePieceOfDataToFile();

        if(!exporter->errorMessage().isEmpty())
            PS_LOG->addMessage(LogInterface::error, LogInterface::step, tr("Erreur lors de l'exportation : %1").arg(exporter->errorMessage()));

        areaData->deletePointCloudIndex();
    }

    setProgress(90);

    if(counter->currentTurn() == counter->nTurns())
    {
        for (int i = 0 ; i < _areas.size() ; i++)
        {
            CT_AbstractExporter* exporter = _areas.at(i)->_exporter;
            exporter->finalizePieceByPieceExport();

            if(!exporter->errorMessage().isEmpty())
                PS_LOG->addMessage(LogInterface::error, LogInterface::step, tr("Erreur lors de l'exportation : %1").arg(exporter->errorMessage()));
        }
    }
}

void PB_StepExportPointsByXYArea::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    if(m_exporter != nullptr)
        m_exporter->saveSettings(writer);
}

bool PB_StepExportPointsByXYArea::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    refreshExporterToUse();

    return (m_exporter != nullptr) ? m_exporter->restoreSettings(reader) : true;
}

void PB_StepExportPointsByXYArea::exporterProgressChanged(int p)
{
    setProgress(float(20 + (m_currentAreaIndex*m_progressRangeForExporter) + ((p*m_progressRangeForExporter)/100)));
}
