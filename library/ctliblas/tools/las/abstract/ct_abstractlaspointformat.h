#ifndef CT_ABSTRACTLASPOINTFORMAT_H
#define CT_ABSTRACTLASPOINTFORMAT_H

#include "ctliblas/tools/las/ct_laspointinfo.h"
#include "ctliblas/readers/headers/ct_lasheader.h"
#include "ct_itemdrawable/abstract/ct_abstractpointattributesscalar.h"

#define CT_WRITE_LAS_SCALAR(information, type) if(information.first == nullptr) { stream << type(0); } else { stream << type(information.first->dValueAt(information.second)); }

/**
 * @brief A tool class that can be use to write a point in a LAS File
 */
class CTLIBLAS_EXPORT CT_AbstractLASPointFormat
{
public:
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
     *        informations necessary for each points to write.
     * @param fastButConsumeMoreMemory : if true the init will be faster but it will consume more memory than if the parameter is false. This is
     *                                   because to be faster you must use an array and to consume less memory you must use a hashtable.
     */
    bool initWrite(const QHash<CT_LasDefine::LASPointAttributesType, CT_AbstractPointAttributesScalar *> &attributes, bool fastButConsumeMoreMemory = false);

    /**
     * @brief Return the info for point at globalIndex
     */
    inline CT_LasPointInfo* infoOfPoint(const size_t &globalIndex) const {
        if(m_infosFaster.empty())
            return m_infos.value(globalIndex, nullptr);

        return &const_cast<CT_AbstractLASPointFormat*>(this)->m_infosFaster[globalIndex];
    }

    /**
     * @brief Write the point 'globalIndex' (index in the global pointCloud)
     */
    virtual CT_LasPointInfo* write(QDataStream &stream, CT_LASHeader* header, const CT_Point &p, const size_t &globalIndex) const = 0;

private:
    // key = globalIndex of the point, value = information of the point
    QHash<size_t, CT_LasPointInfo*>     m_infos;
    // index = globalIndex, value = information of the point
    std::vector<CT_LasPointInfo>        m_infosFaster;

    QMutex                              mMutexInitialization;
    bool                                mInitialized;

protected:

    /**
     * @brief Derivated class must return attribute types that they need to write a point
     */
    virtual QList<CT_LasDefine::LASPointAttributesType> typesToSearch() = 0;

};

#endif // CT_ABSTRACTLASPOINTFORMAT_H
