#include "pb_stepcomputepointmetrics.h"

#include "pb_steppluginmanager.h"

#include "ct_abstractstepplugin.h"
#include "ctliblas/metrics/abstract/ct_abstractmetric_las.h"

#include "ct_view/elements/ctg_configurableelementsselector.h"

#include "tools/ct_configurableelementtools.h"

PB_StepComputePointMetrics::PB_StepComputePointMetrics() : SuperClass()
{
}

PB_StepComputePointMetrics::~PB_StepComputePointMetrics()
{
    qDeleteAll(m_selectedXYZMetrics.begin(), m_selectedXYZMetrics.end());
    m_selectedXYZMetrics.clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepComputePointMetrics::description() const
{
    return tr("Métriques de points (XYZ)");
}

QString PB_StepComputePointMetrics::detailledDescription() const
{
    return CT_ConfigurableElementTools::formatHtmlStepDetailledDescription(pluginStaticCastT<PB_StepPluginManager>()->xyzMetricsAvailable());
}

void PB_StepComputePointMetrics::savePostSettings(SettingsWriterInterface &writer) const
{
    CT_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedXYZMetrics,
                                                                                this,
                                                                                "Metric",
                                                                                writer);

    SuperClass::savePostSettings(writer);
}

bool PB_StepComputePointMetrics::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!CT_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedXYZMetrics,
                                                                                                pluginStaticCastT<PB_StepPluginManager>()->xyzMetricsAvailable(),
                                                                                                this,
                                                                                                "Metric",
                                                                                                reader))
        return false;

    return SuperClass::restorePostSettings(reader);
}

CT_VirtualAbstractStep* PB_StepComputePointMetrics::createNewInstance() const
{
    return new PB_StepComputePointMetrics();
}

void PB_StepComputePointMetrics::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Points"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroup, tr("Groupe"));
    manager.addItem(mInGroup, mInItem, tr("Scène"));
    manager.addItem(mInGroup, mInArea, tr("Emprise de la placette"));
    manager.addItem(mInGroup, mInLasPointsAttributesContainer, tr("Attributs LAS"));
}

bool PB_StepComputePointMetrics::postInputConfigure()
{
    CTG_ConfigurableElementsSelector cd(nullptr, hasChildrens());
    cd.setWindowTitle("Métriques séléctionnées");
    cd.setElementsAvailable(pluginStaticCastT<PB_StepPluginManager>()->xyzMetricsAvailable());
    cd.setElementsSelected(&m_selectedXYZMetrics);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepComputePointMetrics::finalizePostSettings()
{
    for(CT_AbstractConfigurableElement* f : m_selectedXYZMetrics)
    {
        static_cast<CT_AbstractMetric_XYZ*>(f)->finalizeConfiguration();
    }
}

void PB_StepComputePointMetrics::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    const CT_OutAbstractModel* outItemModel = mInItem.outModelSelected(mInResult);

    manager.addResultCopy(mInResult);
    manager.addItem(mInGroup, mOutAttributeList, tr("Métriques") + (outItemModel == nullptr ? QString() : tr(" (%1)").arg(outItemModel->displayableName())));

    for(CT_AbstractConfigurableElement* f : m_selectedXYZMetrics)
    {
        CT_AbstractMetric_XYZ* metric = static_cast<CT_AbstractMetric_XYZ*>(f);
        metric->declareOutputItemAttributeModelsInItem(manager, mOutAttributeList);
    }
}

void PB_StepComputePointMetrics::compute()
{
    for(CT_StandardItemGroup* group : mInGroup.iterateOutputs(mInResult))
    {
        const CT_AbstractItemDrawableWithPointCloud* points = group->singularItem(mInItem);

        if(points != nullptr)
        {
            const CT_AbstractAreaShape2D* plotArea = group->singularItem(mInArea);

            const CT_AreaShape2DData* area = nullptr;

            if(plotArea != nullptr)
                area = &plotArea->getAreaData();

            const CT_StdLASPointsAttributesContainer* lasAtt = group->singularItem(mInLasPointsAttributesContainer);

            CT_ItemAttributeList* outAttributes = new CT_ItemAttributeList();
            group->addSingularItem(mOutAttributeList, outAttributes);

            for(CT_AbstractConfigurableElement* f : m_selectedXYZMetrics)
            {
                CT_AbstractMetric_XYZ* metric = static_cast<CT_AbstractMetric_XYZ*>(f);
                CT_AbstractMetric_LAS* lasMetric = dynamic_cast<CT_AbstractMetric_LAS*>(metric);

                bool ok = false;

                if(lasMetric != nullptr)
                    ok = lasMetric->initLasDatas(points->pointCloudIndex(), area, lasAtt);
                else
                    ok = metric->initDatas(points->pointCloudIndex(), area);

                if (ok)
                    metric->computeMetric(outAttributes);
            }
        }
    }
}
