#ifndef CT_ABSTRACTLASPOINTFORMAT_H
#define CT_ABSTRACTLASPOINTFORMAT_H

#include "ctliblas/tools/las/ct_laspointinfo.h"
#include "ctliblas/readers/headers/ct_lasheader.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"
#include "ct_itemdrawable/ct_pointsattributescolor.h"

#define CT_WRITE_LAS_SCALAR(information, type) if(information.first == nullptr) { stream << type(0); } else { stream << type(information.first->dValueAt(information.second)); }

/**
 * @brief A tool class that can be use to write a point in a LAS File
 */
class CTLIBLAS_EXPORT CT_AbstractLASPointFormat
{
public:
    using AllAttributesCollection = QMultiHash<CT_LasDefine::LASPointAttributesType, const CT_AbstractPointAttributesScalar*>;

    CT_AbstractLASPointFormat();
    virtual ~CT_AbstractLASPointFormat();

    /**
     * @brief Return the size of the points in byte
     */
    virtual size_t sizeInBytes() const = 0;

    /**
     * @brief Clear infos of all points
     */
    void clearInfos();

    /**
     * @brief Call this method before use the method "write" and after "setAttributes". This will create the
     *        ReturnNumber informations for each points to write.
     */
    bool initReturnNumber(const AllAttributesCollection& attributes);

    /**
     * @brief Call this method before use the method "write" and after "setAttributes". This will create all
     *        informations necessary for each points to write.
     */
    bool initWrite(const AllAttributesCollection& attributes, const QList<const CT_PointsAttributesColor*>& colors);

    /**
     * @brief Return the info for point at globalIndex
     */
    inline const CT_LasPointInfo& infoOfPoint(const size_t &globalIndex) const {
        return m_infosFaster[globalIndex];
    }

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual const CT_LasPointInfo& write(QDataStream &stream, CT_LASHeader* header, const CT_Point &p, const size_t &globalIndex) const = 0;

private:
    // index = globalIndex, value = information of the point
    std::vector<CT_LasPointInfo>        m_infosFaster;

    QMutex                              mMutexInitialization;
    bool                                mInitialized;
    bool                                mReturnNumberInitialized;

    void initType(const CT_LasDefine::LASPointAttributesType& type, const AllAttributesCollection& attributes);

protected:

    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch() = 0;

};

#endif // CT_ABSTRACTLASPOINTFORMAT_H
