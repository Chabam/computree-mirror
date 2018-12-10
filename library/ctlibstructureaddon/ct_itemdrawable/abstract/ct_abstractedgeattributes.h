#ifndef CT_ABSTRACTEDGEATTRIBUTES_H
#define CT_ABSTRACTEDGEATTRIBUTES_H

#include "ct_defines.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

/**
 * @brief Represents a cloud attribute for edge.
 */
class CTLIBSTRUCTUREADDON_EXPORT CT_AbstractEdgeAttributes : public CT_AbstractAttributes
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractEdgeAttributes, CT_AbstractAttributes, Edge attributes)

    using SuperClass = CT_AbstractAttributes;

public:
    CT_AbstractEdgeAttributes();
    CT_AbstractEdgeAttributes(CT_ECIR cir);

    /**
     * @brief Copy constructor.
     *
     *        What is copied :
     *          - Pointer of the result and model of the original item.
     *          - Unique ID
     *          - Pointer of base and alternative draw manager
     *          - Displayable name
     *          - Center coordinates
     *          - Default Color
     *          - Shared pointer to edge cloud index registered
     *
     *        What is initialized differently :
     *          - Parent is set to NULL
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_AbstractEdgeAttributes(const CT_AbstractEdgeAttributes& other) = default;

    /**
     * @brief Returns the edge cloud index
     */
    inline const CT_AbstractEdgeCloudIndex* abstractCloudIndex() const { return edgeCloudIndex(); }

    /**
     * @brief Returns the edge cloud index
     */
    const CT_AbstractEdgeCloudIndex* edgeCloudIndex() const;

    /**
     * @brief Returns the edge cloud index registered
     */
    CT_ECIR edgeCloudIndexRegistered() const;

private:
    // TODO
    //static CT_StandardAbstractEdgeAttributesDrawManager AEA_DRAW_MANAGER;

    CT_ECIR                     m_cir;
    CT_AbstractEdgeCloudIndex*  m_aci;
};

#endif // CT_ABSTRACTEDGEATTRIBUTES_H
