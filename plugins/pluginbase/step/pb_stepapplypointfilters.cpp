#include "pb_stepapplypointfilters.h"

#include "pb_steppluginmanager.h"

#include "ct_pointcloudindex/ct_pointcloudindexvector.h"

#include "ct_abstractstepplugin.h"
#include "ct_filter/abstract/ct_abstractfilter_xyz.h"
#include "ctliblas/filters/abstract/ct_abstractfilter_las.h"

#include "tools/ct_configurableelementtools.h"

PB_StepApplyPointFilters::PB_StepApplyPointFilters() : SuperClass()
{
}

PB_StepApplyPointFilters::~PB_StepApplyPointFilters()
{
    qDeleteAll(mOutHandles.begin(), mOutHandles.end());
    mOutHandles.clear();

    qDeleteAll(m_selectedXYZFilters);
    m_selectedXYZFilters.clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepApplyPointFilters::description() const
{
    return tr("1- Filtres de points");
}

// Step detailled description
QString PB_StepApplyPointFilters::detailledDescription() const
{
    return CT_ConfigurableElementTools::formatHtmlStepDetailledDescription(pluginStaticCastT<PB_StepPluginManager>()->xyzFiltersAvailable());
}

void PB_StepApplyPointFilters::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    CT_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedXYZFilters,
                                                                                this,
                                                                                "Filter",
                                                                                writer);
}

bool PB_StepApplyPointFilters::restorePostSettings(SettingsReaderInterface &reader)
{
    if (!CT_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedXYZFilters,
                                                                                               pluginStaticCastT<PB_StepPluginManager>()->xyzFiltersAvailable(),
                                                                                               this,
                                                                                               "Filter",
                                                                                               reader))
    {
        return false;
    }

    return SuperClass::restorePostSettings(reader);
}

CT_VirtualAbstractStep* PB_StepApplyPointFilters::createNewInstance() const
{
    return new PB_StepApplyPointFilters();
}

void PB_StepApplyPointFilters::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Points"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroup, tr("Groupe"));
    manager.addItem(mInGroup, mInItem, tr("Item contenant des points"));
    manager.addItem(mInGroup, mInLasPointsAttributesContainer, tr("Attributs LAS"));
}

bool PB_StepApplyPointFilters::postInputConfigure()
{
    CTG_ConfigurableElementsSelector cd(nullptr, hasChildrens());
    cd.setWindowTitle("Filtres séléctionnées");
    cd.setElementsAvailable(pluginStaticCastT<PB_StepPluginManager>()->xyzFiltersAvailable());
    cd.setElementsSelected(&m_selectedXYZFilters);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepApplyPointFilters::finalizePostSettings()
{
    for(CT_AbstractConfigurableElement* f : m_selectedXYZFilters)
    {
        static_cast<CT_AbstractFilter_XYZ*>(f)->finalizeConfiguration();
    }
}

void PB_StepApplyPointFilters::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    qDeleteAll(mOutHandles.begin(), mOutHandles.end());
    mOutHandles.clear();

    manager.addResultCopy(mInResult);

    for(CT_AbstractConfigurableElement* f : m_selectedXYZFilters)
    {
        CT_HandleOutSingularItem<CT_Scene>* outHandle = new CT_HandleOutSingularItem<CT_Scene>();
        manager.addItem(mInGroup, *outHandle, f->getDetailledDisplayableName());
        mOutHandles.append(outHandle);
    }
}

void PB_StepApplyPointFilters::compute()
{
    for(CT_StandardItemGroup* group : mInGroup.iterateOutputs(mInResult))
    {
        const CT_AbstractItemDrawableWithPointCloud* points = group->singularItem(mInItem);
        const CT_StdLASPointsAttributesContainer* lasAtt = group->singularItem(mInLasPointsAttributesContainer);

        if((points != nullptr) && (points->pointCloudIndex() != nullptr))
        {
            int i = 0;

            for(CT_AbstractConfigurableElement* element : m_selectedXYZFilters)
            {
                CT_AbstractFilter_XYZ* filter = static_cast<CT_AbstractFilter_XYZ*>(element->copy());
                CT_AbstractFilter_LAS* filterLAS = dynamic_cast<CT_AbstractFilter_LAS*>(filter);

                filter->setPointCloud(points);

                if(filterLAS != nullptr)
                    filterLAS->setLASAttributesContainer(lasAtt);

                if(filter->filterPointCloudIndex())
                {
                    if(filter->outputPointCloudIndex()->size() > 0)
                    {
                        CT_Scene* outScene = new CT_Scene(PS_REPOSITORY->registerPointCloudIndex(filter->takeOutputCloudIndex()));

                        auto outHandle = mOutHandles.at(i);
                        group->addSingularItem(*outHandle, outScene);
                    }
                }

                delete filter;

                ++i;
            }
        }
    }
}
