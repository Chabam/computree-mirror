#ifndef CT_ATTRIBUTESNORMAL_H
#define CT_ATTRIBUTESNORMAL_H

#include "ct_normalcloud/abstract/ct_abstractnormalcloud.h"

/**
 * @brief A cloud attributes of type "normals". Normals are applied directly to the object where draw normals.
 */
class CTLIBCLOUDS_EXPORT CT_AttributesNormal
{
public:
    CT_AttributesNormal(CT_AbstractNormalCloud* cloud = nullptr);

    /**
     * @brief Copy constructor
     *
     *        What is copied :
     *          - Normal cloud is copied (call method "copy()") if not nullptr
     *
     *        What is initialized differently :
     */
    CT_AttributesNormal(const CT_AttributesNormal& other);

    virtual ~CT_AttributesNormal();

    const CT_Normal& constNormalAt(const size_t& index) const;

    void setNormalCloud(CT_AbstractNormalCloud* cloud);

    CT_AbstractNormalCloud* normalCloud() const;

    size_t attributesSize() const;

private:
    CT_AbstractNormalCloud  *m_normalCloud;
};

#endif // CT_ATTRIBUTESNORMAL_H
