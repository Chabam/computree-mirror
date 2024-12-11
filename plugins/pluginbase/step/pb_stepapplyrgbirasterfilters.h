#ifndef PB_STEPAPPLYRGBIRASTERFILTERS_H
#define PB_STEPAPPLYRGBIRASTERFILTERS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_element/abstract/ct_abstractconfigurableelement.h"
#include "ct_filter/abstract/ct_abstractfilter.h"
#include "ct_view/elements/ctg_configurableelementsselector.h"

#include "ct_itemdrawable/ct_image2d.h"

class PB_StepApplyRGBIRasterFilters: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:
    PB_StepApplyRGBIRasterFilters();
    ~PB_StepApplyRGBIRasterFilters() final;

    QString description() const final;

    QString detailledDescription() const final;

    QString detailsDescription() const final;

    void savePostSettings(SettingsWriterInterface& writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    bool postInputConfigure() final;

    void finalizePostSettings() final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    CT_HandleInResultGroupCopy<>                                    mInResult;
    CT_HandleInStdZeroOrMoreGroup                                   mInRootGroup;
    CT_HandleInStdGroup<>                                           mInGroup;

    CT_HandleInSingularItem<CT_Image2D<float> >                     mInR;
    CT_HandleInSingularItem<CT_Image2D<float> >                     mInG;
    CT_HandleInSingularItem<CT_Image2D<float> >                     mInB;
    CT_HandleInSingularItem<CT_Image2D<float> >                     mInNIR;
    CT_HandleInSingularItem<CT_Image2D<float>, 0, 1>                mInRE;
    CT_HandleInSingularItem<CT_Image2D<float>, 0, 1>                mInMIR;

    QList<CT_HandleOutSingularItem<CT_Image2D<float> >*>            mOutHandles;

    /**
     * @brief The collection of selected filters to use in the compute method
     */
    QList<CT_AbstractConfigurableElement *> m_selectedRGBIRasterFilters;

};

#endif // PB_STEPAPPLYRGBIRASTERFILTERS_H
