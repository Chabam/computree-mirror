#include "dm_amkglglobalpointscloudmanager.h"

#include "ct_global/ct_context.h"

#include "ct_tools/repository/ct_repositoryaccess.h"
#include "ct_cloud/tools/ct_globalpointcloudmanager.h"

DM_AMKglGlobalPointsCloudManager::DM_AMKglGlobalPointsCloudManager()
{
    CT_RepositoryAccess access(PS_REPOSITORY);
    m_globalCloudManager = access.globalPointCloudManager();
    m_globalCloudManager->addGlobalCloudListener(this);
}

GlobalColorCloudRegistered DM_AMKglGlobalPointsCloudManager::createNewColorCloudAndRegisterIt(GlobalColorCloud *&directcloud) const
{
    GlobalColorCloudRegistered r = PS_REPOSITORY->createNewColorCloud(CT_Repository::SyncWithPointCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalNormalCloudRegistered DM_AMKglGlobalPointsCloudManager::createNewNormalCloudSyncedAndRegisterIt(GlobalNormalCloud *&directcloud) const
{
    GlobalNormalCloudRegistered r = PS_REPOSITORY->createNewNormalCloud(CT_Repository::SyncWithPointCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalInfoCloudRegistered DM_AMKglGlobalPointsCloudManager::createNewInfoCloudSyncedAndRegisterIt(GlobalInfoCloud *&directcloud) const
{
    GlobalInfoCloudRegistered r = PS_REPOSITORY->createNewCloudT< CT_StdCloudRegisteredT<GlobalInfoCloud>, GlobalInfoCloud >(CT_Repository::SyncWithPointCloud);
    directcloud = r->cloudT();
    return r;
}

GlobalGlIndexCloudRegistered DM_AMKglGlobalPointsCloudManager::createNewGlIndexCloudSyncedAndRegisterIt(GlobalGlIndexCloud*& directcloud) const
{
    GlobalGlIndexCloudRegistered r = PS_REPOSITORY->createNewIndexCloudT<CT_CloudIndexStdVectorT >(CT_Repository::SyncWithPointCloud);
    directcloud = r->abstractModifiableCloudIndex();
    return r;
}

size_t DM_AMKglGlobalPointsCloudManager::size() const
{
    return m_accessor.size();
}

bool DM_AMKglGlobalPointsCloudManager::isEmpty() const
{
    return m_accessor.size() == 0;
}

size_t DM_AMKglGlobalPointsCloudManager::countCloud() const
{
    return m_globalCloudManager->getSyncedArray().size();
}

size_t DM_AMKglGlobalPointsCloudManager::cloudBeginIndexAt(const size_t &index) const
{
    return m_globalCloudManager->getSyncedArray().at(int(index))->first();
}

size_t DM_AMKglGlobalPointsCloudManager::cloudSizeAt(const size_t &index) const
{
    return m_globalCloudManager->getSyncedArray().at(int(index))->size();
}

DoublePoint DM_AMKglGlobalPointsCloudManager::getDoublePointAtGlobalIndex(const size_t &globalIndex) const
{
    return m_accessor.pointAt(globalIndex);
}

void DM_AMKglGlobalPointsCloudManager::getDoublePointAtGlobalIndex(const size_t &globalIndex, DoublePoint &p) const
{
    m_accessor.pointAt(globalIndex, p);
}

FloatPoint &DM_AMKglGlobalPointsCloudManager::getFloatPointAtGlobalIndex(const size_t &globalIndex)
{
    return m_accessor.internalPointAt(globalIndex);
}

void DM_AMKglGlobalPointsCloudManager::cloudWillBeDeleted(const size_t &beginIndex, const size_t &size)
{
    informListenerThatACloudWillBeDeleted(beginIndex, size);
}

void DM_AMKglGlobalPointsCloudManager::cloudIndexShiftedAfterThisCloud(const size_t &beginIndex, const size_t &size)
{
    informListenerThatCloudIndexShiftedAfterThisCloud(beginIndex, size);
}

void DM_AMKglGlobalPointsCloudManager::cloudDeleted(const size_t &beginIndex, const size_t &size)
{
    informListenerThatACloudIsDeleted(beginIndex, size);
}

void DM_AMKglGlobalPointsCloudManager::cloudAdded(const size_t &size)
{
    informListenerThatACloudIsAdded(size);
}
