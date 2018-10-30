#include "ct_inresultsbymodel.h"

CT_InResultsByModel::CT_InResultsByModel(const CT_InAbstractResultModel *model)
{
    m_inputResultModel = const_cast<CT_InAbstractResultModel*>(model);
}

CT_InAbstractResultModel* CT_InResultsByModel::getInModel() const
{
    return m_inputResultModel;
}

void CT_InResultsByModel::addResult(const CT_AbstractResult *result)
{
    m_inputResults.append(const_cast<CT_AbstractResult*>(result));
}

const QList<CT_AbstractResult*>& CT_InResultsByModel::getResults() const
{
    return m_inputResults;
}

void CT_InResultsByModel::clearResults()
{
    m_inputResults.clear();
}
