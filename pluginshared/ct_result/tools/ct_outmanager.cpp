#include "ct_outmanager.h"

#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_OutManager::CT_OutManager(CT_VirtualAbstractStep &step) : m_step(step)
{
    _outResultManager = new CT_OutResultManager(_outResultModelManager);
}

CT_OutManager::~CT_OutManager()
{
    delete _outResultManager;
}

CT_OutResultModelManager* CT_OutManager::getResultModelManager() const
{
    return &const_cast<CT_OutManager*>(this)->_outResultModelManager;
}

CT_OutResultManager* CT_OutManager::getResultManager() const
{
    return _outResultManager;
}

bool CT_OutManager::createOutputModels()
{
    // clear all previously result models added
    _outResultModelManager.clearAllResultModels();

    // we ask the step to produce output models, so it will be added to the "output result model manager"
    m_step.createOutResultModelListProtected();

    // we ask the "output result model manager" that from now we don't add other models to it and it can
    // finalize models
    _outResultModelManager.finalizeModels();

    return true;
}
