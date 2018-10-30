#ifndef DISPATCHINFORMATION_H
#define DISPATCHINFORMATION_H

#include <QtGlobal>

/**
 * @brief This class is useful to keep objects dispatch information. When you add objects
 *        to the scene it will be add in a chunk. This class will keep the index of the
 *        first object and the number of objects added for one chunk.
 */
class DispatchInformation {
public:
    typedef size_t BEGIN_TYPE;

    DispatchInformation() : m_begin(INVALID_BEGIN) {}
    DispatchInformation(const BEGIN_TYPE& begin) : m_begin(begin) {}

    virtual ~DispatchInformation() {}

    /**
     * @brief Returns the begin index in the chunk of the first object
     */
    inline const BEGIN_TYPE& begin() const { return m_begin; }

    /**
     * @brief Change the beginning (count and end will not be modified)
     */
    inline void setBegin(const BEGIN_TYPE& begin) { m_begin = begin; }

    /**
     * @brief Add "count" objects more
     */
    virtual void addObjects(const size_t& count) = 0;

    /**
     * @brief Returns the number of object added to the chunk
     */
    virtual size_t count() const = 0;

    /**
     * @brief Returns the index of the last objects
     */
    inline size_t end() const { return begin()+(count()-1); }

    /**
     * @brief You can check if begin() is invalid by test
     *        if it was equal to this value
     */
    static const BEGIN_TYPE INVALID_BEGIN;

private:
    /**
     * @brief Begin index in the chunk of the first object
     */
    BEGIN_TYPE  m_begin;
};

#endif // DISPATCHINFORMATION_H
