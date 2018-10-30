#include "ct_inresultmanager.h"

#include "ct_result/model/inModel/tools/ct_inresultmodelmanager.h"
#include "ct_result/model/inModel/tools/ct_inresultsbymodel.h"
#include "ct_result/abstract/ct_abstractresult.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"

CT_InResultManager::CT_InResultManager(const CT_InResultModelManager& inputResultModelManager) : m_inputResultModelManager(inputResultModelManager)
{
}

CT_InResultManager::~CT_InResultManager()
{
    clearResults();
}

bool CT_InResultManager::retrieveResults(const CT_VirtualAbstractStep* step)
{
    if(step == NULL)
        return false;

    clearResults();

    if(!m_inputResultModelManager.needInputResults())
        return true;

    const QList<CT_InAbstractResultModel*>& models = m_inputResultModelManager.models();

    for(const CT_InAbstractResultModel* model : models) {

        CT_InResultsByModel* resByModel = new CT_InResultsByModel(model);
        m_resultsByModel.append(resByModel);

        if(model->needOutputModel()
                &&  model->isAtLeastOnePossibilitySelected())
        {
            QList<CT_AbstractResult*> lResult = model->searchResultsInStepForPossibilitiesSelected(step);

            if(lResult.isEmpty()) {
                clearResults();
                return false;
            }

            while(!lResult.isEmpty())
                resByModel->addResult(lResult.takeFirst());
        }
    }

    return true;
}

bool CT_InResultManager::existResults() const
{
    return !m_resultsByModel.isEmpty();
}

QList<CT_AbstractResult*> CT_InResultManager::getResults() const
{
    QList<CT_AbstractResult*> l;

    for(const CT_InResultsByModel* resByModel : m_resultsByModel) {
        l.append(resByModel->getResults());
    }

    return l;
}

QList<CT_InResultsByModel*> CT_InResultManager::getResultsSortedByModels() const
{
    return m_resultsByModel;
}

void CT_InResultManager::clearResults()
{
    qDeleteAll(m_resultsByModel.begin(), m_resultsByModel.end());
    m_resultsByModel.clear();
}

void CT_InResultManager::setResultsBusy(bool busy)
{
    for(const CT_InResultsByModel* resByModel : m_resultsByModel) {

        const QList<CT_AbstractResult*>& results = resByModel->getResults();

        for(CT_AbstractResult* result : results) {
            result->setBusy(busy);
        }
    }
}
