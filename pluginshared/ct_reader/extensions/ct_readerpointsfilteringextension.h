#ifndef CT_READERPOINTSFILTERINGEXTENSION_H
#define CT_READERPOINTSFILTERINGEXTENSION_H

#include <functional>

#include "ct_point.h"

/**
 * Class that offer the points filtering extension to your reader
 */
class PLUGINSHAREDSHARED_EXPORT CT_ReaderPointsFilteringExtension
{
public:
    typedef std::function<bool (const CT_Point& pt)> filterPointsFunction;

    CT_ReaderPointsFilteringExtension();
    virtual ~CT_ReaderPointsFilteringExtension() {}

    /**
     * @brief Set the filtering function (can be a lambda expression per example)
     */
    void setPointsFilter(filterPointsFunction f);

    /**
     * @brief Return the filtering function
     */
    filterPointsFunction getPointsFilter() const;

    /**
     * @brief Return true if points must be filtered, false otherwise
     */
    bool isPointFiltered(const CT_Point& pt) const;

private:
    filterPointsFunction    m_pointsFilter;
};

#endif // CT_READERPOINTSFILTERINGEXTENSION_H
