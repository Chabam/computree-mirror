#ifndef CT_HANDLEINBASE_H
#define CT_HANDLEINBASE_H

#include "ct_model/inModel/handle/ct_inhandleminmaxwithmodelt.h"
#include "ct_model/inModel/tools/ct_instdresultmodelpossibility.h"

template<class ModelType, int min = 1, int max = 1>
class CT_HandleInBase : public CT_InHandleMinMaxWithModelT<ModelType, min, max> {
public:

    /**
     * @brief If you want to get the model to use to know how many possibilies has been saved or selected you must call this
     *        method because the method "model()" return the original model that doesn't have possibilities.
     * @param inResult : the handle of the input result that contains the model of this handle
     * @param inResultPossibilityIndex : the result handle can have more that one possibility. In each possibility the model of this handle
     *                                   can have a different number of possibilities. So you must pass here the index of the saved possibility than
     *                                   you want to know something.
     * @return The model that you can use to know how many possibilities was save or selected or etc...
     */
    template<class HandleInResult>
    ModelType* inModelForSavedPossibilities(const HandleInResult& inResult, const int& inResultPossibilityIndex = 0) const {
        MODELS_ASSERT(model() != nullptr);
        MODELS_ASSERT(inResult.model() != nullptr);
        MODELS_ASSERT(inResult.model()->nPossibilitySaved() > inResultPossibilityIndex);
        MODELS_ASSERT(static_cast<CT_InStdResultModelPossibility*>(inResult.model()->possibilitySavedAt(inResultPossibilityIndex))->inResultModel() != nullptr);

        return static_cast<ModelType*>(static_cast<CT_InStdResultModelPossibility*>(inResult.model()->possibilitySavedAt(inResultPossibilityIndex))->inResultModel()->recursiveSearchTheModelThatWasACopiedModelFromThisOriginalModel(model()));
    }

    /**
     * @brief If you want to get the model to use to know how many possibilies has been saved or selected you must call this
     *        method because the method "model()" return the original model that doesn't have possibilities.
     * @param inResult : the handle of the input result that contains the model of this handle
     * @param inResultPossibilityIndex : the result handle can have more that one possibility. In each possibility the model of this handle
     *                                   can have a different number of possibilities. So you must pass here the index of the selected possibility than
     *                                   you want to know something.
     * @return The model that you can use to know how many possibilities was save or selected or etc...
     */
    template<class HandleInResult>
    ModelType* inModelForSelectedPossibilities(const HandleInResult& inResult, const int& inResultSelectedPossibilityIndex = 0) const {
        MODELS_ASSERT(model() != nullptr);
        MODELS_ASSERT(inResult.model() != nullptr);
        MODELS_ASSERT(inResult.model()->nPossibilitySelected() > inResultSelectedPossibilityIndex);
        MODELS_ASSERT(static_cast<CT_InStdResultModelPossibility*>(inResult.model()->possibilitySelectedAt(inResultSelectedPossibilityIndex))->inResultModel() != nullptr);

        return static_cast<ModelType*>(static_cast<CT_InStdResultModelPossibility*>(inResult.model()->possibilitySelectedAt(inResultSelectedPossibilityIndex))->inResultModel()->recursiveSearchTheModelThatWasACopiedModelFromThisOriginalModel(model()));
    }

    /**
     * @brief Returns the out model for the specified selected possibility
     * @param inResult : the handle of the input result that contains the model of this handle
     * @param inResultSelectedPossibilityIndex : the result handle can have more that one possibility. In each possibility the model of this handle
     *                                   can have a different number of possibilities. So you must pass here the index of the selected possibility than
     *                                   you want to know something.
     * @param inModelPossibilitySelectedIndex : the handle can have more that one possibility. You must pass here the index of the selected possibility than
     *                                          you want to get the out model.
     * @return The out model of the specified selected possibility of the input model
     */
    template<class HandleInResult>
    CT_OutAbstractModel* outModelSelected(const HandleInResult& inResult, const int& inResultSelectedPossibilityIndex = 0, const int& inModelSelectedPossibilityIndex = 0) const {
        MODELS_ASSERT(model() != nullptr);
        MODELS_ASSERT(inResult.model() != nullptr);

        if(inResultSelectedPossibilityIndex >= inResult.model()->nPossibilitySelected())
            return nullptr;

        if(static_cast<CT_InStdResultModelPossibility*>(inResult.model()->possibilitySelectedAt(inResultSelectedPossibilityIndex))->inResultModel() == nullptr)
            return nullptr;

        ModelType* inModelForPossibilities = inModelForSelectedPossibilities(inResult, 0);

        if(inModelForPossibilities == nullptr)
            return nullptr;

        if(inModelSelectedPossibilityIndex >= inModelForPossibilities->nPossibilitySelected())
            return nullptr;

        return inModelForPossibilities->possibilitySelectedAt(inModelSelectedPossibilityIndex)->outModel();
    }
};

#endif // CT_HANDLEINBASE_H
