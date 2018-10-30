#ifndef CT_INRESULTMODELMANAGER_H
#define CT_INRESULTMODELMANAGER_H

#include "ct_result/model/inModel/abstract/ct_inabstractresultmodel.h"

class CT_VirtualAbstractStep;
class CT_OutAbstractResultModel;

/**
 * @brief Manage a collection of input result model and their life in memory. Also you can :
 *
 *        * Check if all elements searched by input models of this manager is found in a specific step or recursively.
 *        * Search and save all possibilities, in a specific step or recursively, that match with elements searched by input models of this manager.
 */
class PLUGINSHAREDSHARED_EXPORT CT_InResultModelManager
{
public:
    CT_InResultModelManager();
    ~CT_InResultModelManager();

    /**
     * @brief Add a new model that represent a result to the collection (the life in memory of this model will be
     *        manage by this manager)
     * @param model : the model to add
     * @return Returns false if the model is not valid or if it already exist in the collection (same "uniqueName")
     * @warning If the model is not valid or already exist in the collection it will be deleted from memory and the method will returns false.
     */
    bool addResultModel(CT_InAbstractResultModel *model);

    /**
     * @brief Remove all models from the collection and from the memory
     */
    void clearAllResultModels();

    /**
     * @brief Check if models in this manager find matching output models in the specified step or in the parent
     *        steps (if the input models need to perform a recursive search). This will not only check if input results
     *        have been found but also childrens (like item, item attributes, etc...).
     * @param step : The step where to start the research of elements
     * @return True if all obligatory elements was founded, false otherwise
     * @warning This method will only check if ouput models was founded so input models doesn't save founded possibility and
     *          previously research will not be altered.
     */
    bool checkFromThisStepIfAllInputModelsFindMatchingOutputModels(const CT_VirtualAbstractStep *step);

    /**
     * @brief Search all output models, in the specified step or in the parent steps (if the input models need to perform
     *        a recursive search), that can match with input models and save them in a collection of possibilities
     *        included in input models.
     * @param step : The step where to start the research of elements
     * @return True if all obligatory elements was founded, false otherwise
     * @warning This method will save all output models founded in a collection of possibility so previously research (previously possibilities)
     *          will be lost definitively.
     */
    bool findFromThisStepAllMatchingOutputModelsAndSavePossibilities(const CT_VirtualAbstractStep *step);

    /**
     * @brief Check if all input result models in the collection and their childrens has been correctly configured
     * @param errors : you can pass a pointer to a string list that will contains all errors if the method return false, otherwise the list will be empty
     * @return Returns true if input result models in the collection and their childrens has the minimum of selected possibility that he must have
     */
    bool checkIfAllInputModelsWasCorrectlyConfigured(QStringList* errors = NULL) const;

    /**
     * @brief Returns true if no model has been added
     */
    bool isEmpty() const;

    /**
     * @brief Returns true if at least one model need to find one input result
     */
    bool needInputResults() const;

    /**
     * @brief Returns the collection of models
     */
    const QList<CT_InAbstractResultModel*>& models() const;

    /**
     * @brief Verify if the model already exist in the collection (compare their unique name)
     * @param model : the model to check
     * @return Returns true if the model already exist in this manager
     */
    bool exist(const CT_InAbstractResultModel* model) const;

    /**
     * @brief Save all necessary elements to be able to restore it with the method "restoreSettings"
     */
    void saveSettings(SettingsWriterInterface& writer) const;

    /**
     * @brief Restore the state
     */
    bool restoreSettings(SettingsReaderInterface& reader);

    /**
     * @brief Recursively unselect all possibilities of all models
     */
    void recursiveUnselectAllPossibilities();

    /**
     * @brief Recursively visit all possibilities of all models
     * @param visitor : the visitor to use
     */
    void recursiveVisitPossibilities(std::function<void(CT_InAbstractModel* model, CT_InStdModelPossibility* possibility)>& visitor);

    /**
     * @brief Retrieve all models (input result models and their childrens) and add it in a collection so when you want to search a model
     *        by unique name you will get it in faster time.
     */
    void createSearchModelCollection();

    /**
     * @brief Clear the search model collection created when the method "createSearchModelCollection()" has been called
     */
    void clearSearchModelCollection();

    /**
     * @brief Returns the input result model that has the same unique name
     * @param uniqueName : unique name to compare
     * @return NULL if not founded in the collection
     */
    CT_InAbstractResultModel* resultModelByUniqueName(const QString& uniqueName) const;

    /*!
     *  \brief Retourne le modèle de sortie à partir de son nom.
     *
     *  \return NULL si le modèle n'a pas été trouvé.
     */
    /**
     * @brief Returns the input model that has the same unique name (the collection of all models created with method "createSearchModelCollection()" will be used to
     *        improve performance)
     * @param resultModel : the output result model that contains the model to find
     * @param uniqueName : the unique name of the model to find
     * @return NULL if not founded
     * @warning Be sure that the search model collection is created before call this method, or it will crash !
     */
    CT_InAbstractModel* modelByUniqueName(const CT_OutAbstractResultModel& resultModel, const QString& uniqueName) const;

private:

    typedef QHash<QString,CT_InAbstractModel*>                                          SearchModelCollectionValueType;
    typedef QMultiHash<CT_OutAbstractResultModel*, SearchModelCollectionValueType* >    SearchModelCollectionType;

    enum ToDoWithPossibilities {
        DontSavePossibilities = 0,
        SavePossibilities = 1
    };


    /**
     * @brief Collection of input result model
     */
    QList<CT_InAbstractResultModel*>    m_inputResultModels;

    /**
     * @brief Collection of all input models sorted by name for each out result model selected in possibilities
     */
    SearchModelCollectionType           m_searchModelCollection;

    /**
     * @brief Call this method to check or search output models that match with the collection of input models of this manager and know if all
     *        obligatory elements has been found.
     * @param step : The step where to start the research of elements
     * @param toDoWithPossibilities : Set to "SavePossibilities" if you want that input models of this manager save possibilities. So previously
     *                                collection of possibilities in models will be replaced with new possibilites founded.
     *                                Set to "DontSavePossibilities" if you want that we make a copy of input models of this manager before do the
     *                                process. So previously collection of possibilities will not be altered.
     * @return Returns true if all obligatory elements has been found. False if at least one input model doesn't found a matching element.
     * @warning If an input result model is set to search recursively the algorithm will go up in the tree of steps up to the root step
     */
    bool findFromThisStepAllMatchingOutputModels(const CT_VirtualAbstractStep* step,
                                                 const ToDoWithPossibilities& toDoWithPossibilities);

    /**
     * @brief Call the appropriate method to find output models that match with input model depending on whether the model is recursive or not
     * @param step : The step where to start the research of elements
     * @param inputResultModel : The model to use to do the research
     * @return Returns true if the model does not need output model or if all obligatory elements was founded, false otherwise
     */
    static bool staticFindFromThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(const CT_VirtualAbstractStep* step,
                                                                                        CT_InAbstractResultModel* inputResultModel);

    /**
     * @brief Call it if the input result model must search recursively. It will search recursively by going up from this step to the root
     *        step.
     * @param step : The step where to start the research of elements
     * @param inputResultModel : The model to use to do the research
     * @return Returns true if all obligatory elements was founded, false otherwise
     */
    static bool staticFindRecursivelyFromThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(const CT_VirtualAbstractStep* step,
                                                                                                   CT_InAbstractResultModel* inputResultModel);

    /**
     * @brief Call it to find all matching output models for the input result model in this step only (not recursively)
     * @param step : The step where to start the research of elements
     * @param inputResultModel : The model to use to do the research
     * @return Returns true if all obligatory elements was founded, false otherwise
     */
    static bool staticFindOnlyForThisStepAllMatchingOutputModelsThatMatchWithTheInputModel(const CT_VirtualAbstractStep* step,
                                                                                           CT_InAbstractResultModel* inputResultModel);
};

#endif // CT_INRESULTMODELMANAGER_H
