#ifndef CT_HANDLEOUTITEMATTRIBUTE_H
#define CT_HANDLEOUTITEMATTRIBUTE_H

#include "ct_model/handle/ct_handlewithmultiplemodelt.h"
#include "ct_model/outModel/ct_outstditemattributemodel.h"

template<class ItemAttributeT>
class CT_HandleOutItemAttribute : public CT_HandleWithMultipleModelT<CT_OutStdItemAttributeModel<ItemAttributeT>> {

    using SuperClass = CT_HandleWithMultipleModelT<CT_OutStdItemAttributeModel<ItemAttributeT>>;

public:
    using ItemAttributeType = ItemAttributeT;

    CT_HandleOutItemAttribute() : SuperClass()  {
        static_assert(std::is_convertible<ItemAttributeT, IItemAttributeForModel>::value, "CT_HandleOutItemAttribute is only compatible with classes that inherit from IItemAttributeForModel");
    }

    template<typename... Args>
    ItemAttributeT* createInstance(Args... args) {
        return new ItemAttributeT(args...);
    }
};

#endif // CT_HANDLEOUTITEMATTRIBUTE_H
