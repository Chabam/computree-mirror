#include "tools/attributes/dm_attributesbuildingcollectiont.h"

#include "ct_result/abstract/ct_abstractresult.h"

template<typename Type>
DM_AttributesBuildingCollectionT<Type>::DM_AttributesBuildingCollectionT()
{
}

template<typename Type>
bool DM_AttributesBuildingCollectionT<Type>::buildFrom(CT_VirtualAbstractStep *step)
{
    m_collection.clear();

    if(step == nullptr)
        return false;

    recursiveVisitAttributesFromStep(step, [this](const CT_OutAbstractModel*, Type* tt) -> bool
    {
        if(!m_collection.contains(tt))
            m_collection.append(tt);

        return  true;
    });

    return true;
}

template<typename Type>
bool DM_AttributesBuildingCollectionT<Type>::visitFrom(CT_VirtualAbstractStep* step, Visitor v) const
{
    if(step == nullptr)
        return false;

    return recursiveVisitAttributesFromStep(step, v);
}

template<typename Type>
const QList<Type*>& DM_AttributesBuildingCollectionT<Type>::attributesCollection() const
{
    return m_collection;
}

template<typename Type>
bool DM_AttributesBuildingCollectionT<Type>::recursiveVisitAttributesFromStep(const CT_VirtualAbstractStep* step, const Visitor& v) const
{
    // build attributes from models
    if(!step->visitOutResultModels([&v](const CT_OutAbstractResultModel* resModel) -> bool
    {
        if(resModel->result() == nullptr)
            return true;

        resModel->recursiveVisitOutChildrens([&v](const CT_OutAbstractModel* child) -> bool
        {
            Type* proto = dynamic_cast<Type*>(child->prototype());

            if((proto != nullptr) && (child->result() != nullptr))
            {
                auto iterator = CT_SingleModelIteratorStdStyleForResultGroup<Type>::createCompleteIterator(child);

                for(Type* tt : iterator)
                {
                    if(!v(child, tt))
                        return false;
                }
            }

            return true;
        });

        return true;
    }))
    {
        return false;
    }

    return step->visitChildrens([this, &v](const CT_VirtualAbstractStep*, const CT_VirtualAbstractStep* child) -> bool {
        return this->recursiveVisitAttributesFromStep(child, v);
    });
}
