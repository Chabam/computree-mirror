#ifndef CT_INRESULTSBYMODEL_H
#define CT_INRESULTSBYMODEL_H

#include "pluginShared_global.h"

#include <QList>

class CT_InAbstractResultModel;
class CT_AbstractResult;

/**
 * @brief This class is just a container that links (KEY -> VALUE) a model (key) with a collection of results (value)
 */
class PLUGINSHAREDSHARED_EXPORT CT_InResultsByModel
{
public:
    /**
     * @brief Create this class and pass the specified model
     * @param model : the model that links all results that will be added in the collection
     */
    CT_InResultsByModel(const CT_InAbstractResultModel *model);

    /**
     * @brief Returns the model that links with all results
     */
    CT_InAbstractResultModel* getInModel() const;

    /**
     * @brief Add a new result to this collection
     * @param result : the result to add
     */
    void addResult(const CT_AbstractResult *result);

    /**
     * @brief Returns the collection of results
     */
    const QList<CT_AbstractResult*>& getResults() const;

    /**
     * @brief Remove all results from the collection
     */
    void clearResults();

private:

    /**
     * @brief The model (the key)
     */
    CT_InAbstractResultModel*   m_inputResultModel;

    /**
     * @brief The collection of results (the value)
     */
    QList<CT_AbstractResult*>   m_inputResults;
};

#endif // CT_INRESULTSBYMODEL_H
