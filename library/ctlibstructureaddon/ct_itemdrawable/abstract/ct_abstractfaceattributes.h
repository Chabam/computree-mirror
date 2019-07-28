#ifndef CT_ABSTRACTFACEATTRIBUTES_H
#define CT_ABSTRACTFACEATTRIBUTES_H

#include "ct_defines.h"

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"

#include "ct_itemdrawable/abstract/ct_abstractattributes.h"

/**
 * @brief Represents a cloud attribute for face.
 */
class CTLIBSTRUCTUREADDON_EXPORT CT_AbstractFaceAttributes : public CT_AbstractAttributes
{
    Q_OBJECT
    CT_TYPE_IMPL_MACRO(CT_AbstractFaceAttributes, CT_AbstractAttributes, Face attributes)

    using SuperClass = CT_AbstractAttributes;

public:
    CT_AbstractFaceAttributes();
    CT_AbstractFaceAttributes(CT_FCIR cir);

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
     *          - Shared pointer to face cloud index registered
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     */
    CT_AbstractFaceAttributes(const CT_AbstractFaceAttributes& other) = default;

    /**
     * @brief Returns the face cloud index
     */
    inline const CT_AbstractFaceCloudIndex* abstractCloudIndex() const { return faceCloudIndex(); }

    /**
     * @brief Returns the face cloud index
     */
    const CT_AbstractFaceCloudIndex* faceCloudIndex() const;

    /**
     * @brief Returns the face cloud index registered
     */
    CT_FCIR faceCloudIndexRegistered() const;


private:
    // TODO
    //static CT_StandardAbstractFaceAttributesDrawManager AEA_DRAW_MANAGER;

    CT_FCIR                     m_cir;
    CT_AbstractFaceCloudIndex*  m_aci;
};

#endif // CT_ABSTRACTFACEATTRIBUTES_H
