#include "ct_outresultmodelmanager.h"

CT_OutResultModelManager::CT_OutResultModelManager()
{
}

CT_OutResultModelManager::~CT_OutResultModelManager()
{
    clearAllResultModels();
}

bool CT_OutResultModelManager::addResultModel(CT_OutAbstractResultModel* model)
{
    if(exist(model))
    {
        delete model;
        return false;
    }

    m_resultModels.append(model);
    return true;
}

void CT_OutResultModelManager::finalizeModels()
{
    for(CT_OutAbstractResultModel* model : m_resultModels) {
        model->recursiveFinalize();
    }
}

void CT_OutResultModelManager::clearAllResultModels()
{
    clearSearchModelCollection();

    qDeleteAll(m_resultModels.begin(), m_resultModels.end());
    m_resultModels.clear();
}

bool CT_OutResultModelManager::isEmpty() const
{
    return m_resultModels.isEmpty();
}

const QList<CT_OutAbstractResultModel*>& CT_OutResultModelManager::models() const
{
    return m_resultModels;
}

bool CT_OutResultModelManager::exist(const CT_OutAbstractResultModel* model) const
{
    const QString uniqueName = model->uniqueName();

    for(const CT_OutAbstractResultModel* model : m_resultModels) {
        if(model->uniqueName() == uniqueName)
            return true;
    }

    return false;
}

void CT_OutResultModelManager::createSearchModelCollection()
{
    clearSearchModelCollection();

    for(CT_OutAbstractResultModel* model : m_resultModels) {

        const QList<CT_AbstractModel*> childrens = model->recursiveGetAllModels();

        SearchModelCollectionValueType* map = new SearchModelCollectionValueType();
        map->reserve(childrens.size());

        m_searchModelCollection.insert(model, map);

        for(CT_AbstractModel* child : childrens) {
            Q_ASSERT(!map->contains(child->uniqueName()));

            map->insert(child->uniqueName(), static_cast<CT_OutAbstractModel*>(child));
        }
    }
}

void CT_OutResultModelManager::clearSearchModelCollection()
{
    qDeleteAll(m_searchModelCollection.begin(), m_searchModelCollection.end());
    m_searchModelCollection.clear();
}

CT_OutAbstractResultModel* CT_OutResultModelManager::resultModelByUniqueName(const QString& uniqueName) const
{
    for(CT_OutAbstractResultModel* model : m_resultModels) {
        if(model->uniqueName() == uniqueName)
            return model;
    }

    return NULL;
}

CT_OutAbstractModel* CT_OutResultModelManager::modelByUniqueName(const CT_OutAbstractResultModel& resultModel, const QString& uniqueName) const
{
    SearchModelCollectionValueType* map = m_searchModelCollection.value(const_cast<CT_OutAbstractResultModel*>(&resultModel), NULL);

    Q_ASSERT(map != NULL);

    return map->value(uniqueName, NULL);
}
