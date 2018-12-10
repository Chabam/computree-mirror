#ifndef CT_READERPOINTSFILTERINGEXTENSION_H
#define CT_READERPOINTSFILTERINGEXTENSION_H

#include "ctlibreader_global.h"

#include <functional>

class CT_Point;

/**
 * Class that offer the points filtering extension to your reader
 */
class CTLIBREADER_EXPORT CT_ReaderPointsFilteringExtension
{
public:
    using filterPointsFunction = std::function<bool(const CT_Point&)>;

    CT_ReaderPointsFilteringExtension();
    virtual ~CT_ReaderPointsFilteringExtension() {}

    /**
     * @brief Set the filtering function (can be a lambda expression per example)
     */
    void setPointsFilter(filterPointsFunction f);

    /**
     * @brief Return the filtering function
     */
    filterPointsFunction pointsFilter() const;

    /**
     * @brief Return true if points must be filtered, false otherwise
     */
    virtual bool isPointFiltered(const CT_Point& pt) const;

private:
    filterPointsFunction    m_pointsFilter;
};

#endif // CT_READERPOINTSFILTERINGEXTENSION_H
