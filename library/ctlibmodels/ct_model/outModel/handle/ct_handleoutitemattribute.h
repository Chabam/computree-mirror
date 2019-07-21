#ifndef CT_HANDLEOUTITEMATTRIBUTE_H
#define CT_HANDLEOUTITEMATTRIBUTE_H

#include "ct_model/handle/ct_handlewithmultiplemodelt.h"
#include "ct_model/outModel/ct_outstditemattributemodel.h"

// CT_AbstractHandleOutItemAttribute can only be used with method to add an item attribute in an item. It can
// not be used with other handle to add an item attribute handle !
using CT_AbstractHandleOutItemAttribute = CT_HandleWithMultipleAbstractModel<DEF_CT_OutAbstractIAModel>;

template<class ItemAttributeT>
class CT_HandleOutItemAttribute : public CT_HandleWithMultipleModelT<CT_OutStdItemAttributeModel<ItemAttributeT>> {

    using SuperClass = CT_HandleWithMultipleModelT<CT_OutStdItemAttributeModel<ItemAttributeT>>;

public:
    using ItemAttributeType = ItemAttributeT;

    CT_HandleOutItemAttribute() : SuperClass()  {
        static_assert(std::is_convertible<ItemAttributeT*, IItemAttributeForModel*>::value, "CT_HandleOutItemAttribute is only compatible with classes that inherit from IItemAttributeForModel");
    }

    template<typename... Args>
    ItemAttributeT* createInstance(Args... args) {
        return new ItemAttributeT(args...);
    }
};

#endif // CT_HANDLEOUTITEMATTRIBUTE_H
