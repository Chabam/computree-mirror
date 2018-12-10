#include "dm_amkglglobalfacescloudmanager.h"

#include "ct_accessor/ct_repositoryaccess.h"
#include "ct_cloud/tools/ct_globalfacecloudmanager.h"

DM_AMKglGlobalFacesCloudManager::DM_AMKglGlobalFacesCloudManager()
{
    CT_RepositoryAccess access(PS_REPOSITORY);
    m_globalCloudManager = access.globalFaceCloudManager();
    m_globalCloudManager->addGlobalCloudListener(this);
}

GlobalColorCloudRegistered DM_AMKglGlobalFacesCloudManager::createNewColorCloudAndRegisterIt(GlobalColorCloud *&directcloud) const
{
    GlobalColorCloudRegistered r = PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithFaceCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalNormalCloudRegistered DM_AMKglGlobalFacesCloudManager::createNewNormalCloudSyncedAndRegisterIt(GlobalNormalCloud *&directcloud) const
{
    GlobalNormalCloudRegistered r = PS_REPOSITORY->createNewNormalCloud(CT_Repository::SyncWithFaceCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalInfoCloudRegistered DM_AMKglGlobalFacesCloudManager::createNewInfoCloudSyncedAndRegisterIt(GlobalInfoCloud *&directcloud) const
{
    GlobalInfoCloudRegistered r = PS_REPOSITORY->createNewCloudT< CT_StdCloudRegisteredT<GlobalInfoCloud>, GlobalInfoCloud >(CT_Repository::SyncWithFaceCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalGlIndexCloudRegistered DM_AMKglGlobalFacesCloudManager::createNewGlIndexCloudSyncedAndRegisterIt(GlobalGlIndexCloud*& directcloud) const
{
    GlobalGlIndexCloudRegistered r = PS_REPOSITORY->createNewIndexCloudT<CT_CloudIndexStdVectorT >(CT_Repository::SyncWithFaceCloud);
    directcloud = r->abstractModifiableCloudIndex();
    return r;
}

size_t DM_AMKglGlobalFacesCloudManager::size() const
{
    return m_accessor.size();
}

bool DM_AMKglGlobalFacesCloudManager::isEmpty() const
{
    return m_accessor.size() == 0;
}

size_t DM_AMKglGlobalFacesCloudManager::countCloud() const
{
    return m_globalCloudManager->getSyncedArray().size();
}

size_t DM_AMKglGlobalFacesCloudManager::cloudBeginIndexAt(const size_t &index) const
{
    return m_globalCloudManager->getSyncedArray().at(int(index))->first();
}

size_t DM_AMKglGlobalFacesCloudManager::cloudSizeAt(const size_t &index) const
{
    return m_globalCloudManager->getSyncedArray().at(int(index))->size();
}

void DM_AMKglGlobalFacesCloudManager::cloudWillBeDeleted(const size_t &beginIndex, const size_t &size)
{
    informListenerThatACloudWillBeDeleted(beginIndex, size);
}

void DM_AMKglGlobalFacesCloudManager::cloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size)
{
    informListenerThatCloudIndexShiftedAfterThisCloud(beginIndex, size);
}

void DM_AMKglGlobalFacesCloudManager::cloudDeleted(const size_t &beginIndex, const size_t &size)
{
    informListenerThatACloudIsDeleted(beginIndex, size);
}

void DM_AMKglGlobalFacesCloudManager::cloudAdded(const size_t &size)
{
    informListenerThatACloudIsAdded(size);
}

