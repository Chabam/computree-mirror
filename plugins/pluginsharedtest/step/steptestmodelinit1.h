#ifndef STEPTESTMODELINIT1_H
#define STEPTESTMODELINIT1_H

#include "ct_step/abstract/ct_abstractstepcanbeaddedfirst.h"

#define DEF_OutResult       "rr"
#define DEF_OutSectionGroup    "sgr"
#define DEF_OutClusterGroup    "cgr"
#define DEF_OutClusterItem     "cir"

class StepTestModelInit1 : public CT_AbstractStepCanBeAddedFirst
{
    // IMPORTANT in order to obtain step name
    Q_OBJECT

public:

    /*! \brief Step constructor
     *
     * Create a new instance of the step
     *
     * \param dataInit Step parameters object
     */
    StepTestModelInit1(CT_StepInitializeData &dataInit);

    /*! \brief Step copy
     *
     * Step copy, used when a step is added by step contextual menu
     */
    CT_VirtualAbstractStep* createNewInstance(CT_StepInitializeData &dataInit) override;

protected:

    /*! \brief Input results specification
     *
     * Specification of input results models needed by the step (IN)
     */
    void createInResultModelListProtected() override;

    /*! \brief Output results specification
     *
     * Specification of output results models created by the step (OUT)
     */
    void createOutResultModelListProtected() override;

    /*! \brief Algorithm of the step
     *
     * Step computation, using input results, and creating output results
     */
    void compute() override;

};

#endif // STEPTESTMODELINIT1_H
