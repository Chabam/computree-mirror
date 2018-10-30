#include "pb_stepapplypointfilters.h"

#include "pb_steppluginmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_scene.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_itemdrawable/tools/iterator/ct_groupiterator.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_result/model/inModel/ct_inresultmodelgrouptocopy.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelgrouptocopypossibilities.h"
#include "ct_view/ct_stepconfigurabledialog.h"

#include "ct_abstractstepplugin.h"
#include "ctlibfilters/filters/abstract/ct_abstractfilter_xyz.h"

#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"
#include "ctliblas/filters/abstract/ct_abstractfilter_las.h"

#include "tools/pb_configurableelementtools.h"

#include <QDebug>

// Alias for indexing models
#define DEFin_res "res"
#define DEFin_grp "grp"
#define DEFin_points "points"
#define DEFin_lasAtt "lasAtt"

// Constructor : initialization of parameters
PB_StepApplyPointFilters::PB_StepApplyPointFilters(CT_StepInitializeData &dataInit) : CT_AbstractStep(dataInit)
{
}

PB_StepApplyPointFilters::~PB_StepApplyPointFilters()
{
    qDeleteAll(_modelNames.values());
    _modelNames.clear();

    qDeleteAll(m_selectedXYZFilters);
    m_selectedXYZFilters.clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepApplyPointFilters::getStepDescription() const
{
    return tr("1- Filtres de points");
}

// Step detailled description
QString PB_StepApplyPointFilters::getStepDetailledDescription() const
{
    return PB_ConfigurableElementTools::formatHtmlStepDetailledDescription(getPluginAs<PB_StepPluginManager>()->xyzFiltersAvailable());
}

// Step URL
QString PB_StepApplyPointFilters::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

// Step copy method
CT_VirtualAbstractStep* PB_StepApplyPointFilters::createNewInstance(CT_StepInitializeData &dataInit)
{
    return new PB_StepApplyPointFilters(dataInit);
}

void PB_StepApplyPointFilters::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    PB_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedXYZFilters,
                                                                                this,
                                                                                "Filter",
                                                                                writer);
}

bool PB_StepApplyPointFilters::restorePostSettings(SettingsReaderInterface &reader)
{
    if(!SuperClass::restorePostSettings(reader))
        return false;

    return PB_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedXYZFilters,
                                                                                                   getPluginAs<PB_StepPluginManager>()->xyzFiltersAvailable(),
                                                                                                   this,
                                                                                                   "Filter",
                                                                                                   reader);
}

//////////////////// PROTECTED METHODS //////////////////

// Creation and affiliation of IN models
void PB_StepApplyPointFilters::createInResultModelListProtected()
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Points"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addGroupModel("", DEFin_grp, CT_AbstractItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Item contenant des points"));
    resIn_res->addItemModel(DEFin_grp, DEFin_lasAtt, CT_StdLASPointsAttributesContainer::staticGetType(), tr("Attributs LAS"), "", CT_InAbstractItemModel::C_ChooseOneIfMultiple, CT_InAbstractItemModel::F_IsOptional);
}

bool PB_StepApplyPointFilters::postConfigure()
{
    CTG_ConfigurableElementsSelector cd(NULL, !getStepChildList().isEmpty());
    cd.setWindowTitle("Filtres séléctionnées");
    cd.setElementsAvailable(getPluginAs<PB_StepPluginManager>()->xyzFiltersAvailable());
    cd.setElementsSelected(&m_selectedXYZFilters);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

// Creation and affiliation of OUT models
void PB_StepApplyPointFilters::createOutResultModelListProtected()
{       
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    if(resCpy_res != NULL) {
        QListIterator<CT_AbstractConfigurableElement *> it(m_selectedXYZFilters);
        while (it.hasNext())
        {
            CT_AbstractFilter_XYZ* filter = (CT_AbstractFilter_XYZ*) it.next();
            filter->postConfigure();

            CT_AutoRenameModels* modelName = new CT_AutoRenameModels();
            _modelNames.insert(filter, modelName);

            resCpy_res->addItemModel(DEFin_grp, *modelName, new CT_Scene(), filter->getDetailledDisplayableName());
        }
    }
}

void PB_StepApplyPointFilters::compute()
{
    QList<CT_ResultGroup*> outResultList = getOutResultList();
    CT_ResultGroup* _res = outResultList.at(0);

    // COPIED results browsing
    CT_ResultGroupIterator itCpy_grp(_res, this, DEFin_grp);
    while (itCpy_grp.hasNext() && !isStopped())
    {
        CT_StandardItemGroup* grp = (CT_StandardItemGroup*) itCpy_grp.next();
        
        const CT_AbstractItemDrawableWithPointCloud* points = (CT_AbstractItemDrawableWithPointCloud*)grp->firstItemByINModelName(this, DEFin_points);
        const CT_StdLASPointsAttributesContainer* lasAtt = (CT_StdLASPointsAttributesContainer*)grp->firstItemByINModelName(this, DEFin_lasAtt);

        if (points != NULL)
        {
            QListIterator<CT_AbstractConfigurableElement *> it(m_selectedXYZFilters);
            while (it.hasNext())
            {
                CT_AbstractConfigurableElement* element = it.next();
                CT_AutoRenameModels* modelName = _modelNames.value(element);
                CT_AbstractFilter_XYZ* filter = (CT_AbstractFilter_XYZ*) element->copy();
                CT_AbstractFilter_LAS* filterLAS = dynamic_cast<CT_AbstractFilter_LAS*>(filter);

                if (filter != NULL && modelName != NULL && points->getPointCloudIndex() != NULL)
                {
                    filter->setPointCloud(points);

                    if (filterLAS != NULL)
                        ((CT_AbstractFilter_LAS*) filter)->setLASAttributesContainer(lasAtt);

                    if(filter->filterPointCloudIndex()) {
                        CT_PointCloudIndexVector* outCloud = filter->takeOutputCloudIndex();

                        if (outCloud->size() > 0)
                        {
                            CT_Scene* outScene = new CT_Scene(modelName->completeName(), _res);
                            outScene->updateBoundingBox();
                            grp->addItemDrawable(outScene);

                            // The PCI must be registered to the point repository :
                            outScene->setPointCloudIndexRegistered(PS_REPOSITORY->registerPointCloudIndex(outCloud));

                        } else {delete outCloud;}

                        delete filter;
                    }
                }
            }
        }
    }
}
