#ifndef CT_ABSTRACTPOINTSATTRIBUTES_H
#define CT_ABSTRACTPOINTSATTRIBUTES_H

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

#include "ct_itemdrawable/tools/drawmanager/ct_standardabstractpointsattributesdrawmanager.h"
#include "ct_accessibility/ct_iaccesspointcloud.h"
#include "ct_cloudindex/registered/abstract/ct_abstractcloudindexregisteredt.h"

/**
 * @brief Represents a cloud attribute for points.
 */
class CTLIBSTRUCTUREADDON_EXPORT CT_AbstractPointsAttributes : public CT_AbstractAttributes, public CT_IAccessPointCloud
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractPointsAttributes, CT_AbstractAttributes, Point attributes)

    using SuperClass = CT_AbstractAttributes;

public:
    CT_AbstractPointsAttributes();
    CT_AbstractPointsAttributes(CT_PCIR pcir);

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
     *          - Shared pointer to point cloud index registered
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_AbstractPointsAttributes(const CT_AbstractPointsAttributes& other) = default;

    /**
     * @brief Returns the point cloud index
     */
    inline const CT_AbstractPointCloudIndex* abstractCloudIndex() const { return pointCloudIndex(); }

    /**
     * @brief Returns the point cloud index
     */
    virtual const CT_AbstractPointCloudIndex* pointCloudIndex() const;

    /**
     * @brief Returns the point cloud index registered
     */
    virtual CT_PCIR pointCloudIndexRegistered() const;

private:
    static CT_StandardAbstractPointsAttributesDrawManager APA_DRAW_MANAGER;

    CT_PCIR                     m_pcir;
    CT_AbstractPointCloudIndex* m_apci;
};

#endif // CT_ABSTRACTPOINTSATTRIBUTES_H
