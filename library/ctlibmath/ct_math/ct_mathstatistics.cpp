#include "ct_mathstatistics.h"

double CT_MathStatistics::computeQuantile(QList<double> &values, double quantile, bool sort)
{
    if (sort)
    {
        std::sort(values.begin(), values.end());
    }

    double n = quantile*(values.size() - 1);
    int intPart = (int) n;
    double fractPart = n - (double)intPart;

    if (intPart < 0) {return values.first();}
    if (intPart >= values.size() - 1) {return values.last();}
    if (fractPart == 0) {return values.at(intPart);}
    return values.at(intPart) + fractPart * (values.at(intPart + 1) - values.at(intPart));
}

float CT_MathStatistics::computeQuantile(QList<float> &values, double quantile, bool sort)
{
    if (sort)
    {
        std::sort(values.begin(), values.end());
    }

    float n = float(quantile)*(values.size() - 1);
    int intPart = (int) n;
    float fractPart = n - (float)intPart;

    if (intPart < 0) {return values.first();}
    if (intPart >= values.size() - 1) {return values.last();}
    if (fractPart == 0) {return values.at(intPart);}
    return values.at(intPart) + fractPart * (values.at(intPart + 1) - values.at(intPart));
}
