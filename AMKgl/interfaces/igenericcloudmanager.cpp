#include "igenericcloudmanager.h"

#include "renderer/iglobalcloudlistener.h"

void IGenericCloudManager::informListenerThatACloudWillBeDeleted(const size_t &beginIndex, const size_t &size) {
    foreach (IGlobalCloudListener* listener, m_listener) {
        listener->cloudWillBeDeleted(beginIndex, size);
    }
}

void IGenericCloudManager::informListenerThatCloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size)
{
    foreach (IGlobalCloudListener* listener, m_listener) {
        listener->cloudIndexShiftedAfterThisCloud(beginIndex, size);
    }
}

void IGenericCloudManager::informListenerThatACloudIsDeleted(const size_t &beginIndex, const size_t &size) {
    foreach (IGlobalCloudListener* listener, m_listener) {
        listener->cloudDeleted(beginIndex, size);
    }
}

void IGenericCloudManager::informListenerThatACloudIsAdded(const size_t &size) {
    foreach (IGlobalCloudListener* listener, m_listener) {
        listener->cloudAdded(size);
    }
}
