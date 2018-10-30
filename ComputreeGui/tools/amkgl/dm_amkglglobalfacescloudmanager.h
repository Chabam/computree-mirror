#ifndef DM_AMKGLGLOBALFACESCLOUDMANAGER_H
#define DM_AMKGLGLOBALFACESCLOUDMANAGER_H

#include "interfaces/iglobalfacecloudmanager.h"
#include "ct_cloud/tools/iglobalcloudlistener.h"
#include "ct_accessor/ct_faceaccessor.h"

class DM_AMKglGlobalFacesCloudManager : public IGlobalFaceCloudManager, public IGlobalCloudListener
{
public:
    DM_AMKglGlobalFacesCloudManager();

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
    CT_FaceAccessor            m_accessor;
    CT_GlobalFaceCloudManager* m_globalCloudManager;

protected:
    void cloudWillBeDeleted(const size_t &beginIndex, const size_t &size);
    void cloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size);
    void cloudDeleted(const size_t &beginIndex, const size_t &size);
    void cloudAdded(const size_t &size);
};

#endif // DM_AMKGLGLOBALFACESCLOUDMANAGER_H
