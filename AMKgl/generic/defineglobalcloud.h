#ifndef DEFINECLOUD_H
#define DEFINECLOUD_H

class DummyGlobalInfoCloud;
class DummyGlobalColorCloud;
class DummyGlobalNormalCloud;

#include <QSharedPointer>

namespace AMKgl {

    typedef DummyGlobalInfoCloud                GlobalInfoCloud;
    typedef DummyGlobalColorCloud               GlobalColorCloud;
    typedef DummyGlobalNormalCloud              GlobalNormalCloud;

    typedef QSharedPointer<GlobalInfoCloud>     GlobalInfoCloudRegistered;
    typedef QSharedPointer<GlobalColorCloud>    GlobalColorCloudRegistered;
    typedef QSharedPointer<GlobalNormalCloud>   GlobalNormalCloudRegistered;
}

#endif // DEFINECLOUD_H
