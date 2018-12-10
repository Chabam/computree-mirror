#include "dm_amkglglobaledgescloudmanager.h"

#include "ct_accessor/ct_repositoryaccess.h"
#include "ct_cloud/tools/ct_globaledgecloudmanager.h"

DM_AMKglGlobalEdgesCloudManager::DM_AMKglGlobalEdgesCloudManager()
{
    CT_RepositoryAccess access(PS_REPOSITORY);
    m_globalCloudManager = access.globalEdgeCloudManager();
    m_globalCloudManager->addGlobalCloudListener(this);
}

GlobalColorCloudRegistered DM_AMKglGlobalEdgesCloudManager::createNewColorCloudAndRegisterIt(GlobalColorCloud *&directcloud) const
{
    GlobalColorCloudRegistered r = PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithEdgeCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalNormalCloudRegistered DM_AMKglGlobalEdgesCloudManager::createNewNormalCloudSyncedAndRegisterIt(GlobalNormalCloud *&directcloud) const
{
    GlobalNormalCloudRegistered r = PS_REPOSITORY->createNewNormalCloud(CT_Repository::SyncWithEdgeCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalInfoCloudRegistered DM_AMKglGlobalEdgesCloudManager::createNewInfoCloudSyncedAndRegisterIt(GlobalInfoCloud *&directcloud) const
{
    GlobalInfoCloudRegistered r = PS_REPOSITORY->createNewCloudT< CT_StdCloudRegisteredT<GlobalInfoCloud>, GlobalInfoCloud >(CT_Repository::SyncWithEdgeCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalGlIndexCloudRegistered DM_AMKglGlobalEdgesCloudManager::createNewGlIndexCloudSyncedAndRegisterIt(GlobalGlIndexCloud*& directcloud) const
{
    GlobalGlIndexCloudRegistered r = PS_REPOSITORY->createNewIndexCloudT<CT_CloudIndexStdVectorT >(CT_Repository::SyncWithEdgeCloud);
    directcloud = r->abstractModifiableCloudIndex();
    return r;
}

size_t DM_AMKglGlobalEdgesCloudManager::size() const
{
    return m_accessor.size();
}

bool DM_AMKglGlobalEdgesCloudManager::isEmpty() const
{
    return m_accessor.size() == 0;
}

size_t DM_AMKglGlobalEdgesCloudManager::countCloud() const
{
    return m_globalCloudManager->getSyncedArray().size();
}

size_t DM_AMKglGlobalEdgesCloudManager::cloudBeginIndexAt(const size_t &index) const
{
    return m_globalCloudManager->getSyncedArray().at(int(index))->first();
}

size_t DM_AMKglGlobalEdgesCloudManager::cloudSizeAt(const size_t &index) const
{
    return m_globalCloudManager->getSyncedArray().at(int(index))->size();
}

void DM_AMKglGlobalEdgesCloudManager::cloudWillBeDeleted(const size_t &beginIndex, const size_t &size)
{
    informListenerThatACloudWillBeDeleted(beginIndex, size);
}

void DM_AMKglGlobalEdgesCloudManager::cloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size)
{
    informListenerThatCloudIndexShiftedAfterThisCloud(beginIndex, size);
}

void DM_AMKglGlobalEdgesCloudManager::cloudDeleted(const size_t &beginIndex, const size_t &size)
{
    informListenerThatACloudIsDeleted(beginIndex, size);
}

void DM_AMKglGlobalEdgesCloudManager::cloudAdded(const size_t &size)
{
    informListenerThatACloudIsAdded(size);
}
