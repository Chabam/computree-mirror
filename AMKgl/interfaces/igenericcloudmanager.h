#ifndef IGENERICCLOUDMANAGER_H
#define IGENERICCLOUDMANAGER_H

#include "defineglobalcloud.h"

class IGlobalCloudListener;

using namespace AMKgl;

/**
 * @brief Inherit from this class to let the engine create new cloud of
 *        colors/normals/infos. Cloud must be synced with the global T cloud !
 *
 *        This class let the engine access to the global T cloud too.
 *
 *        And finally this class let the engine register some listener to know
 *        when cloud is added/deleted from the global T cloud.
 *
 *        Used by "IEasyAccess" class.
 */
class IGenericCloudManager
{
public:
    virtual ~IGenericCloudManager() {}

    /**
     * @brief Create a cloud (synced with the global X cloud) that can contains colors for X and return it
     */
    virtual GlobalColorCloudRegistered createNewColorCloudAndRegisterIt(GlobalColorCloud*& directcloud) const = 0;

    /**
     * @brief Create a cloud (synced with the global X cloud) that can contains normals for X and return it
     */
    virtual GlobalNormalCloudRegistered createNewNormalCloudSyncedAndRegisterIt(GlobalNormalCloud*& directcloud) const = 0;

    /**
     * @brief Create cloud (synced with the global X cloud) that can contains informations (selected/visible/etc...) for X and return it
     */
    virtual GlobalInfoCloudRegistered createNewInfoCloudSyncedAndRegisterIt(GlobalInfoCloud*& directcloud) const = 0;

    /**
     * @brief Create cloud (synced with the global X cloud) that can contains indexes of X
     */
    virtual GlobalGlIndexCloudRegistered createNewGlIndexCloudSyncedAndRegisterIt(GlobalGlIndexCloud*& directcloud) const = 0;

    /**
     * @brief Add a listener that will be informed when a cloud is added/deleted
     */
    void addCloudListener(const IGlobalCloudListener* listener) {
        m_listener.append((IGlobalCloudListener*)listener);
    }

    /**
     * @brief Remove a listener
     */
    void removeCloudListener(const IGlobalCloudListener* listener) {
        m_listener.removeOne((IGlobalCloudListener*)listener);
    }

    /**
     * @brief Returns the size of the global T cloud
     */
    virtual size_t size() const = 0;

    /**
     * @brief Return the current number of cloud
     */
    virtual size_t countCloud() const = 0;

    /**
     * @brief Return the first index of the cloud at specified index
     */
    virtual size_t cloudBeginIndexAt(const size_t& index) const = 0;

    /**
     * @brief Return the size of the cloud at specified index
     */
    virtual size_t cloudSizeAt(const size_t& index) const = 0;

    /**
     * @brief Returns true if the global T cloud is empty.
     */
    virtual bool isEmpty() const = 0;

private:
    QList<IGlobalCloudListener*>   m_listener;

protected:
    /**
     * @brief Call this method when a cloud will be deleted from the global X cloud
     */
    void informListenerThatACloudWillBeDeleted(const size_t &beginIndex, const size_t &size);

    /**
     * @brief Call this method when index of a cloud was shifted from the global X cloud
     */
    void informListenerThatCloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size);

    /**
     * @brief Call this method when a cloud is deleted from the global X cloud
     */
    void informListenerThatACloudIsDeleted(const size_t &beginIndex, const size_t &size);

    /**
     * @brief Call this method when a cloud is add to the global X cloud
     */
    void informListenerThatACloudIsAdded(const size_t &size);
};

#endif // IGENERICCLOUDMANAGER_H
