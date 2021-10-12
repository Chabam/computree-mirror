#include "pb_stepcomputerastermetrics.h"

#include "pb_steppluginmanager.h"

#include "ct_abstractstepplugin.h"
#include "ct_metric/abstract/ct_abstractmetric_raster.h"

#include "ct_view/elements/ctg_configurableelementsselector.h"

#include "tools/ct_configurableelementtools.h"

PB_StepComputeRasterMetrics::PB_StepComputeRasterMetrics() : SuperClass()
{
}

PB_StepComputeRasterMetrics::~PB_StepComputeRasterMetrics()
{
    qDeleteAll(m_selectedRasterMetrics);
    m_selectedRasterMetrics.clear();
}

QString PB_StepComputeRasterMetrics::description() const
{
    return tr("2- Métriques de rasters");
}

QString PB_StepComputeRasterMetrics::detailledDescription() const
{
    return CT_ConfigurableElementTools::formatHtmlStepDetailledDescription(pluginStaticCastT<PB_StepPluginManager>()->rasterMetricsAvailable());
}

void PB_StepComputeRasterMetrics::savePostSettings(SettingsWriterInterface &writer) const
{
    CT_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedRasterMetrics,
                                                                                this,
                                                                                "Metric",
                                                                                writer);
    SuperClass::savePostSettings(writer);
}

bool PB_StepComputeRasterMetrics::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!CT_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedRasterMetrics,
                                                                                                pluginStaticCastT<PB_StepPluginManager>()->rasterMetricsAvailable(),
                                                                                                this,
                                                                                                "Metric",
                                                                                                reader))
        return false;

    return SuperClass::restorePostSettings(reader);
}

CT_VirtualAbstractStep* PB_StepComputeRasterMetrics::createNewInstance() const
{
    return new PB_StepComputeRasterMetrics();
}

void PB_StepComputeRasterMetrics::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Rasters"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroup, tr("Groupe"));
    manager.addItem(mInGroup, mInRaster, tr("Raster"));
    manager.addItem(mInGroup, mInArea, tr("Emprise de la placette"));
}

bool PB_StepComputeRasterMetrics::postInputConfigure()
{
    CTG_ConfigurableElementsSelector cd(nullptr, hasChildrens());
    cd.setWindowTitle("Métriques séléctionnées");
    cd.setElementsAvailable(pluginStaticCastT<PB_StepPluginManager>()->rasterMetricsAvailable());
    cd.setElementsSelected(&m_selectedRasterMetrics);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepComputeRasterMetrics::finalizePostSettings()
{
    for(CT_AbstractConfigurableElement* f : m_selectedRasterMetrics)
    {
        static_cast<CT_AbstractMetric_Raster*>(f)->finalizeConfiguration();
    }
}

void PB_StepComputeRasterMetrics::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    const CT_OutAbstractModel* outItemModel = mInRaster.outModelSelected(mInResult);

    manager.addResultCopy(mInResult);
    manager.addItem(mInGroup, mOutAttributeList, tr("Métriques") + (outItemModel == nullptr ? QString() : tr(" (%1)").arg(outItemModel->displayableName())));

    for(CT_AbstractConfigurableElement* f : m_selectedRasterMetrics)
    {
        CT_AbstractMetric_Raster* metric = static_cast<CT_AbstractMetric_Raster*>(f);
        metric->declareOutputItemAttributeModelsInItem(manager, mOutAttributeList);
    }
}

void PB_StepComputeRasterMetrics::compute()
{
    for(CT_StandardItemGroup* group : mInGroup.iterateOutputs(mInResult))
    {
        const CT_AbstractImage2D* raster = group->singularItem(mInRaster);

        if(raster != nullptr)
        {
            const CT_AbstractAreaShape2D* plotArea = group->singularItem(mInArea);

            const CT_AreaShape2DData* area = nullptr;

            if(plotArea != nullptr)
                area = &plotArea->getAreaData();

            CT_ItemAttributeList* outAttributes = new CT_ItemAttributeList();
            group->addSingularItem(mOutAttributeList, outAttributes);

            for(CT_AbstractConfigurableElement* f : m_selectedRasterMetrics)
            {
                CT_AbstractMetric_Raster* metric = static_cast<CT_AbstractMetric_Raster*>(f);

                if (metric->initDatas(raster, area))
                    metric->computeMetric(outAttributes);
            }
        }
    }
}
