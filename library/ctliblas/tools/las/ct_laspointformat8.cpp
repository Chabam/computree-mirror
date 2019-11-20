#include "ct_laspointformat8.h"

CT_LASPointFormat8::CT_LASPointFormat8() : CT_LASPointFormat7()
{
}

size_t CT_LASPointFormat8::sizeInBytes() const
{
    return CT_LASPointFormat7::sizeInBytes() + nirPacketSizeInBytes();
}

const CT_LasPointInfo& CT_LASPointFormat8::write(QDataStream &stream, CT_LASHeader* header, const CT_Point &p, const size_t &globalIndex) const
{
    const CT_LasPointInfo& info = CT_LASPointFormat7::write(stream, header, p, globalIndex);

    writeInfoFormat8(stream, info);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat8::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l = CT_LASPointFormat7::typesToSearch();
    l.append(nirPacketTypesNeeded());

    return l;
}

void CT_LASPointFormat8::writeInfoFormat8(QDataStream &stream, const CT_LasPointInfo& info) const
{
    writeNirPacket(stream, info);
}
