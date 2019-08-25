#ifndef CT_STDLASPOINTSATTRIBUTESCONTAINER_H
#define CT_STDLASPOINTSATTRIBUTESCONTAINER_H

#include "ctliblas/ctliblas_global.h"

#include "ct_itemdrawable/abstract/ct_abstractpointsattributescontainer.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"

#include "ctliblas/tools/las/ct_lasdefine.h"
#include "ctliblas/tools/las/ct_lasdata.h"

/**
 *  This class is a standard container for LAS points attributes
 */
class CTLIBLAS_EXPORT CT_StdLASPointsAttributesContainer : public CT_AbstractPointsAttributesContainer
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_StdLASPointsAttributesContainer, CT_AbstractPointsAttributesContainer, LAS point attributes)

    using SuperClass = CT_AbstractPointsAttributesContainer;

public:     

    CT_StdLASPointsAttributesContainer() = default;

    CT_StdLASPointsAttributesContainer(const CT_StdLASPointsAttributesContainer& other) = default;

    /**
     * @brief Insert the CT_AbstractPointsAttributes at key 'key'. If an attribute already exist it will be replaced.
     *        The attribute is not deleted from memory, you are the owner.
     */
    virtual void insertPointsAttributesAt(CT_LasDefine::LASPointAttributesType key, const CT_AbstractPointAttributesScalar *att);

    /**
     * @brief Return the list of type of attributes present in this container
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> lasPointAttributesTypes() const;

    virtual void getLASDataAt(const size_t i, CT_LASData &data) const;

    /**
     * @brief Return the hash map of attributes
     */
    virtual QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar*> lasPointsAttributes() const;

    CT_ITEM_COPY_IMP(CT_StdLASPointsAttributesContainer)

};

#endif // CT_STDLASPOINTSATTRIBUTESCONTAINER_H
