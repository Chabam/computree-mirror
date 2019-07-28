#ifndef CT_POINTSATTRIBUTESSCALARMASKT_H
#define CT_POINTSATTRIBUTESSCALARMASKT_H

#include "ct_cloud/ct_standardcloudstdvectort.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"
#include "ct_attributes/abstract/ct_abstractattributesscalar.h"

/**
 *  This class is like a CT_PointsAttributesScalarTemplated but the difference is that the cloud of information
 *  can mask the information of the element in cloud to get the real information.
 *
 *  @example You must create a structure with the 'MASK' typedef :
 *
 *           struct myStruct { // information of a point
 *              typedef quint8 MASK; // typedef of the mask type // OBLIGATORY in your struct and named 'MASK'
 *
 *              quint8 entire;   // first element is contained in 3 bits and second element in 5 bits // OBLIGATORY in your struct and named 'entire'
 *           };
 *
 *           // This vector use the structure to contains datas for all points
 *           CT_StandardCloudStdVectorT<myStruct> vector(size);
 *
 *           // Create two CT_PointsAttributesScalarMaskT
 *           // The first must contains a pointer to the vector and delete it when it will be destroyed from memory
 *           // The second contains also a pointer to the vector but doesn't delete it when it will be destroyed from memory
 *           CT_PointsAttributesScalarMaskT<myStruct> firstElement(model, result, 224, 5, vector, true);    // the mask is '0b11100000' in binary, and shift right is 5
 *           CT_PointsAttributesScalarMaskT<myStruct> secondElement(model, result, 31, 0, vector, false);     // the mask is '0b00011111' in binary, and shift right is 0
 *
 *           // Set 6 for the first element and 4 for the second element
 *           myStruct tmp;
 *           tmp.entire = 196; (egual 6 for the first and 4 for the second if use the mask)
 *
 *           vector.tAt(0) = tmp;
 *
 *           double first = firstElement.dValueAt(0);       // first == 6
 *           double second = secondElement.dValueAt(0);     // second == 4
 */
template<typename StructType>
class CT_PointsAttributesScalarMaskT : public CT_AbstractPointAttributesScalar, public CT_AbstractAttributesScalar
{
    CT_TYPE_TEMPLATED_IMPL_MACRO(CT_PointsAttributesScalarMaskT, StructType, CT_AbstractPointAttributesScalar, Point %1 attributes)

    using SuperClass1 = CT_AbstractPointAttributesScalar;
    using SuperClass2 = CT_AbstractAttributesScalar;

public:

    typedef typename StructType::MASK StructMASK;

    CT_PointsAttributesScalarMaskT();
    CT_PointsAttributesScalarMaskT(StructMASK mask,
                                   quint16 shiftRight,
                                   CT_StandardCloudStdVectorT<StructType> *collection,
                                   CT_PCIR pcir,
                                   bool autoDeleteCollection = true);

    CT_PointsAttributesScalarMaskT(StructMASK mask,
                                   quint16 shiftRight,
                                   const StructMASK &min,
                                   const StructMASK &max,
                                   CT_StandardCloudStdVectorT<StructType> *collection,
                                   CT_PCIR pcir,
                                   bool autoDeleteCollection = true);

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
     *          - Pointer of the collection
     *          - Mask
     *          - Shift Right
     *          - Min and max value
     *
     *        What is initialized differently :
     *          - Parent is set to nullptr
     *          - isSelected and isDisplayed is set to false
     *          - Document list is not copied
     *          - Auto delete collection is set to false
     */
    CT_PointsAttributesScalarMaskT(const CT_PointsAttributesScalarMaskT<StructType>& other);

    ~CT_PointsAttributesScalarMaskT() override;

    double dMin() const;

    double dMax() const;

    double dValueAt(const size_t &index) const;

    size_t attributesSize() const;

    /**
     * @brief Returns the collection used by this object
     */
    CT_StandardCloudStdVectorT<StructType>* collection() const;

    CT_ITEM_COPY_IMP(CT_PointsAttributesScalarMaskT<StructType>)

private:
    CT_StandardCloudStdVectorT<StructType>*             m_collection;
    bool                                                m_autoDeleteCollection;
    StructMASK                                          m_mask;
    quint16                                             m_shiftRight;
    StructMASK                                          m_min;
    StructMASK                                          m_max;
};

#include "ct_itemdrawable/ct_pointsattributesscalarmaskt.hpp"

#endif // CT_POINTSATTRIBUTESSCALARMASKT_H
