#ifndef IGLOBALCLOUDLISTENER_H
#define IGLOBALCLOUDLISTENER_H

#include "interfaces/igenericcloudmanager.h"

/**
 * @brief Used by this engine to know when a cloud is added/deleted from the global points/face/edge cloud
 */
class IGlobalCloudListener
{
public:

    virtual ~IGlobalCloudListener() {}

protected:
    friend class IGenericCloudManager;

    /**
     * @brief Called before the global cloud will be imputed of elements between beginIndex and endIndex (endIndex = beginIndex+size).
     */
    virtual void cloudWillBeDeleted(const size_t &beginIndex, const size_t &size) { Q_UNUSED(beginIndex) Q_UNUSED(size) }

    /**
     * @brief Called after the global cloud it's indexes was shifted.
     */
    virtual void cloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size) { Q_UNUSED(beginIndex) Q_UNUSED(size) }

    /**
     * @brief Called after the global cloud was imputed of elements between beginIndex and endIndex (endIndex = beginIndex+size).
     */
    virtual void cloudDeleted(const size_t &beginIndex, const size_t &size) = 0;

    /**
     * @brief Called when the size of the global cloud has increased
     */
    virtual void cloudAdded(const size_t &size) = 0;
};

#endif // IGLOBALCLOUDLISTENER_H
