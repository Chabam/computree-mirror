#ifndef PB_STEPBEGINLOOPTHROOUGHGROUPS02_H
#define PB_STEPBEGINLOOPTHROOUGHGROUPS02_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"
#include "ct_result/ct_resultgroup.h"


class PB_StepBeginLoopThroughGroups02 : public CT_StepBeginLoop
{
    Q_OBJECT
    using SuperClass = CT_StepBeginLoop;

public:
    PB_StepBeginLoopThroughGroups02();

    QString description() const final;

    QString detailledDescription() const final;

    CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    void declareInputModels(CT_StepInModelStructureManager& manager) final;

    void fillPostInputConfigurationDialog(CT_StepConfigurableDialog* preInputConfigDialog) final;

    void declareOutputModels(CT_StepOutModelStructureManager& manager) final;

    void compute() final;

private:
    CT_HandleInResultGroupCopy<>                                m_hInResultCopy;
    CT_HandleInStdZeroOrMoreGroup                               m_hInZeroOrMoreRootGroup;
    CT_HandleInStdGroup<>                                       m_hInGroup;
    CT_HandleInSingularItem<CT_AbstractSingularItemDrawable>    m_hInItem;
    CT_HandleInStdItemAttribute<CT_AbstractCategory::ANY>       m_hInItemAttribute;

    QList<const CT_AbstractSingularItemDrawable*>               _ids;
};

#endif // PB_STEPBEGINLOOPTHROOUGHGROUPS02_H
