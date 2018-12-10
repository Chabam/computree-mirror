#ifndef CT_HANDLEINBASE_H
#define CT_HANDLEINBASE_H

#include "ct_model/handle/ct_handleminmaxwithmodelt.h"
#include "ct_model/inModel/tools/ct_instdresultmodelpossibility.h"

template<class ModelType, int min = 1, int max = 1>
class CT_HandleInBase : public CT_HandleMinMaxWithModelT<ModelType, min, max> {
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
    ModelType* modelForPossibilities(const HandleInResult& inResult, const int& inResultPossibilityIndex = 0) const {
        MODELS_ASSERT(model() != NULL);
        MODELS_ASSERT(inResult.model() != NULL);
        MODELS_ASSERT(inResult.model()->nPossibilitySaved() > inResultPossibilityIndex);
        MODELS_ASSERT(static_cast<CT_InStdResultModelPossibility*>(inResult.model()->possibilitySavedAt(inResultPossibilityIndex))->inResultModel() != NULL);

        return static_cast<ModelType*>(static_cast<CT_InStdResultModelPossibility*>(inResult.model()->possibilitySavedAt(inResultPossibilityIndex))->inResultModel()->recursiveSearchTheModelThatWasACopiedModelFromThisOriginalModel(model()));
    }
};

#endif // CT_HANDLEINBASE_H
