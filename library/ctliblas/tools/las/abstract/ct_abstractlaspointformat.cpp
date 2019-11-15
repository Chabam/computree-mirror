#include "ct_abstractlaspointformat.h"

#include "ct_iterator/ct_pointiterator.h"
#include "ct_pointcloud/ct_internalpointcloud.h"

CT_AbstractLASPointFormat::CT_AbstractLASPointFormat() :
    mInitialized(false)
{
}

CT_AbstractLASPointFormat::~CT_AbstractLASPointFormat()
{
    clearInfos();
}

bool CT_AbstractLASPointFormat::initWrite(const QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *>& attributes, bool fastButConsumeMoreMemory)
{
    QMutexLocker locker(&mMutexInitialization);

    if(mInitialized)
        return true;

    // get type of attribute to search for point format X (from derivated class)
    QList<CT_LasDefine::LASPointAttributesType> types = typesToSearch();

    QListIterator<CT_LasDefine::LASPointAttributesType> itT(types);

    if(itT.hasNext()) {

        if(fastButConsumeMoreMemory)
            m_infosFaster.resize(PS_REPOSITORY->internalConstPointCloud()->size());

        // for each type of attribute
        while(itT.hasNext())
        {
            CT_LasDefine::LASPointAttributesType type = itT.next();

            // get it in the attributes map
            CT_AbstractPointAttributesScalar *scalar = attributes.value(type, nullptr);

            // if exist
            if(scalar != nullptr)
            {
                // get the point cloud index
                const CT_AbstractPointCloudIndex *indexes = scalar->pointCloudIndex();

                if(indexes != nullptr)
                {
                    size_t pIndex = 0;

                    CT_PointIterator it(indexes);

                    // for each index
                    while(it.hasNext()) {

                        // get info for this global point index
                        CT_LasPointInfo *info = nullptr;

                        if(fastButConsumeMoreMemory) {
                            info = &m_infosFaster[it.next().cIndex()];
                        } else {
                            info = m_infos.value(it.next().cIndex(), nullptr);

                            // if info doesn't already exist
                            if(info == nullptr)
                            {
                                // create it
                                info = new CT_LasPointInfo();

                                // insert in the info map
                                m_infos.insert(it.cIndex(), info);
                            }
                        }

                        // and set it the attribute
                        info->setAttribute(type, scalar, pIndex);

                        ++pIndex;
                    }
                }
            }
        }
    }

    mInitialized = true;

    return true;
}

void CT_AbstractLASPointFormat::clearInfos()
{
    QMutexLocker locker(&mMutexInitialization);

    qDeleteAll(m_infos.begin(), m_infos.end());
    m_infos.clear();

    m_infosFaster.resize(0);
    m_infosFaster.shrink_to_fit();

    mInitialized = false;
}
