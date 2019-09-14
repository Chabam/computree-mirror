#ifndef CT_ATTRIBUTESCOLOR_H
#define CT_ATTRIBUTESCOLOR_H

#include "ct_colorcloud/abstract/ct_abstractcolorcloud.h"

/**
 * @brief A cloud attributes of type "color". Colors are applied directly to the object to colorize.
 */
class CTLIBCLOUDS_EXPORT CT_AttributesColor
{
public:
    CT_AttributesColor(CT_AbstractColorCloud* cloud = nullptr, bool autoDeleteColorCloud = true);

    /**
     * @brief Copy constructor
     *
     *        What is copied :
     *          - Color cloud is copied (call method "copy()") if not nullptr
     *
     *        What is initialized differently :
     */
    CT_AttributesColor(const CT_AttributesColor& other);

    virtual ~CT_AttributesColor();

    const CT_Color& constColorAt(const size_t& index) const;

    void setColorCloud(CT_AbstractColorCloud* cloud, bool autoDeleteColorCloud = true);

    CT_AbstractColorCloud* colorCloud() const;

    size_t attributesSize() const;

private:
    CT_AbstractColorCloud*  m_colorCloud;
    bool                    mAutoDeleteColorCloud;
};

#endif // CT_ATTRIBUTESCOLOR_H
