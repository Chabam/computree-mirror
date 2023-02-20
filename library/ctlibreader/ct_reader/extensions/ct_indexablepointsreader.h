#ifndef CT_INDEXABLEPOINTSREADER_H
#define CT_INDEXABLEPOINTSREADER_H

#include "ctlibreader_global.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_shape2ddata/ct_areashape2ddata.h"

/**
 * Class that offer the points filtering extension to your reader
 */
class CTLIBREADER_EXPORT CT_IndexablePointsReader : public CT_AbstractReader
{
    Q_OBJECT

public:

    CT_IndexablePointsReader(int subMenuLevel = 0);
    CT_IndexablePointsReader(const CT_AbstractReader& other);

    virtual bool getPointIndicesInside2DShape(const CT_AreaShape2DData *area2D, size_t &lastIncludedIndex, QList<size_t> &indicesAfterLastIncludedIndex) const = 0;

    virtual QString getFormatCode() const = 0;
};

#endif // CT_INDEXABLEPOINTSREADER_H
