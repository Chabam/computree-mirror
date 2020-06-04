#ifndef DM_ATTRIBUTESBUILDINGCOLLECTIONT_H
#define DM_ATTRIBUTESBUILDINGCOLLECTIONT_H

#include "ct_model/outModel/abstract/ct_outabstractitemmodel.h"
#include "ct_step/abstract/ct_virtualabstractstep.h"
#include "ct_result/ct_resultgroup.h"
#include "ct_iterator/ct_singlemodeliteratorstdstyleforresultgroup.h"

/**
 * @brief Recursively build a collection of attributes available from a step.
 */
template<typename Type>
class DM_AttributesBuildingCollectionT
{
public:
    using Visitor = std::function<bool (const CT_OutAbstractModel*, Type*)>;

    DM_AttributesBuildingCollectionT();

    bool buildFrom(CT_VirtualAbstractStep* step);

    bool visitFrom(CT_VirtualAbstractStep* step, Visitor v) const;

    const QList<Type*>& attributesCollection() const;

private:
    QList<Type*>   m_collection;

    bool recursiveVisitAttributesFromStep(const CT_VirtualAbstractStep* step, const Visitor& v) const;
};

#include "tools/attributes/dm_attributesbuildingcollectiont.hpp"

#endif // DM_ATTRIBUTESBUILDINGCOLLECTIONT_H
