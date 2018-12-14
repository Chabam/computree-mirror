#ifndef CT_HANDLEITERATIONDECORATOR_H
#define CT_HANDLEITERATIONDECORATOR_H

#include "tools/sfinae.h"
#include "ct_iterator/ct_multimodeliteratorstdstyle.h"
#include "ct_iterator/ct_singlemodeliteratorstdstyleforresultgroup.h"
#include "ct_handleiteratort.h"

template<typename SuperClass, typename ItemT, typename ParentT = CT_AbstractItem>
class CT_HandleIterationDecorator : public SuperClass {
    using SingleModelIterator = CT_SingleModelIteratorStdStyleForResultGroup<ItemT, ParentT>;
    using MultiModelIterator = CT_MultiModelIteratorStdStyle<SingleModelIterator, ItemT>;
    using ConstMultiModelIterator = CT_MultiModelIteratorStdStyle<SingleModelIterator, const ItemT>;

public:
    using iterator = MultiModelIterator;
    using final_iterator = CT_HandleIteratorT<iterator>;
    using const_iterator = ConstMultiModelIterator;
    using final_const_iterator = CT_HandleIteratorT<const_iterator>;

    CT_HandleIterationDecorator() = default;

    template<class HandleInResult>
    final_const_iterator iterateInputs(const HandleInResult& inResult) const {
        QVector<DEF_CT_AbstractGroupModelOut*> outModels;
        int currentIndex = 0;

        const CT_InAbstractModel* inResultModel = inResult.model();

        MODELS_ASSERT(inResultModel != NULL);

        const int nResultPossibility = inResultModel->nPossibilitySaved();

        for(int i=0; i<nResultPossibility; ++i) {
            if(inResultModel->possibilitySavedAt(i)->isSelected()) {
                const CT_InStdModelPossibilitySelectionGroup* selectionGroup = this->modelForPossibilities(inResult, i)->possibilitiesGroup();

                outModels.resize(outModels.size() + selectionGroup->nPossibilitySelected());

                for(const CT_InStdModelPossibility* possibility : selectionGroup->selectedPossibilities()) {
                    outModels[currentIndex++] = static_cast<DEF_CT_AbstractGroupModelOut*>(possibility->outModel());
                }
            }
        }

        return final_const_iterator(const_iterator::create(outModels.begin(), outModels.end()), const_iterator());
    }

    template<class HandleInResult>
    final_iterator iterateOutputs(const HandleInResult& inResult) const {
        return internalIterateOutputs(inResult, std::integral_constant<bool, IsAResultModelCopy<HandleInResult::ModelType>::Is>());
    }

private:
    // iterate with a result that is a copy
    template<class HandleInResultCopy>
    final_iterator internalIterateOutputs(const HandleInResultCopy& inResultCopy, std::true_type) const {
        QVector<DEF_CT_AbstractGroupModelOut*> outModels;
        findOutModelsFromResultCopy(inResultCopy, outModels);

        return final_iterator(iterator::create(outModels.begin(), outModels.end()), iterator());
    }

    // iterate with a result that is not a copy
    template<class HandleInResultCopy>
    final_iterator internalIterateOutputs(const HandleInResultCopy&, std::false_type) const {
        static_assert(false, "You can not modify input results, so this iterate method is disabled with an intput handle that is not a copy ! Use method \"iterateInputs(...)\" instead.");
    }

    template<class HandleInResultCopy>
    void findOutModelsFromResultCopy(const HandleInResultCopy& inResultCopy, QVector<DEF_CT_AbstractGroupModelOut*>& outModels) const {
        using InResultModelType = HandleInResultCopy::ModelType;
        using InResultToolType = InResultModelType::ToolToModifyResultModelCopiesType;
        using OutResultModelType = InResultToolType::ModelType;

        int currentIndex = 0;

        const InResultModelType* inResultModel = inResultCopy.model();

        MODELS_ASSERT(inResultModel != NULL);

        InResultToolType* tool = inResultModel->toolToModifyResultModelCopies();

        MODELS_ASSERT(tool != NULL);

        const int nResultPossibility = inResultModel->nPossibilitySaved();

        for(int i=0; i<nResultPossibility; ++i) {
            if(inResultModel->possibilitySavedAt(i)->isSelected()) {
                const CT_InStdModelPossibilitySelectionGroup* selectionGroup = this->modelForPossibilities(inResultCopy, i)->possibilitiesGroup();

                outModels.resize(outModels.size() + selectionGroup->nPossibilitySelected());

                for(const CT_InStdModelPossibility* possibility : selectionGroup->selectedPossibilities()) {
                    const CT_OutAbstractModel* originalOutModel = possibility->outModel();

                    const auto visitor = [&originalOutModel, &outModels, &currentIndex](const OutResultModelType* outResultModel) -> bool {

                        CT_OutAbstractModel* copiedOutModel = outResultModel->recursiveSearchTheModelThatWasACopiedModelFromThisOriginalModel(originalOutModel);

                        Q_ASSERT(copiedOutModel != NULL);

                        outModels[currentIndex++] = static_cast<DEF_CT_AbstractGroupModelOut*>(copiedOutModel);
                        return true;
                    };

                    tool->visitOutResultModelCopies(visitor);
                }
            }
        }
    }
};

#endif // CT_HANDLEITERATIONDECORATOR_H
