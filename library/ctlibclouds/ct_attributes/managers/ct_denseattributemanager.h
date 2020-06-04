#ifndef CT_DENSEATTRIBUTEMANAGER_H
#define CT_DENSEATTRIBUTEMANAGER_H

#include "ct_global/ct_cloudscontext.h"
#include "ct_global/ct_repositorymanager.h"

#include "ct_attributes/managers/abstract/ct_abstractxattributemanager.h"
#include "ct_attributes/setters/ct_denseattributesetter.h"

template<typename T, typename TCIR, int syncWithT>
class CT_DenseAttributeManager : public CT_AbstractXAttributeManager<T>
{
public:
    using Setter = CT_DenseAttributeSetter<T>;
    using SetterPtr = std::unique_ptr<Setter>;

    Setter createAttributesSetter(TCIR cir);
    SetterPtr createAttributesSetterPtr(TCIR cir);

    bool hasBeenSet(const size_t& globalIndex) const final;

    bool visitValues(typename CT_AbstractXAttributeManager<T>::Visitor v) const final;

    size_t numberOfSetValues() const final;

    bool isEmpty() const final;

    const T& tAt(const size_t& globalIndex, bool* hasBeenSet = nullptr) const final;

    const T& tAtLocalIndex(const size_t& localIndex) const final;

    void createCollectionsIfNotCreated();

protected:
    typename CT_DenseAttributeSetter<T>::PtrAttributesCollectionType   mAttributes;
    typename CT_DenseAttributeSetter<T>::PtrAttributeBitCollectionType mBits;
    T                                                                  mDefaultValue;

    bool localIndexToGlobalIndex(const size_t& localIndex, size_t& globalIndex) const;
};

template<typename T, typename TCIR, int syncWithT>
typename CT_DenseAttributeManager<T, TCIR, syncWithT>::Setter CT_DenseAttributeManager<T, TCIR, syncWithT>::createAttributesSetter(TCIR cir)
{
    createCollectionsIfNotCreated();

    return CT_DenseAttributeSetter<T>(cir, mAttributes, mBits);
}

template<typename T, typename TCIR, int syncWithT>
typename CT_DenseAttributeManager<T, TCIR, syncWithT>::SetterPtr CT_DenseAttributeManager<T, TCIR, syncWithT>::createAttributesSetterPtr(TCIR cir)
{
    createCollectionsIfNotCreated();

    return std::make_unique<typename CT_DenseAttributeManager<T, TCIR, syncWithT>::Setter>(cir, mAttributes, mBits);
}

template<typename T, typename TCIR, int syncWithT>
bool CT_DenseAttributeManager<T, TCIR, syncWithT>::hasBeenSet(const size_t& globalIndex) const
{
    if(mAttributes.isNull())
        return false;

    return mBits->cloudT()->value(globalIndex) == true;
}

template<typename T, typename TCIR, int syncWithT>
bool CT_DenseAttributeManager<T, TCIR, syncWithT>::visitValues(typename CT_AbstractXAttributeManager<T>::Visitor v) const
{
    if(mAttributes.isNull())
        return true;

    const auto attributes = mAttributes->cloudT();
    const auto bits = mBits->cloudT();
    const size_t size = bits->size();

    for(size_t i=0; i<size; ++i)
    {
        if(bits->value(i) && !v(i, (*attributes)[i]))
            return false;
    }

    return true;
}

template<typename T, typename TCIR, int syncWithT>
size_t CT_DenseAttributeManager<T, TCIR, syncWithT>::numberOfSetValues() const
{
    if(mAttributes.isNull())
        return false;

    const auto bits = mBits->cloudT();
    const size_t size = bits->size();

    size_t count = 0;
    for(size_t i=0; i<size; ++i)
    {
        if(bits->value(i))
            ++count;
    }

    return count;
}

template<typename T, typename TCIR, int syncWithT>
bool CT_DenseAttributeManager<T, TCIR, syncWithT>::isEmpty() const
{
    if(mAttributes.isNull())
        return true;

    const auto bits = mBits->cloudT();
    const size_t size = bits->size();

    for(size_t i=0; i<size; ++i)
    {
        if(bits->value(i))
            return false;
    }

    return true;
}

template<typename T, typename TCIR, int syncWithT>
const T& CT_DenseAttributeManager<T, TCIR, syncWithT>::tAt(const size_t& globalIndex, bool* hasBeenSet) const
{
    if(mAttributes.isNull())
    {
        if(hasBeenSet != nullptr)
            *hasBeenSet = false;

        return mDefaultValue;
    }

    const auto attributes = mAttributes->cloudT();

    if(hasBeenSet != nullptr)
        (*hasBeenSet) = mBits->cloudT()->value(globalIndex);

    return (*attributes)[globalIndex];
}

template<typename T, typename TCIR, int syncWithT>
const T& CT_DenseAttributeManager<T, TCIR, syncWithT>::tAtLocalIndex(const size_t& localIndex) const
{
    size_t globalIndex = 0;

    if(!localIndexToGlobalIndex(localIndex, globalIndex))
        return mDefaultValue;

    const auto attributes = mAttributes->cloudT();
    return (*attributes)[globalIndex];
}

template<typename T, typename TCIR, int syncWithT>
void CT_DenseAttributeManager<T, TCIR, syncWithT>::createCollectionsIfNotCreated()
{
    if(!mAttributes.isNull())
        return;

    mAttributes = PS_REPOSITORY->createNewCloudT<typename CT_DenseAttributeSetter<T>::RegisteredAttributesCollectionType, typename CT_DenseAttributeSetter<T>::AttributesCollectionType>(CT_Repository::SyncCloudWith(syncWithT));
    mBits = PS_REPOSITORY->createNewCloudT<typename CT_DenseAttributeSetter<T>::RegisteredAttributeBitCollectionType, typename CT_DenseAttributeSetter<T>::AttributeBitCollectionType>(CT_Repository::SyncCloudWith(syncWithT));
}

template<typename T, typename TCIR, int syncWithT>
bool CT_DenseAttributeManager<T, TCIR, syncWithT>::localIndexToGlobalIndex(const size_t& localIndex, size_t& globalIndex) const
{
    if(mAttributes.isNull())
        return false;

    const auto bits = mBits->cloudT();
    const size_t size = bits->size();

    size_t count = 0;
    for(size_t i=0; i<size; ++i)
    {
        if(bits->value(i))
        {
            if(count == localIndex)
            {
                globalIndex = i;
                return true;
            }

            ++count;
        }
    }

    return false;
}

#endif // CT_DENSEATTRIBUTEMANAGER_H

