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

    recursiveBuildAttributesFromStep(step);

    return true;
}

template<typename Type>
const QList<Type*>& DM_AttributesBuildingCollectionT<Type>::attributesCollection() const
{
    return m_collection;
}

template<typename Type>
void DM_AttributesBuildingCollectionT<Type>::recursiveBuildAttributesFromStep(const CT_VirtualAbstractStep* step)
{
    QList<Type*>& collection = m_collection;

    // build attributes from models
    step->visitOutResultModels([&collection](const CT_OutAbstractResultModel* resModel) -> bool {

        resModel->recursiveVisitOutChildrens([&collection](const CT_OutAbstractModel* child) -> bool {

            Type* proto = dynamic_cast<Type*>(child->prototype());

            if(proto != nullptr) {
                auto iterator = CT_SingleModelIteratorStdStyleForResultGroup<Type>::createCompleteIterator(child);

                for(Type* tt : iterator) {
                    if(!collection.contains(tt))
                        collection.append(tt);
                }
            }

            return true;
        });

        return true;
    });

    step->visitChildrens([this](const CT_VirtualAbstractStep*, const CT_VirtualAbstractStep* child) -> bool {
        this->recursiveBuildAttributesFromStep(child);
        return true;
    });
}
