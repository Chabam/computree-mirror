#ifndef CT_LASPOINTFORMAT2_H
#define CT_LASPOINTFORMAT2_H

#include "ctliblas/tools/las/ct_laspointformat0.h"
#include "ctliblas/tools/las/ct_lascolorpacket.h"

class CTLIBLAS_EXPORT CT_LASPointFormat2 : public CT_LASPointFormat0, public CT_LASColorPacket
{
public:
    CT_LASPointFormat2();

    /**
     * @brief Return the size of the points in byte
     */
    virtual size_t sizeInBytes() const;

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual const CT_LasPointInfo& write(QDataStream &stream, CT_LASHeader* header, const CT_Point &p, const size_t &globalIndex) const;

protected:
    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch();

    /**
     * @brief Write only the information of the point format 2
     */
    void writeInfoFormat2(QDataStream &stream, const CT_LasPointInfo& info) const;
};

#endif // CT_LASPOINTFORMAT2_H
