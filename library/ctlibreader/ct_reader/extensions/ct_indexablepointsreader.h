#ifndef CT_INDEXABLEPOINTSREADER_H
#define CT_INDEXABLEPOINTSREADER_H

#include "ctlibreader_global.h"
#include "ct_reader/abstract/ct_abstractreader.h"
#include "ct_shape2ddata/ct_areashape2ddata.h"

/**
 * Class that offer the points filtering extension to your reader
 */
class CTLIBREADER_EXPORT CT_IndexablePointsReader
{

public:

    virtual bool getPointIndicesInside2DShape(const CT_AreaShape2DData *area2D, bool &all, qint64 &lastIncludedIndex, QList<qint64> &indicesAfterLastIncludedIndex) const = 0;

    virtual QString getFormatCode() const = 0;
};

#endif // CT_INDEXABLEPOINTSREADER_H
