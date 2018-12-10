#include "pb_stepcomputepointmetrics.h"

#include "pb_steppluginmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_itemdrawable/abstract/ct_abstractareashape2d.h"
#include "ct_itemdrawable/ct_attributeslist.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_model/inModel/tools/ct_instdmodelpossibility.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_global/ct_context.h"

#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_abstractstepplugin.h"
#include "ctlibmetrics/ct_metric/abstract/ct_abstractmetric_xyz.h"
#include "ctliblas/metrics/abstract/ct_abstractmetric_las.h"

#include "ct_view/elements/ctg_configurableelementsselector.h"

#include "tools/pb_configurableelementtools.h"

#include <QDebug>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_points "points"
#define DEFin_areaShape "shapeArea"
#define DEFin_lasAtt "lasAtt"

// Constructor : initialization of parameters
PB_StepComputePointMetrics::PB_StepComputePointMetrics(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
    _inSceneDisplayableName = "";
}

PB_StepComputePointMetrics::~PB_StepComputePointMetrics()
{
    qDeleteAll(m_selectedXYZMetrics);
    m_selectedXYZMetrics.clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepComputePointMetrics::getStepDescription() const
{
    return tr("1- Métriques de points (XYZ)");
}

// Step detailled description
QString PB_StepComputePointMetrics::getStepDetailledDescription() const
{
    return PB_ConfigurableElementTools::formatHtmlStepDetailledDescription(getPluginAs<PB_StepPluginManager>()->xyzMetricsAvailable());
}

// Step URL
QString PB_StepComputePointMetrics::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

void PB_StepComputePointMetrics::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    PB_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedXYZMetrics,
                                                                                this,
                                                                                "Metric",
                                                                                writer);
}

bool PB_StepComputePointMetrics::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    return PB_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedXYZMetrics,
                                                                                                   getPluginAs<PB_StepPluginManager>()->xyzMetricsAvailable(),
                                                                                                   this,
                                                                                                   "Metric",
                                                                                                   reader);
}

// Step copy method
CT_VirtualAbstractStep* PB_StepComputePointMetrics::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepComputePointMetrics(dataInit);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepComputePointMetrics::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Points"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addStdGroupModel("", DEFin_grp, CT_StandardItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addStdItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Scène"));
    resIn_res->addStdItemModel(DEFin_grp, DEFin_areaShape, CT_AbstractAreaShape2D::staticGetType(), tr("Emprise de la placette"), "", CT_InAbstractModel::C_ChooseOneIfMultiple, CT_InAbstractModel::F_IsOptional);
    resIn_res->addStdItemModel(DEFin_grp, DEFin_lasAtt, CT_StdLASPointsAttributesContainer::staticGetType(), tr("Attributs LAS"), "", CT_InAbstractItemModel::C_ChooseOneIfMultiple, CT_InAbstractItemModel::F_IsOptional);
}

// Semi-automatic creation of step parameters DialogBox
void PB_StepComputePointMetrics::createPostConfigurationDialog()
{
    // Récupération du nom du modèle de la scène d'entrée
    CT_InAbstractResultModel *resultInModel = getInResultModel(DEFin_res);

    CT_OutAbstractResultModel* resultInModelOut = NULL;
    CT_InAbstractSingularItemModel* baseModel = NULL;

    if(!resultInModel->getPossibilitiesSelected().isEmpty())
    {
        resultInModelOut = (CT_OutAbstractResultModel*)resultInModel->getPossibilitiesSelected().first()->outModel();
    }

    if(resultInModelOut != NULL)
    {
        baseModel = (CT_InAbstractSingularItemModel*)PS_MODELS->searchModel(DEFin_points, resultInModelOut, this);
    }

    if(baseModel != NULL && !baseModel->getPossibilitiesSelected().isEmpty())
    {
        _inSceneDisplayableName =  baseModel->getPossibilitiesSelected().first()->outModel()->displayableName();
    }
}

bool PB_StepComputePointMetrics::postConfigure()
{
    CTG_ConfigurableElementsSelector cd(NULL, !getStepChildList().isEmpty());
    cd.setWindowTitle("Métriques séléctionnées");
    cd.setElementsAvailable(getPluginAs<PB_StepPluginManager>()->xyzMetricsAvailable());
    cd.setElementsSelected(&m_selectedXYZMetrics);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

bool PB_StepComputePointMetrics::finalizePostConfiguration()
{
    QListIterator<CT_AbstractConfigurableElement *> it(m_selectedXYZMetrics);
    while (it.hasNext())
    {
        CT_AbstractMetric_XYZ* metric = dynamic_cast<CT_AbstractMetric_XYZ*>(it.next());
        metric->finalizeConfiguration();
    }

    return true;
}


// Creation and affiliation of OUT models
void PB_StepComputePointMetrics::createOutResultModelListProtected()
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy = createNewOutResultModelToCopy(DEFin_res);

    if(resCpy != NULL) {
        resCpy->addItemModel(DEFin_grp, _outMetrics_ModelName, new CT_AttributesList(), tr("Métriques") + (_inSceneDisplayableName.isEmpty() ? "" : tr(" (%1)").arg(_inSceneDisplayableName)));

        QListIterator<CT_AbstractConfigurableElement *> it(m_selectedXYZMetrics);
        while (it.hasNext())
        {
            CT_AbstractMetric_XYZ* metric = dynamic_cast<CT_AbstractMetric_XYZ*>(it.next());
            metric->initAttributesModels(resCpy, _outMetrics_ModelName);
        }
    }
}

void PB_StepComputePointMetrics::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* outRes = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(outRes, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_AbstractItemDrawableWithPointCloud* points = (CT_AbstractItemDrawableWithPointCloud*)grp->firstItemByINModelName(this, DEFin_points);
        const CT_AbstractAreaShape2D* plotArea = (CT_AbstractAreaShape2D*)grp->firstItemByINModelName(this, DEFin_areaShape);
        const CT_StdLASPointsAttributesContainer* lasAtt = (CT_StdLASPointsAttributesContainer*)grp->firstItemByINModelName(this, DEFin_lasAtt);

        if (points != NULL)
        {
            const CT_AreaShape2DData *area = NULL;

            if(plotArea != NULL)
            {
                area = &plotArea->getAreaData();
            }

            CT_AttributesList* outAttributes = new CT_AttributesList(_outMetrics_ModelName.completeName(), outRes);
            grp->addItemDrawable(outAttributes);

            QListIterator<CT_AbstractConfigurableElement *> it(m_selectedXYZMetrics);
            while (it.hasNext())
            {
                CT_AbstractMetric_XYZ* metric = (CT_AbstractMetric_XYZ*) it.next();
                CT_AbstractMetric_LAS* lasMetric = dynamic_cast<CT_AbstractMetric_LAS*>(metric);

                bool ok = false;
                if(lasMetric != NULL)
                {
                    ok = lasMetric->initLasDatas(points->getPointCloudIndex(), area, lasAtt);
                } else
                {
                    ok = metric->initDatas(points->getPointCloudIndex(), area);
                }

                if (ok) {metric->computeMetric(outAttributes);}
            }
        }
    }
}
