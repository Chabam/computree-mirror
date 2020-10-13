#ifndef DEFINEGLOBALCLOUD_H
#define DEFINEGLOBALCLOUD_H

#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"
#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"
#include "ct_cloud/ct_standardcloudstdvectort.h"
#include "ct_cloudindex/abstract/ct_abstractmodifiablecloudindex.h"

#include "ct_colorcloud/registered/ct_standardcolorcloudregistered.h"
#include "ct_normalcloud/registered/ct_standardnormalcloudregistered.h"
#include "ct_cloudindex/registered/abstract/ct_abstractmodifiablecloudindexregistered.h"
#include "ct_cloud/registered/ct_stdcloudregisteredt.h"

#include "elementinfo.h"

#include <QSharedPointer>

namespace AMKgl {

    typedef CT_StandardCloudStdVectorT<ElementInfo> GlobalInfoCloud;
    typedef CT_AbstractColorCloud                   GlobalColorCloud;
    typedef CT_AbstractNormalCloud                  GlobalNormalCloud;
    typedef CT_AbstractModifiableCloudIndex         GlobalGlIndexCloud;

    typedef QSharedPointer<CT_StdCloudRegisteredT<GlobalInfoCloud> >    GlobalInfoCloudRegistered;
    typedef QSharedPointer<CT_StandardColorCloudRegistered>             GlobalColorCloudRegistered;
    typedef QSharedPointer<CT_StandardNormalCloudRegistered>            GlobalNormalCloudRegistered;
    typedef QSharedPointer<CT_AbstractModifiableCloudIndexRegistered >  GlobalGlIndexCloudRegistered;
}

#endif // DEFINEGLOBALCLOUD_H
