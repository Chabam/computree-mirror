#ifndef CT_INRESULTMANAGER_H
#define CT_INRESULTMANAGER_H

#include "pluginShared_global.h"

#include <QList>

class CT_InResultModelManager;
class CT_InResultsByModel;
class CT_VirtualAbstractStep;
class CT_AbstractResult;

/**
 * @brief This class can be used to manage the creation of results and their life in memory.
 *
 *        It will use input models that has been added to the "input result model manager"
 *        to create all input results that can be used.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InResultManager
{
public:
    CT_InResultManager(const CT_InResultModelManager& inputResultModelManager);
    ~CT_InResultManager();

    /**
     * @brief Retrieve all input results according to the input result models added to the "input result model manager"
     * @param step : The algorithm will search in output results of this step and may b recursively to find output results
     *               that match with input models added in the "input result model manager".
     * @return true if all results has been retrieved, false if it was an error (no input result was founded).
     */
    bool retrieveResults(const CT_VirtualAbstractStep* step);

    /**
     * @brief Returns true if at least one input result was retrieved
     */
    bool existResults() const;

    /**
     * @brief Returns the list of input results that has been retrieved
     */
    QList<CT_AbstractResult*> getResults() const;

    /**
     * @brief Returns the list of input results that has been retrieved sorted by input models
     */
    QList<CT_InResultsByModel*> getResultsSortedByModels() const;

    /**
     * @brief Remove all input results from the collection
     */
    void clearResults();

    /**
     * @brief Call the method "setBusy" of all input results in this manager
     * @param busy : the parameter to pass to the method "setBusy" of all input results
     */
    void setResultsBusy(bool busy);

private:

    /**
     * @brief The input result model manager used to get all input models and to find input results
     */
    const CT_InResultModelManager&        m_inputResultModelManager;

    /**
     * @brief The collection of input results sorted by input model (an input model can found multiple input results)
     */
    QList<CT_InResultsByModel*>     m_resultsByModel;
};

#endif // CT_INRESULTMANAGER_H
