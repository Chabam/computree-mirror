#include "pb_stepapplyrgbirasterfilters.h"

#include "pb_steppluginmanager.h"

#include "ct_abstractstepplugin.h"
#include "ct_filter/abstract/ct_abstractfilter_rasterrgbi.h"

#include "tools/ct_configurableelementtools.h"

PB_StepApplyRGBIRasterFilters::PB_StepApplyRGBIRasterFilters() : SuperClass()
{
}

PB_StepApplyRGBIRasterFilters::~PB_StepApplyRGBIRasterFilters()
{
    qDeleteAll(mOutHandles.begin(), mOutHandles.end());
    mOutHandles.clear();

    qDeleteAll(m_selectedRGBIRasterFilters);
    m_selectedRGBIRasterFilters.clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepApplyRGBIRasterFilters::description() const
{
    return tr("Indicateurs radiométriques (raster float)");
}

// Step detailled description
QString PB_StepApplyRGBIRasterFilters::detailledDescription() const
{
    return CT_ConfigurableElementTools::formatHtmlStepDetailledDescription(pluginStaticCastT<PB_StepPluginManager>()->rgbiRasterFiltersAvailable());
}

void PB_StepApplyRGBIRasterFilters::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    CT_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedRGBIRasterFilters,
                                                                                this,
                                                                                "Filter",
                                                                                writer);
}

bool PB_StepApplyRGBIRasterFilters::restorePostSettings(SettingsReaderInterface &reader)
{
    if (!CT_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedRGBIRasterFilters,
                                                                                               pluginStaticCastT<PB_StepPluginManager>()->rgbiRasterFiltersAvailable(),
                                                                                               this,
                                                                                               "Filter",
                                                                                               reader))
    {
        return false;
    }

    return SuperClass::restorePostSettings(reader);
}

CT_VirtualAbstractStep* PB_StepApplyRGBIRasterFilters::createNewInstance() const
{
    return new PB_StepApplyRGBIRasterFilters();
}

void PB_StepApplyRGBIRasterFilters::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Points"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroup, tr("Groupe"));
    manager.addItem(mInGroup, mInR, tr("Raster Rouge"));
    manager.addItem(mInGroup, mInG, tr("Raster Vert"));
    manager.addItem(mInGroup, mInB, tr("Raster Bleu"));
    manager.addItem(mInGroup, mInNIR, tr("Raster PIR"));
    manager.addItem(mInGroup, mInRE, tr("Raster Red Edge (optionnel)"));
    manager.addItem(mInGroup, mInMIR, tr("Raster MIR (optionnel)"));
}

bool PB_StepApplyRGBIRasterFilters::postInputConfigure()
{
    CTG_ConfigurableElementsSelector cd(nullptr, hasChildrens());
    cd.setWindowTitle("Filtres séléctionnés");
    cd.setElementsAvailable(pluginStaticCastT<PB_StepPluginManager>()->rgbiRasterFiltersAvailable());
    cd.setElementsSelected(&m_selectedRGBIRasterFilters);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepApplyRGBIRasterFilters::finalizePostSettings()
{
    for(CT_AbstractConfigurableElement* f : m_selectedRGBIRasterFilters)
    {
        static_cast<CT_AbstractFilter_RasterRGBI*>(f)->finalizeConfiguration();
    }
}

void PB_StepApplyRGBIRasterFilters::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    qDeleteAll(mOutHandles.begin(), mOutHandles.end());
    mOutHandles.clear();

    manager.addResultCopy(mInResult);

    for(CT_AbstractConfigurableElement* f : m_selectedRGBIRasterFilters)
    {
        CT_HandleOutSingularItem<CT_Image2D<float> >* outHandle = new CT_HandleOutSingularItem<CT_Image2D<float> >();
        manager.addItem(mInGroup, *outHandle, f->getDetailledDisplayableName());
        mOutHandles.append(outHandle);
    }
}

void PB_StepApplyRGBIRasterFilters::compute()
{
    for(CT_StandardItemGroup* group : mInGroup.iterateOutputs(mInResult))
    {
        const CT_Image2D<float>* inRasterR = group->singularItem(mInR);
        const CT_Image2D<float>* inRasterG = group->singularItem(mInG);
        const CT_Image2D<float>* inRasterB = group->singularItem(mInB);
        const CT_Image2D<float>* inRasterRE = group->singularItem(mInRE);
        const CT_Image2D<float>* inRasterNIR = group->singularItem(mInNIR);
        const CT_Image2D<float>* inRasterMIR = group->singularItem(mInMIR);

        if(inRasterR != nullptr && inRasterG != nullptr && inRasterB != nullptr && inRasterNIR != nullptr)
        {
            int i = 0;

            for(CT_AbstractConfigurableElement* element : m_selectedRGBIRasterFilters)
            {
                CT_AbstractFilter_RasterRGBI* filter = static_cast<CT_AbstractFilter_RasterRGBI*>(element->copy());

                filter->setInputRaster(inRasterR, inRasterG, inRasterB, inRasterRE, inRasterNIR, inRasterMIR);
                if (filter->filterRaster())
                {
                    CT_Image2D<float>* outRaster = filter->takeOutputRaster();

                    auto outHandle = mOutHandles.at(i);
                    group->addSingularItem(*outHandle, outRaster);
                }

                delete filter;
                ++i;
            }
        }
    }
}
