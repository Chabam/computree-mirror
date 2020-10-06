#ifndef OBJECTSDISPATCHINFORMATION_H
#define OBJECTSDISPATCHINFORMATION_H

#include "dispatchinformation.h"

/**
 * @brief This class is useful to keep objects dispatch information. When you add objects
 *        to the scene it will be add in a chunk. This class will keep the index of the
 *        first object and the number of objects added for one chunk.
 */
class ObjectsDispatchInformation : public DispatchInformation {
public:

    typedef quint32 COUNT_TYPE; // max of 4294967296 elements !

    ObjectsDispatchInformation() : DispatchInformation(), m_count(0) {}
    ObjectsDispatchInformation(const BEGIN_TYPE& begin) : DispatchInformation(begin), m_count(0) {}

    void addObjects(const size_t& count) {
        m_count += (COUNT_TYPE)count;
    }

    size_t count() const { return m_count; }

private:
    /**
     * @brief Number of objects added to the chunk
     */
    COUNT_TYPE  m_count;
};

#endif // OBJECTSDISPATCHINFORMATION_H
