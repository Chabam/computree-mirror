#ifndef CT_SPARSEATTRIBUTEMANAGER_H
#define CT_SPARSEATTRIBUTEMANAGER_H

#include "ct_global/ct_cloudscontext.h"
#include "ct_global/ct_repositorymanager.h"

#include "ct_attributes/managers/abstract/ct_abstractxattributemanager.h"
#include "ct_attributes/setters/ct_sparseattributesetter.h"

template<typename T, typename TCIR>
class CT_SparseAttributeManager : public CT_AbstractXAttributeManager<T>
{
public:
    using PEF = typename TCIR::element_type::element_type;
    using Setter = CT_SparseAttributeSetter<T, PEF>;
    using SetterPtr = std::unique_ptr<Setter>;

    Setter createAttributesSetter(CT_CIR cir);
    SetterPtr createAttributesSetterPtr(CT_CIR cir);

    CT_SparseAttributeManager();

    bool hasBeenSet(const size_t& globalIndex) const final;

    bool visitValues(typename CT_AbstractXAttributeManager<T>::Visitor v) const final;

    size_t numberOfSetValues() const final;

    bool isEmpty() const final;

    const T& tAt(const size_t& globalIndex, bool* hasBeenSet = nullptr) const final;

    const T& tAtLocalIndex(const size_t& localIndex) const final;

    void createCollectionsIfNotCreated();

protected:
    using SetterPtrAttributesCollection = typename Setter::PtrParseAttributesCollectionType;
    using SetterAttributesCollection = typename Setter::template ParseAttributesCollection<PEF>;

    SetterPtrAttributesCollection   mAttributes;
    T                               mDefaultValue;
};

template<typename T, typename TCIR>
CT_SparseAttributeManager<T, TCIR>::CT_SparseAttributeManager() :
    mDefaultValue(T())
{
}

template<typename T, typename TCIR>
typename CT_SparseAttributeManager<T, TCIR>::Setter CT_SparseAttributeManager<T, TCIR>::createAttributesSetter(CT_CIR cir)
{
    this->createCollectionsIfNotCreated();

    return CT_SparseAttributeSetter<T, PEF>(cir, mAttributes);
}

template<typename T, typename TCIR>
typename CT_SparseAttributeManager<T, TCIR>::SetterPtr CT_SparseAttributeManager<T, TCIR>::createAttributesSetterPtr(CT_CIR cir)
{
    this->createCollectionsIfNotCreated();

    return std::make_unique<CT_SparseAttributeManager<T, TCIR>::SetterPtr>(cir, mAttributes);
}

template<typename T, typename TCIR>
bool CT_SparseAttributeManager<T, TCIR>::hasBeenSet(const size_t& globalIndex) const
{
    if(mAttributes.isNull())
        return false;

    return mAttributes->abstractModifiableCloudIndex()->contains(globalIndex);
}

template<typename T, typename TCIR>
bool CT_SparseAttributeManager<T, TCIR>::visitValues(typename CT_AbstractXAttributeManager<T>::Visitor v) const
{
    if(mAttributes.isNull())
        return true;

    const SetterAttributesCollection* collection = dynamic_cast<SetterAttributesCollection*>(mAttributes->abstractModifiableCloudIndex());

    auto it = collection->cbegin();
    auto end = collection->cend();

    while(it != end)
    {
        if(!v((*it).first, (*it).second))
            return false;

        ++it;
    }

    return true;
}

template<typename T, typename TCIR>
size_t CT_SparseAttributeManager<T, TCIR>::numberOfSetValues() const
{
    if(mAttributes.isNull())
        return 0;

    return mAttributes->abstractModifiableCloudIndex()->size();
}

template<typename T, typename TCIR>
bool CT_SparseAttributeManager<T, TCIR>::isEmpty() const
{
    if(mAttributes.isNull())
        return true;

    return (mAttributes->abstractModifiableCloudIndex()->size() == 0);
}

template<typename T, typename TCIR>
const T& CT_SparseAttributeManager<T, TCIR>::tAt(const size_t& globalIndex, bool* hasBeenSet) const
{
    SetterAttributesCollection* c = dynamic_cast<SetterAttributesCollection*>(mAttributes->abstractModifiableCloudIndex());

    const auto it = c->findAtGlobalIndex(globalIndex);

    if(it != c->cend())
    {
        if(hasBeenSet != nullptr)
            (*hasBeenSet) = true;

        return it->second;
    }

    if(hasBeenSet != nullptr)
        (*hasBeenSet) = false;

    return mDefaultValue;
}

template<typename T, typename TCIR>
const T& CT_SparseAttributeManager<T, TCIR>::tAtLocalIndex(const size_t& localIndex) const
{
    SetterAttributesCollection* c = dynamic_cast<SetterAttributesCollection*>(mAttributes->abstractModifiableCloudIndex());

    const auto it = c->findAtLocalIndex(localIndex);

    if(it != c->cend())
        return it->second;

    return mDefaultValue;
}

template<typename T, typename TCIR>
void CT_SparseAttributeManager<T, TCIR>::createCollectionsIfNotCreated()
{
    if(!mAttributes.isNull())
        return;

    mAttributes = PS_REPOSITORY->createNewIndexCloudTSyncWithPEF<PEF, CT_SparseAttributeSetter<T, PEF>::ParseAttributesCollection>();
}

#endif // CT_SPARSEATTRIBUTEMANAGER_H