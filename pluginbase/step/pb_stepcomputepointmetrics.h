#ifndef PB_STEPCOMPUTEPOINTMETRICS_H
#define PB_STEPCOMPUTEPOINTMETRICS_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_element/abstract/ct_abstractconfigurableelement.h"
#include "ct_filter/abstract/ct_abstractfilter.h"

// Inclusion of auto-indexation system

class PB_StepComputePointMetrics: public CT_AbstractStep
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;
    typedef CT_AbstractStep SuperClass;

public:

    PB_StepComputePointMetrics();
    ~PB_StepComputePointMetrics();

    QString description() const;

    QString detailledDescription() const;

    QString getStepURL() const;

    void savePostSettings(SettingsWriterInterface& writer) const override;
    bool restorePostSettings(SettingsReaderInterface& reader) override;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* postInputConfigDialog) final;

    bool postConfigure();

    bool finalizePostConfiguration() override;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:

    // Declaration of autoRenames Variables (groups or items added to In models copies)
        QString                _inSceneDisplayableName;

    /**
     * @brief The collection of selected metrics to use in the compute method
     */
    QList<CT_AbstractConfigurableElement *> m_selectedXYZMetrics;

};

#endif // PB_STEPCOMPUTEPOINTMETRICS_H
