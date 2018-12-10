#ifndef CT_HANDLEOUTITEM_H
#define CT_HANDLEOUTITEM_H

#include "ct_model/handle/ct_handlewithmultiplemodelt.h"
#include "ct_model/outModel/ct_outstdsingularitemmodel.h"

template<class ItemT>
class CT_HandleOutItem : public CT_HandleWithMultipleModelT<CT_OutStdSingularItemModel<ItemT>> {

    using SuperClass = CT_HandleWithMultipleModelT<CT_OutStdSingularItemModel<ItemT>>;

public:
    using ItemType = ItemT;

    CT_HandleOutItem() : SuperClass() {
        static_assert(std::is_convertible<ItemT, ISingularItemDrawableForModel>::value, "CT_HandleOutItem is only compatible with classes that inherit from ISingularItemDrawableForModel");
    }

    template<typename... Args>
    ItemT* createInstance(Args... args) {
        return new ItemT(args...);
    }
};

#endif // CT_HANDLEOUTITEM_H
