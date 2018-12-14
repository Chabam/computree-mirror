#ifndef CT_MATHSTATISTICS_H
#define CT_MATHSTATISTICS_H

#include <math.h>
#include <algorithm>

#include <QList>

#include "ctlibmath_global.h"

class CTLIBMATH_EXPORT CT_MathStatistics
{
public:
    static double computeQuantile(QList<double> &values, double quantile, bool sort = false);
};

#endif // CT_MATHSTATISTICS_H
