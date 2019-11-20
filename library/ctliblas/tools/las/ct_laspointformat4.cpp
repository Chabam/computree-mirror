#include "ct_laspointformat4.h"

CT_LASPointFormat4::CT_LASPointFormat4() : CT_LASPointFormat1()
{
}

size_t CT_LASPointFormat4::sizeInBytes() const
{
    return CT_LASPointFormat1::sizeInBytes() + wavePacketSizeInBytes();
}

const CT_LasPointInfo& CT_LASPointFormat4::write(QDataStream &stream, CT_LASHeader* header, const CT_Point &p, const size_t &globalIndex) const
{
    const CT_LasPointInfo& info = CT_LASPointFormat1::write(stream, header, p, globalIndex);

    writeInfoFormat4(stream, info);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat4::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l = CT_LASPointFormat1::typesToSearch();
    l.append(wavePacketTypesNeeded());

    return l;
}

void CT_LASPointFormat4::writeInfoFormat4(QDataStream &stream, const CT_LasPointInfo& info) const
{
    writeWavePacket(stream, info);
}
