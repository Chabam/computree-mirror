#ifndef DM_AMKGLGLOBALPOINTSCLOUDMANAGER_H
#define DM_AMKGLGLOBALPOINTSCLOUDMANAGER_H

#include "interfaces/iglobalpointcloudmanager.h"
#include "ct_cloud/tools/iglobalcloudlistener.h"
#include "ct_accessor/ct_pointaccessor.h"

class DM_AMKglGlobalPointsCloudManager : public IGlobalPointCloudManager, public IGlobalCloudListener
{
public:
    DM_AMKglGlobalPointsCloudManager();

    GlobalColorCloudRegistered createNewColorCloudAndRegisterIt(GlobalColorCloud*& directcloud) const;
    GlobalNormalCloudRegistered createNewNormalCloudSyncedAndRegisterIt(GlobalNormalCloud*& directcloud) const;
    GlobalInfoCloudRegistered createNewInfoCloudSyncedAndRegisterIt(GlobalInfoCloud*& directcloud) const;
    GlobalGlIndexCloudRegistered createNewGlIndexCloudSyncedAndRegisterIt(GlobalGlIndexCloud*& directcloud) const;

    size_t size() const;
    bool isEmpty() const;

    size_t countCloud() const;
    size_t cloudBeginIndexAt(const size_t& index) const;
    size_t cloudSizeAt(const size_t& index) const;

    DoublePoint getDoublePointAtGlobalIndex(const size_t &globalIndex) const;
    void getDoublePointAtGlobalIndex(const size_t &globalIndex, DoublePoint &p) const;
    FloatPoint& getFloatPointAtGlobalIndex(const size_t &globalIndex);

private:
    CT_PointAccessor            m_accessor;
    CT_GlobalPointCloudManager* m_globalCloudManager;

protected:
    void cloudWillBeDeleted(const size_t &beginIndex, const size_t &size);
    void cloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size);
    void cloudDeleted(const size_t &beginIndex, const size_t &size);
    void cloudAdded(const size_t &size);
};

#endif // DM_AMKGLGLOBALPOINTSCLOUDMANAGER_H
