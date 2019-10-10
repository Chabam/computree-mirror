#include "pb_stepexportpointsbyxyarea.h"

#include "pb_steppluginmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractsingularitemdrawable.h"

#include "ct_abstractstepplugin.h"
#include "ct_exporter/ct_standardexporterseparator.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_loopcounter.h"

#include "exporters/profile/pb_profileexporter.h"

#include "ct_exporter/abstract/ct_abstractexporterattributesselection.h"

#include <QDebug>

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

CT_VirtualAbstractStep* PB_StepExportPointsByXYArea::createNewInstance() const
{
    return new PB_StepExportPointsByXYArea();
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepExportPointsByXYArea::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_InResultModelGroup* resCount = createNewInResultModel(DEFin_resCount, tr("Résultat compteur"), "", true);
    resCount->setStdRootGroup(DEF_inGroupCount);
    resCount->addStdItemModel(DEF_inGroupCount, DEF_inCounter, CT_LoopCounter::staticGetType(), tr("Compteur"));

    CT_InResultModelGroup *resScene = createNewInResultModel(DEFin_resScene, tr("Scène"));
    resScene->setZeroOrMoreRootGroup();
    resScene->addStdGroupModel("", DEFin_grpScene, CT_StandardItemGroup::staticGetType(), tr("Groupe"));
    resScene->addStdItemModel(DEFin_grpScene, DEFin_itemScene, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Scène"));

    CT_InResultModelGroup *resXYAreas = createNewInResultModel(DEFin_resAreas, tr("Emprise"), "", true);
    resXYAreas->setZeroOrMoreRootGroup();
    resXYAreas->addStdGroupModel("", DEFin_grpAreas, CT_StandardItemGroup::staticGetType(), tr("Groupe"));
    resXYAreas->addStdItemModel(DEFin_grpAreas, DEFin_itemAreas, CT_AbstractAreaShape2D::staticGetType(), tr("Emprise"));
    resXYAreas->addStdItemAttributeModel(DEFin_itemAreas, DEFin_itemAreasAtt, QList<QString>() << CT_AbstractCategory::DATA_VALUE, CT_AbstractCategory::ANY, tr("Nom"), "", CT_InAbstractModel::C_ChooseOneIfMultiple, CT_InAbstractModel::F_IsOptional);
}

void PB_StepExportPointsByXYArea::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    CT_OutResultModelGroup *res = createNewOutResultModel(DEFout_res, tr("Résultat"));
    res->setRootGroup(DEFout_grp, new CT_StandardItemGroup(), tr("Groupe"));
}

void PB_StepExportPointsByXYArea::fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog)
{

    postInputConfigDialog->addFileChoice(tr("Répertoire d'export"), CT_FileChoiceButton::OneExistingFolder, "", _dir);
    postInputConfigDialog->addString(tr("Suffixe de nom de fichier"), "", _suffixFileName);

    QStringList list_exportersList = getPluginAs<PB_StepPluginManager>()->exportersForPointsWithPieceByPieceAvailable().keys();

    if (list_exportersList.isEmpty())
        list_exportersList.append(tr("ERREUR : aucun exporter disponible"));

    postInputConfigDialog->addStringChoice(tr("Choix du type de fichier"), "", list_exportersList, m_exporterSelectedClassName);

    postInputConfigDialog->addEmpty();
    postInputConfigDialog->addBool(tr("Nommage selon coordonnées : Xmin_Ymin"), "", "", _nameByCoordinates);
    postInputConfigDialog->addDouble(tr("Offset sur les coodonnées Xmin et Ymin"), "m", -99999, 99999, 2, _nameByCoordinatesOffset);
}

bool PB_StepExportPointsByXYArea::postConfigure()
{
    if(CT_VirtualAbstractStep::postConfigure())
        return configureExporter();

    return false;
}

void PB_StepExportPointsByXYArea::refreshExporterToUse()
{
    if(m_lastExporterSelectedClassName != m_exporterSelectedClassName) {
        delete m_exporter;
        m_exporter = pluginStaticCastT<PB_StepPluginManager>()->exportersForPointsWithPieceByPieceAvailable().value(m_exporterSelectedClassName);

        m_lastExporterSelectedClassName = m_exporterSelectedClassName;

        if(m_exporter != nullptr) {
            m_exporter = m_exporter->copy();
            m_exporter->setMyStep(this);

            CT_AbstractExporterAttributesSelection* exp = dynamic_cast<CT_AbstractExporterAttributesSelection*>(m_exporter);
            if (exp != nullptr)
                exp->setSearchOnlyModels(true);
        }
    }
}

bool PB_StepExportPointsByXYArea::configureExporter()
{
    refreshExporterToUse();

    if((m_exporter != nullptr) && m_exporter->configureExport())
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepExportPointsByXYArea::compute()
{
    QList<CT_ResultGroup*> inResultList = getInputResults();
    CT_ResultGroup* resInCounter = inResultList.at(0);
    CT_ResultGroup* resInScene = inResultList.at(1);
    CT_ResultGroup* resInAreas = inResultList.at(2);

    setProgress(5);

    // First turn: create export files
    CT_LoopCounter* counter = nullptr;

    CT_ResultItemIterator it_Counter(resInCounter, this, DEF_inCounter);
    if (it_Counter.hasNext())
    {
        counter = (CT_LoopCounter*) it_Counter.next();

        if (counter != nullptr && counter->getCurrentTurn() == 1)
        {
            qDeleteAll(_areas);
            _areas.clear();

            if (m_exporter != nullptr)
            {
                CT_ResultGroupIterator itIn_grpAreas(resInAreas, this, DEFin_grpAreas);
                while (itIn_grpAreas.hasNext() && !isStopped())
                {
                    const CT_StandardItemGroup* group = (CT_StandardItemGroup*) itIn_grpAreas.next();

                    CT_AbstractAreaShape2D* area = (CT_AbstractAreaShape2D*)group->firstItemByINModelName(this, DEFin_itemAreas);

                    if (area != nullptr)
                    {
                        CT_AbstractItemAttribute* areaName = area->firstItemAttributeByINModelName(resInAreas, this, DEFin_itemAreasAtt);

                        CT_AbstractExporter* exporterCpy = m_exporter->copy();

                        QString path = "";
                        if (_dir.size() > 0)
                        {
                            path.append(_dir.first());
                            path.append("/");
                        }

                        QString name;
                        if (_nameByCoordinates)
                        {
                            Eigen::Vector3d min, max;
                            area->boundingBox(min, max);

                            name = QString("%1_%2").arg(QString::number(min(0) + _nameByCoordinatesOffset, 'f', 0)).arg(QString::number(min(1) + _nameByCoordinatesOffset, 'f', 0));

                        } else {
                            if (areaName != nullptr)
                            {
                                name = areaName->toString(area, nullptr);
                            } else {
                                name = area->displayableName();
                            }
                        }

                        path.append(name);
                        path.append(_suffixFileName);

                        if (exporterCpy->setExportFilePath(path) && exporterCpy->createExportFileForPieceByPieceExport())
                        {
                            CT_AreaShape2DData* areaBox2D = (CT_AreaShape2DData*) (area->getPointerData()->copy());
                            AreaData* areaData = new AreaData(areaBox2D, exporterCpy);
                            _areas.append(areaData);
                        }
                    }
                }
            }
        }
    }

    for (int i = 0 ; i < _areas.size() ; i++)
    {
        AreaData* areaData = _areas.at(i);
        areaData->setPointCloudIndex(new CT_PointCloudIndexVector());
    }

    setProgress(10);

    QList<AreaData*> allSelectedAreas;

    // Point export
    CT_ResultGroupIterator itIn_grpScene(resInScene, this, DEFin_grpScene);
    while (itIn_grpScene.hasNext() && !isStopped())
    {
        const CT_StandardItemGroup* group = (CT_StandardItemGroup*) itIn_grpScene.next();

        CT_AbstractItemDrawableWithPointCloud* scene = (CT_AbstractItemDrawableWithPointCloud*)group->firstItemByINModelName(this, DEFin_itemScene);

        if (scene != nullptr)
        {
            Eigen::Vector3d minSc, maxSc;
            scene->boundingBox(minSc, maxSc);

            QList<AreaData*> selectedAreas;

            for (int i = 0 ; i < _areas.size() ; i++)
            {
                AreaData* areaData = _areas.at(i);
                CT_AreaShape2DData* area = areaData->_area;

                Eigen::Vector3d minAr, maxAr;
                area->boundingBox(minAr, maxAr);

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

    // Last turn : finalize export files
    if (counter != nullptr && counter->getCurrentTurn() == counter->getNTurns())
    {
        for (int i = 0 ; i < _areas.size() ; i++)
        {
            CT_AbstractExporter* exporter = _areas.at(i)->_exporter;
            exporter->finalizePieceByPieceExport();

            if(!exporter->errorMessage().isEmpty())
                PS_LOG->addMessage(LogInterface::error, LogInterface::step, tr("Erreur lors de l'exportation : %1").arg(exporter->errorMessage()));
        }
    }

    // OUT results creation
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* resOut = outResultList.at(0);
    CT_StandardItemGroup* grpOut= new CT_StandardItemGroup(DEFout_grp, resOut);
    resOut->addGroup(grpOut);

}

void PB_StepExportPointsByXYArea::exporterProgressChanged(int p)
{
    setProgress(20 + (m_currentAreaIndex*m_progressRangeForExporter) + ((p*m_progressRangeForExporter)/100));
}
