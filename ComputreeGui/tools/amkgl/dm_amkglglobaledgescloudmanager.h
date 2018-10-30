#ifndef DM_AMKGLGLOBALEDGESCLOUDMANAGER_H
#define DM_AMKGLGLOBALEDGESCLOUDMANAGER_H

#include "interfaces/iglobaledgecloudmanager.h"
#include "ct_cloud/tools/iglobalcloudlistener.h"
#include "ct_accessor/ct_edgeaccessor.h"

class DM_AMKglGlobalEdgesCloudManager : public IGlobalEdgeCloudManager, public IGlobalCloudListener
{
public:
    DM_AMKglGlobalEdgesCloudManager();

    GlobalColorCloudRegistered createNewColorCloudAndRegisterIt(GlobalColorCloud*& directcloud) const;
    GlobalNormalCloudRegistered createNewNormalCloudSyncedAndRegisterIt(GlobalNormalCloud*& directcloud) const;
    GlobalInfoCloudRegistered createNewInfoCloudSyncedAndRegisterIt(GlobalInfoCloud*& directcloud) const;
    GlobalGlIndexCloudRegistered createNewGlIndexCloudSyncedAndRegisterIt(GlobalGlIndexCloud*& directcloud) const;

    size_t size() const;
    bool isEmpty() const;

    size_t countCloud() const;
    size_t cloudBeginIndexAt(const size_t& index) const;
    size_t cloudSizeAt(const size_t& index) const;

private:
    CT_EdgeAccessor            m_accessor;
    CT_GlobalEdgeCloudManager* m_globalCloudManager;

protected:
    void cloudWillBeDeleted(const size_t &beginIndex, const size_t &size);
    void cloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size);
    void cloudDeleted(const size_t &beginIndex, const size_t &size);
    void cloudAdded(const size_t &size);
};

#endif // DM_AMKGLGLOBALEDGESCLOUDMANAGER_H
