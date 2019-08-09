#ifndef PB_STEPBEGINLOOPTHROOUGHGROUPS02_H
#define PB_STEPBEGINLOOPTHROOUGHGROUPS02_H

#include "ct_step/ct_stepbeginloop.h"
#include "ct_itemdrawable/ct_loopcounter.h"
#include "ct_itemdrawable/ct_standarditemgroup.h"

class PB_StepBeginLoopThroughGroups02 : public CT_StepBeginLoop
{
    Q_OBJECT
    using SuperClass = CT_AbstractStep;
public:
    PB_StepBeginLoopThroughGroups02();

    ~PB_StepBeginLoopThroughGroups02();

    QString description() const;

    QString detailledDescription() const;

    virtual CT_VirtualAbstractStep* createNewInstance() const final;

protected:

    virtual void declareInputModels(CT_StepInModelStructureManager& manager) final;

    virtual void createOutResultModelListProtected(CT_OutResultModelGroup *firstResultModel);

    virtual void createPostConfigurationDialog(int &nTurns);

    virtual void compute(CT_ResultGroup *outRes, CT_StandardItemGroup* group);

private:
    QList<CT_AbstractSingularItemDrawable*>               _ids;

    void recursiveRemoveGroupIfEmpty(CT_StandardItemGroup *parent, CT_StandardItemGroup *group) const;
};

#endif // PB_STEPBEGINLOOPTHROOUGHGROUPS02_H
