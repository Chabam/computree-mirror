#include "ct_lascolorpacket.h"

#include "abstract/ct_abstractlaspointformat.h"

CT_LASColorPacket::CT_LASColorPacket()
{
}

size_t CT_LASColorPacket::colorPacketSizeInBytes() const
{
    return sizeof(m_emptyData);
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASColorPacket::colorPacketTypesNeeded()
{
    QList<CT_LasDefine::LASPointAttributesType> l;

    l.append(CT_LasDefine::Red);
    l.append(CT_LasDefine::Green);
    l.append(CT_LasDefine::Blue);

    return l;
}

void CT_LASColorPacket::writeColorPacket(QDataStream &stream, const CT_LasPointInfo& info) const
{
    if(info.m_color.first != nullptr)
    {
        const CT_Color& color = info.m_color.first->constColorAt(info.m_color.second);
        stream << quint16(color.r()*257);
        stream << quint16(color.g()*257);
        stream << quint16(color.b()*257);
        return;
    }

    // red
    CT_WRITE_LAS_SCALAR(info.m_red, quint16);

    // green
    CT_WRITE_LAS_SCALAR(info.m_green, quint16);

    // blue
    CT_WRITE_LAS_SCALAR(info.m_blue, quint16);
}
