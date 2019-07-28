#ifndef CT_POINTSATTRIBUTESSCALARMASKT_HPP
#define CT_POINTSATTRIBUTESSCALARMASKT_HPP

#include "ct_itemdrawable/ct_pointsattributesscalarmaskt.h"

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT() :
    SuperClass1(),
    SuperClass2(),
    m_collection(nullptr),
    m_autoDeleteCollection(false),
    m_mask(StructMASK(0)),
    m_shiftRight(0),
    m_min(StructMASK(0)),
    m_max(StructMASK(0))
{
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT(StructMASK mask,
                                                                           quint16 shiftRight,
                                                                           CT_StandardCloudStdVectorT<StructType>* collection,
                                                                           CT_PCIR pcir,
                                                                           bool autoDeleteCollection) :
    SuperClass1(pcir),
    SuperClass2(),
    m_collection(collection),
    m_autoDeleteCollection(autoDeleteCollection),
    m_mask(mask),
    m_shiftRight(shiftRight)
{
    auto res = std::minmax_element(m_collection->begin(),
                                   m_collection->end(),
                                   [&mask](const StructType& a, const StructType& b) {
        return (a.entire & mask) < (b.entire & mask);
    });

    m_min = (((*res.first).entire & m_mask) >> m_shiftRight);
    m_max = (((*res.second).entire & m_mask) >> m_shiftRight);
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT(StructMASK mask,
                                                                           quint16 shiftRight,
                                                                           const StructMASK &min,
                                                                           const StructMASK &max,
                                                                           CT_StandardCloudStdVectorT<StructType> *collection,
                                                                           CT_PCIR pcir,
                                                                           bool autoDeleteCollection) :
    SuperClass1(pcir),
    SuperClass2(),
    m_collection(collection),
    m_autoDeleteCollection(autoDeleteCollection),
    m_mask(mask),
    m_shiftRight(shiftRight),
    m_min(min),
    m_max(max)
{
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::CT_PointsAttributesScalarMaskT(const CT_PointsAttributesScalarMaskT<StructType>& other) :
    SuperClass1(other),
    SuperClass2(other),
    m_collection(other.m_collection),
    m_autoDeleteCollection(false),
    m_mask(other.m_mask),
    m_shiftRight(other.m_shiftRight),
    m_min(other.m_min),
    m_max(other.m_max)
{
}

template<typename StructType>
CT_PointsAttributesScalarMaskT<StructType>::~CT_PointsAttributesScalarMaskT()
{
    if(m_autoDeleteCollection)
        delete m_collection;
}

template<typename StructType>
double CT_PointsAttributesScalarMaskT<StructType>::dMin() const
{
    return m_min;
}

template<typename StructType>
double CT_PointsAttributesScalarMaskT<StructType>::dMax() const
{
    return m_max;
}

template<typename StructType>
double CT_PointsAttributesScalarMaskT<StructType>::dValueAt(const size_t &index) const
{
    return ((m_collection->tAt(index).entire & m_mask) >> m_shiftRight);
}

template<typename StructType>
size_t CT_PointsAttributesScalarMaskT<StructType>::attributesSize() const
{
    return m_collection->size();
}

template<typename StructType>
CT_StandardCloudStdVectorT<StructType>* CT_PointsAttributesScalarMaskT<StructType>::collection() const
{
    return m_collection;
}

#endif // CT_POINTSATTRIBUTESSCALARMASKT_HPP
