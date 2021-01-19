#ifndef DM_FAKEATTRIBUTEMANAGERT_H
#define DM_FAKEATTRIBUTEMANAGERT_H

#include "ct_attributes/managers/abstract/ct_abstractxattributemanager.h"

template<typename AttributeType, typename CloudType, typename ApplicableToT>
class DM_FakeAttributeManagerT : public CT_AbstractXAttributeManager<AttributeType>
{
    using SuperClass = CT_AbstractXAttributeManager<AttributeType>;

public:
    using ApplicableTo = ApplicableToT;

    DM_FakeAttributeManagerT() :
        mAttributesCloud(nullptr)
    {
    }

    void setAttributesCloud(CloudType* cloud)
    {
        mAttributesCloud = cloud;
    }

    bool hasBeenSet(const size_t& /*globalIndex*/) const final
    {
        return mAttributesCloud != nullptr;
    }

    bool visitValues(typename CT_AbstractXAttributeManager<AttributeType>::Visitor v) const final
    {
        if(mAttributesCloud == nullptr)
            return true;

        const size_t s = mAttributesCloud->size();

        for(size_t i=0; i<s; ++i)
        {
            const AttributeType& attribute = (*mAttributesCloud)[i];

            if(!v(i, attribute))
                return false;
        }

        return true;
    }

    bool visitAllIndexesSet(typename CT_AbstractXAttributeManager<AttributeType>::IVisitor v) const final
    {
        if(mAttributesCloud == nullptr)
            return true;

        const size_t s = mAttributesCloud->size();

        for(size_t i=0; i<s; ++i)
        {
            if(!v(i))
                return false;
        }

        return true;
    }

    size_t numberOfSetValues() const final
    {
        return (mAttributesCloud != nullptr) ? mAttributesCloud->size() : 0;
    }

    bool isEmpty() const final
    {
        return mAttributesCloud == nullptr;
    }

    const AttributeType& tAt(const size_t& globalIndex, bool* hasBeenSet = nullptr) const final
    {
        if(mAttributesCloud == nullptr)
        {
            if(hasBeenSet != nullptr)
                *hasBeenSet = false;

            return mDefaultValue;
        }

        if(hasBeenSet != nullptr)
            *hasBeenSet = true;

        return (*mAttributesCloud)[globalIndex];
    }

    const AttributeType& tAtLocalIndex(const size_t& localIndex) const final
    {
        return tAt(localIndex);
    }

    bool copyAndModifyAttributesOfSForD(CT_CIR , CT_CIR , typename SuperClass::AttributeModificator) final
    {
        return false;
    }

private:
    CloudType*          mAttributesCloud;
    AttributeType       mDefaultValue;
};

#endif // DM_FAKEATTRIBUTEMANAGERT_H
