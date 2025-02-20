#include "dm_abstractattributescolor.h"

DM_AbstractAttributesColor::DM_AbstractAttributesColor(bool local) : DM_AbstractAttributes(local)
{
    m_syncWith = CT_Repository::SyncWithPointCloud;
}

void DM_AbstractAttributesColor::setSyncWithCloud(CT_Repository::SyncCloudWith syncWith)
{
    m_syncWith = syncWith;
}

CT_Repository::SyncCloudWith DM_AbstractAttributesColor::syncWithCloud() const
{
    return m_syncWith;
}
