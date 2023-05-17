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
    return tr("Export des points de chaque emprise (boucle)");
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

void PB_StepExportPointsByXYArea::fillPreInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog)
{
    const QStringList list_exportersList = pluginStaticCastT<PB_StepPluginManager>()->exportersForPointsWithPieceByPieceAvailable().keys();
    preInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), QString(), list_exportersList, m_exporterSelectedClassName);
}

void PB_StepExportPointsByXYArea::finalizePreSettings()
{
    CT_AbstractExporter* exporter = pluginStaticCastT<PB_StepPluginManager>()->exportersForPointsWithPieceByPieceAvailable().value(m_exporterSelectedClassName, nullptr);

    if(exporter == nullptr) {
        delete m_exporter;
        m_exporter = nullptr;
        return;
    }

    if((m_exporter == nullptr) || (exporter->uniqueName() != m_exporter->uniqueName()))
    {
        delete m_exporter;
        m_exporter = exporter->createInstance();
    }
}

void PB_StepExportPointsByXYArea::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(m_hInCounterResult, tr("Résultat compteur"), QString(), true);
    manager.setZeroOrMoreRootGroup(m_hInCounterResult, m_hInZeroOrMoreGroupCounter);
    manager.addGroup(m_hInZeroOrMoreGroupCounter, m_hInCounterRootGroup);
    manager.addItem(m_hInCounterRootGroup, m_hInLoopCounter, tr("Compteur"));

    if(m_exporter != nullptr)
        m_exporter->declareInputModels(manager);

    manager.addResult(m_hInAreaResult, tr("Résultat emprise"), QString(), true);
    manager.setZeroOrMoreRootGroup(m_hInAreaResult, m_hInZeroOrMoreGroupArea);
    manager.addGroup(m_hInZeroOrMoreGroupArea, m_hInAreaRootGroup);
    manager.addItem(m_hInAreaRootGroup, m_hInArea, tr("Emprise"));
    manager.addItemAttribute(m_hInArea, m_hInAreaAttribute, CT_AbstractCategory::DATA_VALUE, tr("Nom"));
}

void PB_StepExportPointsByXYArea::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{
    postInputConfigDialog->addFileChoice(tr("Répertoire d'export"), CT_FileChoiceButton::OneExistingFolder, QString(), _dir);
    postInputConfigDialog->addString(tr("Suffixe de nom de fichier"), QString(), _suffixFileName);
    postInputConfigDialog->addEmpty();
    postInputConfigDialog->addBool(tr("Renommer selon les coordonnées : Xmin_Ymin"), QString(), QString(), _nameByCoordinates);
    postInputConfigDialog->addDouble(tr("Offset sur les coodonnées Xmin et Ymin"), "m", -99999, 99999, 2, _nameByCoordinatesOffset);
}

void PB_StepExportPointsByXYArea::declareOutputModels(CT_StepOutModelStructureManager& /*manager*/)
{
    // no output
}

bool PB_StepExportPointsByXYArea::postInputConfigure()
{
    if(m_exporter == nullptr)
    {
        QMessageBox::critical(nullptr, tr("Erreur"), tr("Aucun exporter sélectionné"));
        return false;
    }

    if(SuperClass::postInputConfigure())
        return configureExporter();

    return false;
}

bool PB_StepExportPointsByXYArea::configureExporter()
{
    if(m_exporter->configure())
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepExportPointsByXYArea::compute()
{
    // if no exporter : return
    if(m_exporter == nullptr)
    {
        qDeleteAll(_areas);
        _areas.clear();
        return;
    }

    auto itCounter = m_hInLoopCounter.iterateInputs(m_hInCounterResult);
    auto beginCounter = itCounter.begin();
    auto endCounter = itCounter.end();

    // if no counter founded : return
    if(beginCounter == endCounter)
    {
        qDeleteAll(_areas);
        _areas.clear();
        return;
    }

    setProgress(5);

    const CT_LoopCounter* counter = (*beginCounter);

    // if first turn
    if(counter->currentTurn() == 1)
    {
        qDeleteAll(_areas);
        _areas.clear();


        // for each area : compute a special filepath, create an exporter, set it the filepath
        // and create the file for piece by piece export
        for(const CT_AbstractAreaShape2D* area : m_hInArea.iterateInputs(m_hInAreaResult))
        {
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
                {
                    name = areaName->toString(area, nullptr);
                } else
                {
                    name = area->displayableName();
                }

                QFileInfo info(name);
                name = info.baseName();
            }

            path.append(name);
            path.append(_suffixFileName);

            CT_AbstractPieceByPieceExporter* pieceByPieceExporter = m_exporter->createPieceByPieceExporter(path);

            if(pieceByPieceExporter != nullptr)
            {
                CT_AreaShape2DData* areaBox2D = static_cast<CT_AreaShape2DData*>(area->getPointerData()->copy());
                AreaData* areaData = new AreaData(areaBox2D, pieceByPieceExporter);
                _areas.append(areaData);

                pieceByPieceExporter->setPointFilter([area](const size_t&, const CT_Point& point) -> bool
                {
                    return area->contains(point(0), point(1));
                });
            }
        }        
    }

    const int nAreas = _areas.size();

    setProgress(20);

    QMetaObject::Connection con1 = connect(m_exporter, SIGNAL(exportInProgress(int)), this, SLOT(exporterProgressChanged(int)), Qt::DirectConnection);
    QMetaObject::Connection con2 = connect(this, SIGNAL(stopped()), m_exporter, SLOT(cancel()), Qt::DirectConnection);

    QList<CT_AbstractPieceByPieceExporter*> exporters;

    for(int i=0; i<nAreas; ++i)
    {
        AreaData* areaData = _areas.at(i);
        exporters << areaData->_exporter;
    }

    if(!m_exporter->exportOnePieceOfDataToFiles(exporters))
    {
        PS_LOG->addMessage(LogInterface::error, LogInterface::step, tr("Erreur lors de l'exportation"));
    }

    disconnect(con1);
    disconnect(con2);

    setProgress(90);

    // if last turn
    if(counter->currentTurn() == counter->nTurns())
    {
        // finalize the file of each areas
        if(!m_exporter->finalizePieceByPieceExport(exporters, false))
            PS_LOG->addMessage(LogInterface::error, LogInterface::step, tr("Erreur lors de  la finalisation de l'export"));
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

    return (m_exporter != nullptr) ? m_exporter->restoreSettings(reader) : true;
}

void PB_StepExportPointsByXYArea::exporterProgressChanged(int p)
{
    setProgress(float(20 + ((p*70.0)/100.0)));
}
