#include "ct_outresultmanager.h"

#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"
#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_result/model/outModel/tools/ct_outresultmodelmanager.h"

CT_OutResultManager::CT_OutResultManager(CT_OutResultModelManager &resultModelManager)
{
    m_outResultModelManager = &resultModelManager;
}

CT_OutResultManager::~CT_OutResultManager()
{
    clearResults();
}

bool CT_OutResultManager::createResults()
{
    clearResults();

    const QList<CT_OutAbstractResultModel*>& models = m_outResultModelManager->models();

    int id = 0;

    for(CT_OutAbstractResultModel* model : models) {
        CT_ResultInitializeData* data = new CT_ResultInitializeData(id);
        CT_AbstractResult* result = model->createResult(*data);

        if(result == NULL) {
            clearResults();
            return false;
        }

        m_results.append(result);
        ++id;
    }

    return true;
}

QList<CT_AbstractResult*> CT_OutResultManager::getResults() const
{
    return m_results;
}

int CT_OutResultManager::nResults() const
{
    return m_results.size();
}

bool CT_OutResultManager::hasResults() const
{
    return !m_results.isEmpty();
}

CT_AbstractResult* CT_OutResultManager::getResult(int n) const
{
    return m_results.at(n);
}

CT_AbstractResult* CT_OutResultManager::takeResult(int n)
{
    return m_results.takeAt(n);
}

void CT_OutResultManager::clearResults()
{
    qDeleteAll(m_results.begin(), m_results.end());
    m_results.clear();
}

void CT_OutResultManager::finalizeResults()
{
    for(CT_AbstractResult* result : m_results) {
        PS_MODELS->removeCacheForResult(result);

        if(result->isClearedFromMemory())
        {
            result->setComplete();
            emit resultAdded(result);
        }
    }
}
