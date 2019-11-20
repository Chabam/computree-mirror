#include "ct_laspointformat0.h"

#include "ct_point.h"

CT_LASPointFormat0::CT_LASPointFormat0() : CT_AbstractLASPointFormat()
{
}

size_t CT_LASPointFormat0::sizeInBytes() const
{
    return sizeof(m_emptyData);
}

const CT_LasPointInfo& CT_LASPointFormat0::write(QDataStream &stream, CT_LASHeader* header, const CT_Point &p, const size_t &globalIndex) const
{
    const CT_LasPointInfo& info = infoOfPoint(globalIndex);

    qint32 m_x, m_y, m_z;
    header->inverseTransformPoint(p(CT_Point::X), p(CT_Point::Y), p(CT_Point::Z), m_x, m_y, m_z);

    // x / y / z
    stream << m_x << m_y << m_z;

    // intensity
    CT_WRITE_LAS_SCALAR(info.m_i, quint16);

    // return number + number of return + scan direction flag + edge of flight line
    quint8 tmp = 0;

    if(info.m_rn.first != nullptr)
        tmp |= (((quint8)info.m_rn.first->dValueAt(info.m_rn.second)) << 0);

    if(info.m_nor.first != nullptr)
        tmp |= (((quint8)info.m_nor.first->dValueAt(info.m_nor.second)) << 3);

    if(info.m_sdf.first != nullptr)
        tmp |= (((quint8)info.m_sdf.first->dValueAt(info.m_sdf.second)) << 6);

    if(info.m_efl.first != nullptr)
        tmp |= (((quint8)info.m_efl.first->dValueAt(info.m_efl.second)) << 7);

    stream << tmp;

    // classification
    CT_WRITE_LAS_SCALAR(info.m_c, quint8);

    // scan angle rank
    CT_WRITE_LAS_SCALAR(info.m_sar, qint8);

    // user data
    CT_WRITE_LAS_SCALAR(info.m_ud, quint8);

    // point source ID
    CT_WRITE_LAS_SCALAR(info.m_psID, quint16);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat0::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l;
    l << CT_LasDefine::Intensity;
    l << CT_LasDefine::Return_Number;
    l << CT_LasDefine::Number_of_Returns;
    l << CT_LasDefine::Scan_Direction_Flag;
    l << CT_LasDefine::Edge_of_Flight_Line;
    l << CT_LasDefine::Classification;
    l << CT_LasDefine::Scan_Angle_Rank;
    l << CT_LasDefine::User_Data;
    l << CT_LasDefine::Point_Source_ID;

    return l;
}
