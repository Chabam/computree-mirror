#include "tools/attributes/worker/dm_attributescolort.h"

template<typename Type>
DM_AttributesColorT<Type>::DM_AttributesColorT(bool local) : DM_AbstractAttributesColor(local)
{
    m_ac = nullptr;
}

template<typename Type>
bool DM_AttributesColorT<Type>::setTypeAttributes(const Type *ta, const CT_AttributesColor<Type>* ac)
{
    if(ta != dynamic_cast<const Type*>(ac))
        return false;

    setAttributes(ta);
    m_ac = const_cast<CT_AttributesColor<Type>*>(ac);

    return true;
}

template<typename Type>
void DM_AttributesColorT<Type>::attributesDeleted()
{
    m_ac = nullptr;
}

template<typename Type>
CT_AttributesColor<Type>* DM_AttributesColorT<Type>::colorAttributes() const
{
    return m_ac;
}

template<typename Type>
Type* DM_AttributesColorT<Type>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}
