#ifndef PB_STEPAPPLYPOINTFILTERS_H
#define PB_STEPAPPLYPOINTFILTERS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_element/abstract/ct_abstractconfigurableelement.h"
#include "ct_filter/abstract/ct_abstractfilter.h"
#include "ct_view/elements/ctg_configurableelementsselector.h"

#include "ct_itemdrawable/ct_scene.h"
#include "ctliblas/itemdrawable/las/ct_stdlaspointsattributescontainer.h"

class PB_StepApplyPointFilters: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;

public:
    PB_StepApplyPointFilters();
    ~PB_StepApplyPointFilters() final;

    QString description() const final;

    QString detailledDescription() const final;

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
    CT_HandleInSingularItem<CT_AbstractItemDrawableWithPointCloud>  mInItem;
    CT_HandleInSingularItem<CT_StdLASPointsAttributesContainer, 0>  mInLasPointsAttributesContainer; // optionnal

    QList<CT_HandleOutSingularItem<CT_Scene>*>                      mOutHandles;
    /**
     * @brief The collection of selected filters to use in the compute method
     */
    QList<CT_AbstractConfigurableElement *> m_selectedXYZFilters;

};

#endif // PB_STEPAPPLYPOINTFILTERS_H
