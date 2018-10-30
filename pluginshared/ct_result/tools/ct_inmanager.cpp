#include "ct_result/tools/ct_inmanager.h"

CT_InManager::CT_InManager()
{
    _inResultManager = new CT_InResultManager(_inResultModelManager);
    _inResultModelConfigurationManager = new CT_InResultModelConfigurationManager(_inResultModelManager);
}

CT_InManager::~CT_InManager()
{
    delete _inResultManager;
    delete _inResultModelConfigurationManager;
}

CT_InResultModelManager* CT_InManager::getResultModelManager() const
{
    return (CT_InResultModelManager*)&_inResultModelManager;
}

CT_InResultModelConfigurationManager* CT_InManager::getResultModelConfigurationManager() const
{
    return _inResultModelConfigurationManager;
}

CT_InResultManager* CT_InManager::getResultManager() const
{
    return _inResultManager;
}

void CT_InManager::aboutToBeDeleted()
{
    _inResultModelConfigurationManager->aboutToBeDeleted();
}
