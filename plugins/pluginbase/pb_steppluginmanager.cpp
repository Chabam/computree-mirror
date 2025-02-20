/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "pb_steppluginmanager.h"

#include "ct_log/ct_logmanager.h"

#include "ct_actions/ct_actionsseparator.h"
#include "ct_exporter/ct_standardexporterseparator.h"
#include "ct_reader/ct_standardreaderseparator.h"

#include "step/pb_stepgenericexporter.h"
#include "step/pb_stepuseritemselection.h"
#include "step/pb_stepgenericloadfile.h"
#include "step/pb_stepcreatereaderlist.h"
#include "step/pb_steplooponfiles.h"
#include "step/pb_steplooponfilesets.h"
#include "step/pb_steploadfilebyname.h"

#include "step/pb_stepusereadertoloadfiles.h"
#include "step/pb_stepcomputepointmetrics.h"
#include "step/pb_stepcomputerastermetrics.h"

#include "ct_step/ct_stependloop.h"
#include "step/pb_stepbeginloopthroughgroups02.h"
#include "step/pb_stepapplypointfilters.h"
#include "step/pb_stepapplyfloatrasterfilters.h"
#include "step/pb_stepapplyrgbirasterfilters.h"
#include "step/pb_stepexportpointsbyxyarea.h"
#include "step/pb_stepexportattributesinloop.h"
#include "step/pb_stepexportattributesasascii.h"
#include "step/pb_stepexportattributesasraster.h"
#include "step/pb_stepexportattributesasvector.h"

#include "step/pb_stepcreatepointfileindex.h"

#include "ctlibstdactions/action/ct_actionselectitemdrawablegv.h"
#include "ctlibstdactions/action/pb_actionshowitemdatagv.h"

#include "exporters/csv/pb_csvexporter.h"
#include "exporters/profile/pb_profileexporter.h"

#include "exporters/grid2d/pb_grid2dexporter.h"

#include "exporters/grid3d/pb_grid3dexporter.h"
#include "exporters/grid3d/pb_grid3dastableexporter.h"

#include "exporters/xyb/pb_xybexporter.h"
#include "exporters/ascxyz/pb_ascxyzexporter.h"
#include "ctliblas/exporters/ct_exporter_las.h"
#include "ctliblaz/exporters/ct_exporter_laz.h"

#include "exporters/groupdata/pb_groupdataexporter.h"
#include "exporters/mesh/pb_meshobjexporter.h"
#include "exporters/topology/pb_opfexporter.h"
#include "exporters/ascrgb/pb_ascrgbexporter.h"
/*
#include "exporters/xyb/pb_multixybexporter.h"
#include "exporters/pbm/pb_pbmexporter.h"
#include "exporters/pgm/pb_pgmexporter.h"
*/
#include "exporters/gdal/pb_gdalexporter.h"
#include "exporters/ascid/pb_ascidexporter.h"
#include "exporters/polygon2d/pb_polygon2dexporter.h"
#include "exporters/asciitrajectory/pb_asciitrajectory.h"

#include "ctlibio/readers/ct_reader_xyb.h"
#include "ctlibio/readers/ct_reader_ascrgb.h"
#include "ctlibio/readers/ct_reader_obj.h"
#include "ctlibio/readers/ct_reader_ptx.h"
#include "ctlibio/readers/ct_reader_larchitect_grid.h"
#include "ctlibio/readers/ct_reader_opf.h"
#include "ctliblas/readers/ct_reader_lasv2.h"
#include "ctliblaz/readers/ct_reader_laz.h"
#include "ctlibio/readers/ct_reader_gdal.h"
#include "ctlibio/readers/ct_reader_idxyz.h"
#include "ctlibio/readers/ct_reader_terrascanprj.h"
#include "ctlibio/readers/ct_reader_asciigrid3d.h"
//#include "ctlibio/readers/ct_reader_pgm.h"
//#include "ctlibio/readers/ct_reader_pbm.h"
#include "ctlibio/readers/ct_reader_points_ascii.h"
#include "ctlibio/readers/ct_reader_ply.h"
#include "ctlibio/readers/ct_reader_trajectory.h"
#include "ctlibio/readers/ct_reader_ctindex.h"
#include "ctlibio/readers/ct_reader_trj.h"

#include "ctlibpcl/readers/ct_reader_pcd.h"

#include "ctlibmetrics/ct_metric/points/ct_cloudmetrics.h"
#include "ctlibmetrics/ct_metric/abstract/ct_abstractmetric_raster.h"

#include "ct_filter/abstract/ct_abstractfilter_xyz.h"
#include "ct_filter/abstract/ct_abstractfilter_rasterfloat.h"
#include "ct_filter/abstract/ct_abstractfilter_rasterrgbi.h"

#include "tools/ct_configurableelementtools.h"
#include "tools/ct_readerstools.h"
#include "tools/ct_exportertools.h"

#include <QMessageBox>
#include <QSettings>

#ifdef USE_GDAL
#include "readers/tools/gdal/ct_gdaltools.h"
#include "gdal_priv.h"
#endif


PB_StepPluginManager::PB_StepPluginManager() : CT_AbstractStepPlugin()
{
    _logListener = new CT_FileLogListener();
#if defined(__linux__) // Linux
    _logListener->setFilePath(QDir::homePath() + "/.computree/" + "log_computree.log");
#else
    _logListener->setFilePath("log_computree.log");
#endif

//    m_fileLog.setFilePath("./logPB.txt");
//    m_fileLog.setSeverityAccepted(QVector<int>() << LogInterface::debug);
//    m_fileLog.setFilter("pb");

    #ifdef USE_GDAL
    CPLSetErrorHandler(PB_StepPluginManager::staticGdalErrorHandler);
    GDALAllRegister();
    #endif
}

PB_StepPluginManager::~PB_StepPluginManager()
{
    PS_LOG->removeLogListener(_logListener);
    //PS_LOG->removeLogListener(&m_fileLog);
}

QString PB_StepPluginManager::getPluginRISCitation() const
{
    return "TY  - COMP\n"
           "TI  - Plugin Base for Computree\n"
           "AU  - Computree Core Team\n"
           "PY  - 2022\n"
           "PB  - Computree Group\n"
           "UR  - https://computree.onf.fr/?page_id=1344\n"
           "ER  - \n";
}

const QList<CT_AbstractConfigurableElement *> &PB_StepPluginManager::rasterMetricsAvailable() const
{
    return m_rasterMetricsOfAllPlugins;
}

const QList<CT_AbstractConfigurableElement *> &PB_StepPluginManager::xyzMetricsAvailable() const
{
    return m_xyzMetricsOfAllPlugins;
}

const QList<CT_AbstractConfigurableElement *> &PB_StepPluginManager::xyzFiltersAvailable() const
{
    return m_xyzFiltersOfAllPlugins;
}

const QList<CT_AbstractConfigurableElement *> &PB_StepPluginManager::rgbiRasterFiltersAvailable() const
{
    return m_RGBIRasterFiltersOfAllPlugins;
}

const QList<CT_AbstractConfigurableElement *> &PB_StepPluginManager::floatRasterFiltersAvailable() const
{
    return m_floatRasterFiltersOfAllPlugins;
}

const QList<CT_AbstractReader *> &PB_StepPluginManager::readersAvailable() const
{
    return m_readersOfAllPlugins;
}

CT_AbstractReader* PB_StepPluginManager::readerAvailableByUniqueName(const QString &readerClassName) const
{
    for(CT_AbstractReader* reader : m_readersOfAllPlugins) {
        if(reader->uniqueName() == readerClassName)
            return reader;
    }

    return nullptr;
}

const QMap<QString, CT_AbstractExporter *> &PB_StepPluginManager::exportersAvailable() const
{
    return m_exportersOfAllPlugins;
}

const QMap<QString, CT_AbstractExporter *> &PB_StepPluginManager::exportersForPointsWithPieceByPieceAvailable() const
{
    return m_exportersOfAllPluginsForPointsWithPieceByPieceAvailable;
}

bool PB_StepPluginManager::init()
{
    PS_LOG->addPrioritaryLogListener(_logListener);

    //PS_LOG->addNormalLogListener(&m_fileLog);
    PS_LOG->addMessage(LogInterface::debug, LogInterface::plugin, QObject::tr("Plugin_Base initialized"), "pb");

    return CT_AbstractStepPlugin::init();
}

QSettings* PB_StepPluginManager::initQSettings()
{
    QSettings *settings = new QSettings("pluginBase.ini", QSettings::IniFormat);

    return settings;
}

bool PB_StepPluginManager::loadGenericsStep()
{
    addNewWorkflowStep<PB_StepBeginLoopThroughGroups02>(); // doc ok
    addNewWorkflowStep<CT_StepEndLoop>(); // doc ok
    addNewWorkflowStep<PB_StepLoopOnFileSets>(); // doc ok
    addNewWorkflowStep<PB_StepLoopOnFiles>(); // doc ok

    addNewPointsStep<PB_StepApplyPointFilters>(CT_StepsMenu::LP_Filter);
    addNewMetricStep<PB_StepComputePointMetrics>(); // doc ok
    addNewMetricStep<PB_StepComputeRasterMetrics>(); // doc ok

    addNewRastersStep<PB_StepApplyFloatRasterFilters>(CT_StepsMenu::LP_Topography);
    addNewRastersStep<PB_StepApplyRGBIRasterFilters>(CT_StepsMenu::LP_Radiometry);

    addNewGeometricalShapesStep<PB_StepUserItemSelection>(CT_StepsMenu::LP_Filter); // doc ok

    addNewLoadStep<PB_StepCreateReaderList>(); // doc ok
    addNewLoadStep<PB_StepUseReaderToLoadFiles>(); // doc ok
    addNewLoadStep<PB_StepLoadFileByName>(); // doc ok

    addNewExportStep<PB_StepExportPointsByXYArea>(CT_StepsMenu::LP_Points);
    addNewExportStep<PB_StepExportAttributesInLoop>(); // doc ok
    addNewExportStep<PB_StepExportAttributesAsASCII>();
    addNewExportStep<PB_StepExportAttributesAsVector>();
    addNewExportStep<PB_StepExportAttributesAsRaster>();
    addNewExportStep<PB_StepCreatePointFileIndex>();


    return true;
}

bool PB_StepPluginManager::loadOpenFileStep()
{
    return true;
}

bool PB_StepPluginManager::loadCanBeAddedFirstStep()
{
    return true;
}

bool PB_StepPluginManager::loadActions()
{
    clearActions();

    CT_ActionsSeparator *sep = addNewSeparator(new CT_ActionsSeparator(CT_AbstractAction::TYPE_SELECTION));
    sep->addAction(new CT_ActionSelectItemDrawableGV());

    sep = addNewSeparator(new CT_ActionsSeparator(CT_AbstractAction::TYPE_INFORMATION));
    sep->addAction(new PB_ActionShowItemDataGV());

    return true;
}

#ifdef USE_GDAL
bool GDALExporterLessThan(const PB_GDALExporter *s1, const PB_GDALExporter *s2)
{
    return s1->uniqueName() < s2->uniqueName();
}

bool GDALReaderLessThan(const CT_Reader_GDAL *s1, const CT_Reader_GDAL *s2)
{
    return s1->uniqueName() < s2->uniqueName();
}
#endif

bool PB_StepPluginManager::loadExporters()
{
    clearExporters();

    CT_StandardExporterSeparator *sep = addNewSeparator(new CT_StandardExporterSeparator("Exporters"));
    sep->addExporter(new PB_XYBExporter(CT_StepsMenu::LP_Points));
    sep->addExporter(new PB_CSVExporter(-1));
    sep->addExporter(new PB_GroupDataExporter(-1));
    sep->addExporter(new PB_ASCRGBExporter(CT_StepsMenu::LP_Points));
    sep->addExporter(new PB_ASCIDExporter(CT_StepsMenu::LP_Points));
    sep->addExporter(new PB_Grid2DExporter(CT_StepsMenu::LP_Raster));
    sep->addExporter(new PB_MeshObjExporter(CT_StepsMenu::LP_Meshes));
    sep->addExporter(new PB_OPFExporter(CT_StepsMenu::LP_Others));
    sep->addExporter(new CT_Exporter_LAS(CT_StepsMenu::LP_Points));
    sep->addExporter(new CT_Exporter_LAZ(CT_StepsMenu::LP_Points));
    sep->addExporter(new PB_Grid3DExporter(CT_StepsMenu::LP_Voxels));
    sep->addExporter(new PB_Grid3DAsTableExporter(CT_StepsMenu::LP_Voxels));
    sep->addExporter(new PB_Polygon2DExporter(CT_StepsMenu::LP_Vector));
    sep->addExporter(new PB_ProfileExporter());
    sep->addExporter(new PB_AsciiTrajectory());
    sep->addExporter(new PB_ASCXYZExporter(CT_StepsMenu::LP_Points));

    /*
    sep->addExporter(new PB_MultiXYBExporter());
    sep->addExporter(new PB_PbmExporter());
    sep->addExporter(new PB_PgmExporter());
    */

    // create gdal exporters
#ifdef USE_GDAL
    GDALDriverManager *driverManager = GetGDALDriverManager();

    const int count = driverManager->GetDriverCount();

    if(count > 0) {

        QList<PB_GDALExporter*> gdalExpoC;

        for(int i=0; i<count; ++i) {
            GDALDriver *driver = driverManager->GetDriver(i);
            QString name = CT_GdalTools::staticGdalDriverName(driver);

            if(!name.isEmpty())
            {
                if (driver->GetMetadataItem(GDAL_DCAP_CREATE) != nullptr)
                {
                    gdalExpoC.append(new PB_GDALExporter(driver, CT_StepsMenu::LP_Raster, CT_StepsMenu::LP_Vector, CT_StepsMenu::LP_Others));
                }
            }
        }

        if(!gdalExpoC.isEmpty()) {
            sep = addNewSeparator(new CT_StandardExporterSeparator("GDAL"));

            std::sort(gdalExpoC.begin(), gdalExpoC.end(), GDALExporterLessThan);

            QListIterator<PB_GDALExporter*> itGD(gdalExpoC);

            while(itGD.hasNext())
                sep->addExporter(itGD.next());
        }
    }
#endif

    return true;
}

bool PB_StepPluginManager::loadReaders()
{
    clearReaders();

    CT_StandardReaderSeparator* sep = addNewSeparator(new CT_StandardReaderSeparator("Readers"));
    sep->addReader(new CT_Reader_XYB(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_OPF(CT_StepsMenu::LP_Others));
    sep->addReader(new CT_Reader_ASCRGB(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_IDXYZ(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_LASV2(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_LAZ(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_PTX(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_OBJ(CT_StepsMenu::LP_Meshes));
    sep->addReader(new CT_Reader_LArchitect_Grid(CT_StepsMenu::LP_Voxels));
    sep->addReader(new CT_Reader_TerraScanPrj(CT_StepsMenu::LP_Vector));
    sep->addReader(new CT_Reader_AsciiGrid3D(CT_StepsMenu::LP_Raster));
    sep->addReader(new CT_Reader_Points_ASCII(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_PLY(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_Trajectory(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_TRJ(CT_StepsMenu::LP_Points));
    sep->addReader(new CT_Reader_CTIndex(CT_StepsMenu::LP_Points));

    sep->addReader(new CT_Reader_PCD(CT_StepsMenu::LP_Points));

    //
    //sep->addReader(new CT_Reader_PGM());
    //sep->addReader(new CT_Reader_PBM());

    // create gdal readers

#ifdef USE_GDAL
    GDALDriverManager *driverManager = GetGDALDriverManager();

    const int count = driverManager->GetDriverCount();

    if(count > 0) {
        QList<CT_Reader_GDAL*> gdalReaderC;

        for(int i=0; i<count; ++i) {
            GDALDriver *driver = driverManager->GetDriver(i);
            QString name = CT_GdalTools::staticGdalDriverName(driver);

            if(!name.isEmpty())
                gdalReaderC.append(new CT_Reader_GDAL(driver, CT_StepsMenu::LP_Others, CT_StepsMenu::LP_Raster, CT_StepsMenu::LP_Vector));
        }

        if(!gdalReaderC.isEmpty()) {
            std::sort(gdalReaderC.begin(), gdalReaderC.end(), GDALReaderLessThan);

            QListIterator<CT_Reader_GDAL*> itGD(gdalReaderC);

            while(itGD.hasNext()) {
                CT_Reader_GDAL *reader = itGD.next();
                sep = addNewSeparator(new CT_StandardReaderSeparator(reader->displayableName()));
                sep->addReader(reader);
            }
        }
    }
#endif

    return true;
}

bool PB_StepPluginManager::loadFilters()
{
    return true;
}

bool PB_StepPluginManager::loadMetrics()
{
    addNewMetric(new CT_CloudMetrics(getPluginOfficialName()));
    return true;
}

bool PB_StepPluginManager::loadAfterAllPluginsLoaded()
{
    initRasterMetricsCollection();
    initXyzMetricsCollection();
    initXyzFiltersCollection();
    initFloatRasterFiltersCollection();
    initRGBIRasterFiltersCollection();
    initReadersCollection();
    initExportersCollection();

    QList<CT_AbstractExporter*> exporters = m_exportersOfAllPlugins.values();

    for(CT_AbstractExporter* exporter : exporters) {
        if (exporter->subMenuLevel() == -1)
        {
            addNewStep(new PB_StepGenericExporter(exporter->copy()), CT_StepsMenu::LO_Export, "");
        } else {
            addNewStep(new PB_StepGenericExporter(exporter->copy()), CT_StepsMenu::LO_Export, CT_StepsMenu::LevelPredefined(exporter->subMenuLevel()));
        }
    }

    for(CT_AbstractReader* reader : m_readersOfAllPlugins) {
        if (reader->subMenuLevel() == -1)
        {
            addNewStep(new PB_StepGenericLoadFile(reader->copyFull()), CT_StepsMenu::LO_Load, "");
        } else {
            addNewStep(new PB_StepGenericLoadFile(reader->copyFull()), CT_StepsMenu::LO_Load, CT_StepsMenu::LevelPredefined(reader->subMenuLevel()));
        }

    }

    return true;
}
/*
void PB_StepPluginManager::aboutToBeUnloaded()
{
    clearGenericsStep();
    clearOpenFileStep();
    clearCanBeAddedFirstStep();

    qDeleteAll(m_readersOfAllPlugins);
    m_readersOfAllPlugins.clear();

    qDeleteAll(m_exportersOfAllPlugins);
    m_exportersOfAllPlugins.clear();

    qDeleteAll(m_exportersOfAllPluginsForPointsWithPieceByPieceAvailable);
    m_exportersOfAllPluginsForPointsWithPieceByPieceAvailable.clear();
}
*/
void PB_StepPluginManager::initRasterMetricsCollection()
{
    CT_ConfigurableElementTools::initAvailableConfigurableElements<QList<CT_AbstractConfigurableElement *>, QList<CT_AbstractMetric*>>(m_rasterMetricsOfAllPlugins,
                                                                   [](const CT_AbstractStepPlugin* plugin) -> QList<CT_AbstractMetric*> {
                                                                        return plugin->getMetricsAvailable();
                                                                   },
                                                                   [](const CT_AbstractConfigurableElement* metric) -> bool {
                                                                        return dynamic_cast<const CT_AbstractMetric_Raster*>(metric) != nullptr;
                                                                   });
}

void PB_StepPluginManager::initXyzMetricsCollection()
{
    CT_ConfigurableElementTools::initAvailableConfigurableElements<QList<CT_AbstractConfigurableElement *>, QList<CT_AbstractMetric*>>(m_xyzMetricsOfAllPlugins,
                                                                   [](const CT_AbstractStepPlugin* plugin) -> QList<CT_AbstractMetric*> {
                                                                        return plugin->getMetricsAvailable();
                                                                   },
                                                                   [](const CT_AbstractConfigurableElement* metric) -> bool {
                                                                        return dynamic_cast<const CT_AbstractMetric_XYZ*>(metric) != nullptr;
                                                                   });
}


void PB_StepPluginManager::initXyzFiltersCollection()
{
    CT_ConfigurableElementTools::initAvailableConfigurableElements<QList<CT_AbstractConfigurableElement *>, QList<CT_AbstractFilter*>>(m_xyzFiltersOfAllPlugins,
                                                                   [](const CT_AbstractStepPlugin* plugin) -> QList<CT_AbstractFilter*> {
                                                                        return plugin->getFiltersAvailable();
                                                                   },
                                                                   [](const CT_AbstractConfigurableElement* filter) -> bool {
                                                                        return dynamic_cast<const CT_AbstractFilter_XYZ*>(filter) != nullptr;
    });
}

void PB_StepPluginManager::initRGBIRasterFiltersCollection()
{
    CT_ConfigurableElementTools::initAvailableConfigurableElements<QList<CT_AbstractConfigurableElement *>, QList<CT_AbstractFilter*>>(m_RGBIRasterFiltersOfAllPlugins,
                                                                   [](const CT_AbstractStepPlugin* plugin) -> QList<CT_AbstractFilter*> {
                                                                        return plugin->getFiltersAvailable();
                                                                   },
                                                                   [](const CT_AbstractConfigurableElement* filter) -> bool {
                                                                        return dynamic_cast<const CT_AbstractFilter_RasterRGBI*>(filter) != nullptr;
                                                                   });

}


void PB_StepPluginManager::initFloatRasterFiltersCollection()
{
    CT_ConfigurableElementTools::initAvailableConfigurableElements<QList<CT_AbstractConfigurableElement *>, QList<CT_AbstractFilter*>>(m_floatRasterFiltersOfAllPlugins,
                                                                   [](const CT_AbstractStepPlugin* plugin) -> QList<CT_AbstractFilter*> {
                                                                        return plugin->getFiltersAvailable();
                                                                   },
                                                                   [](const CT_AbstractConfigurableElement* filter) -> bool {
                                                                        return dynamic_cast<const CT_AbstractFilter_RasterFloat*>(filter) != nullptr;
                                                                   });
}

void PB_StepPluginManager::initReadersCollection()
{
    CT_ReadersTools::initAvailableReaders(m_readersOfAllPlugins, nullptr);
}

void PB_StepPluginManager::initExportersCollection()
{
    CT_ExporterTools::initAvailableExporters(m_exportersOfAllPlugins, nullptr);
    CT_ExporterTools::initAvailableExporters(m_exportersOfAllPluginsForPointsWithPieceByPieceAvailable,
                                             [](const CT_AbstractExporter* exporter)->bool {
                                                 return exporter->canExportPoints() && exporter->canExportPieceByPiece();
                                             });
}

#ifdef USE_GDAL
void PB_StepPluginManager::staticGdalErrorHandler(CPLErr eErrClass, int /*err_no*/, const char *msg)
{
    int severity = LogInterface::trace;

    switch(eErrClass) {
    case CE_Debug : severity = LogInterface::debug;
        break;

    case CE_Warning: severity = LogInterface::warning;
        break;

    case CE_Failure: severity = LogInterface::error;
        break;

    case CE_Fatal: severity = LogInterface::fatal;
        break;

    case CE_None:
        break;
    }

    PS_LOG->addMessage(severity, LogInterface::plugin, QString(msg));
}
#endif
