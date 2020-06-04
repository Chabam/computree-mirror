#include "ct_itemdrawable/ct_pointsattributesscalarmaskt.h"

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT() :
    m_mask(StructMASK(0)),
    m_shiftRight(0),
    SuperClass()
{
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT(const CT_PointsAttributesScalarMaskT<StructType>& other) :
    m_mask(other.m_mask),
    m_shiftRight(other.m_shiftRight),
    SuperClass(other)
{
}

template<typename StructType>
typename CT_PointsAttributesScalarMaskT<StructType>::StructMASK CT_PointsAttributesScalarMaskT<StructType>::convertScalarOfManagerToScalar(const StructType& value) const
{
    return ((value.entire & m_mask) >> m_shiftRight);
}
