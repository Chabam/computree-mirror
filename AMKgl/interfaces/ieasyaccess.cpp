#include "ieasyaccess.h"

// specialisation
template<>
IGlobalPointCloudManager* IEasyAccess::getGlobalCloudManager() const {
    return getGlobalPointCloudManager();
}

template<>
IGlobalEdgeCloudManager* IEasyAccess::getGlobalCloudManager() const {
    return getGlobalEdgeCloudManager();
}

template<>
IGlobalFaceCloudManager* IEasyAccess::getGlobalCloudManager() const {
    return getGlobalFaceCloudManager();
}
