#ifndef CT_OUTRESULTMODELMANAGER_H
#define CT_OUTRESULTMODELMANAGER_H

#include "ct_result/model/outModel/abstract/ct_outabstractresultmodel.h"

#include <QList>

/**
 * @brief This class manager the life in memory of result models and their childrens. A result model is the root of a tree
 *        that contains others models like item, item attribute, etc...
 */
class PLUGINSHAREDSHARED_EXPORT CT_OutResultModelManager
{
public:
    CT_OutResultModelManager();
    ~CT_OutResultModelManager();

    /**
     * @brief Add a new model that represent a result to the collection (the life in memory of this model will be
     *        manage by this manager)
     * @param model : the model to add
     * @return Returns false if the model is not valid or if it already exist in the collection (same "uniqueName")
     * @warning If the model is not valid or already exist in the collection it will be deleted from memory and the method will returns false.
     */
    bool addResultModel(CT_OutAbstractResultModel* model);

    /**
     * @brief Call this method when you know that you will never add other items to result models after that or add other result model
     *        to this manager.
     */
    void finalizeModels();

    /**
     * @brief Remove from the collection and from the memory all models added to this manager. Called in the destructor of
     *        this class.
     */
    void clearAllResultModels();

    /**
     * @brief Returns true if no model has been added
     */
    bool isEmpty() const;

    /**
     * @brief Returns the collection of models
     */
    const QList<CT_OutAbstractResultModel *>& models() const;

    /**
     * @brief Verify if the model already exist in the collection (compare their unique name)
     * @param model : the model to check
     * @return Returns true if the model already exist in this manager
     */
    bool exist(const CT_OutAbstractResultModel *model) const;

    /**
     * @brief Retrieve all models (output result models and their childrens) and add it in a collection so when you want to search a model
     *        by unique name you will get it in faster time.
     */
    void createSearchModelCollection();

    /**
     * @brief Clear the search model collection created when the method "createSearchModelCollection()" has been called
     */
    void clearSearchModelCollection();

    /**
     * @brief Returns the output result model that has the same unique name
     * @param uniqueName : unique name to compare
     * @return NULL if not founded in the collection
     */
    CT_OutAbstractResultModel* resultModelByUniqueName(const QString& uniqueName) const;

    /**
     * @brief Returns the output model that has the same unique name (the collection of all models created with method "createSearchModelCollection()" will be used to
     *        improve performance)
     * @param resultModel : the output result model that contains the model to find
     * @param uniqueName : the unique name of the model to find
     * @return NULL if not founded
     * @warning Be sure that the search model collection is created before call this method, or it will crash !
     */
    CT_OutAbstractModel* modelByUniqueName(const CT_OutAbstractResultModel& resultModel, const QString& uniqueName) const;

private:
    typedef QHash<QString,CT_OutAbstractModel*>                                         SearchModelCollectionValueType;
    typedef QMultiHash<CT_OutAbstractResultModel*, SearchModelCollectionValueType* >    SearchModelCollectionType;

    /**
     * @brief Collection of models to manage
     */
    QList<CT_OutAbstractResultModel*>   m_resultModels;

    /**
     * @brief Collection of all output models sorted by name for each out result model
     */
    SearchModelCollectionType   m_searchModelCollection;
};

#endif // CT_OUTRESULTMODELMANAGER_H
