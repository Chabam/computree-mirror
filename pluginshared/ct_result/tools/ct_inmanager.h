#ifndef CT_INMANAGER_H
#define CT_INMANAGER_H

#include "ct_result/model/inModel/tools/ct_inresultmodelmanager.h"
#include "ct_result/model/inModel/tools/ct_inresultmodelconfigurationmanager.h"
#include "ct_result/tools/ct_inresultmanager.h"

/**
 * @brief This class contains all input managers
 */
class PLUGINSHAREDSHARED_EXPORT CT_InManager
{
public:
    CT_InManager();
    ~CT_InManager();

    /**
     * @brief Returns the input result models manager
     */
    CT_InResultModelManager* getResultModelManager() const;

    /**
     * @brief Returns the input result models configuration manager
     */
    CT_InResultModelConfigurationManager* getResultModelConfigurationManager() const;

    /**
     * @brief Returns the input results manager
     */
    CT_InResultManager* getResultManager() const;

    /**
     * @brief Must be called to inform that it will be deleted
     * @warning Call it in GUI thread
     */
    void aboutToBeDeleted();

private:

    CT_InResultModelManager                 _inResultModelManager;
    CT_InResultModelConfigurationManager    *_inResultModelConfigurationManager;
    CT_InResultManager                      *_inResultManager;
};

#endif // CT_INMANAGER_H
