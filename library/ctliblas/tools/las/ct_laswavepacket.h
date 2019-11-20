#ifndef CT_LASWAVEPACKET_H
#define CT_LASWAVEPACKET_H

#include "ctliblas/tools/las/ct_lasdefine.h"
#include "ctliblas/tools/las/ct_laspointinfo.h"

#include <QDataStream>

class CTLIBLAS_EXPORT CT_LASWavePacket
{
public:
    CT_LASWavePacket();

    /**
     * @brief Return the size of the wave packet in bytes
     */
    size_t wavePacketSizeInBytes() const;

    /**
     * @brief Returns the types needed to write the wave packet
     */
    QList<CT_LasDefine::LASPointAttributesType> wavePacketTypesNeeded();

    /**
     * @brief Write the information for the wave packet
     */
    void writeWavePacket(QDataStream &stream, const CT_LasPointInfo& info) const;

private:
    char  m_emptyData[29];
};

#endif // CT_LASWAVEPACKET_H
