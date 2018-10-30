#ifndef CT_OUTMANAGER_H
#define CT_OUTMANAGER_H

#include "ct_outresultmanager.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelmanager.h"

class CT_VirtualAbstractStep;

/**
 * @brief This class manage the creation of output models and output results of a step
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutManager
{
public:
    CT_OutManager(CT_VirtualAbstractStep &step);
    ~CT_OutManager();

    /**
     * @brief Returns the "output result model manager"
     */
    CT_OutResultModelManager* getResultModelManager() const;

    /**
     * @brief Returns the "output result manager"
     */
    CT_OutResultManager* getResultManager() const;

    /**
     * @brief Call all necessary methods of CT_OutResultModelManager, CT_VirtualAbstractStep, ... to create output models
     * @return Returns always true
     */
    bool createOutputModels();

private:
    /**
     * @brief Step to use to call the method "createOutResultModelListProtected"
     */
    CT_VirtualAbstractStep&         m_step;

    CT_OutResultModelManager        _outResultModelManager;
    CT_OutResultManager             *_outResultManager;
};

#endif // CT_OUTMANAGER_H
