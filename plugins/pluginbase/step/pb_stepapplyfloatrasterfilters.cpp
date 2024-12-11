#include "pb_stepapplyfloatrasterfilters.h"

#include "pb_steppluginmanager.h"

#include "ct_abstractstepplugin.h"
#include "ct_filter/abstract/ct_abstractfilter_rasterfloat.h"

#include "tools/ct_configurableelementtools.h"

PB_StepApplyFloatRasterFilters::PB_StepApplyFloatRasterFilters() : SuperClass()
{
}

PB_StepApplyFloatRasterFilters::~PB_StepApplyFloatRasterFilters()
{
    qDeleteAll(mOutHandles.begin(), mOutHandles.end());
    mOutHandles.clear();

    qDeleteAll(m_selectedFloatRasterFilters);
    m_selectedFloatRasterFilters.clear();
}

// Step description (tooltip of contextual menu)
QString PB_StepApplyFloatRasterFilters::description() const
{
    return tr("Indicateurs topographiques *");
}

// Step detailled description
QString PB_StepApplyFloatRasterFilters::detailledDescription() const
{    

    if (this->isAPrototype())
    {
        return tr("Cette étape regroupe un ensemble d'indicateurs topographiques disponibles dans les différents plugins actifs.<br><br>"
                  "A partir d'une raster d'entrée (en général un MNT), un ensemble de rasters correspondants à différents produits dérivés peuvent être calculés en bloc.<br><br>"
                  "Une partie de ces indicateurs sont aussi disponibles sous forme d'étapes autonomes (facilitant les tests de paramétrage).<br>Les étapes concernées sont suivies d'une *."
                  "<br><br>"
                  "<strong><a href=\"#metricsList\">La liste des indicateurs disponibles</a> est fournie en dernière partie de cette page.</strong>"
                  "<br><br>");
    }
    return tr("Cette étape regroupe un ensemble d'indicateurs topographiques disponibles dans les différents plugins actifs.<br><br>"
              "A partir d'une raster d'entrée (en général un MNT), un ensemble de rasters correspondants à différents produits dérivés peuvent être calculés en bloc.<br><br>"
              "Une partie de ces indicateurs sont aussi disponibles sous forme d'étapes autonomes (facilitant les tests de paramétrage).<br>Les étapes concernées sont suivies d'une *."
              "<br><br>"
              "<strong><a href=\"#metricsList\">La liste des indicateurs calculés</a> est fournie en dernière partie de cette page.</strong>"
              "<br><br>");
}

QString PB_StepApplyFloatRasterFilters::detailsDescription() const
{
    if (this->isAPrototype())
    {
        return tr("Chaque indicateur séléctionné génèrera un raster en sortie.<br><br>"
                  "</div><div><h2 id=\"metricsList\">Liste des indicateurs disponibles :</h2>%1")
                .arg(CT_ConfigurableElementTools::formatHtmlStepDetailledDescription(pluginStaticCastT<PB_StepPluginManager>()->floatRasterFiltersAvailable()));
    }

    return tr("Chaque indicateur séléctionné génèrera un raster en sortie.<br><br>"
              "</div><div><h2 id=\"metricsList\">Liste des indicateurs calculés :</h2>%1")
            .arg(CT_ConfigurableElementTools::formatHtmlStepDetailledDescription(m_selectedFloatRasterFilters));
}


void PB_StepApplyFloatRasterFilters::savePostSettings(SettingsWriterInterface &writer) const
{
    SuperClass::savePostSettings(writer);

    CT_ConfigurableElementTools::saveSettingsOfACollectionOfConfigurableElement(m_selectedFloatRasterFilters,
                                                                                this,
                                                                                "Filter",
                                                                                writer);
}

bool PB_StepApplyFloatRasterFilters::restorePostSettings(SettingsReaderInterface &reader)
{
    if (!CT_ConfigurableElementTools::restoreSettingsOfConfigurableElementAndSaveItInACollection(m_selectedFloatRasterFilters,
                                                                                               pluginStaticCastT<PB_StepPluginManager>()->floatRasterFiltersAvailable(),
                                                                                               this,
                                                                                               "Filter",
                                                                                               reader))
    {
        return false;
    }

    return SuperClass::restorePostSettings(reader);
}

CT_VirtualAbstractStep* PB_StepApplyFloatRasterFilters::createNewInstance() const
{
    return new PB_StepApplyFloatRasterFilters();
}

void PB_StepApplyFloatRasterFilters::declareInputModels(CT_StepInModelStructureManager& manager)
{
    manager.addResult(mInResult, tr("Points"));
    manager.setZeroOrMoreRootGroup(mInResult, mInRootGroup);
    manager.addGroup(mInRootGroup, mInGroup, tr("Groupe"));
    manager.addItem(mInGroup, mInItem, tr("Raster (float)"));
}

bool PB_StepApplyFloatRasterFilters::postInputConfigure()
{
    CTG_ConfigurableElementsSelector cd(nullptr, hasChildrens());
    cd.setWindowTitle("Filtres séléctionnés");
    cd.setElementsAvailable(pluginStaticCastT<PB_StepPluginManager>()->floatRasterFiltersAvailable());
    cd.setElementsSelected(&m_selectedFloatRasterFilters);

    if(cd.exec() == QDialog::Accepted)
    {
        setSettingsModified(true);
        return true;
    }

    return false;
}

void PB_StepApplyFloatRasterFilters::finalizePostSettings()
{
    for(CT_AbstractConfigurableElement* f : m_selectedFloatRasterFilters)
    {
        static_cast<CT_AbstractFilter_RasterFloat*>(f)->finalizeConfiguration();
    }
}

void PB_StepApplyFloatRasterFilters::declareOutputModels(CT_StepOutModelStructureManager& manager)
{
    qDeleteAll(mOutHandles.begin(), mOutHandles.end());
    mOutHandles.clear();

    manager.addResultCopy(mInResult);

    for(CT_AbstractConfigurableElement* f : m_selectedFloatRasterFilters)
    {
        CT_HandleOutSingularItem<CT_Image2D<float> >* outHandle = new CT_HandleOutSingularItem<CT_Image2D<float> >();
        manager.addItem(mInGroup, *outHandle, f->getDetailledDisplayableName());
        mOutHandles.append(outHandle);
    }
}

void PB_StepApplyFloatRasterFilters::compute()
{
    for(CT_StandardItemGroup* group : mInGroup.iterateOutputs(mInResult))
    {
        const CT_Image2D<float>* inRaster = group->singularItem(mInItem);

        if(inRaster != nullptr)
        {
            int i = 0;

            for(CT_AbstractConfigurableElement* element : m_selectedFloatRasterFilters)
            {
                CT_AbstractFilter_RasterFloat* filter = static_cast<CT_AbstractFilter_RasterFloat*>(element->copy());

                filter->setInputRaster(inRaster);
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
