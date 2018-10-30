#ifndef POINTSDISPATCHINFORMATION_H
#define POINTSDISPATCHINFORMATION_H

#include "dispatchinformation.h"

/**
 * @brief This class is useful to keep points dispatch information. When you add multiple global points
 *        to the scene it will be dispatched in multiple chunk. This class will keep the index of the
 *        first point and the number of points added for one chunk.
 */
class PointsDispatchInformation : public DispatchInformation {
public:
    typedef quint32 COUNT_TYPE; // max of 4294967296 elements !

    PointsDispatchInformation() :  m_count(0), DispatchInformation() {}
    PointsDispatchInformation(const BEGIN_TYPE& begin) : m_count(0), DispatchInformation(begin) {}

    void addObjects(const size_t& count) {
        m_count += (COUNT_TYPE)count;
    }

    size_t count() const { return m_count; }

private:
    /**
     * @brief Number of points added to the chunk
     */
    COUNT_TYPE  m_count;
};

#endif // POINTSDISPATCHINFORMATION_H
