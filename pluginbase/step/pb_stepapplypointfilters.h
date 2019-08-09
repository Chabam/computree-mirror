#ifndef PB_STEPAPPLYPOINTFILTERS_H
#define PB_STEPAPPLYPOINTFILTERS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_element/abstract/ct_abstractconfigurableelement.h"
#include "ct_filter/abstract/ct_abstractfilter.h"
#include "ct_view/elements/ctg_configurableelementsselector.h"

// Inclusion of auto-indexation system

class PB_StepApplyPointFilters: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;
    typedef CT_AbstractStep SuperClass;

public:

    PB_StepApplyPointFilters();
    ~PB_StepApplyPointFilters();

    QString description() const;

    QString detailledDescription() const;

    QString getStepURL() const;

    CT_VirtualAbstractStep* createNewInstance() const final;

    void savePostSettings(SettingsWriterInterface& writer) const override;
    bool restorePostSettings(SettingsReaderInterface &reader) override;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    bool postConfigure();

    bool finalizePostConfiguration() override;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    // Declaration of autoRenames Variables (groups or items added to In models copies)

    QMap<CT_AbstractConfigurableElement*,
    /**
     * @brief The collection of selected filters to use in the compute method
     */
    QList<CT_AbstractConfigurableElement *> m_selectedXYZFilters;

};

#endif // PB_STEPAPPLYPOINTFILTERS_H
