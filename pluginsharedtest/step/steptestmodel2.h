#ifndef STEPTESTMODEL2_H
#define STEPTESTMODEL2_H

#include "ct_step/abstract/ct_abstractstep.h"
#include "ct_tools/model/ct_autorenamemodels.h"

class StepTestModel2 : public CT_VirtualAbstractStep
{
    // IMPORTANT in order to obtain step name
    Q_OBJECT
    typedef CT_VirtualAbstractStep SuperClass;

public:
    StepTestModel2(CT_StepInitializeData &dataInit);

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit) override;

protected:
    void createInResultModelListProtected() override;
    void createOutResultModelListProtected() override;
    void compute() override;

public:
    CT_AutoRenameModels m_arInGroup;
    CT_AutoRenameModels m_arInPointCluster;
    CT_AutoRenameModels m_arOutEllipse;

};

#endif // STEPTESTMODEL2_H
