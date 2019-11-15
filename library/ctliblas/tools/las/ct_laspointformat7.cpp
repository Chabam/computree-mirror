#include "ct_laspointformat7.h"

CT_LASPointFormat7::CT_LASPointFormat7() : CT_LASPointFormat6()
{
}

size_t CT_LASPointFormat7::sizeInBytes() const
{
    return CT_LASPointFormat6::sizeInBytes() + colorPacketSizeInBytes();
}

CT_LasPointInfo* CT_LASPointFormat7::write(QDataStream &stream, CT_LASHeader* header, const CT_Point &p, const size_t &globalIndex) const
{
    CT_LasPointInfo *info = CT_LASPointFormat6::write(stream, header, p, globalIndex);

    writeInfoFormat7(stream, info);

    return info;
}

QList<CT_LasDefine::LASPointAttributesType> CT_LASPointFormat7::typesToSearch()
{
    QList<CT_LasDefine::LASPointAttributesType> l = CT_LASPointFormat6::typesToSearch();
    l.append(colorPacketTypesNeeded());

    return l;
}

void CT_LASPointFormat7::writeInfoFormat7(QDataStream &stream, CT_LasPointInfo *info) const
{
    writeColorPacket(stream, info);
}
