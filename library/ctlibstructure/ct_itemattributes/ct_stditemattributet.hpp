#ifndef CT_STDITEMATTRIBUTET_HPP
#define CT_STDITEMATTRIBUTET_HPP

#include "ct_stditemattributet.h"

template <typename VType>
CT_StdItemAttributeT<VType>::CT_StdItemAttributeT() : SuperClass(),
    m_data(VType())
{
}

template <typename VType>
CT_StdItemAttributeT<VType>::CT_StdItemAttributeT(const CT_AbstractCategory *category,
                                                  const VType& data) : SuperClass(category),
    m_data(data)
{
}

template <typename VType>
CT_AbstractItemAttribute* CT_StdItemAttributeT<VType>::copy(const DEF_CT_OutAbstractIAModel* model, const CT_AbstractResult* result) const
{
    auto c = new CT_StdItemAttributeT<VType>(*this);
    c->setModel(model);
    c->setResult(result);

    return c;
}

template <typename VType>
VType CT_StdItemAttributeT<VType>::data(const CT_AbstractItem*) const
{
    return m_data;
}

#endif // CT_STDITEMATTRIBUTET_HPP
