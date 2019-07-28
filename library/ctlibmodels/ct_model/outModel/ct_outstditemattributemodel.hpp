#include "ct_outstditemattributemodel.h"

template<class ItemAttribute>
CT_OutStdItemAttributeModel<ItemAttribute>::CT_OutStdItemAttributeModel(const ICategoryForModel* category,
                                                                        const QString& displayableName,
                                                                        const QString& shortDescription,
                                                                        const QString& detailledDescription,
                                                                        ItemAttribute* prototype) : SuperClass(displayableName)
{
    if(prototype == nullptr) {
        prototype = new ItemAttribute();
        prototype->setCategory(static_cast<const ItemAttribute::CategoryType*>(category));
    }

    Q_ASSERT(prototype->category() == category);

    setPrototype(prototype);
    setShortDescription(shortDescription);
    setDetailledDescription(detailledDescription);
}

template<class ItemAttribute>
CT_OutAbstractModel* CT_OutStdItemAttributeModel<ItemAttribute>::copy() const
{
    return new CT_OutStdItemAttributeModel(*this);
}
