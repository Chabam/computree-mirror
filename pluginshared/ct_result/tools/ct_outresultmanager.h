#ifndef CT_OUTRESULTMANAGER_H
#define CT_OUTRESULTMANAGER_H

#include "pluginShared_global.h"

#include <QObject>
#include <QList>

class CT_AbstractResult;
class CT_OutResultModelManager;

/**
 * @brief This class can be used to manage the creation of results and their life in memory.
 *
 *        It will use OUTPUT models that has been added to the "OUTPUT result model manager"
 *        to create all results that must be created.
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutResultManager : public QObject
{
    Q_OBJECT

public:
    CT_OutResultManager(CT_OutResultModelManager &resultModelManager);
    ~CT_OutResultManager();

    /**
     * @brief Create results than you can use after that to add new items.
     * @return Returns false if a problem happened at the creation of one result, true
     *         otherwise (remember that it can return true if no output models is defined, so
     *         no results was created).
     */
    bool createResults();

    /**
     * @brief Call this method when you know that you will never add other items to the result after that, and only
     *        if you have finished your compute with no erros. So the signal "resultAdded" will be emmited to
     *        inform that a new finalized result is available.
     */
    void finalizeResults();

    /**
     * @brief Returns the collection of results created
     */
    QList<CT_AbstractResult*> getResults() const;

    /**
     * @brief Returns the number of results created
     */
    int nResults() const;

    /**
     * @brief Returns true if at least one result has been created
     */
    bool hasResults() const;

    /**
     * @brief Returns the result at the specified index
     * @param n : index in the collection
     * @warning There is no check for the validity of the value of "n", you must be sure that it was inside the interval [0;nResults()]
     */
    CT_AbstractResult* getResult(int n) const;

    /**
     * @brief Remove from the collection and returns the result at the specified index
     * @param n : index in the collection
     * @warning There is no check for the validity of the value of "n", you must be sure that it was inside the interval [0;nResults()].
     *          You are now the manager of the life in memory of the result !
     */
    CT_AbstractResult* takeResult(int n);

    /**
     * @brief Remove from the collection and from the memory all results created
     */
    void clearResults();

private:
    /**
     * @brief The "OUTPUT result model manager" to use to iterate over all
     *        models to create results.
     */
    CT_OutResultModelManager*           m_outResultModelManager;

    /**
     * @brief The collection of created results
     */
    QList<CT_AbstractResult*>           m_results;

signals:
    /**
     * @brief This signal is emmited when a result is finalized (no more items will be added to this result after that)
     * @param res : the result that was finalized
     */
    void resultAdded(const CT_AbstractResult *res);
};

#endif // CT_OUTRESULTMANAGER_H
