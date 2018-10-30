#ifndef STEPTESTMODELINIT2_H
#define STEPTESTMODELINIT2_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"

class StepTestModelInit2 : public CT_AbstractStepCanBeAddedFirst
{
    // IMPORTANT in order to obtain step name
    Q_OBJECT
    typedef CT_AbstractStepCanBeAddedFirst SuperClass;

public:
    StepTestModelInit2(CT_StepInitializeData &dataInit);

    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit) override;

protected:
    void createInResultModelListProtected() override;
    void createOutResultModelListProtected() override;
    void compute() override;
};

#endif // STEPTESTMODELINIT2_H
