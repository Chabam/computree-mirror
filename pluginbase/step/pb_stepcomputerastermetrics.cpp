#include "pb_stepcomputerastermetrics.h"

#include "pb_steppluginmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/abstract/ct_abstractimage2d.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
#include "ct_itemdrawable/ct_attributeslist.h"

#include "ct_abstractstepplugin.h"

#include "ctlibmetrics/ct_metric/abstract/ct_abstractmetric_raster.h"

#include "ct_view/elements/ctg_configurableelementsselector.h"

#include "tools/pb_configurableelementtools.h"

#include <QDebug>

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
    return PB_ConfigurableElementTools::formatHtmlStepDetailledDescription(getPluginAs<PB_StepPluginManager>()->rasterMetricsAvailable());
}

QString PB_StepComputeRasterMetrics::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

void PB_StepComputeRasterMetrics::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    PB_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedRasterMetrics,
                                                                                this,
                                                                                "Metric",
                                                                                writer);
}

bool PB_StepComputeRasterMetrics::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    return PB_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedRasterMetrics,
                                                                                                   getPluginAs<PB_StepPluginManager>()->rasterMetricsAvailable(),
                                                                                                   this,
                                                                                                   "Metric",
                                                                                                   reader);
}

CT_VirtualAbstractStep* PB_StepComputeRasterMetrics::createNewInstance()
{
    return new PB_StepComputeRasterMetrics();
}

//////////////////// PROTECTED METHODS //////////////////

void PB_StepComputeRasterMetrics::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Points"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addStdGroupModel("", DEFin_grp, CT_StandardItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addStdItemModel(DEFin_grp, DEFin_raster, CT_AbstractImage2D::staticGetType(), tr("Raster"));
    resIn_res->addStdItemModel(DEFin_grp, DEFin_areaShape, CT_AbstractAreaShape2D::staticGetType(), tr("Emprise de la placette"), "", CT_InAbstractModel::C_ChooseOneIfMultiple, CT_InAbstractModel::F_IsOptional);
}

bool PB_StepComputeRasterMetrics::postConfigure()
{
    CTG_ConfigurableElementsSelector cd(nullptr, !getStepChildList().isEmpty());
    cd.setWindowTitle("Métriques séléctionnées");
    cd.setElementsAvailable(getPluginAs<PB_StepPluginManager>()->rasterMetricsAvailable());
    cd.setElementsSelected(&m_selectedRasterMetrics);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

bool PB_StepComputeRasterMetrics::finalizePostConfiguration()
{
    QListIterator<CT_AbstractConfigurableElement *> it(m_selectedRasterMetrics);
    while (it.hasNext())
    {
        CT_AbstractMetric_Raster* metric = (CT_AbstractMetric_Raster*) it.next();
        metric->finalizeConfiguration();
    }

    return true;
}

void PB_StepComputeRasterMetrics::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    if(resCpy_res != nullptr) {
        resCpy_res->addItemModel(DEFin_grp, _outMetrics_ModelName, new CT_AttributesList(), tr("Métriques calculées"));

        QListIterator<CT_AbstractConfigurableElement *> it(m_selectedRasterMetrics);
        while (it.hasNext())
        {
            CT_AbstractMetric_Raster* metric = (CT_AbstractMetric_Raster*) it.next();
            metric->initAttributesModels(resCpy_res, _outMetrics_ModelName);
        }
    }
}

void PB_StepComputeRasterMetrics::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* outRes = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(outRes, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();

        const CT_AbstractImage2D* raster = (CT_AbstractImage2D*)grp->firstItemByINModelName(this, DEFin_raster);
        const CT_AbstractAreaShape2D* plotArea = (CT_AbstractAreaShape2D*)grp->firstItemByINModelName(this, DEFin_areaShape);

        if (raster != nullptr)
        {
            CT_AttributesList* outAttributes = new CT_AttributesList(_outMetrics_ModelName.completeName(), outRes);
            grp->addItemDrawable(outAttributes);

            QListIterator<CT_AbstractConfigurableElement *> it(m_selectedRasterMetrics);
            while (it.hasNext())
            {
                CT_AbstractMetric_Raster* metric = (CT_AbstractMetric_Raster*) it.next();

                if (metric != nullptr)
                {
                    const CT_AreaShape2DData* areaData = nullptr;
                    if (plotArea != nullptr) {areaData = &plotArea->getAreaData();}

                    if (metric->initDatas(raster, areaData))
                    {
                        metric->computeMetric(outAttributes);
                    }
                }
            }
        }
    }
}
