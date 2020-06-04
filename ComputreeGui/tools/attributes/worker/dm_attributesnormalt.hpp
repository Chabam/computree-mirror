#include "tools/attributes/worker/dm_attributesnormalt.h"

#include "ct_global/ct_context.h"

template<typename Type>
DM_AttributesNormalT<Type>::DM_AttributesNormalT(bool local) : DM_AbstractAttributesNormal(local)
{
    m_an = nullptr;
}

template<typename Type>
bool DM_AttributesNormalT<Type>::setTypeAttributes(const Type *ta, const CT_AttributesNormal<Type> *an)
{
    if(ta != dynamic_cast<const Type*>(an))
        return false;

    setAttributes(ta);
    m_an = const_cast<CT_AttributesNormal<Type>*>(an);

    return true;
}

template<typename Type>
void DM_AttributesNormalT<Type>::attributesDeleted()
{
    m_an = nullptr;
}

template<typename Type>
CT_AttributesNormal<Type>* DM_AttributesNormalT<Type>::normalAttributes() const
{
    return m_an;
}

template<typename Type>
Type* DM_AttributesNormalT<Type>::abstractTypeAttributes() const
{
    return dynamic_cast<Type*>(abstractAttributes());
}
