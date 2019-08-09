#include "pb_stepapplypointfilters.h"

#include "pb_steppluginmanager.h"

#include "ct_itemdrawable/abstract/ct_abstractitemdrawablewithpointcloud.h"
#include "ct_itemdrawable/ct_scene.h"

#include "ct_abstractstepplugin.h"
#include "ctlibfilters/filters/abstract/ct_abstractfilter_xyz.h"

#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"
#include "ctliblas/filters/abstract/ct_abstractfilter_las.h"

#include "tools/pb_configurableelementtools.h"

#include <QDebug>

PB_StepApplyPointFilters::PB_StepApplyPointFilters() : SuperClass()
{
}

PB_StepApplyPointFilters::~PB_StepApplyPointFilters()
{
    qDeleteAll(_modelNames.values());
    _modelNames.clear();

    qDeleteAll(m_selectedXYZFilters);
    m_selectedXYZFilters.clear();
}

QString PB_StepApplyPointFilters::description() const
{
    return tr("1- Filtres de points");
}

QString PB_StepApplyPointFilters::detailledDescription() const
{
    return PB_ConfigurableElementTools::formatHtmlStepDetailledDescription(getPluginAs<PB_StepPluginManager>()->xyzFiltersAvailable());
}

QString PB_StepApplyPointFilters::getStepURL() const
{
    //return tr("STEP URL HERE");
    return CT_AbstractStep::getStepURL(); //by default URL of the plugin
}

CT_VirtualAbstractStep* PB_StepApplyPointFilters::createNewInstance()
{
    return new PB_StepApplyPointFilters();
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

void PB_StepApplyPointFilters::declareInputModels(CT_StepInModelStructureManager& manager)
{
    CT_InResultModelGroupToCopy *resIn_res = createNewInResultModelForCopy(DEFin_res, tr("Points"));
    resIn_res->setZeroOrMoreRootGroup();
    resIn_res->addStdGroupModel("", DEFin_grp, CT_StandardItemGroup::staticGetType(), tr("Groupe"));
    resIn_res->addStdItemModel(DEFin_grp, DEFin_points, CT_AbstractItemDrawableWithPointCloud::staticGetType(), tr("Item contenant des points"));
    resIn_res->addStdItemModel(DEFin_grp, DEFin_lasAtt, CT_StdLASPointsAttributesContainer::staticGetType(), tr("Attributs LAS"), "", CT_InAbstractItemModel::C_ChooseOneIfMultiple, CT_InAbstractItemModel::F_IsOptional);
}

bool PB_StepApplyPointFilters::postConfigure()
{
    CTG_ConfigurableElementsSelector cd(nullptr, !getStepChildList().isEmpty());
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

bool PB_StepApplyPointFilters::finalizePostConfiguration()
{
    QListIterator<CT_AbstractConfigurableElement *> it(m_selectedXYZFilters);

    while (it.hasNext())
    {
        CT_AbstractFilter_XYZ* filter = (CT_AbstractFilter_XYZ*) it.next();
        filter->finalizeConfiguration();
    }

    return true;
}

void PB_StepApplyPointFilters::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    CT_OutResultModelGroupToCopyPossibilities *resCpy_res = createNewOutResultModelToCopy(DEFin_res);

    if(resCpy_res != nullptr) {
        QListIterator<CT_AbstractConfigurableElement *> it(m_selectedXYZFilters);
        while (it.hasNext())
        {
            CT_AbstractFilter_XYZ* filter = (CT_AbstractFilter_XYZ*) it.next();

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

        if (points != nullptr)
        {
            QListIterator<CT_AbstractConfigurableElement *> it(m_selectedXYZFilters);
            while (it.hasNext())
            {
                CT_AbstractConfigurableElement* element = it.next();
                                CT_AbstractFilter_XYZ* filter = (CT_AbstractFilter_XYZ*) element->copy();
                CT_AbstractFilter_LAS* filterLAS = dynamic_cast<CT_AbstractFilter_LAS*>(filter);

                if (filter != nullptr && modelName != nullptr && points->pointCloudIndex() != nullptr)
                {
                    filter->setPointCloud(points);

                    if (filterLAS != nullptr)
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
