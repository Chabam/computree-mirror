#include "ct_abstractlaspointformat.h"

#include "ct_iterator/ct_pointiterator.h"
#include "ct_pointcloud/ct_internalpointcloud.h"

CT_AbstractLASPointFormat::CT_AbstractLASPointFormat() :
    mInitialized(false),
    mReturnNumberInitialized(false)
{
}

CT_AbstractLASPointFormat::~CT_AbstractLASPointFormat()
{
    clearInfos();
}

bool CT_AbstractLASPointFormat::initWrite(const AllAttributesCollection& attributes, const QList<const CT_PointsAttributesColor*>& colors)
{
    QMutexLocker locker(&mMutexInitialization);

    if(mInitialized)
        return true;

    // get type of attribute to search for point format X (from derivated class)
    QList<CT_LasDefine::LASPointAttributesType> types = typesToSearch();

    if(!types.isEmpty())
    {
        if(!mReturnNumberInitialized)
            m_infosFaster.resize(PS_REPOSITORY->internalConstPointCloud()->size());
        else
            types.removeOne(CT_LasDefine::Return_Number);

        if(!colors.isEmpty())
        {
            bool removed = types.removeOne(CT_LasDefine::Red);
            removed |= types.removeOne(CT_LasDefine::Green);
            removed |= types.removeOne(CT_LasDefine::Blue);

            if(removed)
            {
                for(const CT_PointsAttributesColor* color : colors)
                {
                    // get the point cloud index
                    const CT_AbstractPointCloudIndex* indexes = color->pointCloudIndex();

                    if(indexes != nullptr)
                    {
                        size_t pIndex = 0;

                        CT_PointIterator it(indexes);

                        // for each index
                        while(it.hasNext())
                        {
                            // get info for this global point index and set it the attribute
                            m_infosFaster[it.next().cIndex()].setColorAttribute(color, pIndex);
                            ++pIndex;
                        }
                    }
                }
            }
        }

        // for each type of attribute
        for(CT_LasDefine::LASPointAttributesType type : types)
        {
            initType(type, attributes);
        }
    }

    mInitialized = true;

    return true;
}

void CT_AbstractLASPointFormat::initType(const CT_LasDefine::LASPointAttributesType& type, const AllAttributesCollection& attributes)
{
    const QList<const CT_AbstractPointAttributesScalar*> scalarsForType = attributes.values(type);

    for(const CT_AbstractPointAttributesScalar* scalar : scalarsForType)
    {
        // get the point cloud index
        const CT_AbstractPointCloudIndex* indexes = scalar->pointCloudIndex();

        if(indexes != nullptr)
        {
            size_t pIndex = 0;

            CT_PointIterator it(indexes);

            // for each index
            while(it.hasNext())
            {
                // get info for this global point index and set it the attribute
                m_infosFaster[it.next().cIndex()].setAttribute(type, scalar, pIndex);
                ++pIndex;
            }
        }
    }
}

void CT_AbstractLASPointFormat::clearInfos()
{
    QMutexLocker locker(&mMutexInitialization);

    m_infosFaster.resize(0);
    m_infosFaster.shrink_to_fit();

    mInitialized = false;
    mReturnNumberInitialized = false;
}

bool CT_AbstractLASPointFormat::initReturnNumber(const CT_AbstractLASPointFormat::AllAttributesCollection& attributes)
{
    QMutexLocker locker(&mMutexInitialization);

    if(mInitialized || mReturnNumberInitialized)
        return true;

    m_infosFaster.resize(PS_REPOSITORY->internalConstPointCloud()->size());
    initType(CT_LasDefine::Return_Number, attributes);

    mReturnNumberInitialized = true;
    return true;
}
